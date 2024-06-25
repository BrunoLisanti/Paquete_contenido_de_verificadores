#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>
#include <time.h>
#include <ifaddrs.h>
#include <linux/limits.h>
//#include <unistd.h>
//

//tamaño paquete
#define BYTES 1024


//Path donde esta la pagina
char rootPath[PATH_MAX];
//Script que ejecuta al ejecutar al REST
char pluScript[255];
//puerto
char port[6];
//Script que se ejecuta para la conexion con el server principal.
char connection_script[255];

//tipos de archivos de
struct {
	const char *ext;
	const char *filetype;
    } extensions [] = {
	{"css", "text/css"  },
    {"js",  "application/javascript" },
    {"json","application/json" },
	{"gif", "image/gif" },
	{"jpg", "image/jpeg"},
	{"jpeg","image/jpeg"},
	{"png", "image/png" },
	{"zip", "image/zip" },
	{"gz",  "image/gz"  },
	{"tar", "image/tar" },
	{"htm", "text/html" },
	{"html","text/html" },
	{"woff","application/x-font-woff" },
	{"ttf","application/octet-stream" }, 
	{"webm","video/webm" },
	{0,0} };

int listenfd;

void iniciaServer(char *);
void response(int);
void sendErrorMsg(int fdaccept);

int main(int argc, char* argv[])
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;

    //Parametros por defectos.
    strcpy(port,"5000");
    strcpy(rootPath,"./etc");
    strcpy(pluScript,"./getplu");
    strcpy(connection_script, "./server_conn > /dev/null 2>&1");

    //Parseamos los argumentos
    int c = 0;
    while ((c = getopt (argc, argv, "p:r:s:c:")) != -1)
    {
        switch (c)
        {
            case 'r':
                strcpy(rootPath,optarg);
                break;
            case 'p':
                strcpy(port,optarg); 
                break;
            case 's':
                strcpy(pluScript,optarg);
                break;
            case 'c':
                strcpy(connection_script,optarg);
                break;
            case '?':
                fprintf(stderr,"Uso: -r directorio de root de la pagina, -p puerto en el que esta escuchando.\n");
                exit(1);
            default:
                exit(1);
        }
    }

    printf("Servidor iniciado en el puerto: %s, directorio root: %s\n script de conexion: %s \n",port,rootPath,connection_script);

    //Arrancamos el server
    iniciaServer(port);

    // Aceptando conexiones
    while (1)
    {
        addrlen = sizeof(clientaddr);
        int acceptfd = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (acceptfd < 0)
        {
            perror("Error en accept()\n");
        }
        else
        {
            int pid;
            if ( (pid = fork()) ==0 )
            {
                close(listenfd);
                if ( fork() == 0 )
                    response(acceptfd);
                exit(0);
            }
            else
            {
                int status;
                close(acceptfd);
                waitpid(pid, &status, 0);
            }
        }

    }

    return 0;
}

void iniciaServer(char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0)
    {
        perror("getaddrinfo() error");
        exit(1);
    }

    for (p = res; p!=NULL; p=p->ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }

    if (p==NULL)
    {
        perror("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}

//Conexion cliente
void response(int acceptfd)
{
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 99999 );

    rcvd=recv(acceptfd, mesg, 99999, 0);

    if (rcvd<0)    // error
        fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    // socket cerrado
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    // mensaje recibido
    {
        printf("Request recibido:\n%s \n", mesg);
        reqline[0] = strtok (mesg, " \t\n");
        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(acceptfd, "HTTP/1.0 400 Bad Request\n", 25);
            }
            else
            {

                if ( strncmp(reqline[1], "/\0", 6)==0 )
                {
                    strcpy(reqline[1],"/index.html");
                }

                strcpy(path, rootPath);
                strcpy(&path[strlen(rootPath)], reqline[1]);
                printf("Enviando archivo: %s\n", path);
                // Nos fijamos que tipo de tipo mime soporta
                char buffer[BYTES+1];
                int buflen=strlen(path);
                const char * fstr = (char *)0;
                int len = 0;
                int i = 0;
                for(i=0;extensions[i].ext != 0;i++)
                {
                    len = strlen(extensions[i].ext);
                    if( !strncmp(&path[buflen-len], extensions[i].ext, len))
                    {
                        fstr =extensions[i].filetype;
                        break;
                    }
                }
                if(fstr == 0)
                {
                    printf("Extension de archivo no soportada.\n");
                }
                else
                {
                    printf("Mime type: %s.\n", fstr);
                }

                //Mandamos el archivo si es un mimetipe soportado
                if ((fstr != 0) && (fd=open(path, O_RDONLY))!=-1 )    //Archivo encontrado
                {
                    sprintf(buffer,"HTTP/1.1 200 OK\nContent-Type: %s\n\n", fstr);
                    write(acceptfd,buffer,strlen(buffer));
                    //send(acceptfd, "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                    {
                        write(acceptfd, data_to_send, bytes_read);
                    }
                }
                else
                {
                   write(acceptfd, "HTTP/1.0 400 Bad Request\n", 25);
                }
            }
        }
        else if ( strncmp(reqline[0], "POST\0", 5)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");

            //printf("req1 = %s, req2 = %s\n", reqline[1],reqline[2]);

            char* pch = strtok(NULL," \n");

            if ( strncmp(reqline[1], "/\0", 2)==0 )
            {
                while (pch != NULL)
                {
                    if ( strncmp(pch, "barcode=", 8)==0 )
                    {
                        char*barcode = strtok(pch, "=");
                        //printf("tok1: %s\n", barcode);//barcode
                        barcode = strtok(NULL, "=");
                        //printf("tok2: %s\n", barcode);//XXXXXXXXX
                        char cmd[BYTES];
                        sprintf(cmd,"%s %s\n", pluScript,barcode);
                        printf("Ejecutando %s", cmd);
                        char buf[10000];
                        FILE *f = popen(cmd,"r");
                        if( f != NULL )
                        {
                            fgets(buf,sizeof(buf),f);
                            pclose(f);
                            printf("resultado del popen: %s size:%d\n", buf, strlen(buf));
                            send(acceptfd, "HTTP/1.1 200 OK\n\n", 17, 0);
                            send(acceptfd, buf, strlen(buf), 0);
                        }
                    }
                    pch = strtok (NULL, " \n");
                }
            }
            else if ( strncmp(reqline[1], "/conn\0", 6) == 0) //Endpoint agregado para ejecutar script de conexion, no funciona.
            {
/*		int status = system(connection_script);
		printf("\n%i\n", status);*/
				FILE *f = popen(connection_script, "r");
				if (f)
					pclose(f);
				/*
                pid_t pid = fork();

                if (pid < 0) 
                {
                    perror("fork");
                    exit(1);
                } 
                else if (pid == 0) 
                {
                    FILE *fp = popen(connection_script, "r");
                    if (fp == NULL) 
                    {
                        perror("popen");
                        exit(1);
                    }
                    pclose(fp);
                    exit(0);
                }*/
            }
            else
            {
                write(acceptfd, "HTTP/1.0 400 Bad Request\n", 25);
            }
        }
    }
}

