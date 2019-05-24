#include <math.h>
#include "tabla_hash.h"

TablaHash * crearTablaHash(ListaDePalabras * universo)
{
    TablaHash * tablaHash = armarTablaHash(universo->cantidad);

    ListaDePalabras * listasDePalabras[tablaHash->tamano];
    for (int i=0; i < tablaHash->tamano; i++) {
        listasDePalabras[i] = crearListaDePalabras(universo->cantidad / tablaHash->cantidad + 1);
    }

    Palabra * palabraActual;
    int palabraHasheada;

    for (int i = 0; i < universo->cantidad; i++) {
        palabraActual = universo->palabras[i];
        palabraHasheada = hashearPalabra(palabraActual) % tablaHash->tamano;
        agregarPalabraALista(palabraActual, listasDePalabras[palabraHasheada]);
    }

    for (int i=0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = crearBucket(listasDePalabras[i]);
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

    fwprintf(L"%d\n", tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano, i++) {
        imprimirBucketEnArchivo(tablaHash.buckets[i], archivo);
    }
}

unsigned long hashearPalabra(Palabra palabra)
{
    return djb2(palabra);
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return ceil(sqrt((float) cantidadPalabras));
}
