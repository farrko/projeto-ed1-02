#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "line.h"

struct line_t {
  size_t id;
  double x1, y1;
  double x2, y2;
  char *color;
};

line_t *line_init(size_t id, double x1, double y1, double x2, double y2, char *color) {
  line_t *line = malloc(sizeof(line_t));
  if (line == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  line->id = id;
  line->x1 = x1;
  line->y1 = y1;
  line->x2 = x2;
  line->y2 = y2;
 
  char *_color = malloc(sizeof(color) + 1);
  strcpy(_color, color);
  line->color = _color;

  return line;
}

void line_destroy(void *line) {
  line_t *l = (line_t *) line;

  if (l->color != NULL) free(l->color);
  free(l);
}

void line_set_x1(line_t *line, double x1) {
  line->x1 = x1;
}

void line_set_y1(line_t *line, double y1) {
  line->y1 = y1;
}
void line_set_x2(line_t *line, double x2) {
  line->x2 = x2;
}

void line_set_y2(line_t *line, double y2) {
  line->y2 = y2;
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

double line_get_x1(line_t *line) {
  return line->x1;
}

double line_get_y1(line_t *line) {
  return line->y1;
}

double line_get_x2(line_t *line) {
  return line->x2;
}

double line_get_y2(line_t *line) {
  return line->y2;
}

char *line_get_color(line_t *line) {
  return line->color;
}

line_t *line_clone(line_t *line, size_t id) {
  return line_init(id, line->x1, line->y1, line->x2, line->y2, line->color);
}
