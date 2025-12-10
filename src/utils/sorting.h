#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>



/** @brief    Ordena uma array utilizando insertion sort.
  *
  * @param    array    Array a ser ordenada.
  * @param    length   Quantidade de elementos na array.
  * @param    size     Tamanho, em bytes, de cada elemento da array.
  * @param    compare  Função utilizada para comparar dois elementos.
  *
  * @warning  A função assume que o array contém (length) elementos válidos.
  */
void insertion_sort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *));



/** @brief    Ordena uma array utilizando mergesort.
  *
  * @param    array    Array a ser ordenada.
  * @param    length   Quantidade de elementos na array.
  * @param    size     Tamanho, em bytes, de cada elemento da array.
  * @param    compare  Função utilizada para comparar dois elementos.
  *
  * @warning  Esta função realiza alocações internas para combinar subarrays durante o merge.
  */
void mergesort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *));



/** @brief    Variante híbrida do mergesort que utiliza insertion sort para subarrays pequenos.
  *
  * @param    array      Array a ser ordenada.
  * @param    length     Quantidade de elementos na array.
  * @param    size       Tamanho, em bytes, de cada elemento da array.
  * @param    compare    Função utilizada para comparar dois elementos.
  * @param    threshold  Menor tamanho possível para uma subarray. A partir de (threshold), o algoritmo se utilizará de
  *                      insertion sort para ordenar a subarray.
  *
  * @warning  Quando o tamanho de um subarray é menor ou igual a threshold, a função aplica insertion sort,
  *           ao invés de continuar a divisão recursiva do mergesort.
  */
void mixed_mergesort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *), size_t threshold);

#endif
