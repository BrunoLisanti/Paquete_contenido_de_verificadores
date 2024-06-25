//#include "pack1.h"
/* Codigos de ERROR retornados por el modulo que chequea la licencia */
#define LIC_NOLAN	-1	/* No tiene Lan */
#define LIC_NOFILE  -2	/* No Existe archivo lic.dat */
#define LIC_ERRNUM  -3  /* Numero de licencia incorrecto */

#define VERSION_NUMBER 7.00

/* Declaracion de funciones */
void SetFileLicencia(const char *LicFile);
void SetVersionNumber(const char *Version);

int check_licencia(unsigned long *serial_number);
int set_licencia(unsigned long number);

#define FALSE 0
#define TRUE 1
#define NIL -1
#define OK 0
#define ERROR -1
#define EQUAL	0
#define O_BINARY 0

//#include "unpack1.h"

