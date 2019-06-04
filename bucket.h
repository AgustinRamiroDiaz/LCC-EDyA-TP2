#ifndef BUCKET
#define BUCKET

#include "palabra.h"

/*
Bucket es una estructura que utilizamos para generar el hash perfecto.
Comenzamos con las palabras de nuestro universo, a las cuales
les aplicamos la funcion principal de hasheo.
Las palabras que tengan el mismo hash quedan agrupadas.
Mediante la iteracion sobre constantes de hasheo generamos un bucket para cada
lista de palabras en el cual el hash es perfecto.

Bucket almacena:
    una constanteDeHasheo para utilizar en la funcion de hasheo secundaria
    la capacidad maxima de palabras del bucket
    el tamano de la tabla hash
    la tabla hash que representamos como una lista de palabras
donde la posicion representa el hasheo de la palabra

Nota: Con el uso de buckets obtenemos un hash perfecto para cada lista de palabras
y ademas como el tamano es igual a la capacidad
obtenemos un hashing minimal, lo cual lo convierte en un hash perfecto minimal
*/
typedef struct {
    unsigned long constanteDeHasheo;
    int tamano, capacidad;
    Palabra ** tablaHash;
} Bucket;

// Crea un bucket que contenga a todas las palabras de la lista
Bucket * crearBucket(ListaDePalabras listaDePalabras);

// Arma un bucket vacio con la memoria reservada para la cantidad de palabras
Bucket * armarBucket(int cantidadDePalabras);

// Calcula el tamano del bucket en base a la cantidad de palabras
int calcularTamanoDeBucket(int cantidadDePalabras);

// Llena la tabla hash del bucket a partir de la lista de palabras,
// eligiendo una constante de hasheo de forma que sus hashes no colisionen
void llenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras);

// Intenta llenar la tabla hash del bucket con la constante de hasheo
// sin que haya colisiones de la lista de palabras. Devuelve veradero si 
// pudo y falso si no
int probarLlenarBucket(Bucket * bucket, ListaDePalabras listaDePalabras, unsigned long constanteDeHasheo);

// Limpia el bucket poniendo en null las palabras de su tabla hash
void limpiarBucket(Bucket * bucket);

// Obtiene el hash secundario de la palabra con la constante de hasheo del bucket
int obtenerHashSecundario(Bucket bucket, Palabra palabra);

// Imprime en el archivo la información necesaria sobre el bucket
// para luego poder levantarlo desde el mismo
void imprimirBucketEnArchivo(Bucket bucket, FILE * archivo);

// Levanta un bucket desde el archivo
Bucket * cargarBucketDesdeArchivo(FILE * archivo);

// Guarda las palabras del archivo en la tabla hash del bucket
void cargarPalabrasEnBucketDesdeArchivo(Bucket * bucket, FILE * archivo);

// Libera la memoria reservada por las palabras de la tabla hash del bucket
void liberarPalabrasEnBucket(Bucket * bucket);

// Libera la memoria reservada por el bucket
void liberarBucket(Bucket * bucket);

#endif
