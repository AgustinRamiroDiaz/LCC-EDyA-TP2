#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include "tabla_hash.h"


int main(int argc, char const *argv[])
{
    setlocale(LC_CTYPE, "");

    FILE * archivo = fopen("listado-general.txt", "r");
    ListaDePalabras * universo = crearListaDePalabrasDesdeArchivo(archivo);
    TablaHash * tablaHash = crearTablaHash(universo);

    imprimirTablaHashEnArchivo(*tablaHash, "tabla-hash.txt");

    return 0;
}
