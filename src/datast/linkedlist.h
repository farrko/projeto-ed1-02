#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>

typedef struct llist_t llist_t;
typedef struct node_t node_t;

/** @brief    Inicialização de um node.
  *
  * @param    value       Pointer para o valor guardado pelo node.
  * @param    destructor  Function pointer para uma função que destrua o valor em value.
  *
  * @return   Um node que contém valor, destrutor e nenhum link para outro node.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
node_t *node_init(void *value, void (*destructor)(void*));

/** @brief    Obter o valor guardado em um node.
  *
  * @param    node  Node a qual será obtido o valor.
  *
  * @return   Um void pointer para o valor do node.
  */
void *node_getvalue(node_t *node);

/** @brief    Destrói um node e o seu valor associado.
  * 
  * @param    node  O node a ser destruído.
  */
void node_destroy(node_t *node);

/** @brief    Inicialização de uma linked list.
  *
  * @return   Uma instância de uma linked list.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
llist_t *llist_init();

/** @brief    Destrói uma linked list por completo, incluindo seus nodes e valores associados.
  * 
  * @param    llist Pointer para uma linked list.
  */
void llist_destroy(llist_t *llist);


/** @brief    Obtém o node da head de uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  *
  * @return   O node apontado pela head da linked list.
  */
node_t *llist_get_head(llist_t *llist);

/** @brief    Obtém o node da tail de uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  *
  * @return   O node apontado pela tail da linked list.
  */
node_t *llist_get_tail(llist_t *llist);

/** @brief    Obtém a quantidade de elementos em uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  *
  * @return   A quantidade de elementos da linked list.
  */
size_t llist_get_length(llist_t *llist);



/** @brief    Obtém o node no index especificado de uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  * @param    index Index do node desejado.
  *
  * @return   O node na posição especificada.
  * @warning  Caso o index especificado não estiver dentro do tamanho da linked list, ou a mesma for vazia, o valor de retorno será NULL.
  */
node_t *llist_getat_index(llist_t *llist, size_t index);



/** @brief    Remove o node no index especificado de uma linked list e o retorna.
  *
  * @param    llist Pointer para uma linked list.
  * @param    index Index do node desejado.
  *
  * @return   O node na posição especificada.
  * @warning  Caso o index especificado não estiver dentro do tamanho da linked list, ou a mesma for vazia, o valor de retorno será NULL.
  */
node_t *llist_popat_index(llist_t *llist, size_t index);

/** @brief    Remove o node no começo da linked list e o retorna.
  *
  * @param    llist Pointer para uma linked list.
  *
  * @return   O node do começo da linked list.
  * @warning  Caso o index especificado não estiver dentro do tamanho da linked list, ou a mesma for vazia, o valor de retorno será NULL.
  */
node_t *llist_popat_start(llist_t *llist);

/** @brief    Remove o node no final da linked list e o retorna.
  *
  * @param    llist Pointer para uma linked list
  *
  * @return   O node no final da linked list.
  * @warning  Caso o index especificado não estiver dentro do tamanho da linked list, ou a mesma for vazia, o valor de retorno será NULL.
  */
node_t *llist_popat_end(llist_t *llist);



/** @brief    Insere um node na posição especificada em uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  * @param    node  Node a ser inserido.
  * @param    index Posição na qual será inserido o node.
  *
  * @warning  O node não será inserido caso o index especificado seja maior do que o tamanho atual da lista.
  * @warning  Ao inserir um node no index I, os elementos daquele ponto em diante serão movidos para a direita. Ou seja, dado o index de inserção I, o elemento atualmente na posição I passará a ser o elemento na posição I + 1 após a inserção.
  */
void llist_insertat_index(llist_t *llist, node_t *node, size_t index);

/** @brief    Insere um node na primeira posição de uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  * @param    node  Node a ser inserido.
  *
  * @warning  Esta função irá deslocar todos os elementos atualmente na lista por uma posição para a direita.
  */
void llist_insertat_start(llist_t *llist, node_t *node);

/** @brief    Insere um node na última posição de uma linked list.
  *
  * @param    llist Pointer para uma linked list.
  * @param    node  Node a ser inserido.
  */
void llist_insertat_end(llist_t *llist, node_t *node);


/** @brief    Destrói um node na posição especificada.
  *
  * @param    llist Pointer para uma linked list.
  * @param    index Posição na qual o node será destruído.
  */
void llist_destroyat(llist_t *llist, size_t index);

#endif // !LINKEDLIST_H
