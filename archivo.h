#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <wchar.h>
#include <stdio.h>

// Abre el archivo en el modo dado
FILE * abrirArchivo(char * nombreDeArchivo, char * modo);

// Cierra el archivo
void cerrarArchivo(FILE * archivo);

// Retorna verdadero si el caracter es una letra y falso si no
int esLetra(wchar_t caracter);

// Retorna verdadero si el caracter es una letra no especial y falso si no
int esLetraNormal(wchar_t caracter);

// Retorna verdadero si el caracter es una letra especial y falso si no
int esLetraEspecial(wchar_t caracter);

// Retorna verdadero si el caracter marca el fin de una palabra y falso si no
int esFinDePalabra(wchar_t caracter);

// Retorna verdadero si el caracter es el ultimo de la linea y falso si no
int esFinDeLinea(wchar_t caracter);

// Retorna verdadero si el caracter es WEOF y falso si no
int esFinDeArchivo(wchar_t caracter);

#endif
