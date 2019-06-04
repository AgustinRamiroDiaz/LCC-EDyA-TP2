#ifndef HASH
#define HASH

#include "palabra.h"

// Funcion principal de hasheo que asigna una palabra
// a un bucket
unsigned long funcionHashPrincipal(Palabra palabra);

// Algoritmo de hasheo utilizado en la funcion de hasheo
// principal
unsigned long djb2(Palabra palabra);

// Funcion de hasheo secundaria que asigna una palabra
// a una posicion dentro de su bucket
unsigned long funcionHashSecundaria(Palabra palabra, unsigned long constante);

#endif
