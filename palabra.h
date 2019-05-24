#include <wchar.h>

typedef struct {
    int longitud;
    wchar_t * letras;
} Palabra;

typedef struct {
    int cantidad;
    Palabra * palabras;
} ListaDePalabras;
