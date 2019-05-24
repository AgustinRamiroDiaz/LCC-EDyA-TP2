#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include "bucket.h"
#include "hash.h"

//FUNCIONES SOBRE ARCHIVOS
void archivo_a_matriz(char* nombreArchivoEntrada, wchar_t** listaPalabras){
  FILE* inputFile = fopen(nombreArchivoEntrada, "r");
  wchar_t buff[69];

  for(int i = 0; fwscanf(inputFile, L"%ls", buff) == 1; i++){
    listaPalabras[i] = malloc(sizeof(wchar_t) * (wcslen(buff) + 1));
    assert(listaPalabras[i] != NULL);
    wcscpy(listaPalabras[i], buff);
  }
  fclose(inputFile);
}


int main(int argc, char const *argv[])
{
  setlocale(LC_CTYPE, "");

  int cantidadPalabrasEntrada = 80379;

  int bucketArraySize = 10000;
  Bucket bucketArray[bucketArraySize];
  for (size_t i = 0; i < bucketArraySize; bucketArray[i] = bucket_crear(10000), i++);

  int keyArray[bucketArraySize];

  hash_archivo_a_buckets(&djb2, "listado-generado.txt", bucketArray, bucketArraySize);

  for (size_t i = 0; i < bucketArraySize; i++) {
    bucket_imprimir(bucketArray[i]);
    // printf("%d\n", keyArray[i]);
  }

  printf("Facu was here\n");

  buscar_keys(&hashxd, bucketArray, bucketArraySize, keyArray);

  printf("Tu princesa esta en otro castillo\n");

  return 0;
}
