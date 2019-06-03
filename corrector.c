#include <locale.h>
#include "sugerencias.h"
#include "archivo.h"
#include "tabla_hash.h"
#include "constantes.h"

FILE * obtenerArchivoDeEntrada(int argc, char const *argv[])
{
    char * nombreArchivoDeEntrada;

    if (argc > 1) {
        nombreArchivoDeEntrada = argv[1];
    } else {
        nombreArchivoDeEntrada = ARCHIVO_TEXTO_ENTRADA_POR_DEFECTO;
    }
    FILE * archivoDeEntrada = abrirArchivo(nombreArchivoDeEntrada, "r");

    return archivoDeEntrada;
}

FILE * obtenerArchivoDeCorrecciones(int argc, char const *argv[])
{
    char * nombreArchivoDeCorrecciones;

    if (argc > 2) {
        nombreArchivoDeCorrecciones = argv[2];
    } else {
        nombreArchivoDeCorrecciones = ARCHIVO_CORRECCIONES_POR_DEFECTO;
    }
    FILE * archivoDeCorrecciones = abrirArchivo(nombreArchivoDeCorrecciones, "w");

    return archivoDeCorrecciones;
}

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");

    FILE * archivoDeTablaHash = abrirArchivo(ARCHIVO_TABLA_HASH, "r");
    FILE * archivoDeEntrada = obtenerArchivoDeEntrada(argc, argv);
    FILE * archivoDeCorrecciones = obtenerArchivoDeCorrecciones(argc, argv);

    TablaHash * tablaHash = cargarTablaHashDesdeArchivo(archivoDeTablaHash);

    corregirArchivo(archivoDeEntrada, archivoDeCorrecciones, *tablaHash);

    cerrarArchivo(archivoDeTablaHash);
    cerrarArchivo(archivoDeEntrada);
    cerrarArchivo(archivoDeCorrecciones);

    return 0;
}