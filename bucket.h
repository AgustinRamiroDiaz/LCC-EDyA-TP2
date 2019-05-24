#include "palabra.h"

typedef struct {
    unsigned long constanteDeHasheo;
    int tamano, capacidad;
    Palabra * tablaHash;
} Bucket;

Bucket * crearBucket(ListaDePalabras listaDePalabras);

Bucket * armarBucket(int cantidadDePalabras);

int calcularTamanoDeBucket(int cantidadDePalabras);

void llenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras);

int probarLlenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras, unsigned long constanteDeHasheo);

void vaciarBucket(Bucket * bucket);

void imprimirBucketEnArchivo(Bucket bucket, FILE * archivo);
