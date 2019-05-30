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

    if(bucket->capacidad == 0) return 0;

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


    for (int i = 0;i < palabra.longitud - 1; i++) {
        palabraCopiada = copiarPalabra(palabra);
        intercambiarLetras(palabraCopiada, i, i + 1);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
    }


    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        wprintf(L"Ale was here\n");
        for (int i = 1; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            agregarLetra(palabraCopiada, caracter, i);
            wprintf(L"palabra copiada: %ls\n", palabraCopiada->letras);
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
    }

    return listaDeSugerencias;
}


void sugerirOLiberar(TablaHash tablaHash, Palabra * palabra, ListaDePalabras * listaDeSugerencias)
{
    if (palabraEnTablaHash(tablaHash, *palabra)){
        agregarPalabraALista(palabra, listaDeSugerencias);
    } else {
        liberarPalabra(palabra);
    }
}

void corregirArchivo(FILE * archivo, TablaHash tablaHash)
{
    wchar_t caracter = fgetwc(archivo), buffer[LARGO_MAXIMO_PALABRA];
    Palabra * palabraActual;
    int contadorDeLineas = 1;
    for (size_t i = 0; caracter != WEOF; i++) {
        wprintf(L"%lc\n", caracter);
        buffer[i] = caracter;
        if (caracter == L':' || caracter == L';' || caracter == L',' || caracter == L'.' || caracter == L'?' || caracter == L'!') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            fgetwc(archivo);
            i = 0;
        }
        if (caracter == L' ') {
            wprintf(L"Termina la palabra\n");
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            i = 0;

            wprintf(L"salio?\n");
        }
        if (caracter == L'\n' || caracter == L'\r') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            contadorDeLineas++;
            i = 0;
        }

        caracter = fgetwc(archivo);
    }
}

void imprimirSugerencias(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias)
{
    wprintf(L"Linea %d, \"%ls\" no esta en el diccionario.\n", linea, palabra.letras);
    wprintf(L"Quizas quiso decir: ");
    for (size_t i = 0; i < listaDeSugerencias.cantidad; i++) {
        wprintf(L"%ls ", listaDeSugerencias.palabras[i]);
    }
    wprintf(L"\n");
}
