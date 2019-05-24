#include "palabra.h"

ListaDePalabras * crearListaDePalabras(int cantidadDePalabras)
{
    ListaDePalabras * listaDePalabras = malloc(sizeof(ListaDePalabras));
    listaDePalabras->capacidad = cantidadDePalabras;

    return listaDePalabras;
}

void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras)
{
    if (listaDePalabras->cantidad == listaDePalabras->capacidad) {
        listaDePalabras->capacidad *= 2;
        listaDePalabras->palabras = realloc(sizeof(Palabra) * listaDePalabras->capacidad);
    }

    listaDePalabras->palabras[listaDePalabras->cantidad];
    listaDePalabras->cantidad++;
}

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo)
{
    ListaDePalabras * listaDePalabras = crearListaDePalabras(1024);
    wchar_t buffer[100];
    while(fwscanf(archivo, L"%ls", buffer) == 1) {
        agregarPalabraALista(crearPalabra(buffer), listaDePalabras);
    }

    return listaDePalabras;
}

Palabra * crearPalabra(wchar_t * letras)
{
    Palabra * palabra = malloc(sizeof(Palabra));

    palabra->longitud = wcslen(letras);
    palabra->letras = malloc(sizeof(wchar_t) * palabra->longitud);
    wcscpy(palabra->letras, letras);

    return palabra;
}
