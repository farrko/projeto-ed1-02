#include <stdlib.h>
#include <stdio.h>

#include "polygon.h"

struct polygon_t {
  llist_t *vertices;
};

polygon_t *py_init() {
  polygon_t *py = malloc(sizeof(polygon_t));
  if (py == NULL) {
    printf("Erro na alocação de memória.");
    exit(1);
  }

  py->vertices = llist_init();

  return py;
}

void py_add_vertex(polygon_t *py, point_t *point) {
  point_t *point_clone = point_init(point_get_x(point), point_get_y(point));
  llist_insertat_end(py->vertices, node_init(point_clone, point_destroy));
}

llist_t *py_get_vertices(polygon_t *py) {
  return py->vertices;
}
