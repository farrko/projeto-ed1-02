#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "text.h"

struct text_t {
  size_t id;
  double x, y;
  char *anchor;
  char *color;
  char *border_color;
  char *ffam;
  char *fweight;
  char *fsize;
  char *content;
};

text_t *text_init(size_t id, double x, double y, char *anchor, char *color, char *border_color, char *ffam, char *fweight, char *fsize, char *content) {
  text_t *text = malloc(sizeof(text_t));
  if (text == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  text->id = id;
  text->x = x;
  text->y = y;
  text->anchor = anchor;
  text->color = color;
  text->border_color = border_color;
  text->ffam = ffam;
  text->fweight = fweight;
  text->fsize = fsize;
  text->content = content;

  return text;
}

void text_destroy(void *text) {
  text_t *t = (text_t *) text;

  if (t->anchor != NULL) free(t->anchor);
  if (t->color != NULL) free(t->color);
  if (t->border_color != NULL) free(t->border_color);
  if (t->ffam != NULL) free(t->ffam);
  if (t->fweight != NULL) free(t->fweight);
  if (t->fsize != NULL) free(t->fsize);
  if (t->content != NULL) free(t->content);

  free(t);
}

void text_set_x(text_t *text, double x) {
  text->x = x;
}

void text_set_y(text_t *text, double y) {
  text->y = y;
}

void text_set_anchor(text_t *text, char *anchor) {
  text->anchor = anchor;
}

void text_set_color(text_t *text, char *color) {
  if (text->color != NULL) free(text->color);
  text->color = color;
}

void text_set_border_color(text_t *text, char *border_color) {
  if (text->border_color != NULL) free(text->border_color);
  text->border_color = border_color;
}

void text_set_ffam(text_t *text, char *ffam) {
  if (text->ffam != NULL) free(text->ffam);
  text->ffam = ffam;
}

void text_set_fweight(text_t *text, char *fweight) {
  if (text->fweight != NULL) free(text->fweight);
  text->fweight = fweight;
}

void text_set_fsize(text_t *text, char *fsize) {
  if (text->fsize != NULL) free(text->fsize);
  text->fsize = fsize;
}

void text_set_content(text_t *text, char *content) {
  if (text->content != NULL) free(text->content);
  text->content = content;
}

size_t text_get_id(text_t *text) {
  return text->id;
}

double text_get_x(text_t *text) {
  return text->x;
}

double text_get_y(text_t *text) {
  return text->y;
}

char *text_get_anchor(text_t *text) {
  return text->anchor;
}

char *text_get_color(text_t *text) {
  return text->color;
}

char *text_get_border_color(text_t *text) {
  return text->border_color;
}

char *text_get_ffam(text_t *text) {
  return text->ffam;
}

char *text_get_fweight(text_t *text) {
  return text->fweight;
}

char *text_get_fsize(text_t *text) {
  return text->fsize;
}

char *text_get_content(text_t *text) {
  return text->content;
}

double text_get_area(text_t *text) {
  size_t len = strlen(text->content);
  return 20.0 * len;
}

text_t *text_clone(text_t *text, size_t id) {
  char *anchor = malloc(strlen(text->anchor) + 1);
  char *color = malloc(8);
  char *border_color = malloc(8);
  char *ffam = malloc(strlen(text->ffam) + 1);
  char *fweight = malloc(strlen(text->fweight) + 1);
  char *fsize = malloc(strlen(text->fsize) + 1);
  char *content = malloc(128);

  if (!anchor || !color || !border_color || !ffam || !fweight || !fsize || !content) {
      printf("Erro na alocação de memória.\n");
      exit(1);
  }

  strcpy(anchor, text->anchor);
  strcpy(color, text->color);
  strcpy(border_color, text->border_color);
  strcpy(ffam, text->ffam);
  strcpy(fweight, text->fweight);
  strcpy(fsize, text->fsize);
  strcpy(content, text->content);

  return text_init(id, text->x, text->y, anchor, color, border_color, ffam, fweight, fsize, content);
}

line_t *text_line_collision(text_t *text) {
  size_t len = strlen(text->content);
  size_t cl = 10 * len;

  double x1, x2; 

  if (strcmp(text->anchor, "start") == 0) {
    x1 = text->x;
    x2 = x1 + cl;
  }

  if (strcmp(text->anchor, "middle") == 0) {
    x1 = text->x - (cl / 2.0);
    x2 = text->x + (cl / 2.0);
  }

  if (strcmp(text->anchor, "end") == 0) {
    x1 = text->x - cl;
    x2 = text->x;
  }

  line_t *line = line_init(0, x1, text->y, x2, text->y, NULL);
  return line;
}

void text_swap_colors(text_t *text) {
  char *color = text->color;
  char *border_color = text->border_color;

  text->color = border_color;
  text->border_color = color;
}
