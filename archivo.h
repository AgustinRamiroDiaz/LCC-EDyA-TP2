#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <wchar.h>
#include <stdio.h>

//Abre el archivo con un modo dado
FILE * abrirArchivo(char * nombreDeArchivo, char * modo);

//Cierra el archivo
void cerrarArchivo(FILE * archivo);

//Retorna verdadero si el caracter es una letra y faslo si no
int esLetra(wchar_t caracter);

//Retorna verdadero si el caracter es a,b,...,z minuscula o mayuscula y faslo si no
int esLetraNormal(wchar_t caracter);

//Retorna verdadero si el caracter esta en "áéíóúäëïöüñ" y faslo si no
int esLetraEspecial(wchar_t caracter);

//Retorna verdadero si el caracter esta en " :;,.?!" o es fin de linea y faslo si no
int esFinDePalabra(wchar_t caracter);

//Retorna verdadero si el caracter es fin de linea y faslo si no
int esFinDeLinea(wchar_t caracter);

//Retorna verdadero si el caracter es '\0' y falso si no
int esFinDeArchivo(wchar_t caracter);

#endif
