#include <locale.h>
#include <assert.h>
#include "archivo.h"
#include "tabla_hash.h"

int main(int argc, char const *argv[])
{
    setlocale(LC_CTYPE, "");

    FILE * archivo = abrirArchivo("listado-general.txt", "r");
    ListaDePalabras * universo = crearListaDePalabrasDesdeArchivo(archivo);
    TablaHash * tablaHash = crearTablaHash(universo), * tablaHash2;

    if (verificarTablaHash(*tablaHash, *universo)) {
        imprimirTablaHashEnArchivo(*tablaHash, "tabla-hash.txt");
    } else {
        printf("No se pudo hashear el universo\n");
    }

    tablaHash2 = cargarTablaHashDesdeArchivo("tabla-hash.txt");
    imprimirTablaHashEnArchivo(*tablaHash2, "tabla-hash2.txt");

    return 0;
}
