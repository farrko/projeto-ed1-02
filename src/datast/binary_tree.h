#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "nodes.h"

#include <stdbool.h>

typedef struct bitree_t bitree_t;

bitree_t *bt_init(node_t *root, int (*compare)(const void *, const void *));

void bt_destroy(bitree_t *bt);

void bt_insert_node(bitree_t *bt, node_t *node);

node_t *bt_search_node(bitree_t *bt, void *value);

void bt_remove_node(bitree_t *bt, void *value);

node_t *bt_get_root(bitree_t *bt);

#endif
