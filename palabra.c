#include "palabra.h"

Palabra * copiarPalabra(Palabra * palabra)
{
    Palabra * copia = malloc(sizeof(Palabra));
    copia->longitud = palabra->longitud;
    wcscpy(copia->letras, palabra->letras);

    return copia;
}
