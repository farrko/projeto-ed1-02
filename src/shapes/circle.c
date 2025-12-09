#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "circle.h"

#include "point.h"

struct circle_t {
  size_t id;
  point_t *origin;
  double radius;
  char *color;
  char *border_color;
};

circle_t *circle_init(size_t id, double x, double y, double radius, char *color, char *border_color) {
  circle_t *circle = malloc(sizeof(circle_t));
  if (circle == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  circle->id = id;

  point_t *origin = point_init(x, y);
  circle->origin = origin;

  circle->radius = radius;

  char *_color = malloc(strlen(color) + 1);
  strcpy(_color, color);
  circle->color = _color;

  char *_border_color = malloc(strlen(border_color) + 1);
  strcpy(_border_color, border_color);
  circle->border_color = _border_color;

  return circle;
}

void circle_destroy(void *circle) {
  circle_t *c = (circle_t *) circle;

  if (c->color != NULL) free(c->color);
  if (c->border_color != NULL) free(c->border_color);

  point_destroy(c->origin);

  free(circle);
}

void circle_set_x(circle_t *circle, double x) {
  point_set_x(circle->origin, x);
}

void circle_set_y(circle_t *circle, double y) {
  point_set_y(circle->origin, y);
}

void circle_set_radius(circle_t *circle, double radius) {
  circle->radius = radius;
}

void circle_set_color(circle_t *circle, char *color) {
  if (circle->color != NULL) free(circle->color);
 
  char *_color = malloc(strlen(color) + 1);
  strcpy(_color, color);
  circle->color = _color;
}

void circle_set_border_color(circle_t *circle, char *border_color) {
  if (circle->border_color != NULL) free(circle->border_color);
 
  char *_border_color = malloc(strlen(border_color) + 1);
  strcpy(_border_color, border_color);
  circle->border_color = _border_color;
}

size_t circle_get_id(circle_t *circle) {
  return circle->id;
}

point_t *circle_get_origin(circle_t *circle) {
  return circle->origin;
}

double circle_get_x(circle_t *circle) {
  return point_get_x(circle->origin);
}

double circle_get_y(circle_t *circle) {
  return point_get_y(circle->origin);
}

double circle_get_radius(circle_t *circle) {
  return circle->radius;
}

char *circle_get_color(circle_t *circle) {
  return circle->color;
}

char *circle_get_border_color(circle_t *circle) {
  return circle->border_color;
}

double circle_get_area(circle_t *circle) {
  return pow(circle->radius, 2) * 3.1415;
}

circle_t *circle_clone(circle_t *circle, size_t id) {
  return circle_init(id, point_get_x(circle->origin), point_get_y(circle->origin), circle->radius, circle->color, circle->border_color);
}
