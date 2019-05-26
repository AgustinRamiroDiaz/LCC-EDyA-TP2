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
    tablaHash->cantidadDeElementos = cantidadDePalabras;

    return tablaHash;
}

void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras)
{
    for (int i=0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = crearBucket(*gruposDePalabras[i]);
        //printf("%d: %ld\n", i, tablaHash->buckets[i]->constanteDeHasheo);
    }
}

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * listaDePalabras)
{
    Palabra * palabraActual;
    int numeroDeBucket;
    ListaDePalabras ** gruposDePalabras = crearGruposDePalabras(tablaHash, listaDePalabras);

    for (int i = 0; i < listaDePalabras->cantidad; i++) {
        palabraActual = listaDePalabras->palabras[i];
        numeroDeBucket = obtenerHashPrincipal(*tablaHash, *palabraActual);
        agregarPalabraALista(palabraActual, gruposDePalabras[numeroDeBucket]);
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

    fwprintf(archivo, L"%d %d\n", tablaHash.cantidadDeElementos, tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano; i++) {
        imprimirBucketEnArchivo(*tablaHash.buckets[i], archivo);
    }

    cerrarArchivo(archivo);
}

int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra)
{
    return funcionHashPrincipal(palabra) % tablaHash.tamano;
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return cantidadDePalabras / ELEMENTOS_POR_BUCKET + 1;
}

int verificarTablaHash(TablaHash tablaHash, ListaDePalabras universo)
{
    int numeroDeBucket, esValida = 1;
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

    int posicionEnBucket = obtenerHashSecundario(*bucket, palabra);
    Palabra * palabraEncontrada = bucket->tablaHash[posicionEnBucket];

    return sonPalabrasIguales(palabra, *palabraEncontrada);
}

TablaHash * cargarTablaHashDesdeArchivo(char * nombreDeArchivo)
{
    int cantidadDeElementos, cantidadDeBuckets;

    FILE * archivo = abrirArchivo(nombreDeArchivo, "r");
    fscanf(archivo, "%d %d", &cantidadDeElementos ,&cantidadDeBuckets);
    TablaHash * tablaHash = armarTablaHash(cantidadDeElementos);
    
    for (int i = 0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = cargarBucketDesdeArchivo(archivo);
        if (i==1) {
            printf("%ld %d\n", tablaHash->buckets[i]->constanteDeHasheo, tablaHash->buckets[i]->capacidad);
        }

    }

    cerrarArchivo(archivo);

    return tablaHash;
}