/****************************************************************************
*                                                                           *
*       Nombre del modulo:                             PROTECT.C	    	*
*                                                                           *
*       Descripcion del modulo:                                             *
*		Rutinas basicas para proteccion del ECR.Usa la direccion de ethernet*
*		como clave para generar el archivo de proteccion.					*
*       Version:                                               1.0          *
*                                                                           *
*       Autor:                          Omar Barneto                        *
*       Sector:                         Departamento de Desarrollo          *
*       Compania:                                   HASAR S.A.I.C.          *
*                                                                           *
*****************************************************************************/

// Estos .h estan incluidos aca para no ser alcanzados por 
// el pack(1) que se encuentra en mthasys.h
// Eduardo (16/04/2002)

#include <sys/stat.h>
#include <sys/types.h> 
#include <utime.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include <ifaddrs.h>
#include <linux/limits.h>
#include <sys/ioctl.h>
#include <net/if.h>

#ifdef MT
#include <mthasys.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
//#include <io.h>
//#include <dos.h>
#include <string.h>
//#include <process.h>
#include <math.h>
#include <stdlib.h>
//#include "sysdef.h"
#include "protect.h"
//#include "files.h"
//#include "version.h"
//#include "dmem.h"

// Archivo donde se registra el nro.de licencia 
#define LICENCIA	"lic.dat" 	

// Archivo donde se registran los numeros de los modulos del ECR habilitados 
#define LICENCIA1	"lic1.dat" 

// Archivo donde se registra el nro.con la cantidad de terminales habilitadas
#define LICENCIA2	"lic2.dat" 
											
#ifndef LINUX
#define ETHER_ADD_LEN   6
typedef unsigned char eth_address[ETHER_ADD_LEN+1];
#endif

static unsigned long get_licencia_number(unsigned long dir_eth);
static int get_ethaddr(void);
static int get_version(void);
static unsigned long addr; /* Direccion de ethernet convertida a long */
static void decrypt(char *s, int len);

static char *FileLicencia=NULL;
static char *VersionNumber=NULL;
// Setea el nombre del archivo donde se guarda la licencia
void SetFileLicencia(const char *LicFile)
{
	FileLicencia=(char *)LicFile;
}
// Setea el nro.de version. Version debe tener el formato X.XX
void SetVersionNumber(const char *Version)
{
	VersionNumber = (char *)Version;
}
const char *
GetLicenceFile (const char *LicFile)
{
	static char file_lic[100];
	char *Home = getenv ("HOME");

	sprintf (file_lic, "%s/%s", Home, (FileLicencia != NULL ) ? FileLicencia : LicFile);
	return file_lic;
}
// Retorna TRUE si el buffer pasado tiene size bytes en cero
static int IsNull(char *s,int size){
	int i;
	for(i=0 ; i < size ; i++,s++){
		if( *s != 0)
			return FALSE;
	}
	return TRUE;
}
/* Lee la direccion de ethernet y lo deja convertido a long en variable addr
   La direccion de ethernet convertida a long la multiplica por 1000 y le 
   suma el nro.de version de ecr expresado en 3 digitos(Ej:4.20 = 420)
*/
static int
get_ethaddr(void)
{	
	int fd,i;
	eth_address _eth_addr;
	int peso,error;
	int version;
	
	if( addr )
		return OK;	
	/* Llama al programa que me guarda la direccion de ethernet en el archivo
	   ethaddr */	
/*        
	if( (error=system("./tstlan ethaddr")) < 0 )
	{
		printf("Error ejecutando tstlan ethaddr\n");
		return ERROR;
	}
	if( (fd=open("ethaddr",O_RDONLY|O_BINARY)) < 0 )
	{
		printf("No pudo abrir archivo ethaddr\n");
		return ERROR;	
	}	
	
    int size = read(fd,_eth_addr,ETHER_ADD_LEN);

    printf("***Valor de read ethaddr %d, buff: [%2.2x] [%2.2x] [%2.2x]\n",size, _eth_addr[0],_eth_addr[1],_eth_addr[2] );
	close(fd);
*/    
    //Obtenemos la macadress 
    int s;
    struct ifreq ifr;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, "eth0");
    ioctl(s, SIOCGIFHWADDR, &ifr);
    for (int i=0; i<ETHER_ADD_LEN; i++)
    {
        _eth_addr[i] = (unsigned char)ifr.ifr_hwaddr.sa_data[i];
    }
    
    //printf("***Valor de ethaddr buff: [%2.2x] [%2.2x] [%2.2x]\n",_eth_addr[0],_eth_addr[1],_eth_addr[2] );
    
    /* Verifico que la direccion de MAC no sea cero */
	if( IsNull((char*)_eth_addr,ETHER_ADD_LEN) )
	{
		printf("No pudo encontrar MAC ADDRESS\n");
		return ERROR;
	}
	/* Devuelve el numero de version */
	version = get_version();
	
	/* Convierto el numero a long */
    printf("Imprimo la MAC Address: ");
	for(i=0,addr=0,peso=1 ; i < ETHER_ADD_LEN ; i++)
	{
        printf(" %x ",_eth_addr[i]);
		addr += (_eth_addr[i]*peso);
		peso *= 2;
	}
    printf("\n");
    
	/* Al valor obtenido lo afecto por el numero de version para que al 
	   actualizar la version se modifique el numero de serie y haya que
	   darle una nueva licencia */
	addr *= 1000;
	addr += version;   
	return OK;
}

/* Chequea si el programa fue licenciado correctamente.
   Si no lo fue deja en serial_number el numero de ethernet expresado 
   como long */
int check_licencia(unsigned long *serial_number)
{
	unsigned long licencia,number,n;
	int fd;
	const char *File = GetLicenceFile (LICENCIA);

	//printf("archivo de licencia: %s\n", File);
	*serial_number=0;
	if( get_ethaddr() == ERROR )
	{
		return LIC_NOLAN;
	}
	*serial_number = addr;
	
	if( (fd=open(File,O_RDWR|O_BINARY)) < 0 )
	{
		return LIC_NOFILE;
	}
	read(fd,&number,sizeof(number));

	close(fd);

	if( (n=get_licencia_number(addr)) != number)
	{
		return LIC_ERRNUM;
	}
	return OK;	
}

/* Guarda el nro.de licencia en archivo de licencia */
int set_licencia(unsigned long number)
{	
	int fd;
	const char *File = GetLicenceFile (LICENCIA);
	
	if( (fd=open(File,O_RDWR|O_BINARY|O_CREAT,0666)) < 0 )
	{
		return ERROR;
	}
	write(fd,&number,sizeof(number));
	
	close(fd);

	return OK;
}

/* Retorna el numero de licencia encriptado en base a la direccion de 
   ethernet */
unsigned long
get_licencia_number(unsigned long dir_eth)
{
	return (dir_eth*113*(~dir_eth));	
}
/* Retorna el numero de version del ECR */
static  int 
get_version(void)
{
	int n;
	char buf[10];

	// Si se le seteo un nro.de version retorna dicho numero
	if( VersionNumber != NULL )
		return atol(VersionNumber)*100;
	sprintf(buf,"%.0f",VERSION_NUMBER*100);
	n=atol(buf);
	return n;
}


