#ifndef TABLA_HASH
#define TABLA_HASH

#include "bucket.h"

typedef struct {
    int tamano;
    int cantidadDeElementos;
    Bucket ** buckets;
} TablaHash;

TablaHash * crearTablaHash(ListaDePalabras * universo);

TablaHash * armarTablaHash(int cantidadDePalabras);

void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras);

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

ListaDePalabras ** crearGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo);

int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra);

int calcularCantidadDeBuckets(int cantidadDePalabras);

int verificarTablaHash(TablaHash tablaHash, ListaDePalabras universo);

int palabraEnTablaHash(TablaHash tablaHash, Palabra palabra);

TablaHash * cargarTablaHashDesdeArchivo(char * nombreDeArchivo);

ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash);

void sugerirOLiberar(TablaHash tablaHash, Palabra * palabra, ListaDePalabras * listaDeSugerencias);

void corregirArchivo(FILE * archivo, TablaHash tablaHash);

void imprimirSugerencias(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias);


#endif
