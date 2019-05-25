#ifndef PALABRA
#define PALABRA

#include <wchar.h>

typedef struct {
    int longitud;
    wchar_t * letras;
} Palabra;

typedef struct {
    int cantidad, capacidad;
    Palabra * palabras;
} ListaDePalabras;

ListaDePalabras * crearListaDePalabras(int cantidadDePalabras);

void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras);

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo);

Palabra * crearPalabra(wchar_t * letras);

#endif
