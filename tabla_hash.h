#ifndef TABLA_HASH
#define TABLA_HASH

#include "bucket.h"

/*
TablaHash es una estructura que almacena:
    tamano de la tabla hash
    cantidadDeElementos de la tabla, es decir los que no sean NULL
    un array de punteros a buckets, los cuales contienen todas las sub-tablas hash
*/
typedef struct {
    int tamano;
    int cantidadDeElementos;
    Bucket ** buckets;
} TablaHash;

//Crea una tabla hash a partir del universo
TablaHash * crearTablaHash(ListaDePalabras * universo);

//Arma una tabla hash vacia con el espacio reservado para la cantidad de palabras
TablaHash * armarTablaHash(int cantidadDePalabras);

//Crea los buckets en la tabla hash a partir de los grupos de palabras
void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras);

//Agrupa las palabras por su hash principal en un array
ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

//Arma un array de listas de palabras vacias con su respectivo espacio reservado
ListaDePalabras ** armarGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras);

/*
Imprime la tabla hash en el archivo de la forma:

cantidadDeElementos tamano
bucket1
...
buckettamano
*/
void imprimirTablaHashEnArchivo(TablaHash tablaHash, FILE * archivoDeTablaHash);

//Calcula el valor del hash principal
int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra);

//Calcula la cantidad de buckets en funcion de la cantidad de palabras
//y de los elementos por bucket
int calcularCantidadDeBuckets(int cantidadDePalabras);

//Verifica todas las palabras del universo esten en la tabla hash
int verificarTablaHash(TablaHash tablaHash, ListaDePalabras universo);

//Retorna verdadero si la palabra esta en la tabla hash y falso si no
int palabraEnTablaHash(TablaHash tablaHash, Palabra palabra);

//Levanta la tabla hash desde el archivo
TablaHash * cargarTablaHashDesdeArchivo(FILE * archivoDeTablaHash);

void liberarTablaHash(TablaHash * tablaHash);

#endif
