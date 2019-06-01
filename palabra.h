#ifndef PALABRA
#define PALABRA

#include <wchar.h>

typedef struct {
    int longitud;
    wchar_t * letras;
} Palabra;

typedef struct {
    int cantidad, capacidad;
    Palabra ** palabras;
} ListaDePalabras;

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo);

ListaDePalabras * armarListaDePalabras(int cantidadDePalabras);

void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras);

void agrandarListaDePalabras(ListaDePalabras * listaDePalabras);

Palabra * crearPalabra(wchar_t * letras);

int sonPalabrasIguales(Palabra primeraPalabra, Palabra segundaPalabra);

int sonPalabrasDistintas(Palabra primeraPalabra, Palabra segundaPalabra);

void intercambiarLetras(Palabra * palabra, int primeraPosicion, int segundaPosicion);

void intercambiarLetrasAdyacentes(Palabra * palabra, int posicion);

wchar_t reemplazarLetra(Palabra * palabra, wchar_t letra, int posicion);

void agregarLetra(Palabra * palabra, wchar_t letra, int posicion);

wchar_t eliminarLetra(Palabra * palabra, int posicion);

Palabra * cargarPalabraDesdeArchivo(FILE * archivo);

Palabra * copiarPalabra(Palabra palabra);

void liberarListaDePalabras(ListaDePalabras * listaDePalabras);

void liberarPalabra(Palabra * palabra);

ListaDePalabras * separarPalabra(Palabra palabra, int posicion);

Palabra * unirListaDePalabras(ListaDePalabras listaDePalabras, wchar_t * separador);

int caracteresTotalesEnLista(ListaDePalabras listaDePalabras);

#endif
