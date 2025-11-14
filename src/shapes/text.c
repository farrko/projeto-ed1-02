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

  char *_anchor = malloc(sizeof(anchor) + 1);
  strcpy(_anchor, anchor);
  text->anchor = _anchor;

  char *_color = malloc(sizeof(color) + 1);
  strcpy(_color, color);
  text->color = _color;

  char *_border_color = malloc(sizeof(border_color) + 1);
  strcpy(_border_color, border_color);
  text->border_color = _border_color;

  char *_ffam = malloc(sizeof(ffam) + 1);
  strcpy(_ffam, ffam);
  text->ffam = _ffam;

  char *_fweight = malloc(sizeof(fweight) + 1);
  strcpy(_fweight, fweight);
  text->fweight = _fweight;

  char *_fsize = malloc(sizeof(fsize) + 1);
  strcpy(_fsize, fsize);
  text->fsize = _fsize;

  char *_content = malloc(sizeof(content) + 1);
  strcpy(_content, content);
  text->content = _content;

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
  if (text->anchor != NULL) free(anchor); 
 
  char *_anchor = malloc(sizeof(anchor) + 1);
  strcpy(_anchor, anchor);
  text->anchor = _anchor;
}

void text_set_color(text_t *text, char *color) {
  if (text->color != NULL) free(text->color);

  char *_color = malloc(sizeof(color) + 1);
  strcpy(_color, color);
  text->color = _color;
}

void text_set_border_color(text_t *text, char *border_color) {
  if (text->border_color != NULL) free(text->border_color);

  char *_border_color = malloc(sizeof(border_color) + 1);
  strcpy(_border_color, border_color);
  text->border_color = _border_color;
}

void text_set_ffam(text_t *text, char *ffam) {
  if (text->ffam != NULL) free(text->ffam);

  char *_ffam = malloc(sizeof(ffam) + 1);
  strcpy(_ffam, ffam);
  text->ffam = _ffam;
}

void text_set_fweight(text_t *text, char *fweight) {
  if (text->fweight != NULL) free(text->fweight);

  char *_fweight = malloc(sizeof(fweight) + 1);
  strcpy(_fweight, fweight);
  text->fweight = _fweight;
}

void text_set_fsize(text_t *text, char *fsize) {
  if (text->fsize != NULL) free(text->fsize);

  char *_fsize = malloc(sizeof(fsize) + 1);
  strcpy(_fsize, fsize);
  text->fsize = _fsize;
}

void text_set_content(text_t *text, char *content) {
  if (text->content != NULL) free(text->content);

  char *_content = malloc(sizeof(content) + 1);
  strcpy(_content, content);
  text->content = _content;
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

text_t *text_clone(text_t *text, size_t id) {
  return text_init(id, text->x, text->y, text->anchor, text->color, text->border_color, text->ffam, text->fweight, text->fsize, text->content);
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
