#include <math.h>
#include <malloc.h>
#include "tabla_hash.h"
#include "hash.h"

TablaHash * crearTablaHash(ListaDePalabras * universo)
{
    TablaHash * tablaHash = armarTablaHash(universo->cantidad);
    int elementosPorBucket = universo->cantidad / tablaHash->tamano + 1;

    ListaDePalabras * listasDePalabras[tablaHash->tamano];
    for (int i=0; i < tablaHash->tamano; i++) {
        listasDePalabras[i] = crearListaDePalabras(elementosPorBucket);
    }

    Palabra * palabraActual;
    int palabraHasheada;

    for (int i = 0; i < universo->cantidad; i++) {
        palabraActual = universo->palabras[i];
        palabraHasheada = hashearPalabra(*palabraActual) % tablaHash->tamano;
        agregarPalabraALista(palabraActual, listasDePalabras[palabraHasheada]);
    }

    for (int i=0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = crearBucket(*listasDePalabras[i]);
    }

    return tablaHash;
}


TablaHash * armarTablaHash(int cantidadDePalabras)
{
    TablaHash * tablaHash = malloc(sizeof(TablaHash));
    tablaHash->tamano = calcularCantidadDeBuckets(cantidadDePalabras);
    tablaHash->buckets = malloc(sizeof(Bucket) * tablaHash->tamano);
}

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo)
{
    FILE * archivo = fopen(nombreDeArchivo, "w");

    fwprintf(archivo, L"%d\n", tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano; i++) {
        imprimirBucketEnArchivo(*tablaHash.buckets[i], archivo);
    }
}

unsigned long hashearPalabra(Palabra palabra)
{
    return djb2(palabra);
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return ceil(pow(cantidadDePalabras, 25.0/30.0));
}
