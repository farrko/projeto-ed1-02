#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

struct llist_t {
  node_t *head;
  node_t *tail;
  size_t length;
};

llist_t *llist_init() {
  llist_t *llist = malloc(sizeof(llist_t));
  if (llist == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  llist->head = NULL;
  llist->tail = NULL;
  llist->length = 0;

  return llist;
}

void llist_destroy(llist_t *llist) {
  node_t *current = llist->head;

  for (size_t i = 0; i < llist->length; i++) {
    node_t *next = node_get_rpt(current);

    node_destroy(current);

    current = next;
  }

  free(llist);
}

node_t *llist_get_head(llist_t *llist) {
  return llist->head;
}

node_t *llist_get_tail(llist_t *llist) {
  return llist->tail;
}

size_t llist_get_length(llist_t *llist) {
  return llist->length;
}

node_t *llist_getat_index(llist_t *llist, size_t index) {
  if (index >= llist->length) return NULL;

  node_t *current = llist->head;
  for (size_t i = 0; i < index; i++) {
    current = node_get_rpt(current);
  }

  return current;
}

node_t *llist_popat_index(llist_t *llist, size_t index) {
  if (index >= llist->length) return NULL;

  if (llist->length == 1) {
    node_t *node = llist->head;

    llist->head = NULL;
    llist->tail = NULL;

    llist->length--;

    return node;
  }

  if (!index) {
    node_t *current = llist->head;
    node_t *next = node_get_rpt(current);

    node_set_lpt(next, NULL);
    llist->head = next;

    llist->length--;

    return current;
  }

  if (index == llist->length - 1) {
    node_t *current = llist->tail;
    node_t *prev = node_get_lpt(current);

    node_set_rpt(prev, NULL);
    llist->tail = prev;

    llist->length--;

    return current;
  }

  node_t *current = llist_getat_index(llist, index);
  node_t *prev = node_get_lpt(current);
  node_t *next = node_get_rpt(current);

  node_set_rpt(prev, next);
  node_set_lpt(next, prev);

  llist->length--;

  return current;
}

node_t *llist_popat_start(llist_t *llist) {
  return llist_popat_index(llist, 0);
}

node_t *llist_popat_end(llist_t *llist) {
  if (!llist->length) return NULL;

  return llist_popat_index(llist, llist->length - 1);
}

void llist_insertat_index(llist_t *llist, node_t *node, size_t index) {
  if (index > llist->length) return;

  node_set_lpt(node, NULL);
  node_set_rpt(node, NULL);

  if (!llist->length) {
    llist->head = node;
    llist->tail = node;

    llist->length++;

    return;
  }

  if (!index) {
    node_t *head = llist->head;
    node_set_rpt(node, head);
    node_set_lpt(head, node);
    llist->head = node;

    llist->length++;

    return;
  }

  if (index == llist->length) {
    node_t *tail = llist->tail;
    node_set_rpt(tail, node);
    node_set_lpt(node, tail);
    llist->tail = node;

    llist->length++;

    return;
  }

  node_t *current = llist->head;
  for (size_t i = 0; i < index; i++) {
    current = node_get_rpt(current);
  }

  node_t *prev = node_get_lpt(current);

  node_set_rpt(prev, node);
  node_set_lpt(node, prev);
  node_set_rpt(node, current);
  node_set_lpt(current, node);

  llist->length++;
}

void llist_insertat_start(llist_t *llist, node_t *node) {
  llist_insertat_index(llist, node, 0);
}

void llist_insertat_end(llist_t *llist, node_t *node) {
  llist_insertat_index(llist, node, llist->length);
}

void llist_destroyat(llist_t *llist, size_t index) {
  node_t *node = llist_popat_index(llist, index);
  if (node == NULL) return;

  node_destroy(node);
}
