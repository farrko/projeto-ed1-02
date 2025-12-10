#ifndef POLYGON_H
#define POLYGON_H

#include "../datast/linkedlist.h"
#include "point.h"
#include "shapes.h"
#include <stdbool.h>

typedef struct polygon_t polygon_t;



/** @brief    Inicializa um polígono vazio.
  *
  * @return   Uma instância de polygon_t sem vértices.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
polygon_t *py_init();



/** @brief    Destrói um polígono, incluindo todos os vértices armazenados.
  *
  * @param    py Pointer para um polígono.
  */
void py_destroy(polygon_t *py);



/** @brief    Adiciona um vértice ao final da lista de vértices de um polígono.
  *
  * @param    py     Pointer para um polígono.
  * @param    point  Ponto que será inserido como vértice.
  *
  * @warning  Esta função realiza uma cópia do ponto, então ela não se torna a dona do ponto original.
  */
void py_add_vertex(polygon_t *py, point_t *point);



/** @brief    Obtém a lista de vértices de um polígono.
  *
  * @param    py Pointer para um polígono.
  *
  * @return   Uma linked list contendo todos os vértices na ordem armazenada.
  */
llist_t *py_get_vertices(polygon_t *py);



/** @brief    Verifica se um polígono possui overlap com um shape.
  *
  * @param    py     Pointer para um polígono.
  * @param    shape  Pointer para um shape.
  *
  * @return   true caso exista interseção; false caso contrário.
  */
bool polygon_overlap(polygon_t *py, shape_t *shape);

#endif
