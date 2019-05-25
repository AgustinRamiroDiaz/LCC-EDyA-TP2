#ifndef PALABRA
#define PALABRA

#include <wchar.h>

typedef struct {
    int longitud;
    wchar_t * letras;
} Palabra;

typedef struct {
    int cantidad, capacidad;
    Palabra ** palabras;
} ListaDePalabras;

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo);

ListaDePalabras * crearListaDePalabras(int cantidadDePalabras);

void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras);

void agrandarListaDePalabras(ListaDePalabras * listaDePalabras);

Palabra * crearPalabra(wchar_t * letras);

#endif
