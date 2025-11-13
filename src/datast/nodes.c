#include <stdlib.h>
#include <stdio.h>

#include "nodes.h"

struct node_t {
  void *value;
  void (*destructor)(void *);
  struct node_t *lpt;
  struct node_t *rpt;
};

node_t *node_init(void *value, void (*destructor)(void*)) {
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  node->value = value;
  node->destructor = destructor;
  node->lpt = NULL;
  node->rpt = NULL;

  return node;
}

void node_set_value(node_t *node, void *value) {
  if (node->value != NULL) node->destructor(value);

  node->value = value;
}

void node_set_destructor(node_t *node, void (*destructor)(void *)) {
  node->destructor = destructor;
}

void node_set_lpt(node_t *node, node_t *lpt) {
  node->lpt = lpt;
}

void node_set_rpt(node_t *node, node_t *rpt) {
  node->rpt = rpt;
}

void *node_get_value(node_t *node) {
  return node->value;
}

node_t *node_get_lpt(node_t *node) {
  return node->lpt;
}

node_t *node_get_rpt(node_t *node) {
  return node->rpt;
}

void node_destroy(node_t *node) {
  if (node->value != NULL) node->destructor(node->value);

  free(node);
}


