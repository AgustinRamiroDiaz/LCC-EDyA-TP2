#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include <assert.h>

//FUNCIONES HASH
typedef unsigned long (*FuncionHash) (wchar_t *str);

typedef unsigned long (*FuncionHashKey) (wchar_t *str, int key);

unsigned long
djb2(wchar_t *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

unsigned long
hashxd(wchar_t *str, int key)
{
  unsigned long hash = key;
  int c;

  while (c = *str++){
    hash = (hash ^ c) + ((hash<<26)+(hash>>6));
  }

  return hash;
}

unsigned long
sdbm(wchar_t *str)
{
  unsigned long hash = 0;
  int c;

  while (c = *str++)
      hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

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


//BUCKET
typedef struct {
  wchar_t** listaPalabras;
  int cantidadPalabras;
  int maxPalabras;
} _bucket;

typedef _bucket* Bucket;

Bucket bucket_crear(int maxPalabras){
  Bucket bucket = malloc(sizeof(_bucket));
  bucket->cantidadPalabras = 0;
  bucket->maxPalabras = maxPalabras;
  bucket->listaPalabras = malloc(sizeof(wchar_t*) * maxPalabras);
  return bucket;
}

void bucket_insertar(Bucket bucket, wchar_t* palabra){
  if (bucket->cantidadPalabras == bucket->maxPalabras){
    bucket->maxPalabras *= 2;
    bucket->listaPalabras = realloc(bucket->listaPalabras, sizeof(wchar_t*) * bucket->maxPalabras);
  }
  wcscpy(bucket->listaPalabras[bucket->cantidadPalabras], palabra);
  bucket->cantidadPalabras++;
}

void bucket_imprimir(Bucket bucket){
  for (size_t i = 0; i < bucket->cantidadPalabras; i++) {
    wprintf(L"%ls\n", bucket->listaPalabras[i]);
  }
}

//Guarda todas las palabras dentro de los buckets correspondientes a aplicarles el hasheo
void hash_palabras_a_buckets(FuncionHash hash, wchar_t** listaPalabras, int cantidadPalabrasEntrada, Bucket* bucketArray, int bucketArraySize){
  int palabraHasheada;
  for (size_t i = 0; i < cantidadPalabrasEntrada; i++) {
    palabraHasheada = hash(listaPalabras[i]) % bucketArraySize;
    bucket_insertar(bucketArray[palabraHasheada], listaPalabras[i]);
    printf("%d\n", i);
    //wprintf(L"%ls", listaPalabras[i]);
  }
}

int buscar_key_de_bucket(FuncionHashKey hashKey, Bucket bucket){
  int arrayDestinoTamano = pow(bucket->cantidadPalabras, 2), arrayDestino[arrayDestinoTamano], palabraHasheada;
  for (int key = 0; ; key++) {
    cero_array(arrayDestino, arrayDestinoTamano);
    for (size_t i = 0; i < bucket->cantidadPalabras; i++) {
      palabraHasheada = hashKey(bucket->listaPalabras[i], key);
      arrayDestino[palabraHasheada]++;
    }
    if (!hay_colisiones(arrayDestino, arrayDestinoTamano)) return key;
  }
  assert(0);
}

//La mejor funcion del mundo NP
void buscar_keys(FuncionHashKey hashKey, Bucket* bucketArray, int bucketArraySize, int* keyArray){
  for (size_t i = 0; i < bucketArraySize; i++) {
    keyArray[i] = buscar_key_de_bucket(hashKey, bucketArray[i]);
  }
}


//FUNCIONES SOBRE ARCHIVOS
void archivo_a_matriz(char* nombreArchivoEntrada, wchar_t** listaPalabras){
  FILE* inputFile = fopen(nombreArchivoEntrada, "r");
  wchar_t buff[69];

  for(int i = 0; fwscanf(inputFile, L"%ls", buff) == 1; i++){
    listaPalabras[i] = malloc(sizeof(wchar_t) * (wcslen(buff)+1));
    wcscpy(listaPalabras[i], buff);
  }

  fclose(inputFile);
}

void imprimir_matriz(wchar_t** listaPalabras, int cantidadPalabras){
  for (size_t i = 0; i < cantidadPalabras; i++) {
    wprintf(L"%ls\n", listaPalabras[i]);
  }
}

int main(int argc, char const *argv[])
{
  setlocale(LC_CTYPE, "");

  int cantidadPalabrasEntrada = 80379;

  int bucketArraySize = 10000;
  Bucket bucketArray[bucketArraySize];
  for (size_t i = 0; i < bucketArraySize; bucketArray[i] = bucket_crear(20000), i++);

  int keyArray[bucketArraySize];

  wchar_t* listaPalabras[cantidadPalabrasEntrada];

  //Guardamos las palabras del diccionario dado en listaPalabras
  archivo_a_matriz("listado-general.txt", listaPalabras);

  //Guardamos las palabras en sus correspondientes buckets
  hash_palabras_a_buckets(&djb2, listaPalabras, cantidadPalabrasEntrada, bucketArray, bucketArraySize);

  buscar_keys(&hashxd, bucketArray, bucketArraySize, keyArray);

  printf("Tu princesa esta en otro castillo\n");

  return 0;
}
