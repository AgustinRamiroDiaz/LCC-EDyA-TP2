#include <malloc.h>
#include <assert.h>
#include "bucket.h"
#include "hash.h"
#include "constantes.h"

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
    bucket->tablaHash = malloc(sizeof(Palabra*) * bucket->tamano);
    bucket->capacidad = cantidadDePalabras;

    return bucket;
}

int calcularTamanoDeBucket(int cantidadDePalabras)
{
    return cantidadDePalabras * FACTOR_TAMANO_CAPACIDAD_BUCKET;
}

void llenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras)
{
    int esValida = 0;
    unsigned long constanteDeHasheo;

    for (constanteDeHasheo = 1; constanteDeHasheo && !esValida; constanteDeHasheo++) {
        esValida = probarLlenarBucket(bucket, listaDePalabras, constanteDeHasheo);

        if (esValida) {
            bucket->constanteDeHasheo = constanteDeHasheo;
        }
    }

    assert(constanteDeHasheo);
}

int probarLlenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras, unsigned long constanteDeHasheo)
{
    int palabraHasheada, colisiona = 0;
    Palabra * palabraActual, * palabraEncontrada;

    limpiarBucket(bucket);
    bucket->constanteDeHasheo = constanteDeHasheo;

    for (int i = 0; i < listaDePalabras.cantidad && !colisiona; i++) {
        palabraActual = listaDePalabras.palabras[i];
        palabraHasheada = obtenerHashSecundario(*bucket, *palabraActual);
        palabraEncontrada = bucket->tablaHash[palabraHasheada];

        if (NULL == palabraEncontrada) {
            bucket->tablaHash[palabraHasheada] = palabraActual;
        } else if (sonPalabrasDistintas(*palabraActual, *palabraEncontrada)) {
            colisiona = 1;
        }
    }

    return !colisiona;
}

void limpiarBucket(Bucket * bucket)
{
    for (int i=0; i < bucket->tamano; i++) {
        bucket->tablaHash[i] = NULL;
    }
}

void imprimirBucketEnArchivo(Bucket bucket, FILE * archivo)
{
    fwprintf(archivo, L"%ld %d\n", bucket.constanteDeHasheo, bucket.capacidad);

    Palabra * palabraActual;
    for (int i = 0; i < bucket.tamano; i++) {
        palabraActual = bucket.tablaHash[i];

        if (palabraActual) {
            fwprintf(archivo, L"%d %ls\n", i, palabraActual->letras);
        }
    }
}

int obtenerHashSecundario(Bucket bucket, Palabra palabra)
{
    return funcionHashSecundaria(palabra, bucket.constanteDeHasheo) % bucket.tamano;
}

Bucket * cargarBucketDesdeArchivo(FILE * archivo)
{
    unsigned long constanteDeHasheo;
    int cantidadDeElementos;

    fwscanf(archivo, L"%ld %d", &constanteDeHasheo, &cantidadDeElementos);
    Bucket * bucket = armarBucket(cantidadDeElementos);
    bucket->constanteDeHasheo = constanteDeHasheo;
    cargarPalabrasEnBucketDesdeArchivo(bucket, archivo);

    return bucket;
}

void cargarPalabrasEnBucketDesdeArchivo(Bucket * bucket, FILE * archivo)
{
    int posicionEnBucket;

    for (int i = 0; i < bucket->capacidad; i++) {
        fwscanf(archivo, L"%d", &posicionEnBucket);
        bucket->tablaHash[posicionEnBucket] = cargarPalabraDesdeArchivo(archivo);
    }
}

void liberarPalabrasEnBucket(Bucket * bucket)
{
    Palabra * palabraActual;

    for (int i = 0; i < bucket->capacidad; i++) {
        palabraActual = bucket->tablaHash[i];

        if (NULL != palabraActual) {
            liberarPalabra(palabraActual);
        }
    }
}

void liberarBucket(Bucket * bucket)
{
    free(bucket->tablaHash);
    free(bucket);
}
