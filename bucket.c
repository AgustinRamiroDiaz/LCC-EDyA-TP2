#include <malloc.h>
#include <assert.h>
#include "bucket.h"
#include "hash.h"

Bucket * crearBucket(ListaDePalabras listaDePalabras)
{
    Bucket * bucket = armarBucket(listaDePalabras.cantidad);
    llenarBucket(bucket, listaDePalabras);

    return bucket;
}

Bucket * armarBucket(int cantidadDePalabras)
{
    Bucket * bucket = malloc(sizeof(Bucket));
    bucket->tamano = calcularTamanoDeBucket(cantidadDePalabras);
    bucket->tablaHash = malloc(sizeof(Palabra) * bucket->tamano);
    bucket->capacidad = cantidadDePalabras;

    return bucket;
}

int calcularTamanoDeBucket(int cantidadDePalabras)
{
    return cantidadDePalabras;
}

void llenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras)
{
    int esValida = 0;
    unsigned long constanteDeHasheo;

    for (constanteDeHasheo = 1; constanteDeHasheo && !esValida; constanteDeHasheo++) {
        esValida = probarLlenarBucket(bucket, listaDePalabras, constanteDeHasheo);

        if (esValida) {
            bucket->constanteDeHasheo = constanteDeHasheo;
            printf("%ld\n", constanteDeHasheo);
        }
    }

    assert(constanteDeHasheo);
}

int probarLlenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras, unsigned long constanteDeHasheo)
{
    int palabraHasheada, colisiona = 0;
    Palabra * palabraActual;
    vaciarBucket(bucket);

    for (int i = 0; i < listaDePalabras.cantidad && !colisiona; i++) {
        palabraActual = listaDePalabras.palabras[i];
        palabraHasheada = hashConConstante(*palabraActual, constanteDeHasheo) % bucket->tamano;

        if (bucket->tablaHash[palabraHasheada] == NULL) {
            bucket->tablaHash[palabraHasheada] = palabraActual;
        } else {
            colisiona = 1;
        }
    }

    return !colisiona;
}

void vaciarBucket(Bucket * bucket)
{
    for (int i=0; i < bucket->tamano; i++) {
        bucket->tablaHash[i] = NULL;
    }
}

void imprimirBucketEnArchivo(Bucket bucket, FILE * archivo)
{
    fwprintf(archivo, L"%d %d\n", bucket.constanteDeHasheo, bucket.capacidad);

    Palabra * palabraActual;
    for (int i = 0; i < bucket.tamano; i++) {
        palabraActual = bucket.tablaHash[i];

        if (palabraActual) {
            fwprintf(archivo, L"%d %ls\n", i, palabraActual->letras);
        }
    }
}
