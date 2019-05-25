#ifndef HASH
#define HASH

#include "palabra.h"

unsigned long djb2(Palabra palabra);

unsigned long hashConConstante(Palabra palabra, unsigned long constante);

unsigned long sdbm(wchar_t *str);

#endif
