#include <wchar.h>
#include "hash.h"
#include "constantes.h"


unsigned long funcionHashPrincipal(Palabra palabra)
{
  return djb2(palabra);
}

unsigned long djb2(Palabra palabra)
{
  unsigned long hash = CONSTANTE_HASHEO;

  unsigned long letra;
  for (unsigned long i = 0; letra = palabra.letras[i]; i++) {
      hash = ((hash << 5) + hash) + letra;
  }

  return hash;
}

unsigned long funcionHashSecundaria(Palabra palabra, unsigned long constante)
{
  unsigned long hash = constante;
  unsigned long letra;

  for (unsigned long i = 0; letra = palabra.letras[i]; i++) {
      hash = (hash ^ letra) + ((hash << 26) + (hash >> 6));
  }

  return hash;
}
