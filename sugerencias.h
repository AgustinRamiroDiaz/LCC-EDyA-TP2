#ifndef SUGERENCIAS
#define SUGERENCIAS

#include "tabla_hash.h"

void corregirArchivo(FILE * archivo, TablaHash tablaHash);

void verificarPalabra(wchar_t * letrasLeidas, TablaHash tablaHash, int numeroDeLinea);

ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash);

void generarSugerenciasIntercambiandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasAgregandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasReemplazandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasSeparandoPalabras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

int sugerirSiExiste(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

int sugerirOLiberar(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void imprimirSugerencias(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias);


#endif