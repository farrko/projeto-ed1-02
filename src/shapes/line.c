#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "line.h"

#include "point.h"

struct line_t {
  size_t id;
  point_t *p1;
  point_t *p2;
  char *color;
};

line_t *line_init(size_t id, double x1, double y1, double x2, double y2, char *color) {
  line_t *line = malloc(sizeof(line_t));
  if (line == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  line->id = id;

  point_t *p1 = point_init(x1, y1);
  line->p1 = p1;

  point_t *p2 = point_init(x2, y2);
  line->p2 = p2;
 
  char *_color = malloc(sizeof(color) + 1);
  strcpy(_color, color);
  line->color = _color;

  return line;
}

void line_destroy(void *line) {
  line_t *l = (line_t *) line;

  if (l->color != NULL) free(l->color);

  point_destroy(l->p1);
  point_destroy(l->p2);

  free(l);
}

void line_set_x1(line_t *line, double x1) {
  point_set_x(line->p1, x1);
}

void line_set_y1(line_t *line, double y1) {
  point_set_y(line->p1, y1);
}
void line_set_x2(line_t *line, double x2) {
  point_set_x(line->p2, x2);
}

void line_set_y2(line_t *line, double y2) {
  point_set_y(line->p1, y2);
}

void line_set_color(line_t *line, char *color) {
  if (line->color != NULL) free(line->color);
  
  char *_color = malloc(sizeof(color) + 1);
  strcpy(_color, color);
  line->color = _color;
}

size_t line_get_id(line_t *line) {
  return line->id;
}

point_t *line_get_p1(line_t *line) {
  return line->p1;
}

point_t *line_get_p2(line_t *line) {
  return line->p2;
}


double line_get_x1(line_t *line) {
  return point_get_x(line->p1);
}

double line_get_y1(line_t *line) {
  return point_get_y(line->p1);
}

double line_get_x2(line_t *line) {
  return point_get_x(line->p2);
}

double line_get_y2(line_t *line) {
  return point_get_y(line->p2);
}

char *line_get_color(line_t *line) {
  return line->color;
}

line_t *line_clone(line_t *line, size_t id) {
  return line_init(id, point_get_x(line->p1), point_get_y(line->p1), point_get_x(line->p2), point_get_y(line->p2), line->color);
}
