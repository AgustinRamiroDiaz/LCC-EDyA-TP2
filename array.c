#include "array.h"

//FUNCIONES SOBRE ARRAY
int hay_colisiones(int array[], int tamano){
  for (size_t i = 0; i < tamano; i++)
  {
    if (array[i] > 1) return 1;
  }
  return 0;
}

void cero_array(int array[], int tamano){
  for (size_t i = 0; i < tamano; array[i] = 0, i++);
}

void imprimir_matriz(wchar_t** listaPalabras, int cantidadPalabras){
  for (size_t i = 0; i < cantidadPalabras; i++) {
    wprintf(L"%ls\n", listaPalabras[i]);
  }
}
