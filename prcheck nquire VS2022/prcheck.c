#include <WinSock2.h>
//#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <winsock2.h>

typedef int socklen_t;

#define DEFAULT_PORT        3334
#define DEFAULT_BUFFER      512

int		eanSearch(szBuffXml, szBuff, clienteip);

int			iPort      = DEFAULT_PORT;
BOOL		bInterface = FALSE,
			bRecvOnly  = FALSE;
char		szAddress[128];
char		botones[5];
char		sucursal[100];
char		plufile[200];
char		modeloVerif[200];
int			tipoVerif;
char		txt[300];
FILE * 		fp;
int			isOpen=0;
//char		ver[]="2.58 - 17/10/2016";
char		ver[]="2.59 - 03/04/2023";
FILE * debug;
struct stat		tam;

struct mensaje {
	char header[100];
	int largoHeader;
	char description[100];
	int largoDescription;
	char description2[100];
	int largoDescription2;
	char descriptionNotFound[100];
	int largoDescriptionNotFound;
	char price[100];
	int largoPrice;
	char end[100];
	int largoEnd;
	char button[4];
	int largoButton;
	char buttonOff[4];
	int largoButtonOff;
	// Nuevos
	char price_combo[100];
	int largo_price_combo;
}msg;

struct msgBoton {
	char msg[100];
	int largoMsg;
} msgBotones[4];


int setTipoVerif(char * data);


void usage()
{
    printf("usage: prcheck [-p:x] [-x:PLUFILE] [-v:TIPO_DE_VERIFICADOR] [-b:1234] -s:SUCURSAL\n\n");
    printf("       -p:x      numero de puerto\n");
    printf("       -x:str    path donde se encuentra el archivos de plu.\n");
	printf("       -v:str    Modelo del verificador de precios.\n");
	printf("                 SG15, SG20 o NQUIRE\n");
	printf("       -b:[1234] Nro(s) de boton(es) a habilitar (Solo para SG20).\n");
	printf("       -s:str	 Código de sucursal, sirve para dejar registro de en nombre de archivo de consultas.\n");
	printf("Ejemplo:\n");
	printf("		prcheck -p:4334 -x:plu.asc -v:SG20 -b:23 -s:538\n");
	printf("el ejemplo anterior indica, que escucha en el puerto 4334, toma los precios\n");
	printf("del archivo plu.asc, el modelo de verificador es el SG20, \n");
	printf("que habilite los botones 2 y 3, por último indica que la sucursal donde está\n\n");
	printf("corriendo es la nro. 538, también puede ser un nombre de sucursal.\n\n");
    ExitProcess(1);
}

void registro(char *msg)
{
	/*	Genera archivo de registro
	*	Cada línea tiene fecha y hora, y separado por pipe un caracter msg
	*/
	char nombreFecha[200];
	struct tm *now;
	time_t tval;
	FILE *fpReg;

	tval = time(NULL);
	now = localtime(&tval);

	sprintf(nombreFecha,"registro.%s.%02d%02d%02d.txt",sucursal, now->tm_mday, now->tm_mon + 1, now->tm_year - 100);

	if ( (fpReg=fopen(nombreFecha,"a")) == NULL )
	{
		printf("Error al crear archivo de registos\n");
	}

	fprintf(fpReg,"%02d%02d%02d-%02d:%02d:%02d|%s\n",now->tm_mday, now->tm_mon + 1, now->tm_year - 100, now->tm_hour, now->tm_min, now->tm_sec, msg);
	fflush(fpReg);
	fclose(fpReg);
}

void print(char *msg)
{
	/*	Escribe en .log
	*/
	if (stat("prcheck.log", &tam) == 0)
	{
		if (tam.st_size > 600000) {
			
			if ( (debug=fopen("prcheck.log","w")) == NULL )
			{
				printf("2) Error al crear archivo de log en cero\n");
			}
			fclose(debug);
			if ( (debug=fopen("prcheck.log","a")) == NULL )
			{
				printf("4) Error al crear archivo de log en cero\n");
			}
		}
	}
	fprintf(debug,"%s\n",msg);
	printf("%s\n",msg);
	fflush(debug);
}

void desactivarBotones(SOCKET sock)
{
	char str1[400];
	int nLeft;
    int idx;
	int ret;
	unsigned int i;
	memset(str1,0x0,sizeof(str1));

	// Esta casi todo de mas... solo son prueba...
	// Activa el boton ok.
	//sprintf(str1,"S0030000FC\nS22406000000060000000E0C00000E3C000004380000041800000D2C00300D2E01300E0C0321\nS224060020180F380E0F1F3F3C073F3F38073F3F30033F3F28013F3F00013F3F20033F3F30B5\nS22406004007352F38013E1F20013F3F2001333B200007380000073800000F3C00001F1E0040\nS224060060000618000006180000061800000618000002100000033000000130000000200067\nS804000000FB\n");
	memcpy(str1,msg.buttonOff,msg.largoButtonOff);

	for (i=1; i < 5; i++)
	{
		// Piso el nro de boton a activar;
		str1[2] = i + '0';
		nLeft = msg.largoButtonOff;
		idx = 0;

		while(nLeft > 0)
		{
			ret = send(sock, &str1[idx], nLeft, 0);
			if (ret == 0)
				break;
			else if (ret == SOCKET_ERROR)
			{
				break;
			}
			nLeft -= ret;
			idx += ret;
		}
	}

/*	S0030000FC
S22406000000060000000E0C00000E3C000004380000041800000D2C00300D2E01300E0C0321
S224060020180F380E0F1F3F3C073F3F38073F3F30033F3F28013F3F00013F3F20033F3F30B5
S22406004007352F38013E1F20013F3F2001333B200007380000073800000F3C00001F1E0040
S224060060000618000006180000061800000618000002100000033000000130000000200067
S804000000FB*/

}

void cargoMensajesBotones(char nro, struct msgBoton *msgBotonera)
{
	char str[sizeof(msgBotonera->msg)];
	FILE *fp;

	if ((fp=fopen("config.ini","r"))==NULL)
		return;

	fgets(str,sizeof(msgBotonera->msg),fp);
	while (!feof(fp)) {
		if ( str[0] == nro ) {
			strcpy(msgBotonera->msg,str+2);
			msgBotonera->largoMsg = strlen(msgBotonera->msg);
			fclose(fp);
			return;
		}
		fgets(str,sizeof(msgBotonera->msg),fp);
	}

	fclose(fp);
}

void activarBotones(SOCKET sock)
{
	char str1[400];
	int nLeft;
    int idx;
	int ret;
	unsigned int i;
	memset(str1,0x0,sizeof(str1));
	memset(&msgBotones,0x0,sizeof(msgBotones));

	// Esta casi todo de mas... solo son prueba...
	// Activa el boton ok.
	//sprintf(str1,"S0030000FC\nS22406000000060000000E0C00000E3C000004380000041800000D2C00300D2E01300E0C0321\nS224060020180F380E0F1F3F3C073F3F38073F3F30033F3F28013F3F00013F3F20033F3F30B5\nS22406004007352F38013E1F20013F3F2001333B200007380000073800000F3C00001F1E0040\nS224060060000618000006180000061800000618000002100000033000000130000000200067\nS804000000FB\n");
	memcpy(str1,msg.button,msg.largoButton);

	for (i=0; i < strlen(botones); i++)
	{
		// Inicializo los mensajes
		// a ver.. voy a explicar esto un poco.. porque la compliqué de verdad.
		// en botones[i] tengo el nro de boton a configurar, entonces le resto 1, para poder setear el mensaje
		// en la estructura msgBotones, en su correspondiente indice. Por ejemplo:
		// Para el boton nro 3, la posicion en la estructura msgBotones es la 2.
		// Así llamamos a msgBotones[2].
		// Luis Soria, el heredero del MISMISIMO DILBERT.

		strcpy(msgBotones[botones[i]-'1'].msg,"SIN CONFIGURACION  ");
		msgBotones[botones[i]-'1'].largoMsg = strlen(msgBotones[botones[i]-'1'].msg);

		cargoMensajesBotones(botones[i],&msgBotones[botones[i]-'1']);

		// Piso el nro de boton a activar;
		str1[2] = botones[i];
		nLeft = msg.largoButton;
		idx = 0;

		while(nLeft > 0)
		{
			ret = send(sock, &str1[idx], nLeft, 0);
			if (ret == 0)
				break;
			else if (ret == SOCKET_ERROR)
			{
				break;
			}
			nLeft -= ret;
			idx += ret;
		}
	}

/*	S0030000FC
S22406000000060000000E0C00000E3C000004380000041800000D2C00300D2E01300E0C0321
S224060020180F380E0F1F3F3C073F3F38073F3F30033F3F28013F3F00013F3F20033F3F30B5
S22406004007352F38013E1F20013F3F2001333B200007380000073800000F3C00001F1E0040
S224060060000618000006180000061800000618000002100000033000000130000000200067
S804000000FB*/


}

void setMsg(int tipo)
{
	/*	Dado el tipo de verificador, copiar al struct msg cada campo.
	*	¿A qué se refiere cada secuencia de hexadecimales?
	*/
	int i;
	switch(tipo)
	{
		case 0: //SG15
		case 2: //NQUIRE
			//msg->header[0] = {0x1b,0x42,0x30,0x1b,0x24,0x1b,0x2e,0x31}; // + ean
			//char middle[] = {0x3,0x1b,0x42,0x30,0x1b,0x2e,0x34}; // + descripcion
			//char middle1[] = {0x3,0x1b,0x42,0x31,0x1b,0x2e,0x37}; // + precio
			//char middle2[] = {0x3,0x1b,0x42,0x30,0x1b,0x2e,0x37}; // Auxiliar para ean no encontrados
			//char end[] = {0x3};
			/*	Se abren corchetes para que el string aux salga del scope cada vez
			*/
			{
				char aux[]= {0x1b,0x42,0x30,0x1b,0x24,0x1b,0x2e,0x31,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.header[i] = aux[i];
				}
				msg.largoHeader=i;
			}
			{
				char aux[]= {0x3,0x1b,0x42,0x30,0x1b,0x2e,0x34,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.description[i] = aux[i];
				}
				msg.largoDescription=i;
			}
			{
				char aux[]= {0x3,0x1b,0x42,0x30,0x1b,0x2e,0x34,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.description2[i] = aux[i];
				}
				msg.largoDescription2=i;
			}
			{
				char aux[]= {0x3,0x1b,0x42,0x31,0x1b,0x2e,0x37,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.price[i] = aux[i];
				}
				msg.largoPrice=i;
			}
			{
				char aux[]= {0x3,0x1b,0x42,0x30,0x1b,0x2e,0x37,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.descriptionNotFound[i] = aux[i];
				}
				msg.largoDescriptionNotFound=i;
			}
			{
				char aux[]= {0x3,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.end[i] = aux[i];
				}
				msg.largoEnd=i;
			}
			break;
		case 1: // SG20
			/*char header[] = {0x1b,0x25,0x1b,0x27,0x30,0x30,0x1b,0x3d,0x31,0x31,0x30,0x30}; // + ean
			char description[] = {0x1b,0x2a,0x1b,0x2a,0x1b,0x2d,0x1b,0x3d,0x31,0x30,0x30,0x30}; // + descripcion
			char description2[] = {0x1b,0x2a,0x1b,0x2d}; // + descripcion2
			char descriptionNotFound[] = {0x1b,0x2a,0x1b,0x2a,0x1b,0x2d,0x1b,0x3d,0x31,0x30,0x30,0x30}; // Auxiliar para ean no encontrados
			char price[] = {0x1b,0x2a,0x1b,0x2d,0x1b,0x3d,0x32,0x30,0x31,0x30}; // + precio
			char end[] = {0x1b,0x3d,0x31,0x30,0x30,0x30};*/
			{
				char aux[]= {0x1b,0x25,0x1b,0x27,0x30,0x30,0x1b,0x3d,0x31,0x31,0x30,0x30,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.header[i] = aux[i];
				}
				msg.largoHeader=i;
			}
			{
				char aux[]= {0x1b,0x2a,0x1b,0x2a,0x1b,0x2d,0x1b,0x3d,0x31,0x30,0x30,0x30,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.description[i] = aux[i];
				}
				msg.largoDescription=i;
			}
			{
				char aux[]= {0x1b,0x2a,0x1b,0x2d,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.description2[i] = aux[i];
				}
				msg.largoDescription2=i;
			}
			{
				char aux[]= {0x1b,0x2a,0x1b,0x2d,0x1b,0x3d,0x32,0x30,0x31,0x30,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.price[i] = aux[i];
				}
				msg.largoPrice=i;
			}
			{
				char aux[]= {0x1b,0x2a,0x1b,0x2a,0x1b,0x2d,0x1b,0x3d,0x31,0x30,0x30,0x30,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.descriptionNotFound[i] = aux[i];
				}
				msg.largoDescriptionNotFound=i;
			}
			{
				char aux[]= {0x1b,0x3d,0x31,0x30,0x30,0x30,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.end[i] = aux[i];
				}
				msg.largoEnd=i;
			}
			{
				char aux[]= {0x1b,0x3b,0x31,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.button[i] = aux[i];
				}
				msg.largoButton=i;
			}
			{
				char aux[]= {0x1b,0x3c,0x31,'\0'};
				for (i=0; i < aux[i] != '\0'; i++) {
					msg.buttonOff[i] = aux[i];
				}
				msg.largoButtonOff=i;
			}
			break;
	}
}
  
void ValidateArgs(int argc, char **argv)
{
    int i;
	
    for(i = 1; i < argc; i++)
    {
        if ((argv[i][0] == '-') || (argv[i][0] == '/'))
        {
            switch (tolower(argv[i][1]))
            {
                case 'p':
                    iPort = atoi(&argv[i][3]);
                    break;
                case 'x':
                    strcpy(plufile, &argv[i][3]);
                    break;
				case 'v':
                    strcpy(modeloVerif, &argv[i][3]);
                    break;
				case 'b':
                    strcpy(botones, &argv[i][3]);
                    break;
				case 's':
                    strcpy(sucursal, &argv[i][3]);
                    break;
                default:
                    usage();
                    break;
            }
        }
    }
	if ( i < 5 ) {
		usage();
	}
	if (setTipoVerif(modeloVerif) == -1)
		usage();
	//Valido que los botones a activar sean del 1 al 4
	for (i=0; (unsigned) i < strlen(botones); i++)
	{
		if (botones[i] != '1' && botones[i] != '2' && botones[i] != '3' && botones[i] != '4')
			usage();
	}
	// Flujo de datos a enviar / protocolo ascii
	setMsg(tipoVerif);
	
}

int setTipoVerif(char * data)
{
	tipoVerif=-1;
	if (strcmp("SG15", data)==0) tipoVerif=0;
	if (strcmp("SG20", data)==0) tipoVerif=1;
	if (strcmp("NQUIRE", data)==0) tipoVerif=2;
	
	return tipoVerif;

}

int buscoEnter(char * str, int largo)
{
	int i;
	for (i=0; i < largo; i++)
	{
		if ( str[i] == 13 ) {
			return 0;
		}
	}
	return 1;
}

void accionBoton(SOCKET sock,char *data)
{
	char str1[400];
	int nLeft;
    int idx;
	int ret;

	memset(str1,0x0,sizeof(str1));
	
	memset(str1,0x1b,1);
	memset(str1+1,0x25,1);

	//Formateo el mensaje. En data[3] viene el mensaje para el boton presionado.
	memcpy(str1+2,msg.description,msg.largoDescription);
	memcpy(str1+2+msg.largoDescription,msgBotones[data[3]-'1'].msg, msgBotones[data[3]-'1'].largoMsg);
	//memcpy(str1+2+msg.largoDescription+msgBotones[data[3]-'1'].largoMsg,msg.description2, msg.largoDescription2);
	//memcpy(str1+2+msg.largoDescription+msgBotones[data[3]-'1'].largoMsg+msg.largoDescription2,"LUIS SORIA", 10);
	memcpy(str1+2+msg.largoDescription+msgBotones[data[3]-'1'].largoMsg,msg.end, msg.largoEnd);

	nLeft = 2+msgBotones[data[3]-'1'].largoMsg+msg.largoDescription+msg.largoEnd;
	idx = 0;

	while(nLeft > 0)
	{
		ret = send(sock, &str1[idx], nLeft, 0);
		if (ret == 0)
			break;
		else if (ret == SOCKET_ERROR)
		{
			break;
		}
		nLeft -= ret;
		idx += ret;
	}
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET        sock=(SOCKET)lpParam;
    char          szBuffXml[DEFAULT_BUFFER];
	char          szBuff[DEFAULT_BUFFER];
	char          bufferAux[DEFAULT_BUFFER];
    int           read_error,
                  nLeft,
                  idx,
				  totalBytes,
				  bytes,
				  ret;
	char		txtDeb[300];
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(addr);
	int res;
	char clientip[20];
 
	if (tipoVerif == 1) {
		// Desactivo todos los botones
		desactivarBotones(sock);
		// Activo los botones requeridos.
		activarBotones(sock);
	}

    while(1)
    {
		/*****************/
		/* lee datos TCP */
		/*****************/
		memset(szBuffXml,0x0,DEFAULT_BUFFER);
		memset(szBuff,0x0,DEFAULT_BUFFER);

		totalBytes=0;
		do {					
			bytes = recv(sock, bufferAux, DEFAULT_BUFFER, 0);
		
			if (bytes == 0) {
				sprintf(txtDeb, "ERROR: recepcion TCP.\r\n");
				print(txtDeb);
				read_error = 1;
				break;
			}

			if (bytes == SOCKET_ERROR) {
				printf("recv failed: %d\n", WSAGetLastError());
				read_error = 1;
				break;
			}

			/*sprintf(msg,"Log del paquete a recibido \n");
				for (ppp=0;ppp<80;ppp++) {
					if ( (ppp % 8) == 0 ) 
						strcat(msg,"\n");
					sprintf(aux,"%02.02X ", bufferAux[ppp]);
					strcat(msg,aux);
				}
				strcat(msg,"\n");
				dispmsg(msg);
					*/
			memcpy(szBuffXml + totalBytes, bufferAux, bytes);
			totalBytes+=bytes;
		} while (buscoEnter(szBuffXml,totalBytes)!=0);
		
		/*ret = recv(sock, szBuffXml, DEFAULT_BUFFER, 0);
        if (ret == 0)
            break;
        else if (ret == SOCKET_ERROR)
        {
            sprintf(txt,"recv() ha fallado: %d\n", WSAGetLastError());
			print(txt);
			break;
        }
        szBuff[ret] = '\0';*/
		sprintf(txtDeb,"Lectura Socket sock=%d, Bytes=%d \r\n",sock,totalBytes);
		print(txtDeb);

		if ( read_error == 1 )
			break;

		// Si presionaron un boton, respondo un mensaje y continuo esperando datos.
		if (strncmp(szBuffXml,"BUT",3)==0) {
			accionBoton(sock,szBuffXml);
			continue;
		}
        
		/********************/
		/* Formatea mensaje */
		/********************/

		res = getpeername(sock, (struct sockaddr *)&addr, &addr_size);
		strcpy(clientip,inet_ntoa(addr.sin_addr));

		if ( eanSearch(szBuffXml, szBuff, clientip) < 0 ) {
			Sleep(5000);
			if ( eanSearch(szBuffXml, szBuff, clientip) < 0 ) {
				break;
			}
		}
		
		/*print("Log del paquete a enviar \n");
		for (ppp=0;ppp<80;ppp++) {
			if ( (ppp % 8) == 0 ) 
				print("\n");
			sprintf(txt,"%02.02X ", szBuff[ppp]);
			print(txt);
		}*/
		print("\n");
		print("\n /***********************************/ \n");

        if (!bRecvOnly)
        {
            nLeft = sizeof(szBuff);
            idx = 0;

            while(nLeft > 0)
            {
                ret = send(sock, &szBuff[idx], nLeft, 0);
				sprintf(txtDeb,"\n finalizo el send con %d \n", ret);
				print(txtDeb);
                if (ret == 0)
                    break;
                else if (ret == SOCKET_ERROR)
                {
                    sprintf(txtDeb,"send() ha fallado: %d\n", 
                        WSAGetLastError());
					print(txtDeb);
                    break;
                }
                nLeft -= ret;
                idx += ret;
			}
        }
    }
    return 0;
}


int main(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        sListen,
                  sClient;
    int           iAddrSize;
    HANDLE        hThread;
    DWORD         dwThreadId;
    struct sockaddr_in local,
                       client;

	if ( (debug=fopen("prcheck.log","a")) == NULL )
	{
		printf("1) Error al crear archivo de log\n");
	}
	fclose(debug);

	if (stat("prcheck.log", &tam) == 0)
	{
		if (tam.st_size > 600000) {
			
			if ( (debug=fopen("prcheck.log","w")) == NULL )
			{
				printf("2) Error al crear archivo de log en cero\n");
			}
		} else {
			if ( (debug=fopen("prcheck.log","a")) == NULL )
			{
				printf("3) Error al crear archivo de log\n");
			}
		}
	}

    ValidateArgs(argc, argv);

	sprintf(txt,"Interfase Hasar Sistemas SRL Ver.:%s\n",ver);
	print(txt);
	print("***************************************\n");
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        print("Fallo al cargar Winsock!\n");
        return 1;
    }
	sprintf(txt,"Puerto:%d - Path del Plu:%s\n\n",iPort, plufile);
	print(txt);
    
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sListen == SOCKET_ERROR)
    {
        sprintf(txt,"socket() ha fallado: %d\n", WSAGetLastError());
		print(txt);
        return 1;
    }

    if (bInterface)
    {
		local.sin_addr.s_addr = inet_addr(szAddress);
        if (local.sin_addr.s_addr == INADDR_NONE)
            usage();
    }
    else
        local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons((u_short)iPort);

    if (bind(sListen, (struct sockaddr *)&local, 
            sizeof(local)) == SOCKET_ERROR)
    {
        sprintf(txt,"bind() ha fallado: %d\n", WSAGetLastError());
		print(txt);
        return 1;
    }
    listen(sListen, 15);
 

    while (1)
    {
        iAddrSize = sizeof(client);
        sClient = accept(sListen, (struct sockaddr *)&client,
                        &iAddrSize);
        sprintf(txt,"Modelo PRCHECK = %s - Conexion TCP address=%s\n", modeloVerif, inet_ntoa(client.sin_addr));
		print(txt);

        if (sClient == INVALID_SOCKET)
        {        
            sprintf(txt,"accept() ha fallado: %d\n", WSAGetLastError());
			print(txt);
            break;
        }
        sprintf(txt,"Cliente aceptado: %s:%d\n\n", 
            inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		print(txt);

        hThread = CreateThread(NULL, 0, ClientThread, 
                    (LPVOID)sClient, 0, &dwThreadId);
        if (hThread == NULL)
        {
            sprintf(txt,"CreateThread() ha fallado: %d\n", GetLastError());
			print(txt);
            break;
        }
        CloseHandle(hThread);
    }
    closesocket(sListen);
    
    WSACleanup();
	return 0;
}

int eanSearch(char * msg_aux, char * buff, char * dirIp)
{
	char precio[20];
	char price_combo[20];
	char ean[150];
	char desc[50];
	char aux[DEFAULT_BUFFER];
	char eanFinal[DEFAULT_BUFFER];
	unsigned char aux_asc[350];
	char aux_1[150];
	unsigned int ppp;
	int pos;
	int flag=0;
	int flag_desc;
	int flag_price;
	int cont=0;
	char aux_desc[20];
	int efe=0;
	int pipes=0;
	int init=15;
	char precioStr[100];
	char price_combo_str[100];
	
	memset(buff,0x0,DEFAULT_BUFFER);
	memset(precio,0x0,20);
	memset(price_combo, 0x0, 20);
	memset(ean,0x0,150);
	memset(desc,0x0,50);
	memset(aux,0x0,DEFAULT_BUFFER);
	memset(eanFinal,0x0,DEFAULT_BUFFER);
	memset(aux_asc,0x0,350);
	memset(aux_1,0x0,150);
	memset(aux_desc,0x0,20);
	
    for (ppp=0;ppp<strlen(msg_aux);ppp++) {
        if ( msg_aux[ppp] < '0' || msg_aux[ppp] > '9' ) {
            efe++;
            continue;
        }
        aux[ppp - efe]=msg_aux[ppp];
    }

	 sprintf(txt,"aux ean %s\n", aux);
    print(txt);
    // le pongo el fin, en el DIGITO VERIFICADOR
    aux[ppp - (efe + 1)]='\0';
    sprintf(txt,"nulo aux ean %s\n", aux);
    print(txt);
    if ( strlen(aux) < 12 ) {
        strncpy(eanFinal,"000000000000",12-strlen(aux));
        strcat(eanFinal,aux);
    } else
        strcpy(eanFinal,aux);

    strcpy(aux,eanFinal);

	sprintf(txt,"busco ean %s\n", aux);
	print(txt);

	if ( isOpen == 1 ) {
		return -1;
	}

	if ( (fp=fopen(plufile,"r")) == NULL ) {
		print("Error al abrir archivo de articulos\n");
		return -1;
	}
	isOpen=1;

	print("OK Abriendo archivo de articulos\n");

	fgets(aux_asc,100,fp);
		//(fp,"%s%s",aux_asc);
	
	while(!(feof(fp))) {
	
		pos=0;
		pipes=0;
		for (ppp=0; ppp < strlen(aux_asc); ppp++) {
			if ( ppp == 0 && aux_asc[ppp] == '8' ) flag=1;
			
			if ( aux_asc[ppp] == '|' && pipes == 0) {
				pipes++;
				ppp++;
			}

			if ( pipes < 1 )
				continue;

			if ( aux_asc[ppp] == '|' ) {
				init=ppp+1;
				break;
			}

			ean[pos]=aux_asc[ppp];
			pos++;
		}
		ean[pos]='\0';
		
		//Si lei un Link, voy al principio del archivo y comienzo a leer otra vez.
		if ( !(strncmp(aux,ean,12)) &&  flag == 1 ) {
			pos=0;
			for (ppp=init; ppp < strlen(aux_asc); ppp++) {
				if ( aux_asc[ppp] == '|' ) {
					break;
				}
				ean[pos]=aux_asc[ppp];
				pos++;
			}
			ean[pos]='\0';
			strncpy(aux,ean,12);
			rewind(fp);
			fgets(aux_asc,100,fp);
			flag=0;
			continue;
		} else {
			flag=0;
		}

		if ( !(strncmp(aux,ean,12)) ){
				
			sprintf(txt,"Encontre ean |%s|\n", ean);
			print(txt);

			pos=0;
			flag_desc=0;
			flag_price = 0;

			printf("\n%s\n", aux_asc);

			//Aux_asc = Linea de plu no completa, sino q descripcion en adelante, los ultimos 4
			for (ppp=init; ppp < strlen(aux_asc); ppp++) {
				if ( cont >= 2 ) { // Se saltan las dos columnas previas a la descripción
					if (aux_asc[ppp] == '|' && flag_desc == 0 && flag_price == 0) {
							desc[pos]='\0'; // Marcar final del texto de descripción
							printf("Descripcion: %s\n", desc);
							pos=0;
							flag_desc=1; // Encontró descripción
							continue; // Continuamos a buscar el precio
					}

					printf("El caracter es: %c, Flag_desc: %i, Flag_price: %i, Cont: %i \n", aux_asc[ppp], flag_desc, flag_price, cont);
					
					if (aux_asc[ppp] == '|' && flag_desc == 1 && flag_price == 0) { // Ya encontró la descripción, entonces sigue precio	
							precio[pos]='\0';
							printf("Precio: %s\n", precio);
							flag_price = 1;
							pos = 0;
							continue; // Fin
					}

					if (aux_asc[ppp] == '|' && flag_desc == 1 && flag_price == 1 && cont > 2) {
						price_combo[pos] = '\0';
						printf("Precio Oferta: %s\n", price_combo);
						printf("\nSaliendo\n");
						break;
					}

					if (aux_asc[ppp] != '|' && flag_desc == 0 && flag_price == 0) // Copiamos en descripción, porque todavía no se encontró (flag en 0)
						desc[pos] = aux_asc[ppp];
					if (aux_asc[ppp] != '|' && flag_desc == 1 && flag_price == 0) // Copiamos en precio, porque ya se encontró descripción (flag en 1)
						precio[pos] = aux_asc[ppp];
					if (aux_asc[ppp] != '|' && flag_desc == 1 && flag_price == 1 && cont >= 3)
						price_combo[pos] = aux_asc[ppp];
					
					pos++;
				}
				
				if (aux_asc[ppp] == '|')
				{
					cont++;
				}

			}


			sprintf(txt,"Encontre precio |%.2f|\n", atof(precio)/100);
			sprintf(precioStr,"%d%02d", atoi(precio)/100,atoi(precio)%100);
			print(txt);

			sprintf(txt, "Encontre precio combo |%.2f|\n", atof(price_combo));
			print(txt);

			sprintf(aux_1,"$ %.2f", atof(precio)/100);

			sprintf(txt,"Encontre desc |%s|\n", desc);
			print(txt);

			strcpy(aux_desc,"");
			if (tipoVerif != 2) // Si no es NQUIRE.
				strcpy(aux_desc,"Descripcion:");


			if ( !(strncmp(aux,ean,strlen(ean))) ) {
				sprintf(txt,"Encontre ean %s con |%s|\n", ean, desc);
				print(txt);

				sprintf(txt,"%s|%s|E|%s|000|%s",ean,dirIp,precioStr,sucursal);
				registro(txt);

				strncat(buff, msg.header, msg.largoHeader);
				strncat(buff + msg.largoHeader, ean, strlen(ean));
				strncat(buff + msg.largoHeader + strlen(ean), msg.description, msg.largoDescription);
				strncat(buff + msg.largoHeader + msg.largoDescription + strlen(ean), aux_desc, strlen(aux_desc));
				strncat(buff + msg.largoHeader + msg.largoDescription + strlen(aux_desc) + strlen(ean), msg.description2, msg.largoDescription2);
				strncat(buff + msg.largoHeader + msg.largoDescription + msg.largoDescription2 + strlen(aux_desc) + strlen(ean), desc, strlen(desc));
				strncat(buff + msg.largoHeader + msg.largoDescription + msg.largoDescription2 + strlen(aux_desc) + strlen(ean) + strlen(desc), msg.price, msg.largoPrice);
				strncat(buff + msg.largoHeader + msg.largoDescription + msg.largoDescription2 + strlen(aux_desc) + msg.largoPrice + strlen(ean) + strlen(desc), aux_1, strlen(aux_1));
				strncat(buff + msg.largoHeader + msg.largoDescription + msg.largoDescription2 + strlen(aux_desc) + msg.largoPrice + strlen(aux_1) + strlen(ean) + strlen(desc), msg.end, msg.largoEnd);
				
				/*strncat(buff, msg.header, 8);
				strncat(buff + 8, ean, strlen(ean));
				strncat(buff + 8 + strlen(ean), msg.description, 7);
				strncat(buff + 15 + strlen(ean), desc, strlen(desc));
				strncat(buff + 15 + strlen(ean) + strlen(desc), middle1, 7);
				strncat(buff + 22 + strlen(ean) + strlen(desc), aux_1, strlen(aux_1));
				strncat(buff + 22 + strlen(ean) + strlen(desc) + strlen(aux_1), end, 1);*/
					
				fclose(fp);
				isOpen=0;
				return 0;
			}
		}
		fgets(aux_asc,100,fp);
	}
	
	eanFinal[12] = '\0';
	sprintf(txt,"%s|%s|N|000|000|%s",eanFinal,dirIp,sucursal);
	registro(txt);

	print("\n \n No se encontro el registro buscado \n");
	memcpy(buff, msg.header, msg.largoHeader);
	memcpy(buff + msg.largoHeader, aux, 12);
	memcpy(buff + msg.largoHeader + 12, msg.descriptionNotFound, msg.largoDescriptionNotFound);
	memcpy(buff + msg.largoHeader + msg.largoDescriptionNotFound + 12, "EAN NO ENCONTRADO",17);
	memcpy(buff + msg.largoHeader + msg.largoDescriptionNotFound + 29, msg.description2, msg.largoDescription2);
	memcpy(buff + msg.largoHeader + msg.largoDescriptionNotFound + msg.largoDescription2 + 29, "CONSULTE EN CAJA",16);
	memcpy(buff + msg.largoHeader + msg.largoDescriptionNotFound + msg.largoDescription2 + 45, msg.end, msg.largoEnd);
	/*strncat(buff, header, 8);
	strncat(buff + 8, aux, 12);
	strncat(buff + 8 + 12, middle,7);
	strncat(buff + 15 + 12, "EAN NO ENCONTRADO",17);
	strncat(buff + 32 + 12, middle2,7);
	strncat(buff + 39 + 12, "CONSULTE EN CAJA",16);
	strncat(buff + 55 + 12, end,1);*/
				
	fclose(fp);
	isOpen=0;
	return 0;		
}
