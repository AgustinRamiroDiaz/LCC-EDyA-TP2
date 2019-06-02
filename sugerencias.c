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

    generarSugerenciasModificandoPalabra(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasAgregandoLetras(palabra, tablaHash, listaDeSugerencias);
    ListaDePalabras * palabrasConEliminaciones = generarSugerenciasEliminandoLetras(palabra, tablaHash, listaDeSugerencias);

    if (listaDeSugerencias->cantidad < TAMANO_INICIAL_LISTA_SUGERENCIAS) {
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

    for (int i = 0; i < cantidadDePalabras && listaDeSugerencias->cantidad < TAMANO_INICIAL_LISTA_SUGERENCIAS; i++) {
        palabraActual = palabrasConEliminaciones->palabras[i];
        generarSugerenciasModificandoPalabra(*palabraActual, tablaHash, listaDeSugerencias);
        
        if (palabraActual->longitud > 1) {
            nuevasPalabrasConEliminaciones = generarSugerenciasEliminandoLetras(*palabraActual, tablaHash, listaDeSugerencias);
            cantidadDePalabras += nuevasPalabrasConEliminaciones->cantidad;
            concatenarListasDePalabras(palabrasConEliminaciones, nuevasPalabrasConEliminaciones);
        }
    }
}

void generarSugerenciasModificandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    generarSugerenciasIntercambiandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasReemplazandoLetras(palabra, tablaHash, listaDeSugerencias);
    generarSugerenciasSeparandoPalabras(palabra, tablaHash, listaDeSugerencias);
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

ListaDePalabras * generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias)
{
    int existe;
    Palabra * palabraCopiada;
    ListaDePalabras * palabrasGeneradas = armarListaDePalabras(TAMANO_INICIAL_LISTA);

    for (int i = 0; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        eliminarLetra(palabraCopiada, i);
        existe = sugerirSiExiste(palabraCopiada, tablaHash, listaDeSugerencias);
        
        if (!existe) {
            agregarPalabraALista(palabraCopiada, palabrasGeneradas);
        }
    }

    return palabrasGeneradas;
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
        Palabra * sugerencias = unirListaDePalabras(listaDeSugerencias, L", ");
        wprintf(L"Quizas quiso decir:\n%ls", sugerencias->letras);
    } else {
        wprintf(L"No se encontraron sugerencias.");
    }
    wprintf(L"\n\n");
}