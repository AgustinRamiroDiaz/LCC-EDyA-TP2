#include <malloc.h>
#include "palabra.h"
#include "constantes.h"

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo)
{
    Palabra * palabra;
    ListaDePalabras * listaDePalabras = armarListaDePalabras(TAMANO_INICIAL_LISTA);

    while(palabra = cargarPalabraDesdeArchivo(archivo)) {
        agregarPalabraALista(palabra, listaDePalabras);
    }

    return listaDePalabras;
}

ListaDePalabras * armarListaDePalabras(int cantidadDePalabras)
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

int sonPalabrasIguales(Palabra primeraPalabra, Palabra segundaPalabra)
{
    if (primeraPalabra.longitud != segundaPalabra.longitud) {
        return 0;
    }

    return 0 == wcscmp(primeraPalabra.letras, segundaPalabra.letras);
}

int sonPalabrasDistintas(Palabra primeraPalabra, Palabra segundaPalabra)
{
    return !sonPalabrasIguales(primeraPalabra, segundaPalabra);
}

void intercambiarLetras(Palabra * palabra, int primeraPosicion, int segundaPosicion)
{
    wchar_t primeraLetra = palabra->letras[primeraPosicion];
    wchar_t segundaLetra = reemplazarLetra(palabra, primeraLetra, segundaPosicion);
    reemplazarLetra(palabra, segundaLetra, primeraPosicion);
}

wchar_t reemplazarLetra(Palabra * palabra, wchar_t letra, int posicion)
{
    wchar_t eliminada = palabra->letras[posicion];
    palabra->letras[posicion] = letra;

    return eliminada;
}

void agregarLetra(Palabra * palabra, wchar_t letra, int posicion)
{
    palabra->longitud++;
    palabra->letras = realloc(palabra->letras, sizeof(wchar_t) * (palabra->longitud + 1));

    for (int i = palabra->longitud; i > posicion; i--) {
        palabra->letras[i] = palabra->letras[i-1];
    }
    palabra->letras[posicion] = letra;

}

wchar_t eliminarLetra(Palabra * palabra, int posicion)
{
    wchar_t eliminada = palabra->letras[posicion];

    for (int i = posicion; i < palabra->longitud; i++) {
        palabra->letras[i] = palabra->letras[i+1];
    }
    palabra->longitud--;
    palabra->letras = realloc(palabra->letras, sizeof(wchar_t) * (palabra->longitud + 1));

    return eliminada;
}

Palabra * cargarPalabraDesdeArchivo(FILE * archivo)
{
    wchar_t buffer[LARGO_MAXIMO_PALABRA];

    if (1 == fwscanf(archivo, L" %ls ", buffer)){
        return crearPalabra(buffer);
    }

    return NULL;
}

Palabra * copiarPalabra(Palabra palabra)
{
    return crearPalabra(palabra.letras);
}

void liberarListaDePalabras(ListaDePalabras * listaDePalabras)
{
    for (int i = 0; i < listaDePalabras->cantidad; i++) {
        liberarPalabra(listaDePalabras->palabras[i]);
    }
    free(listaDePalabras);
}

void liberarPalabra(Palabra * palabra)
{
    free(palabra->letras);
    free(palabra);
}

ListaDePalabras * separarPalabra(Palabra palabra, int posicion)
{
    ListaDePalabras * listaDePalabras = armarListaDePalabras(2);
    wchar_t buffer[LARGO_MAXIMO_PALABRA];

    wcsncpy(buffer, palabra.letras, posicion);
    buffer[posicion] = L'\0';
    agregarPalabraALista(crearPalabra(buffer), listaDePalabras);

    wcsncpy(buffer, palabra.letras + posicion, palabra.longitud - posicion);
    buffer[palabra.longitud - posicion] = L'\0';
    agregarPalabraALista(crearPalabra(buffer), listaDePalabras);

    return listaDePalabras;
}

Palabra * unirListaDePalabras(ListaDePalabras listaDePalabras, wchar_t * separador)
{
    int caracteresEnLista = caracteresTotalesEnLista(listaDePalabras);
    int longitudDeSeparador = wcslen(separador);
    int caracteresEnSeparadores = longitudDeSeparador * (listaDePalabras.cantidad - 1);

    int nuevoLargo = caracteresEnLista + caracteresEnSeparadores + 1;
    wchar_t buffer[nuevoLargo];

    int posicion = 0;
    for (int i = 0; i < listaDePalabras.cantidad; i++)
    {
        if (i) {
            wcscpy(buffer + posicion, separador);
            posicion += longitudDeSeparador;
        }
        wcscpy(buffer + posicion, listaDePalabras.palabras[i]->letras);
        posicion += listaDePalabras.palabras[i]->longitud;
    }
    buffer[posicion] = L'\0';

    return crearPalabra(buffer);
}

int caracteresTotalesEnLista(ListaDePalabras listaDePalabras)
{
    int cantidad = 0;

    for (int i = 0; i < listaDePalabras.cantidad; i++) {
        cantidad += listaDePalabras.palabras[i]->longitud;
    }

    return cantidad;
}