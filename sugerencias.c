#include "sugerencias.h"
#include "constantes.h"
#include "archivo.h"
#include <stdio.h>
#include <wchar.h>

void corregirArchivo(FILE * archivoDeEntrada, FILE * archivoDeCorrecciones, TablaHash tablaHash)
{
    Palabra * palabraLeida;
    wchar_t caracter, buffer[LARGO_MAXIMO_PALABRA];
    int longitudDePalabra = 0, numeroDeLinea = 1, finDeArchivo = 0;

    while (!finDeArchivo) {
        caracter = fgetwc(archivoDeEntrada);

        if (esLetra(caracter)) {
            buffer[longitudDePalabra] = caracter;
            longitudDePalabra++;
        } else {
            if (longitudDePalabra && esFinDePalabra(caracter)) {
                buffer[longitudDePalabra] = L'\0';

                palabraLeida = crearPalabra(buffer);
                verificarPalabra(palabraLeida, tablaHash, numeroDeLinea, archivoDeCorrecciones);
                liberarPalabra(palabraLeida);

                longitudDePalabra = 0;
            }

            if (esFinDeLinea(caracter)) {
                numeroDeLinea++;
            }
        }

        finDeArchivo = esFinDeArchivo(caracter);
    }
}

void verificarPalabra(Palabra * palabra, TablaHash tablaHash, int numeroDeLinea, FILE * archivoDeCorrecciones)
{
    if (!palabraEnTablaHash(tablaHash, *palabra)) {
        ListaDePalabras * listaDeSugerencias = generarSugerencias(*palabra, tablaHash);
        imprimirSugerenciasEnArchivo(*palabra, numeroDeLinea, *listaDeSugerencias, archivoDeCorrecciones);
        liberarListaDePalabras(listaDeSugerencias);
    }
}

ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash)
{
    ListaDePalabras * listaDeSugerencias = armarListaDePalabras(CANTIDAD_MINIMA_SUGERENCIAS);

    generarSugerenciasModificandoPalabra(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasAgregandoLetras(palabra, tablaHash, listaDeSugerencias);
    ListaDePalabras * palabrasConEliminaciones = generarSugerenciasEliminandoLetras(palabra, tablaHash, listaDeSugerencias);

    if (listaDeSugerencias->cantidad < CANTIDAD_MINIMA_SUGERENCIAS) {
        generarSugerenciasParaEliminaciones(palabrasConEliminaciones, tablaHash, listaDeSugerencias);
    }
    liberarListaDePalabras(palabrasConEliminaciones);

    return listaDeSugerencias;
}

void generarSugerenciasParaEliminaciones(ListaDePalabras * palabrasConEliminaciones, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraActual;
    int cantidadDePalabras = palabrasConEliminaciones->cantidad;
    ListaDePalabras * nuevasPalabrasConEliminaciones;

    for (int i = 0; i < cantidadDePalabras && listaDeSugerencias->cantidad < CANTIDAD_MINIMA_SUGERENCIAS; i++) {
        palabraActual = palabrasConEliminaciones->palabras[i];
        generarSugerenciasModificandoPalabra(*palabraActual, tablaHash, listaDeSugerencias);

        if (palabraActual->longitud > 1) {
            nuevasPalabrasConEliminaciones = generarSugerenciasEliminandoLetras(*palabraActual, tablaHash, listaDeSugerencias);
            cantidadDePalabras += nuevasPalabrasConEliminaciones->cantidad;
            concatenarListasDePalabrasSinRepetir(palabrasConEliminaciones, nuevasPalabrasConEliminaciones);
            liberarListaDePalabras(nuevasPalabrasConEliminaciones);
        }
    }
    
    while (listaDeSugerencias->cantidad > CANTIDAD_MINIMA_SUGERENCIAS) {
        eliminarUltimaPalabraDeLista(listaDeSugerencias);
    }
}

void generarSugerenciasModificandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    generarSugerenciasIntercambiandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasReemplazandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasSeparandoPalabra(palabra, tablaHash, listaDeSugerencias);
}

void generarSugerenciasIntercambiandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada = copiarPalabra(palabra);

    for (int pos = 0; pos < palabra.longitud - 1; pos++) {
        intercambiarLetrasAdyacentes(palabraCopiada, pos);
        sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);
        intercambiarLetrasAdyacentes(palabraCopiada, pos);
    }

    liberarPalabra(palabraCopiada);
}

void generarSugerenciasAgregandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;

    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        palabraCopiada = copiarPalabra(palabra);
        agregarLetra(palabraCopiada, caracter, 0);

        for (int pos = 0; pos < palabra.longitud - 1; pos++) {
            intercambiarLetrasAdyacentes(palabraCopiada, pos);
            sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);
        }
        liberarPalabra(palabraCopiada);
    }

    int cantidadDeLetras = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeLetras; i++) {
        palabraCopiada = copiarPalabra(palabra);
        agregarLetra(palabraCopiada, LETRAS_ESPECIALES[i], 0);

        for (int pos = 0; pos < palabra.longitud - 1; pos++) {
            intercambiarLetrasAdyacentes(palabraCopiada, pos);
            sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);
        }
        liberarPalabra(palabraCopiada);
    }
}

void generarSugerenciasReemplazandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;

    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        for (int pos = 0; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, caracter, pos);
            sugerirYLiberar(palabraCopiada, tablaHash, listaDeSugerencias);
        }
    }

    int cantidadDeLetras = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeLetras; i++) {
        for (int pos = 0; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, LETRAS_ESPECIALES[i], pos);
            sugerirYLiberar(palabraCopiada, tablaHash, listaDeSugerencias);
        }
    }
}

ListaDePalabras * generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    int existe;
    Palabra * palabraCopiada;
    ListaDePalabras * palabrasGeneradas = armarListaDePalabras(CAPACIDAD_INICIAL_LISTA);

    for (int i = 0; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        eliminarLetra(palabraCopiada, i);
        existe = sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);

        if (existe) {
            liberarPalabra(palabraCopiada);
        } else {
            agregarPalabraALista(palabraCopiada, palabrasGeneradas);
        }
    }

    return palabrasGeneradas;
}

void generarSugerenciasSeparandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabrasUnidas;
    ListaDePalabras * nuevasPalabras;

    int primeraPalabraExiste, segundaPalabraExiste, fueAgregada;

    for (int i = 1; i < palabra.longitud; i++) {
        nuevasPalabras = separarPalabra(palabra, i);

        primeraPalabraExiste = palabraEnTablaHash(tablaHash, *nuevasPalabras->palabras[0]);
        segundaPalabraExiste = palabraEnTablaHash(tablaHash, *nuevasPalabras->palabras[1]);

        if (primeraPalabraExiste && segundaPalabraExiste) {
            palabrasUnidas = unirPalabrasEnLista(*nuevasPalabras, L" ");
            fueAgregada = agregarPalabraAListaSiNoEstaRepetida(palabrasUnidas, listaDeSugerencias);

            if (!fueAgregada) {
                liberarPalabra(palabrasUnidas);
            }
        }

        liberarListaDePalabras(nuevasPalabras);
    }
}

int sugerirSiExiste(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    int fueAgregada = 0, palabraExiste = palabraEnTablaHash(tablaHash, *palabra);

    if (palabraExiste) {
        Palabra * palabraCopiada = copiarPalabra(*palabra);

        fueAgregada = agregarPalabraAListaSiNoEstaRepetida(palabraCopiada, listaDeSugerencias);

        if (!fueAgregada) {
            liberarPalabra(palabraCopiada);
        }
    }

    return fueAgregada;
}

int sugerirYLiberar(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    int palabraFueSugerida = sugerirSiExiste(palabra, tablaHash, listaDeSugerencias);
    liberarPalabra(palabra);

    return palabraFueSugerida;
}

void imprimirSugerenciasEnArchivo(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias, FILE * archivoDeCorrecciones)
{
    fwprintf(archivoDeCorrecciones, L"Linea %d, \"%ls\" no esta en el diccionario.\n", linea, palabra.letras);

    if (listaDeSugerencias.cantidad) {
        fwprintf(archivoDeCorrecciones, L"Quizas quiso decir:\n");
        for (int i = 0; i < listaDeSugerencias.cantidad; i++) {
            if (i) {
                fwprintf(archivoDeCorrecciones, L", ");
            }
            fwprintf(archivoDeCorrecciones, L"%ls", listaDeSugerencias.palabras[i]->letras);
        }
    } else {
        fwprintf(archivoDeCorrecciones, L"No se encontraron sugerencias.");
    }
    fwprintf(archivoDeCorrecciones, L"\n\n");
}
