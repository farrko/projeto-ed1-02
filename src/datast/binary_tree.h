#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "nodes.h"
#include <stdbool.h>

typedef struct bitree_t bitree_t;



/** @brief    Inicializa uma binary tree.
  *
  * @param    root     Node inicial da árvore. Pode ser NULL para uma árvore vazia.
  * @param    compare  Função responsável por comparar dois valores armazenados nos nodes.
  *
  * @return   Uma instância de binary tree.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
bitree_t *bt_init(node_t *root, int (*compare)(const void *, const void *));



/** @brief    Destrói uma binary tree por completo, incluindo todos os nodes e valores associados.
  *
  * @param    bt Pointer para uma binary tree.
  */
void bt_destroy(bitree_t *bt);



/** @brief    Insere um node na binary tree.
  *
  * @param    bt    Pointer para uma binary tree.
  * @param    node  Node a ser inserido.
  *
  * @warning  A inserção segue o critério definido pela função compare fornecida na inicialização.
  */
void bt_insert_node(bitree_t *bt, node_t *node);



/** @brief    Busca um node contendo um valor específico.
  *
  * @param    bt     Pointer para uma binary tree.
  * @param    value  Valor a ser buscado.
  *
  * @return   O node correspondente ao valor informado.
  * @warning  Caso o valor não seja encontrado, o valor de retorno será NULL.
  */
node_t *bt_search_node(bitree_t *bt, void *value);



/** @brief    Remove um node contendo o valor especificado.
  *
  * @param    bt     Pointer para uma binary tree.
  * @param    value  Valor do node a ser removido.
  *
  * @warning  Se o valor não existir na tree, nenhuma remoção será realizada.
  */
void bt_remove_node(bitree_t *bt, void *value);



/** @brief    Obtém o node raiz da binary tree.
  *
  * @param    bt Pointer para uma binary tree.
  *
  * @return   A raiz da binary tree.
  */
node_t *bt_get_root(bitree_t *bt);

#endif
