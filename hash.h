#ifndef HASH
#define HASH

#include "palabra.h"

unsigned long funcionHashPrincipal(Palabra palabra);

unsigned long djb2(Palabra palabra);

unsigned long funcionHashSecundaria(Palabra palabra, unsigned long constante);

unsigned long sdbm(Palabra palabra);

#endif
