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
    for (int i = 0;i < palabra.longitud - 1; i++) {
        palabraCopiada = copiarPalabra(palabra);
        intercambiarLetras(palabraCopiada, i, i + 1);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
    }


    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        for (int i = 1; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            agregarLetra(palabraCopiada, caracter, i);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
        for (int i = 0; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, caracter, i);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }

    for (int i = 0; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        eliminarLetra(palabraCopiada, i);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
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
    Palabra * palabraCopiada;

    for (int i = 0;i < palabra.longitud - 1; i++) {
        palabraCopiada = copiarPalabra(palabra);
        intercambiarLetras(palabraCopiada, i, i + 1);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
    }
}

void generarSugerenciasAgregandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;

    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        for (int pos = 1; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            agregarLetra(palabraCopiada, caracter, pos);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }

    int cantidadDeLetras = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeLetras; i++) {
        for (int pos = 1; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            agregarLetra(palabraCopiada, LETRAS_ESPECIALES[i], pos);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }
}


void generarSugerenciasReemplazandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;

    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        for (int pos = 0; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, caracter, pos);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }
    
    int cantidadDeLetras = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeLetras; i++) {
        for (int pos = 0; pos < palabra.longitud; pos++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, LETRAS_ESPECIALES[i], pos);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }
}

void generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;

    for (int i = 0; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        eliminarLetra(palabraCopiada, i);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
    }
}

void generarSugerenciasSeparandoPalabras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    Palabra * palabraCopiada;
    ListaDePalabras * nuevasPalabras;
    
    int primeraPalabraExiste, segundaPalabraExiste;

    for (int i = 1; i < palabra.longitud; i++) {
        nuevasPalabras = separarPalabra(palabra, i);

        primeraPalabraExiste = palabraEnTablaHash(tablaHash, *nuevasPalabras->palabras[0]);
        segundaPalabraExiste = palabraEnTablaHash(tablaHash, *nuevasPalabras->palabras[1]);

        if(primeraPalabraExiste && segundaPalabraExiste){
            palabraCopiada = crearPalabra(wcscat(nuevasPalabras->palabras[0]->letras, nuevasPalabras->palabras[1]->letras));
            agregarPalabraALista(palabraCopiada, listaDeSugerencias);
        }
    }
}

void sugerirOLiberar(TablaHash tablaHash, Palabra * palabra, ListaDePalabras * listaDeSugerencias)
{
    if (palabraEnTablaHash(tablaHash, *palabra)){
        agregarPalabraALista(palabra, listaDeSugerencias);
    } else {
        liberarPalabra(palabra);
    }
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