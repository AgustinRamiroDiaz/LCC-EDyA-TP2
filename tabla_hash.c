#include <math.h>
#include <malloc.h>
#include "tabla_hash.h"
#include "hash.h"
#include "archivo.h"
#include "constantes.h"

#include <assert.h>

TablaHash * crearTablaHash(ListaDePalabras * universo)
{
    TablaHash * tablaHash = armarTablaHash(universo->cantidad);
    ListaDePalabras ** gruposDePalabras = agruparPalabras(tablaHash, universo);

    crearBuckets(tablaHash, gruposDePalabras);
    liberarGruposDePalabras(gruposDePalabras, tablaHash->tamano);

    return tablaHash;
}

TablaHash * armarTablaHash(int cantidadDePalabras)
{
    TablaHash * tablaHash = malloc(sizeof(TablaHash));
    tablaHash->tamano = calcularCantidadDeBuckets(cantidadDePalabras);
    tablaHash->buckets = malloc(sizeof(Bucket) * tablaHash->tamano);
    tablaHash->cantidadDeElementos = cantidadDePalabras;

    return tablaHash;
}

void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras)
{
    for (int i=0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = crearBucket(*gruposDePalabras[i]);
    }
}

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * listaDePalabras)
{
    Palabra * palabraActual;
    int numeroDeBucket;
    ListaDePalabras ** gruposDePalabras = armarGruposDePalabras(tablaHash, listaDePalabras);

    for (int i = 0; i < listaDePalabras->cantidad; i++) {
        palabraActual = listaDePalabras->palabras[i];
        numeroDeBucket = obtenerHashPrincipal(*tablaHash, *palabraActual);
        agregarPalabraALista(palabraActual, gruposDePalabras[numeroDeBucket]);
    }

    return gruposDePalabras;
}

ListaDePalabras ** armarGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras)
{
    int elementosPorBucket = ceil((float) palabras->cantidad / (float) tablaHash->tamano);
    ListaDePalabras ** gruposDePalabras = malloc(sizeof(ListaDePalabras*) * tablaHash->tamano);

    for (int i=0; i < tablaHash->tamano; i++) {
        gruposDePalabras[i] = armarListaDePalabras(elementosPorBucket);
    }

    return gruposDePalabras;
}

void imprimirTablaHashEnArchivo(TablaHash tablaHash, FILE * archivoDeTablaHash)
{
    fwprintf(archivoDeTablaHash, L"%d %d\n", tablaHash.cantidadDeElementos, tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano; i++) {
        imprimirBucketEnArchivo(*tablaHash.buckets[i], archivoDeTablaHash);
    }
}

int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra)
{
    return funcionHashPrincipal(palabra) % tablaHash.tamano;
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return ceil((float) cantidadDePalabras / (float) ELEMENTOS_POR_BUCKET);
}

int verificarTablaHash(TablaHash tablaHash, ListaDePalabras universo)
{
    int numeroDeBucket, esValida = 1;

    if (tablaHash.cantidadDeElementos != universo.cantidad){
        return 0;
    }

    Palabra * palabraActual;

    for (int i = 0; i < universo.cantidad && esValida; i++) {
        palabraActual = universo.palabras[i];
        esValida = palabraEnTablaHash(tablaHash, *palabraActual);
    }

    return esValida;
}

int palabraEnTablaHash(TablaHash tablaHash, Palabra palabra)
{
    int numeroDeBucket = obtenerHashPrincipal(tablaHash, palabra);
    Bucket * bucket = tablaHash.buckets[numeroDeBucket];

    if (0 == bucket->capacidad) {
        return 0;
    }

    int posicionEnBucket = obtenerHashSecundario(*bucket, palabra);
    Palabra * palabraEncontrada = bucket->tablaHash[posicionEnBucket];

    return sonPalabrasIguales(palabra, *palabraEncontrada);
}

TablaHash * cargarTablaHashDesdeArchivo(FILE * archivoDeTablaHash)
{
    int cantidadDeElementos, cantidadDeBuckets;

    fwscanf(archivoDeTablaHash, L"%d %d", &cantidadDeElementos ,&cantidadDeBuckets);
    TablaHash * tablaHash = armarTablaHash(cantidadDeElementos);

    for (int i = 0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = cargarBucketDesdeArchivo(archivoDeTablaHash);
    }

    return tablaHash;
}

void liberarTablaHashYPalabras(TablaHash * tablaHash)
{
    for (int i = 0; i < tablaHash->tamano; i++) {
        liberarPalabrasEnBucket(tablaHash->buckets[i]);
    }
    liberarTablaHash(tablaHash);
}

void liberarTablaHash(TablaHash * tablaHash)
{
    for (int i = 0; i < tablaHash->tamano; i++) {
        liberarBucket(tablaHash->buckets[i]);
    }
    free(tablaHash->buckets);
    free(tablaHash);
}

void liberarGruposDePalabras(ListaDePalabras ** gruposDePalabras, int cantidad)
{
    for (int i = 0; i < cantidad; i++) {
        free(gruposDePalabras[i]->palabras);
        free(gruposDePalabras[i]);
    }
    free(gruposDePalabras);
}
