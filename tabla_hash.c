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
    printf("Tamano tablaHash: %d\n\n", tablaHash->tamano);

    for (int i=0; i < tablaHash->tamano; i++) {
        //printf("Bucket numero: %d\n", i);
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

    wchar_t aux1, aux2;
    Palabra * palabraCopiada;
    palabraCopiada = copiarPalabra(palabra);

    for (int i = 0;i < palabra.longitud - 1; i++) {
        intercambiarLetras(palabraCopiada, i, i + 1);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        intercambiarLetras(palabraCopiada, i, i + 1);
    }


    for (wchar_t caracter = L'a'; caracter <= L'z'; caracter++) {
        palabraCopiada = copiarPalabra(palabra);
        agregarLetra(palabraCopiada, caracter, 1);
        for (int i = 1; i < palabra.longitud; i++) {
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
            intercambiarLetras(palabraCopiada, i, i + 1);
        }

        for (int i = 0; i < palabra.longitud; i++) {
            palabraCopiada = copiarPalabra(palabra);
            reemplazarLetra(palabraCopiada, caracter, i);
            sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        }
    }

    palabraCopiada = copiarPalabra(palabra);
    aux1 = eliminarLetra(palabraCopiada, 0);
    for (int i = 0; i < palabra.longitud; i++) {
        wprintf(L"Palabra: %ls\n", palabraCopiada->letras);
        sugerirOLiberar(tablaHash, palabraCopiada, listaDeSugerencias);
        aux2 = aux1;
        aux1 = palabraCopiada->letras[i];
        palabraCopiada->letras[i] = aux2;
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

    // for (size_t profundiad = 0; listaDeSugerencias->cantidad < 5; profundiad++) {
    //     for (size_t i = 0; i < profundiad; i++) {
    //         /* code */
    //     }
    // }

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
        buffer[i] = caracter;
        if (caracter == L':' || caracter == L';' || caracter == L',' || caracter == L'.' || caracter == L'?' || caracter == L'!') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            if (!palabraEnTablaHash(tablaHash, *palabraActual))
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            fgetwc(archivo);
            i = -1;
        }
        if (caracter == L' ') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            if (!palabraEnTablaHash(tablaHash, *palabraActual))
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            i = -1;
        }
        if (caracter == L'\n' || caracter == L'\r') {
            buffer[i] = L'\0';
            palabraActual = crearPalabra(buffer);
            if (!palabraEnTablaHash(tablaHash, *palabraActual))
            imprimirSugerencias(*palabraActual, contadorDeLineas, *generarSugerencias(*palabraActual, tablaHash));
            contadorDeLineas++;
            i = -1;
        }

        caracter = fgetwc(archivo);
    }
}

void imprimirSugerencias(Palabra palabra, int linea, ListaDePalabras listaDeSugerencias)
{
    wprintf(L"Linea %d, \"%ls\" no esta en el diccionario.\n", linea, palabra.letras);
    wprintf(L"Quizas quiso decir: ");
    for (size_t i = 0; i < listaDeSugerencias.cantidad; i++) {
        wprintf(L"%ls ", listaDeSugerencias.palabras[i]->letras);
    }
    wprintf(L"\n\n");
}
