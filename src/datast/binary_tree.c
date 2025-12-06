#include "binary_tree.h"
#include "nodes.h"

#include <stdlib.h>
#include <stdio.h>

struct bitree_t {
  node_t *root;
  int (*compare)(const void *, const void *);
};

static void node_free_rec(node_t *node) {
  if (node == NULL) return;

  node_free_rec(node_get_lpt(node));
  node_free_rec(node_get_rpt(node));
  node_destroy(node);
}

bitree_t *bt_init(node_t *root, int (*compare)(const void *, const void *)) {
  bitree_t *bt = malloc(sizeof(bitree_t));
  if (bt == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  bt->root = root;
  bt->compare = compare;
  return bt;
}

void bt_destroy(bitree_t *bt) {
  node_free_rec(bt->root);
  free(bt);
}

void bt_insert_node(bitree_t *bt, node_t *node) {
  node_set_lpt(node, NULL);
  node_set_rpt(node, NULL);

  if (bt->root == NULL) {
    bt->root = node;
    return;
  }

  node_t *prev_node = NULL;
  node_t *current_node = bt->root;

  while(current_node != NULL) {
    int compare_result = bt->compare(node_get_value(node), node_get_value(current_node));

    if (!compare_result) {
      printf("Erro na inserção de nodes na árvore: nodes iguais.\n");
      return;
    }

    prev_node = current_node;

    if (compare_result < 0) {
      current_node = node_get_lpt(current_node);
    } else current_node = node_get_rpt(current_node);
  }

  if (bt->compare(node_get_value(node), node_get_value(prev_node)) < 0) {
    node_set_lpt(prev_node, node);
  } else node_set_rpt(prev_node, node);
}

node_t *bt_search_node(bitree_t *bt, void *value) {
  node_t *current_node = bt->root;

  while(current_node != NULL) {
    int compare_result = bt->compare(value, node_get_value(current_node));

    if (!compare_result) return current_node;

    if (compare_result < 0) {
      current_node = node_get_lpt(current_node);
    } else current_node = node_get_rpt(current_node);
  }

  return NULL;
}

static node_t *find_inorder_successor(node_t *node) {
  if (node == NULL) return NULL;

  while (node_get_lpt(node) != NULL) {
    node = node_get_lpt(node);
  }

  return node;
}

static node_t *node_delete_rec(bitree_t *bt, node_t *node, void *value, void **removed) {
  if (node == NULL) return NULL;

  int compare = bt->compare(value, node_get_value(node));

  node_t *left = node_get_lpt(node);
  node_t *right = node_get_rpt(node);

  if (compare > 0) {
    node_set_rpt(node, node_delete_rec(bt, right, value, removed));
  } else if (compare < 0) {
    node_set_lpt(node, node_delete_rec(bt, left, value, removed));
  } else {
    *removed = node_get_value(node);
  
    /*  EXPLICAÇÃO DA REMOÇÃO DE NODE
     *
     *  Este ponto do código é alcançado quando o node com valor igual a value for encontrado.
     *  Em uma remoção, este ponto do código será alcançado uma única vez para leaf nodes e nodes com apenas uma conexão
     *  e duas vezes na remoção de um node com duas conexões (uma vez no node em si, e uma vez no inorder successor).
     *
     *  Caso o node seja um leaf node ou apresente apenas uma conexão, ele simplesmente será liberado da memória.
     *  Caso o node tenha duas conexões, ele não pode ser deletado. O seu valor será trocado com o valor do inorder successor,
     *  este sendo o qual deverá ser deletado.
     *
     *  O inorder successor, por definição, ou é um leaf node ou possui apenas uma conexão (à direita). No caso do node inicial a ser deletado
     *  possua duas conexões, ele não será deletado, o seu valor será trocado pelo valor do inorder successor, que, então, será deletado.
     */

    if (left == NULL || right == NULL) free(node);

    if (left == NULL && right == NULL) return NULL;
    if (left == NULL) return right;
    if (right == NULL) return left;

    void *ios_v = NULL;

    node_t *inorder_successor = find_inorder_successor(right);
    node_set_value(node, node_get_value(inorder_successor));
    node_set_destructor(node, node_get_destructor(inorder_successor));
    node_set_rpt(node, node_delete_rec(bt, right, node_get_value(inorder_successor), &ios_v));
  }

  return node;
}

void *bt_remove_node(bitree_t *bt, void *value) {
  void *removed = NULL;
  bt->root = node_delete_rec(bt, bt->root, value, &removed);
  return removed;
}

node_t *bt_get_root(bitree_t *bt) {
  return bt->root;
}
