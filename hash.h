#include "palabra.h"

typedef unsigned long (*FuncionHash) (wchar_t *str);

typedef unsigned long (*FuncionHashKey) (wchar_t *str, int key);

unsigned long
djb2(wchar_t *str);

unsigned long
hashConConstante(Palabra palabra, unsigned long constante);

unsigned long
sdbm(wchar_t *str);
