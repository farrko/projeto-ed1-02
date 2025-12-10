#ifndef NODES_H
#define NODES_H

typedef struct node_t node_t;
typedef void (*destructor)(void *);

/** @brief    Inicialização de um node.
  *
  * @param    value       Pointer para o valor guardado pelo node.
  * @param    destructor  Function pointer para uma função que destrua o valor em value.
  *
  * @return   Um node que contém valor, destrutor e nenhum link para outro node.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
node_t *node_init(void *value, void (*destructor)(void*));



/** @brief    Define o valor associado a um node.
  *
  * @param    node  Pointer para um node.
  * @param    value Novo valor a ser armazenado no node.
  */
void node_set_value(node_t *node, void *value);



/** @brief    Define o destrutor associado ao valor de um node.
  *
  * @param    node        Pointer para um node.
  * @param    destructor  Função responsável por destruir o valor apontado pelo node.
  */
void node_set_destructor(node_t *node, void (*destructor)(void *));



/** @brief    Define o ponteiro para o node à esquerda (left pointer).
  *
  * @param    node Pointer para um node.
  * @param    lpt  Pointer para o node à esquerda.
  */
void node_set_lpt(node_t *node, node_t *lpt);



/** @brief    Define o ponteiro para o node à direita (right pointer).
  *
  * @param    node Pointer para um node.
  * @param    rpt  Pointer para o node à direita.
  */
void node_set_rpt(node_t *node, node_t *rpt);



/** @brief    Obtém o valor armazenado em um node.
  *
  * @param    node Pointer para um node.
  *
  * @return   O valor armazenado no node.
  */
void *node_get_value(node_t *node);



/** @brief    Obtém o destrutor associado ao node.
  *
  * @param    node Pointer para um node.
  *
  * @return   O destrutor do node.
  */
destructor node_get_destructor(node_t *node);



/** @brief    Obtém o node apontado pelo left pointer.
  *
  * @param    node Pointer para um node.
  *
  * @return   O node à esquerda.
  */
node_t *node_get_lpt(node_t *node);



/** @brief    Obtém o node apontado pelo right pointer.
  *
  * @param    node Pointer para um node.
  *
  * @return   O node à direita.
  */
node_t *node_get_rpt(node_t *node);



/** @brief    Destrói um node, incluindo o valor associado caso um destrutor tenha sido definido.
  *
  * @param    node Pointer para um node.
  */
void node_destroy(node_t *node);

#endif
