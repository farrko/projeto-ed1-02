#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shapes/shapes.h"

static void txt_print_shape(FILE *txt, shape_t *shape);

static llist_t *shape_to_barriers(node_t *shape, char seg_orientation, size_t *highest_id) {
  shape_t *s = node_get_value(shape);

  llist_t *barriers = llist_init();
  
  switch(shape_get_type(s)) {
    case CIRCLE: {
      circle_t *c = shape_as_circle(s);
      double radius = circle_get_radius(c);
      
      line_t *l;

      if (seg_orientation == 'v') {
        l = line_init(++(*highest_id), circle_get_x(c), circle_get_y(c) + radius, circle_get_x(c), circle_get_y(c) - radius, circle_get_border_color(c));
      } else {
        l = line_init(++(*highest_id), circle_get_x(c) + radius, circle_get_y(c), circle_get_x(c) - radius, circle_get_y(c), circle_get_border_color(c));
      }

      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l)));
      break;
    }

    case RECTANGLE: {
      rectangle_t *r = shape_as_rectangle(s);

      double rx = rect_get_x(r);
      double ry = rect_get_y(r);
      double rw = rect_get_width(r);
      double rh = rect_get_height(r);

      line_t *l1 = line_init(++(*highest_id), rx, ry, rx + rw, ry, rect_get_border_color(r));
      line_t *l2 = line_init(++(*highest_id), rx, ry, rx, ry + rh, rect_get_border_color(r));
      line_t *l3 = line_init(++(*highest_id), rx + rw, ry + rh, rx + rw, ry, rect_get_border_color(r));
      line_t *l4 = line_init(++(*highest_id), rx + rw, ry + rh, rx, ry + rh, rect_get_border_color(r));

      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l1)));
      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l2)));
      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l3)));
      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l4)));

      break;
    }

    case LINE: {
      llist_insertat_end(barriers, shape_as_node(shape_clone(s, ++(*highest_id))));
      break;
    }

    case TEXT: {
      text_t *t = shape_as_text(s);

      line_t *l = text_line_collision(t);
      line_set_color(l, text_get_border_color(t));

      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l)));
      break;
    }
  }

  node_destroy(shape);

  return barriers;
}

static void command_a(llist_t *shapes, llist_t *barriers, size_t *highest_id, size_t i, size_t j, char seg_orientation, FILE *txt) {
  size_t shapes_len = llist_get_length(shapes);
  node_t *current = llist_get_head(shapes);
  
  size_t shapes_removed = 0;
  for (size_t i = 0; i < shapes_len; i++) {
    shape_t *shape = node_get_value(current);
    size_t shape_id = shape_get_id(shape);

    if (i < shape_id || shape_id > j) {
      current = node_get_rpt(current);
      continue;
    }

    fprintf(txt, "\n\n- TRANSFORMAÇÃO DE FORMA EM ANTEPARO - ORIGINAL: \n");
    txt_print_shape(txt, shape);
  
    node_t *next = node_get_rpt(current);
    node_t *removed = llist_popat_index(shapes, i - shapes_removed);
    current = next;

    llist_t *new_barriers = shape_to_barriers(removed, seg_orientation, highest_id);
    size_t nb_len = llist_get_length(new_barriers);

    fprintf(txt, "\n\n- NOVOS ANTEPAROS: \n");
    for (size_t i = 0; i < nb_len; i++) {
      node_t *popped = llist_popat_start(new_barriers);
      txt_print_shape(txt, node_get_value(popped));

      llist_insertat_end(barriers, popped);
    }

    shapes_removed++;
  }
}

static void command_d() {
  
}

static void command_p();
static void command_cln();

void qry_processing(char *qrypath, char *txtpath, llist_t *shapes, size_t highest_id, char *qrysvg) {
  FILE *qry = fopen(qrypath, "r");
  if (qry == NULL) {
    printf("Erro na leitura do arquivo .qry.\n");
    exit(1);
  }

  FILE *txt = fopen(txtpath, "w");

  char *qrysfx = malloc(strlen(qrysvg) + 26); // 26 -> 1 do traço + 24 do tamanho máximo de sfx + null term
  strncpy(qrysfx, qrysvg, strlen(qrysvg) - 4);

  llist_t *barriers = llist_init();

  char str[256];
  while(fgets(str, 256, qry)) {
    // Transformação de formas em anteparos
    if (strncmp(str, "a", 1) == 0) {
      size_t i, j;
      char seg_orientation;

      sscanf(str, "%*s %zu %zu %c", &i, &j, &seg_orientation);
      fprintf(txt, "a %zu %zu %c\n", i, j, seg_orientation);

      command_a(shapes, barriers, &highest_id, i, j, seg_orientation, txt);
    }

    // Bomba de destruição
    if (strncmp(str, "d", 1) == 0) {
      double x, y;
      char *sfx = malloc(24);

      sscanf(str, "%*s %lf %lf %s", &x, &y, sfx);
      fprintf(txt, "d %lf %lf %s\n", x, y, sfx);
      
      command_d();

      free(sfx);
    }

    // Bomba de pintura
    if (strncmp(str, "p", 1) == 0) {
      double x, y;
      char *color = malloc(8);
      char *sfx = malloc(24);

      sscanf(str, "%*s %lf %lf %s %s", &x, &y, color, sfx);
      fprintf(txt, "p %lf %lf %s %s\n", x, y, color, sfx);

      command_p();

      free(color);
      free(sfx);
    }

    // Bomba de clonagem
    if (strncmp(str, "cln", 3) == 0) {
      double x, y, dx, dy;
      char *sfx = malloc(24);

      sscanf(str, "%*s %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sfx);
      fprintf(txt, "cln %lf %lf %lf %lf %s\n", x, y, dx, dy, sfx);

      command_cln();

      free(sfx);
    }
  }


  fclose(qry);
  fclose(txt);
}

static void txt_print_shape(FILE *txt, shape_t *shape) {
  switch (shape_get_type(shape)) {
    case CIRCLE: {
      circle_t *circle = shape_as_circle(shape);
      fprintf(txt, "- Círculo - id: %zu\n", circle_get_id(circle));
      fprintf(txt, "\tx: %lf\n", circle_get_x(circle));
      fprintf(txt, "\ty: %lf\n", circle_get_y(circle));
      fprintf(txt, "\traio: %lf\n", circle_get_radius(circle));
      fprintf(txt, "\tcor: %s\n", circle_get_color(circle));
      fprintf(txt, "\tcor de borda: %s\n", circle_get_border_color(circle));
      break;
    }

    case RECTANGLE: {
      rectangle_t *rect = shape_as_rectangle(shape);
      fprintf(txt, "- Retângulo - id: %zu\n", rect_get_id(rect));
      fprintf(txt, "\tx: %lf\n", rect_get_x(rect));
      fprintf(txt, "\ty: %lf\n", rect_get_y(rect));
      fprintf(txt, "\tlargura: %lf\n", rect_get_width(rect));
      fprintf(txt, "\taltura: %lf\n", rect_get_height(rect));
      fprintf(txt, "\tcor: %s\n", rect_get_color(rect));
      fprintf(txt, "\tcor de borda: %s\n", rect_get_border_color(rect));
      break;
    }

    case LINE: {
      line_t *line = shape_as_line(shape);
      fprintf(txt, "- Linha - id: %zu\n", line_get_id(line));
      fprintf(txt, "\tx1: %lf\n", line_get_x1(line));
      fprintf(txt, "\ty1: %lf\n", line_get_y1(line));
      fprintf(txt, "\tx2: %lf\n", line_get_x2(line));
      fprintf(txt, "\ty2: %lf\n", line_get_y2(line));
      fprintf(txt, "\tcor: %s\n", line_get_color(line));
      break;
    }

    case TEXT: {
      text_t *text = shape_as_text(shape);
      fprintf(txt, "- Texto - id: %zu\n", text_get_id(text));
      fprintf(txt, "\tx: %lf\n", text_get_x(text));
      fprintf(txt, "\ty: %lf\n", text_get_y(text));
      fprintf(txt, "\tâncora: %s\n", text_get_anchor(text));
      fprintf(txt, "\tcor: %s\n", text_get_color(text));
      fprintf(txt, "\tcor de borda: %s\n", text_get_border_color(text));
      fprintf(txt, "\tfamília: %s\n", text_get_ffam(text));
      fprintf(txt, "\tpeso: %s\n", text_get_fweight(text));
      fprintf(txt, "\ttamanho: %s\n", text_get_fsize(text));
      fprintf(txt, "\tconteúdo: %s\n", text_get_content(text));
      break;
    }
  }
}
