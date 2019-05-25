#ifndef TABLA_HASH
#define TABLA_HASH

#include "bucket.h"

typedef struct {
    int tamano;
    Bucket ** buckets;
} TablaHash;

TablaHash * crearTablaHash(ListaDePalabras * universo);

TablaHash * armarTablaHash(int cantidadDePalabras);

void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras);

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

ListaDePalabras ** crearGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo);

unsigned long hashearPalabra(Palabra palabra);

int calcularCantidadDeBuckets(int cantidadDePalabras);

#endif
