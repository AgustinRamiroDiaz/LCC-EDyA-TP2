#include <wchar.h>

typedef struct {
    int longitud;
    wchar_t * letras;
} Palabra;

typedef struct {
    int cantidad, capacidad;
    Palabra * palabras;
} ListaDePalabras;

ListaDePalabras * crearListaDePalabras(int cantidadDePalabras);

void agregarPalabraALista(ListaDePalabras * listaDePalabras, Palabra * palabra);
