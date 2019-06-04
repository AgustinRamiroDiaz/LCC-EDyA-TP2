#ifndef SUGERENCIAS
#define SUGERENCIAS

#include "tabla_hash.h"

// Imprime en el archivo de salida las correcciones para el texto en el
// archivo de entrada
void corregirArchivo(FILE * archivoDeEntrada, FILE * archivoDeCorrecciones, TablaHash tablaHash);

// Imprime en el archivo de salida las correcciones para la palabra
void verificarPalabra(Palabra * palabra, TablaHash tablaHash, int numeroDeLinea, FILE * archivoDeCorrecciones);

// Devuelve una lista de palabras con las sugerencias para la palabra
ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash);

// Genera sugerencias con distancia mayor a 1, utilizando
// palabras obtenidas a partir de eliminaciones de letras
// de una palabra
void generarSugerenciasParaEliminaciones(ListaDePalabras * palabrasConEliminaciones, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Genera sugerencias aplicando modificaciones a la palabra
void generarSugerenciasModificandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Genera sugerencias intercambiando letras de la palabra
void generarSugerenciasIntercambiandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Genera sugerencias agregando letras a la palabra
void generarSugerenciasAgregandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Genera sugerencias reemplazando letras de la palabra
void generarSugerenciasReemplazandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Genera sugerencias eliminando letras de la palabra,
// y devuelve las palabras generadas que no hayan sido
// sugeridas
ListaDePalabras * generarSugerenciasEliminandoLetras(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Genera sugerencias separando a la palabra en dos.
// Sugiere el resultado si ambas partes son palabras
// existentes
void generarSugerenciasSeparandoPalabra(Palabra palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Agrega a la palabra a la lista de sugerencias si es 
// una palabra existente
int sugerirSiExiste(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Agrega a la palabra a la lista de sugerencias si es 
// una palabra existente, y libera la palabra original
int sugerirYLiberar(Palabra * palabra, TablaHash tablaHash, ListaDePalabras * listaDeSugerencias);

// Imprime la lista de sugerencias para una palabra en el archivo de salida
void imprimirSugerenciasEnArchivo(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias, FILE * archivoDeCorrecciones);

#endif