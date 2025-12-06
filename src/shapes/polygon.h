#ifndef POLYGON_H
#define POLYGON_H

#include "../datast/linkedlist.h"
#include "point.h"

typedef struct polygon_t polygon_t;

polygon_t *py_init();

void py_add_vertex(polygon_t *py, point_t *point);

llist_t *py_get_vertices(polygon_t *py);

#endif
