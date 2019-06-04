#ifndef TABLA_HASH
#define TABLA_HASH

#include "bucket.h"

/*
TablaHash es una estructura que almacena
el tamano de la tabla hash (cantidad de buckets),
la cantidadDeElementos de la tabla (palabras en el universo)
y un array de punteros a buckets
*/
typedef struct {
    int tamano;
    int cantidadDeElementos;
    Bucket ** buckets;
} TablaHash;

// Crea una tabla hash a partir del universo
TablaHash * crearTablaHash(ListaDePalabras * universo);

// Arma una tabla hash vacia con el espacio reservado para la cantidad de palabras
TablaHash * armarTablaHash(int cantidadDePalabras);

// Crea los buckets en la tabla hash a partir de los grupos de palabras
void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras);

// Agrupa las palabras por su hash principal en un array
ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

// Agrupa palabras segun su hash principal. Devuelve los grupos
// como listas de palabras
ListaDePalabras ** armarGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

// Imprime en el archivo la información necesaria sobre la tabla hash
// para luego poder levantarla desde el mismo
void imprimirTablaHashEnArchivo(TablaHash tablaHash, FILE * archivoDeTablaHash);

// Calcula el valor del hash principal de una palabra
int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra);

// Calcula la cantidad de buckets en funcion de la cantidad de palabras
// y de los elementos por bucket
int calcularCantidadDeBuckets(int cantidadDePalabras);

// Verifica que todas las palabras del universo esten en la tabla hash
int verificarTablaHash(TablaHash tablaHash, ListaDePalabras universo);

// Retorna verdadero si la palabra esta en la tabla hash y falso si no
int palabraEnTablaHash(TablaHash tablaHash, Palabra palabra);

// Levanta la tabla hash desde el archivo
TablaHash * cargarTablaHashDesdeArchivo(FILE * archivoDeTablaHash);

// Libera la memoria reservada por la tabla hash
// y todas las palabras que contiene
void liberarTablaHashYPalabras(TablaHash * tablaHash);

// Libera la memoria reservada por la tabla hash
void liberarTablaHash(TablaHash * tablaHash);

// Libera la memoria reservada por los grupos de palabras con 
// los que se crean los buckets
void liberarGruposDePalabras(ListaDePalabras ** gruposDePalabras, int cantidad);

#endif
