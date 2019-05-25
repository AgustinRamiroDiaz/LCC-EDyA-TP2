#include <wchar.h>
#include "hash.h"
#include "constantes.h"

unsigned long djb2(Palabra palabra)
{
  unsigned long hash = CONSTANTE_HASHEO;

  int letra;
  for (int i=0; letra = palabra.letras[i]; i++) {
      hash = ((hash << 5) + hash) + letra;
  }

  return hash;
}

unsigned long hashConConstante(Palabra palabra, unsigned long constante)
{
  unsigned long hash = constante;

  int letra;
  for (int i=0; letra = palabra.letras[i]; i++) {
      hash = (hash ^ letra) + ((hash << 26) + (hash >> 6));
  }

  return hash;
}

unsigned long sdbm(wchar_t *str)
{
  unsigned long hash = 0;
  int c;

  while (c = *str++)
      hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}
