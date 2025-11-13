#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rectangle.h"

struct rectangle_t {
  size_t id;
  double x, y;
  double width, height;
  char *color;
  char *border_color;
};

rectangle_t *rect_init(size_t id, double x, double y, double width, double height, char *color, char *border_color) {
  rectangle_t *rectangle = malloc(sizeof(rectangle_t));
  if (rectangle == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  rectangle->id = id;
  rectangle->x = x;
  rectangle->y = y;
  rectangle->width = width;
  rectangle->height = height;
  rectangle->color = color;
  rectangle->border_color = border_color;

  return rectangle;
}

void rect_destroy(void *rectangle) {
  rectangle_t *rect = (rectangle_t *) rectangle;

  if (rect->color != NULL) free(rect->color);
  if (rect->border_color != NULL) free(rect->border_color);

  free(rect);
}

void rect_set_x(rectangle_t *rect, double x) {
  rect->x = x;
}

void rect_set_y(rectangle_t *rect, double y) {
  rect->y = y;
}

void rect_set_width(rectangle_t *rect, double width) {
  rect->width = width;
}

void rect_set_height(rectangle_t *rect, double height) {
  rect->height = height;
}

void rect_set_color(rectangle_t *rect, char *color) {
  if (rect->color != NULL) free(rect->color);
  rect->color = color;
}

void rect_set_border_color(rectangle_t *rect, char *border_color) {
  if (rect->border_color != NULL) free(rect->border_color);
  rect->border_color = border_color;
}

size_t rect_get_id(rectangle_t *rect) {
  return rect->id;
}

double rect_get_x(rectangle_t *rect) {
  return rect->x;
}

double rect_get_y(rectangle_t *rect) {
  return rect->y;
}

double rect_get_width(rectangle_t *rect) {
  return rect->width;
}

double rect_get_height(rectangle_t *rect) {
  if (rect == NULL) return 0;
  return rect->height;
}

char *rect_get_color(rectangle_t *rect) {
  return rect->color;
}

char *rect_get_border_color(rectangle_t *rect) {
  return rect->border_color;
}

double rect_get_area(rectangle_t *rect) {
  return rect->width * rect->height;
}

rectangle_t *rect_clone(rectangle_t *rect, size_t id) {
  char *color = malloc(8);
  if (color == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  char *border_color = malloc(8);
  if (border_color == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  strcpy(color, rect->color);
  strcpy(border_color, rect->border_color);

  return rect_init(id, rect->x, rect->y, rect->width, rect->height, color, border_color);
}

void rect_swap_colors(rectangle_t *rect) {
  char *color = rect->color;
  char *border_color = rect->border_color;

  rect->color = border_color;
  rect->border_color = color;
} 
