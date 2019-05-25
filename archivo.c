#include <stdio.h>
#include "archivo.h"

FILE * abrirArchivo(char * nombreDeArchivo, char * modo)
{
    return fopen(nombreDeArchivo, modo);
}

void cerrarArchivo(FILE * archivo)
{
    fclose(archivo);
}
