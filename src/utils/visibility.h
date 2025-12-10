#ifndef VISIBILITY_H
#define VISIBILITY_H

#include "../shapes/polygon.h"
#include "../datast/linkedlist.h"
#include "../shapes/point.h"

/** @brief    Gera um polígono que representa a região de visibilidade a partir de um ponto no espaço.
  *
  * @param    barriers    Uma lista de barreiras que impedem o campo de visão
  * @param    origin      O ponto de origem, de onde será calculada a região de visibilidade.
  * @param    sort_type   Um caractere representando o tipo de algoritmo de sorting a ser utilizado (q = quicksort, m = mixed mergesort)
  * @param    threshold   Valor a ser utilizado como threshold para o mixed mergesort (ver sorting.h)
  *
  * @return   Um polígono que representa a região de visibilidade.
  */
polygon_t *generate_visibility_polygon(llist_t *barriers, point_t *origin, char sort_type, size_t threshold);

#endif
