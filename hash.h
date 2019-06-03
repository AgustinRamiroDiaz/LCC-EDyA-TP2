#ifndef HASH
#define HASH

#include "palabra.h"

//Principal funcion de hasheo que se usa en la primer instancia
//para generar los buckets
unsigned long funcionHashPrincipal(Palabra palabra);

//Funcion de hasheo
unsigned long djb2(Palabra palabra);

//Funcion de hasheo secundaria la cual se aplica a los buckets
//generados a partir de la funcion de hash principal
//con el fin de ser un hash perfecto
unsigned long funcionHashSecundaria(Palabra palabra, unsigned long constante);

#endif
