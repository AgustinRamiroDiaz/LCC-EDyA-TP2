#include "sugerencias.h"
#include "constantes.h"
#include "archivo.h"
#include <stdio.h>
#include <wchar.h>

void corregirArchivo(FILE * archivo, TablaHash tablaHash)
{
    Palabra * palabraLeida;
    wchar_t caracter, buffer[LARGO_MAXIMO_PALABRA];
    int longitudDePalabra = 0, numeroDeLinea = 1, finDeArchivo = 0;

    while (!finDeArchivo) {
        caracter = fgetwc(archivo);
        if (esLetra(caracter)) {
            buffer[longitudDePalabra] = caracter;
            longitudDePalabra++;
        } else if (longitudDePalabra && esFinDePalabra(caracter)) {
            buffer[longitudDePalabra] = L'\0';
            palabraLeida = crearPalabra(buffer);
            verificarPalabra(buffer, tablaHash, numeroDeLinea);

            longitudDePalabra = 0;
        }

        if (esFinDeLinea(caracter)) {
            numeroDeLinea++;
        }

        finDeArchivo = esFinDeArchivo(caracter);
    }
    /*for (size_t i = 0; caracter != WEOF; i++) {
        buffer[i] = caracter;
        if (caracter == L':' || caracter == L';' || caracter == L',' || caracter == L'.' || caracter == L'?' || caracter == L'!') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            if (!palabraEnTablaHash(tablaHash, *palabraActual))
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            fgetwc(archivo);
            i = -1;
        }
        if (caracter == L' ') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            if (!palabraEnTablaHash(tablaHash, *palabraActual))
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            i = -1;
        }
        if (caracter == L'\n' || caracter == L'\r') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            if (!palabraEnTablaHash(tablaHash, *palabraActual))
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            contadorDeLineas++;
            i = -1;
        }

        caracter = fgetwc(archivo);
    }*/
}

void verificarPalabra(wchar_t * letrasLeidas, TablaHash tablaHash, int numeroDeLinea)
{
    Palabra * palabra = crearPalabra(letrasLeidas);

    if (!palabraEnTablaHash(tablaHash, *palabra)) {
        ListaDePalabras * listaDeSugerencias = generarSugerencias(*palabra, tablaHash);
        imprimirSugerencias(*palabra, numeroDeLinea, *listaDeSugerencias);
    }
}

ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash)
{
    ListaDePalabras * listaDeSugerencias = armarListaDePalabras(TAMANO_INICIAL_LISTA_SUGERENCIAS);

    generarSugerenciasIntercambiandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasAgregandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasReemplazandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasEliminandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasSeparandoPalabras(palabra, tablaHash, listaDeSugerencias);

    return listaDeSugerencias;

    /*
    wchar_t aux1, aux2;
    Palabra * palabraCopiada;
    palabraCopiada = copiarPalabra(palabra);

    for (int i = 0;i < palabra.longitud - 1; i++) {
        intercambiarLetras(palabraCopiada, i, i + 1);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        intercambiarLetras(palabraCopiada, i, i + 1);
    }


    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        palabraCopiada = copiarPalabra(palabra);
        agregarLetra(palabraCopiada, caracter, 1);
        for (int i = 1; i < palabra.longitud; i++) {
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
            intercambiarLetras(palabraCopiada, i, i + 1);
        }

        for (int i = 0; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, caracter, i);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }

    palabraCopiada = copiarPalabra(palabra);
    aux1 = eliminarLetra(palabraCopiada, 0);
    for (int i = 0; i < palabra.longitud; i++) {
        wprintf(L"Palabra: %ls\n", palabraCopiada->letras);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        aux2 = aux1;
        aux1 = palabraCopiada->letras[i];
        palabraCopiada->letras[i] = aux2;
    }

    ListaDePalabras * dobleSugerencia;
    for (int i = 1; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        dobleSugerencia = separarPalabra(*palabraCopiada, i);
        if(palabraEnTablaHash(tablaHash, *dobleSugerencia->palabras[0])
        &&palabraEnTablaHash(tablaHash, *dobleSugerencia->palabras[1])){
            palabraCopiada = crearPalabra(wcscat(dobleSugerencia->palabras[0]->letras, dobleSugerencia->palabras[1]->letras));
            agregarPalabraALista(palabraCopiada, listaDeSugerencias);
        } else {
            liberarPalabra(palabraCopiada);
        }
    }*/
}

void generarSugerenciasIntercambiandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada = copiarPalabra(palabra);

    for (int pos = 0; pos < palabra.longitud - 1; pos++) {
        intercambiarLetras(palabraCopiada, pos, pos + 1);
        sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);
        intercambiarLetras(palabraCopiada, pos, pos + 1);
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
            intercambiarLetras(palabraCopiada, pos, pos + 1);
            sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);
        }
        liberarPalabra(palabraCopiada);
    }

    int cantidadDeLetras = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeLetras; i++) {
        palabraCopiada = copiarPalabra(palabra);
        agregarLetra(palabraCopiada, LETRAS_ESPECIALES[i], 0);

        for (int pos = 0; pos < palabra.longitud - 1; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            intercambiarLetras(palabraCopiada, pos, pos + 1);
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
            sugerirOLiberar(palabraCopiada, tablaHash, listaDeSugerencias);
        }
    }
    
    int cantidadDeLetras = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeLetras; i++) {
        for (int pos = 0; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, LETRAS_ESPECIALES[i], pos);
            sugerirOLiberar(palabraCopiada, tablaHash, listaDeSugerencias);
        }
    }
}

void generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;

    for (int i = 0; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        eliminarLetra(palabraCopiada, i);
        sugerirOLiberar(palabraCopiada, tablaHash, listaDeSugerencias);
    }
}

void generarSugerenciasSeparandoPalabras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabrasUnidas;
    ListaDePalabras * nuevasPalabras;
    
    int primeraPalabraExiste, segundaPalabraExiste;

    for (int i = 1; i < palabra.longitud; i++) {
        nuevasPalabras = separarPalabra(palabra, i);

        primeraPalabraExiste = palabraEnTablaHash(tablaHash, *nuevasPalabras->palabras[0]);
        segundaPalabraExiste = palabraEnTablaHash(tablaHash, *nuevasPalabras->palabras[1]);

        if (primeraPalabraExiste && segundaPalabraExiste) {
            palabrasUnidas = unirListaDePalabras(*nuevasPalabras, L" ");
            agregarPalabraALista(palabrasUnidas, listaDeSugerencias);
        }

        liberarListaDePalabras(nuevasPalabras);
    }
}

int sugerirSiExiste(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    int palabraExiste = palabraEnTablaHash(tablaHash, *palabra);

    if (palabraExiste){
        agregarPalabraALista(palabra, listaDeSugerencias);
    }

    return palabraExiste;
}

int sugerirOLiberar(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    int palabraFueSugerida = sugerirSiExiste(palabra, tablaHash, listaDeSugerencias);
    
    if (!palabraFueSugerida){
        liberarPalabra(palabra);
    }

    return palabraFueSugerida;
}

void imprimirSugerencias(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias)
{
    wprintf(L"Linea %d, \"%ls\" no esta en el diccionario.\n", linea, palabra.letras);
    
    if (listaDeSugerencias.cantidad) {
        wprintf(L"Quizas quiso decir: ");
        for (int i = 0; i < listaDeSugerencias.cantidad; i++) {
            wprintf(L"%ls ", listaDeSugerencias.palabras[i]->letras);
        }
    } else {
        wprintf(L"No se encontraron sugerencias.");
    }
    wprintf(L"\n\n");
}