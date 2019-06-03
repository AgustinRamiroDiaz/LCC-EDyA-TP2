#include <malloc.h>
#include "palabra.h"
#include "constantes.h"

ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo)
{
    Palabra * palabra;
    ListaDePalabras * listaDePalabras = armarListaDePalabras(CAPACIDAD_INICIAL_LISTA);

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

Palabra * cargarPalabraDesdeArchivo(FILE * archivo)
{
    wchar_t buffer[LARGO_MAXIMO_PALABRA];

    if (1 == fwscanf(archivo, L"% ls ", buffer)){
        return crearPalabra(buffer);
    }

    return NULL;
}

Palabra * crearPalabra(wchar_t * letras)
{
    Palabra * palabra = malloc(sizeof(Palabra));

    palabra->longitud = wcslen(letras);
    palabra->letras = malloc(sizeof(wchar_t) * (palabra->longitud + 1));
    wcscpy(palabra->letras, letras);

    return palabra;
}

void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras)
{
    assert(palabra);

    if (listaDePalabras->cantidad == listaDePalabras->capacidad) {
        agrandarListaDePalabras(listaDePalabras);
    }

    listaDePalabras->palabras[listaDePalabras->cantidad] = palabra;
    listaDePalabras->cantidad++;
}

void agrandarListaDePalabras(ListaDePalabras * listaDePalabras)
{
    listaDePalabras->palabras = realloc(listaDePalabras->palabras, sizeof(Palabra) * nuevaCapacidad);
    listaDePalabras->capacidad *= FACTOR_AGRANDAR_CAPACIDAD_LISTA;
}

Palabra * copiarPalabra(Palabra palabra)
{
    return crearPalabra(palabra.letras);
}

void liberarListaDePalabras(ListaDePalabras * listaDePalabras)
{
    for (size_t i = 0; i < listaDePalabras->cantidad; i++) {
        liberarPalabra(listaDePalabras->palabras[i]);
    }
    free(listaDePalabras->palabras);
    free(listaDePalabras);
}

void liberarPalabra(Palabra * palabra)
{
    free(palabra->letras);
    free(palabra);
}

int sonPalabrasIguales(Palabra primeraPalabra, Palabra segundaPalabra)
{
    if (primeraPalabra.longitud != segundaPalabra.longitud) {
        return 0;
    }

    return !wcscmp(primeraPalabra.letras, segundaPalabra.letras);
}

int sonPalabrasDistintas(Palabra primeraPalabra, Palabra segundaPalabra)
{
    return !sonPalabrasIguales(primeraPalabra, segundaPalabra);
}



void intercambiarLetrasAdyacentes(Palabra * palabra, int posicion)
{
    intercambiarLetras(palabra, posicion, posicion + 1);
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

    for (size_t i = palabra->longitud; i > posicion; i--) {
        palabra->letras[i] = palabra->letras[i - 1];
    }
    palabra->letras[posicion] = letra;
}

wchar_t eliminarLetra(Palabra * palabra, int posicion)
{
    wchar_t eliminada = palabra->letras[posicion];

    for (int i = posicion; i < palabra->longitud; i++) {
        palabra->letras[i] = palabra->letras[i + 1];
    }
    palabra->letras = realloc(palabra->letras, sizeof(wchar_t) * (palabra->longitud));
    palabra->longitud--;

    return eliminada;
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

Palabra * unirPalabrasEnLista(ListaDePalabras listaDePalabras, wchar_t * separador)
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

int agregarPalabraAListaSiNoEstaRepetida(Palabra * palabra, ListaDePalabras * listaDePalabras)
{
    int estaEnLista = palabraEstaEnLista(*palabra, *listaDePalabras);

    if (!estaEnLista) {
        agregarPalabraALista(palabra, listaDePalabras);
    }

    return !estaEnLista;
}

void concatenarListasDePalabrasSinRepetir(ListaDePalabras * destino, ListaDePalabras * origen)
{
    Palabra * palabraActual;

    for (int i = 0; i < origen->cantidad; i++) {
        palabraActual = origen->palabras[i];

        if (!palabraEstaEnLista(*origen->palabras[i], *destino)) {
            agregarPalabraALista(copiarPalabra(*origen->palabras[i]), destino);
        }
    }
}

int palabraEstaEnLista(Palabra palabra, ListaDePalabras listaDePalabras)
{
    int encontrada = 0;
    Palabra * palabraActual;

    for (int i = 0; i < listaDePalabras.cantidad && !encontrada; i++) {
        palabraActual = listaDePalabras.palabras[i];
        encontrada = sonPalabrasIguales(palabra, *palabraActual);
    }

    return encontrada;
}

void eliminarUltimaPalabraDeLista(ListaDePalabras * listaDePalabras)
{
    int posicionABorrar = listaDePalabras->cantidad - 1;
    liberarPalabra(listaDePalabras->palabras[posicionABorrar]);
    listaDePalabras->cantidad--;
}
