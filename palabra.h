#ifndef PALABRA
#define PALABRA

#include <wchar.h>

/*
Palabra es una estructura que almacena
un array de letras y la longitud del mismo
*/
typedef struct {
    int longitud;
    wchar_t * letras;
} Palabra;

/*
ListaDePalabras es una estructura que almacena
un array de punteros a palabras,
la cantidad de palabras actuales y
la capacidad total del array de punteros a palabras
*/
typedef struct {
    int cantidad, capacidad;
    Palabra ** palabras;
} ListaDePalabras;

// Crea una lista de palabras con todas las palabras del archivo
ListaDePalabras * crearListaDePalabrasDesdeArchivo(FILE * archivo);

// Arma una ListaDePalabras vacía
ListaDePalabras * armarListaDePalabras(int cantidadDePalabras);

// Agrega la palabra al final de la lista de palabras
void agregarPalabraALista(Palabra * palabra, ListaDePalabras * listaDePalabras);

// Agranda el espacio reservado para las palabras de la lista
void agrandarListaDePalabras(ListaDePalabras * listaDePalabras);

// Crea una palabra a partir de una cadena
Palabra * crearPalabra(wchar_t * letras);

// Retorna verdadero si las palabras son iguales y falso si no
int sonPalabrasIguales(Palabra primeraPalabra, Palabra segundaPalabra);

// Retorna verdadero si las palabras son distintas y falso si no
int sonPalabrasDistintas(Palabra primeraPalabra, Palabra segundaPalabra);

// Intercambia la letra de una palabra en una posicion con su siguiente
void intercambiarLetrasAdyacentes(Palabra * palabra, int posicion);

// Intercambia 2 letras de lugar en la palabra
void intercambiarLetras(Palabra * palabra, int primeraPosicion, int segundaPosicion);

// Reemplaza una letra de una palabra
wchar_t reemplazarLetra(Palabra * palabra, wchar_t letra, int posicion);

// Agrega una letra a la palabra
void agregarLetra(Palabra * palabra, wchar_t letra, int posicion);

// Elimina una letra de la palabra
wchar_t eliminarLetra(Palabra * palabra, int posicion);

// Retorna una palabra de lo leido del archivo
Palabra * cargarPalabraDesdeArchivo(FILE * archivo);

// Retorna una copia de la palabra
Palabra * copiarPalabra(Palabra palabra);

// Libera la memoria reservada de la lista de palabras
void liberarListaDePalabras(ListaDePalabras * listaDePalabras);

// Libera la memoria reservada de la palabra
void liberarPalabra(Palabra * palabra);

// Retorna una lista de palabras con las sub-palabras de la palabra
ListaDePalabras * separarPalabra(Palabra palabra, int posicion);

// Une las palabras en la lista de palabras con el separador de por medio
Palabra * unirPalabrasEnLista(ListaDePalabras listaDePalabras, wchar_t * separador);

// Calcula la cantidad de caracteres en la lista de palabras
int caracteresTotalesEnLista(ListaDePalabras listaDePalabras);

// Agrega la palabra a la lista de palabras si no esta en ella
int agregarPalabraAListaSiNoEstaRepetida(Palabra * palabra, ListaDePalabras * listaDePalabras);

// Concatena las listas de palabras sin repeticiones de palabras
void concatenarListasDePalabrasSinRepetir(ListaDePalabras * destino, ListaDePalabras * origen);

// Retorna 1 si la palabra esta en lista y 0 si no
int palabraEstaEnLista(Palabra palabra, ListaDePalabras listaDePalabras);

// Elimina la ultima palabra de la lista
void eliminarUltimaPalabraDeLista(ListaDePalabras * listaDePalabras);

// Pasa las letras de la palabra a minuscula
void pasarPalabraAMinuscula(Palabra * palabra);

// Devuelve la letra pasada a minuscula
wchar_t obtenerLetraEnMinuscula (wchar_t letra);

#endif
