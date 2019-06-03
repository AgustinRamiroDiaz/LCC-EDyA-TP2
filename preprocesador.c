#include <locale.h>
#include "archivo.h"
#include "tabla_hash.h"
#include "constantes.h"

FILE * obtenerArchivoDeUniverso(int argc, char const *argv[])
{
    char const * nombreArchivoDeUniverso;

    if (argc > 1) {
        nombreArchivoDeUniverso = argv[1];
    } else {
        nombreArchivoDeUniverso = ARCHIVO_UNIVERSO_DEFECTO;
    }
    FILE * archivoDeUniverso = abrirArchivo((char *) nombreArchivoDeUniverso, "r");

    return archivoDeUniverso;
}

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");

    FILE * archivoDeUniverso = obtenerArchivoDeUniverso(argc, argv);
    FILE * archivoDeTablaHash = abrirArchivo((char *) ARCHIVO_TABLA_HASH, "w");

    ListaDePalabras * universo = crearListaDePalabrasDesdeArchivo(archivoDeUniverso);
    TablaHash * tablaHash = crearTablaHash(universo);

    if (verificarTablaHash(*tablaHash, *universo)) {
        imprimirTablaHashEnArchivo(*tablaHash, archivoDeTablaHash);
        wprintf(L"Se generó la tabla hash\n");
    } else {
        wprintf(L"No se pudo hashear el universo\n");
    }

    liberarListaDePalabras(universo);
    liberarTablaHash(tablaHash);
    cerrarArchivo(archivoDeUniverso);
    cerrarArchivo(archivoDeTablaHash);

    return 0;
}
