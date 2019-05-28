#include <math.h>
#include <malloc.h>
#include "tabla_hash.h"
#include "hash.h"
#include "archivo.h"
#include "constantes.h"

#include <assert.h>

TablaHash * crearTablaHash(ListaDePalabras * universo)
{
    TablaHash * tablaHash = armarTablaHash(universo->cantidad);
    ListaDePalabras ** gruposDePalabras = agruparPalabras(tablaHash, universo);

    crearBuckets(tablaHash, gruposDePalabras);

    return tablaHash;
}

TablaHash * armarTablaHash(int cantidadDePalabras)
{
    TablaHash * tablaHash = malloc(sizeof(TablaHash));
    tablaHash->tamano = calcularCantidadDeBuckets(cantidadDePalabras);
    tablaHash->buckets = malloc(sizeof(Bucket) * tablaHash->tamano);
    tablaHash->cantidadDeElementos = cantidadDePalabras;

    return tablaHash;
}

void crearBuckets(TablaHash * tablaHash, ListaDePalabras ** gruposDePalabras)
{
    for (int i=0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = crearBucket(*gruposDePalabras[i]);
    }
}

ListaDePalabras ** agruparPalabras(TablaHash * tablaHash, ListaDePalabras * listaDePalabras)
{
    Palabra * palabraActual;
    int numeroDeBucket;
    ListaDePalabras ** gruposDePalabras = crearGruposDePalabras(tablaHash, listaDePalabras);

    for (int i = 0; i < listaDePalabras->cantidad; i++) {
        palabraActual = listaDePalabras->palabras[i];
        numeroDeBucket = obtenerHashPrincipal(*tablaHash, *palabraActual);
        agregarPalabraALista(palabraActual, gruposDePalabras[numeroDeBucket]);
    }

    return gruposDePalabras;
}

ListaDePalabras ** crearGruposDePalabras(TablaHash * tablaHash, ListaDePalabras * palabras)
{
    int elementosPorBucket = palabras->cantidad / tablaHash->tamano + 1;
    ListaDePalabras ** gruposDePalabras = malloc(sizeof(ListaDePalabras*) * tablaHash->tamano);

    for (int i=0; i < tablaHash->tamano; i++) {
        gruposDePalabras[i] = armarListaDePalabras(elementosPorBucket);
    }

    return gruposDePalabras;
}

void imprimirTablaHashEnArchivo(TablaHash tablaHash, char * nombreDeArchivo)
{
    FILE * archivo = abrirArchivo(nombreDeArchivo, "w");

    fwprintf(archivo, L"%d %d\n", tablaHash.cantidadDeElementos, tablaHash.tamano);
    for (int i = 0; i < tablaHash.tamano; i++) {
        imprimirBucketEnArchivo(*tablaHash.buckets[i], archivo);
    }

    cerrarArchivo(archivo);
}

int obtenerHashPrincipal(TablaHash tablaHash, Palabra palabra)
{
    return funcionHashPrincipal(palabra) % tablaHash.tamano;
}

int calcularCantidadDeBuckets(int cantidadDePalabras)
{
    return cantidadDePalabras / ELEMENTOS_POR_BUCKET + 1;
}

int verificarTablaHash(TablaHash tablaHash, ListaDePalabras universo)
{
    int numeroDeBucket, esValida = 1;
    Palabra * palabraActual;

    for (int i = 0; i < universo.cantidad && esValida; i++) {
        palabraActual = universo.palabras[i];
        esValida = palabraEnTablaHash(tablaHash, *palabraActual);
    }

    return esValida;
}

int palabraEnTablaHash(TablaHash tablaHash, Palabra palabra)
{
    int numeroDeBucket = obtenerHashPrincipal(tablaHash, palabra);
    Bucket * bucket = tablaHash.buckets[numeroDeBucket];

    int posicionEnBucket = obtenerHashSecundario(*bucket, palabra);
    Palabra * palabraEncontrada = bucket->tablaHash[posicionEnBucket];

    return sonPalabrasIguales(palabra, *palabraEncontrada);
}

TablaHash * cargarTablaHashDesdeArchivo(char * nombreDeArchivo)
{
    int cantidadDeElementos, cantidadDeBuckets;

    FILE * archivo = abrirArchivo(nombreDeArchivo, "r");
    fwscanf(archivo, L"%d %d", &cantidadDeElementos ,&cantidadDeBuckets);
    TablaHash * tablaHash = armarTablaHash(cantidadDeElementos);

    for (int i = 0; i < tablaHash->tamano; i++) {
        tablaHash->buckets[i] = cargarBucketDesdeArchivo(archivo);
    }

    cerrarArchivo(archivo);

    return tablaHash;
}

ListaDePalabras * generarSugerencias(Palabra palabra, TablaHash tablaHash)
{
    ListaDePalabras * listaDeSugerencias = armarListaDePalabras(TAMANO_INICIAL_LISTA_SUGERENCIAS);

    Palabra * palabraCopiada;

    for (size_t i = 0;i < palabra.longitud - 1; i++) {
        palabraCopiada = copiarPalabra(palabra);
        intercambiarLetras(palabraCopiada, i, i + 1);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
    }

    for (size_t caracter = 65; i < 91; i++) {
        for (size_t i = 1; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            agregarLetra(palabraCopiada, caracter, i);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
        for (size_t i = 0; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, caracter, i);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }

    for (size_t i = 0; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        eliminarLetra(palabraCopiada, i);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
    }

    listaDePalabras * dobleSugerencia;
    for (size_t i = 1; i < palabra.longitud; i++) {
        palabraCopiada = copiarPalabra(palabra);
        dobleSugerencia = separarPalabra(palabraCopiada, i);
        if(palabraEnTablaHash(dobleSugerencia->palabras[0], tablaHash)
         &&palabraEnTablaHash(dobleSugerencia->palabras[1], tablaHash)){
             palabraCopiada = crearPalabra(wcscat(dobleSugerencia->palabras[0], dobleSugerencia->palabras[1]));
             agregarPalabraALista(palabra, listaDeSugerencias);
         } else {
             liberarPalabra(palabraCopiada);
         }
    }
}

void sugerirOLiberar(TablaHash tablaHash, Palabra * palabra, listaDeSugerencias * listaDeSugerencias)
{
    if (palabraEnTablaHash(tablaHash, palabra)){
        agregarPalabraALista(palabra, listaDeSugerencias);
    } else {
        liberarPalabra(palabra);
    }
}
