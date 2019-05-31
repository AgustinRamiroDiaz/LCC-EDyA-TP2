#include "archivo.h"
#include "constantes.h"

FILE * abrirArchivo(char * nombreDeArchivo, char * modo)
{
    return fopen(nombreDeArchivo, modo);
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
    int encontrado = 0;
    wchar_t caracteresPosibles[7] = {L' ', L':', L';', L',', L'.', L'?', L'!'};

    for (int i = 0; i < 6 && !encontrado; i++) {
        encontrado = (caracter == caracteresPosibles[i]);
    }
    
    return (encontrado || esFinDeLinea(caracter));
}

int esFinDeLinea(wchar_t caracter)
{
    int encontrado = 0;
    wchar_t caracteresPosibles[2] = {L'\n', L'\r'};

    for (int i = 0; i < 2 && !encontrado; i++) {
        encontrado = (caracter == caracteresPosibles[i]);
    }

    return (encontrado || esFinDeArchivo(caracter));
}

int esFinDeArchivo(wchar_t caracter)
{
    return WEOF == caracter;
}
