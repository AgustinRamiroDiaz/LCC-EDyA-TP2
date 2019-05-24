#include "bucket.h"

typedef struct {
    int tamano;
    Bucket * buckets;
} TablaHash;

TablaHash * crearTablaHash(ListaDePalabras * universo);

TablaHash * armarTablaHash(int cantidadDePalabras);

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo);

unsigned long hashearPalabra(Palabra palabra);

int calcularCantidadDeBuckets(int cantidadDePalabras);
