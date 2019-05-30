#include <locale.h>
#include <assert.h>
#include "archivo.h"
#include "tabla_hash.h"

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");
    char archivoDeEntrada[100] = "listado-general.txt", tablaHashGenerada[100] = "tabla-hash.txt", tablaHashGenerada2[100] = "tabla-hash2.txt";

    FILE * archivo = abrirArchivo(archivoDeEntrada, "r");
    ListaDePalabras * universo = crearListaDePalabrasDesdeArchivo(archivo);

    TablaHash * tablaHash = crearTablaHash(universo), * tablaHash2;


    if (verificarTablaHash(*tablaHash, *universo)) {
        imprimirTablaHashEnArchivo(*tablaHash, tablaHashGenerada);
        printf("Se genero el archivo: %s\n", tablaHashGenerada);
    } else {
        printf("No se pudo hashear el universo\n");
    }

    tablaHash2 = cargarTablaHashDesdeArchivo(tablaHashGenerada);

    wprintf(L"Comienzan las sugerencias\n");

    FILE * archivoTest = abrirArchivo("Test.txt", "r");
    corregirArchivo(archivoTest, *tablaHash2);

    printf("Fin del main\n");

    return 0;
}
