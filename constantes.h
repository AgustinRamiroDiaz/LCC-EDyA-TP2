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
static const wchar_t SIGNOS_ESPECIALEs[] = L" :;,.?!";
static const wchar_t SALTOS_DE_LINEA[] = L"\n\r";

static const char ARCHIVO_TABLA_HASH[] = "tabla-hash.txt";
static const char ARCHIVO_UNIVERSO_DEFECTO[] = "universo.txt";
static const char ARCHIVO_TEXTO_ENTRADA_POR_DEFECTO[] = "texto.txt";
static const char ARCHIVO_CORRECCIONES_POR_DEFECTO[] = "correcciones.txt";

#endif
