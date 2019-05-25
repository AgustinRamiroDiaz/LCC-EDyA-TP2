#include <malloc.h>
#include "palabra.h"

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo)
{
    ListaDePalabras * listaDePalabras = crearListaDePalabras(1024);
    wchar_t buffer[500];

    while(fwscanf(archivo, L"%ls", buffer) == 1) {
        agregarPalabraALista(crearPalabra(buffer), listaDePalabras);
    }

    return listaDePalabras;
}

ListaDePalabras * crearListaDePalabras(int cantidadDePalabras)
{
    ListaDePalabras * listaDePalabras = malloc(sizeof(ListaDePalabras));
    listaDePalabras->palabras = malloc(sizeof(Palabra) * cantidadDePalabras);
    listaDePalabras->capacidad = cantidadDePalabras;
    listaDePalabras->cantidad = 0;

    return listaDePalabras;
}

void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras)
{
    if (listaDePalabras->cantidad == listaDePalabras->capacidad) {
        agrandarListaDePalabras(listaDePalabras);
    }

    listaDePalabras->palabras[listaDePalabras->cantidad] = palabra;
    listaDePalabras->cantidad++;
}

void agrandarListaDePalabras(ListaDePalabras * listaDePalabras)
{
    int nuevaCapacidad = listaDePalabras->capacidad * 2;
    listaDePalabras->palabras = realloc(listaDePalabras->palabras, sizeof(Palabra) * nuevaCapacidad);
    listaDePalabras->capacidad = nuevaCapacidad;
}


Palabra * crearPalabra(wchar_t * letras)
{
    Palabra * palabra = malloc(sizeof(Palabra));

    palabra->longitud = wcslen(letras);
    palabra->letras = malloc(sizeof(wchar_t) * (palabra->longitud + 1));
    wcscpy(palabra->letras, letras);

    return palabra;
}
