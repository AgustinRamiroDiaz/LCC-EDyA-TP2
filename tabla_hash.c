#include <math.h>
#include <malloc.h>
#include "tabla_hash.h"
#include "hash.h"
#include "archivo.h"
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
        tablaHash->buckets[i] = crearBucket(*gruposDePalabras[i]);
    }
}

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * listaDePalabras)
{
    Palabra * palabraActual;
    int palabraHasheada;
    ListaDePalabras ** gruposDePalabras = crearGruposDePalabras(tablaHash, listaDePalabras);

    for (int i = 0; i < listaDePalabras->cantidad; i++) {
        palabraActual = listaDePalabras->palabras[i];
        palabraHasheada = obtenerHashPrincipal(*tablaHash, *palabraActual);
        agregarPalabraALista(palabraActual, gruposDePalabras[palabraHasheada]);
    }

    return gruposDePalabras;
}

ListaDePalabras ** crearGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras)
{
    int elementosPorBucket = palabras->cantidad / tablaHash->tamano + 1;
    ListaDePalabras ** gruposDePalabras = malloc(sizeof(ListaDePalabras*) * tablaHash->tamano);

    for (int i=0; i < tablaHash->tamano; i++) {
        gruposDePalabras[i] = crearListaDePalabras(elementosPorBucket);
    }

    return gruposDePalabras;
}

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo)
{
    FILE * archivo = abrirArchivo(nombreDeArchivo, "w");

    fwprintf(archivo, L"%d\n", tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano; i++) {
        imprimirBucketEnArchivo(*tablaHash.buckets[i], archivo);
    }

    cerrarArchivo(archivo);
}

int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra)
{
    return djb2(palabra) % tablaHash.tamano;
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return ceil(pow(cantidadDePalabras, POTENCIA_TAMANO_BUCKETS));
}
