#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <wchar.h>
#include <stdio.h>

//Abre el archivo con un modo dado
FILE * abrirArchivo(char * nombreDeArchivo, char * modo);

//Cierra el archivo
void cerrarArchivo(FILE * archivo);

//Retorna 1 si el caracter es una letra y 0 si no
int esLetra(wchar_t caracter);

//Retorna 1 si el caracter es a,b,...,z minuscula o mayuscula
int esLetraNormal(wchar_t caracter);

//Retorna 1 si el caracter esta en "áéíóúäëïöüñ"
int esLetraEspecial(wchar_t caracter);

int esFinDePalabra(wchar_t caracter);

int esFinDeLinea(wchar_t caracter);

int esFinDeArchivo(wchar_t caracter);

#endif
