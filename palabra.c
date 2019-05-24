#include "palabra.h"

ListaDePalabras * crearListaDePalabras(int cantidadDePalabras)
{
    ListaDePalabras * listaDePalabras = malloc(sizeof(ListaDePalabras));
    listaDePalabras->capacidad = cantidadDePalabras;

    return listaDePalabras;
}

void agregarPalabraALista(ListaDePalabras * listaDePalabras, Palabra * palabra)
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
}
