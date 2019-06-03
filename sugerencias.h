#ifndef SUGERENCIAS
#define SUGERENCIAS

#include "tabla_hash.h"

void corregirArchivo(FILE * archivoDeEntrada, FILE * archivoDeCorrecciones, TablaHash tablaHash);

void verificarPalabra(Palabra * palabra, TablaHash tablaHash, int numeroDeLinea, FILE * archivoDeCorrecciones);

ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash);

void generarSugerenciasParaEliminaciones(ListaDePalabras * palabrasConEliminaciones, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasModificandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasIntercambiandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasAgregandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasReemplazandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

ListaDePalabras * generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void generarSugerenciasSeparandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

int sugerirSiExiste(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

int sugerirYLiberar(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

void imprimirSugerenciasEnArchivo(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias, FILE * archivoDeCorrecciones);

#endif