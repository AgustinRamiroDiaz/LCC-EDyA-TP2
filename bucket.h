#include "palabra.h"

typedef struct {
    unsigned long constanteDeHasheo;
    int tamano, capacidad;
    Palabra * tablaHash;
} Bucket;
