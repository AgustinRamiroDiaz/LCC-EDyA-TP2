#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <wchar.h>
#include <stdio.h>

FILE * abrirArchivo(char * nombreDeArchivo, char * modo);

void cerrarArchivo(FILE * archivo);

#endif
