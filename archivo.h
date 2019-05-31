#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <wchar.h>
#include <stdio.h>

FILE * abrirArchivo(char * nombreDeArchivo, char * modo);

void cerrarArchivo(FILE * archivo);

int esLetra(wchar_t caracter);

int esLetraNormal(wchar_t caracter);

int esLetraEspecial(wchar_t caracter);

int esFinDePalabra(wchar_t caracter);

int esFinDeLinea(wchar_t caracter);

int esFinDeArchivo(wchar_t caracter);

#endif
