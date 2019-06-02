#ifndef CONSTANTES
#define CONSTANTES

#include <wchar.h>

static const int CONSTANTE_HASHEO = 5381;
static const int FACTOR_TAMANO_BUCKET = 1;
static const float ELEMENTOS_POR_BUCKET = 4;

static const int TAMANO_INICIAL_LISTA = 1024;
static const int CANTIDAD_MINIMA_SUGERENCIAS = 5;
static const int LARGO_MAXIMO_PALABRA = 500;

static const wchar_t LETRAS_ESPECIALES[] = L"áéíóúäëïöüñ";

#endif
