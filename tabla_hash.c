#include <math.h>
#include <malloc.h>
#include "tabla_hash.h"
#include "hash.h"
#include "constantes.h"

TablaHash * crearTablaHash(ListaDePalabras * universo)
{
    TablaHash * tablaHash = armarTablaHash(universo->cantidad);
    ListaDePalabras ** gruposDePalabras = agruparPalabras(tablaHash, universo);

    crearBuckets(tablaHash, gruposDePalabras);

    return tablaHash;
}

TablaHash * armarTablaHash(int cantidadDePalabras)
{
    TablaHash * tablaHash = malloc(sizeof(TablaHash));
    tablaHash->tamano = calcularCantidadDeBuckets(cantidadDePalabras);
    tablaHash->buckets = malloc(sizeof(Bucket) * tablaHash->tamano);
}

void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras)
{
    for (int i=0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = crearBucket(*listasDePalabras[i]);
    }
}

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * palabras)
{
    ListaDePalabras ** gruposDePalabras = crearGruposDePalabras(tablaHash, palabras);
    Palabra * palabraActual;
    int palabraHasheada;

    for (int i = 0; i < palabras->cantidad; i++) {
        palabraActual = palabras->palabras[i];
        palabraHasheada = obtenerHashPrincipal(*tablaHash, *palabraActual);
        agregarPalabraALista(palabraActual, gruposDePalabras[palabraHasheada]);
    }

    return gruposDePalabras;
}

ListaDePalabras ** crearGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras)
{
    int elementosPorBucket = palabras->cantidad / tablaHash->tamano + 1;
    ListaDePalabras * listasDePalabras = malloc(sizeof(ListaDePalabras*) * tablaHash->tamano);

    for (int i=0; i < tablaHash->tamano; i++) {
        listasDePalabras[i] = crearListaDePalabras(elementosPorBucket);
    }

    return listaDePalabras;
}

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo)
{
    FILE * archivo = fopen(nombreDeArchivo, "w");

    fwprintf(archivo, L"%d\n", tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano; i++) {
        imprimirBucketEnArchivo(*tablaHash.buckets[i], archivo);
    }
}

int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra)
{
    return djb2(palabra) % tablaHash.tamano;
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return ceil(pow(cantidadDePalabras, POTENCIA_TAMANO_BUCKETS));
}
