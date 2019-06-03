#include <assert.h>
#include "archivo.h"
#include "constantes.h"

FILE * abrirArchivo(char * nombreDeArchivo, char * modo)
{
    FILE * archivo = fopen(nombreDeArchivo, modo);
    assert(NULL != archivo);

    return archivo;
}

void cerrarArchivo(FILE * archivo)
{
    fclose(archivo);
}

int esLetra(wchar_t caracter)
{
    return esLetraNormal(caracter) || esLetraEspecial(caracter);
}

int esLetraNormal(wchar_t caracter)
{
    int esLetraMinuscula = L'a' <= caracter && caracter <= L'z';
    int esLetraMayuscula = L'A' <= caracter && caracter <= L'Z';

    return esLetraMinuscula || esLetraMayuscula;
}

int esLetraEspecial(wchar_t caracter)
{
    int encontrado = 0, cantidadDeCaracteres = wcslen(LETRAS_ESPECIALES);

    for (int i = 0; i < cantidadDeCaracteres && !encontrado; i++) {
        encontrado = (caracter == LETRAS_ESPECIALES[i]);
    }

    return encontrado;
}

int esFinDePalabra(wchar_t caracter)
{
    int encontrado = 0, cantidadDeCaracteres = wcslen(SIGNOS_ESPECIALEs);

    for (int i = 0; i < cantidadDeCaracteres && !encontrado; i++) {
        encontrado = (caracter == SIGNOS_ESPECIALEs[i]);
    }

    return (encontrado || esFinDeLinea(caracter));
}

int esFinDeLinea(wchar_t caracter)
{
    int encontrado = 0, cantidadDeCaracteres = wcslen(SALTOS_DE_LINEA);

    for (int i = 0; i < cantidadDeCaracteres && !encontrado; i++) {
        encontrado = (caracter == SALTOS_DE_LINEA[i]);
    }

    return (encontrado || esFinDeArchivo(caracter));
}

int esFinDeArchivo(wchar_t caracter)
{
    return WEOF == caracter;
}
