#ifndef POLYGON_H
#define POLYGON_H

#include "../datast/linkedlist.h"
#include "point.h"
#include "shapes.h"
#include <stdbool.h>

typedef struct polygon_t polygon_t;

polygon_t *py_init();

void py_destroy(polygon_t *py);

void py_add_vertex(polygon_t *py, point_t *point);

llist_t *py_get_vertices(polygon_t *py);

bool polygon_overlap(polygon_t *py, shape_t *shape);

#endif
