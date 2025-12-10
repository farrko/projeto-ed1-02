#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "../shapes/shapes.h"
#include "../shapes/polygon.h"
#include "../utils/visibility.h"
#include "svg.h"

static void txt_print_shape(FILE *txt, shape_t *shape);

static svg_t *qrysvg_sfx(svg_t *qrysvg, char *sfx) {
  char *original_path = svg_get_path(qrysvg);
  if (strcmp(sfx, "-") == 0) return qrysvg;

  char *qry_sfx = malloc(strlen(original_path) + strlen(sfx) + 2);  // 1 do traço + 1 do null character
  sprintf(qry_sfx, "%.*s-%s.svg", (int) (strlen(original_path) - 4), original_path, sfx);

  svg_t *svg_sfx = svg_init(qry_sfx);

  return svg_sfx;
}

static llist_t *shape_to_barriers(node_t *shape, char seg_orientation, size_t *highest_id) {
  shape_t *s = node_get_value(shape);

  llist_t *barriers = llist_init();
  
  switch(shape_get_type(s)) {
    case CIRCLE: {
      circle_t *c = shape_as_circle(s);
      double radius = circle_get_radius(c);
      
      line_t *l;

      if (seg_orientation == 'v') {
        l = line_init(++*highest_id, circle_get_x(c), circle_get_y(c) + radius, circle_get_x(c), circle_get_y(c) - radius, circle_get_border_color(c));
      } else {
        l = line_init(++*highest_id, circle_get_x(c) + radius, circle_get_y(c), circle_get_x(c) - radius, circle_get_y(c), circle_get_border_color(c));
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

      line_t *l1 = line_init(++*highest_id, rx, ry, rx + rw, ry, rect_get_border_color(r));
      line_t *l2 = line_init(++*highest_id, rx, ry, rx, ry + rh, rect_get_border_color(r));
      line_t *l3 = line_init(++*highest_id, rx + rw, ry + rh, rx + rw, ry, rect_get_border_color(r));
      line_t *l4 = line_init(++*highest_id, rx + rw, ry + rh, rx, ry + rh, rect_get_border_color(r));

      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l1)));
      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l2)));
      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l3)));
      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l4)));

      break;
    }

    case LINE: {
      llist_insertat_end(barriers, shape_as_node(shape_clone(s, ++*highest_id)));
      break;
    }

    case TEXT: {
      text_t *t = shape_as_text(s);

      line_t *l = text_line_collision(t);
      line_set_id(l, ++*highest_id);
      line_set_color(l, text_get_border_color(t));

      llist_insertat_end(barriers, shape_as_node(shape_init(LINE, l)));
      break;
    }
  }

  node_destroy(shape);

  return barriers;
}

static llist_t *shapes_inside_polygon(llist_t *shapes, polygon_t *py) {
  node_t *current = llist_get_head(shapes);
  size_t shapes_len = llist_get_length(shapes);
  size_t amount_removed = 0;

  llist_t *inside_polygon = llist_init();

  for (size_t i = 0; i < shapes_len; i++) {
    node_t *next = node_get_rpt(current);

    shape_t *s = node_get_value(current);
    bool overlap = polygon_overlap(py, s);

    current = next;
    if (!overlap) continue;

    llist_insertat_end(inside_polygon, llist_popat_index(shapes, i - amount_removed));
    amount_removed++;
  }

  return inside_polygon;
}

static void command_a(llist_t *shapes, llist_t *barriers, size_t *highest_id, size_t i, size_t j, char seg_orientation, FILE *txt) {
  size_t shapes_len = llist_get_length(shapes);
  node_t *current = llist_get_head(shapes);
  
  size_t shapes_removed = 0;
  for (size_t k = 0; k < shapes_len; k++) {
    shape_t *shape = node_get_value(current);
    size_t shape_id = shape_get_id(shape);

    if (shape_id < i || shape_id > j) {
      current = node_get_rpt(current);
      continue;
    }

    fprintf(txt, "\n\n- TRANSFORMAÇÃO DE FORMA EM ANTEPARO - ORIGINAL: \n");
    txt_print_shape(txt, shape);
  
    node_t *next = node_get_rpt(current);
    node_t *removed = llist_popat_index(shapes, k - shapes_removed);
    current = next;

    llist_t *new_barriers = shape_to_barriers(removed, seg_orientation, highest_id);
    size_t nb_len = llist_get_length(new_barriers);

    fprintf(txt, "\n\n- NOVOS ANTEPAROS: \n");
    for (size_t l = 0; l < nb_len; l++) {
      node_t *popped = llist_popat_start(new_barriers);
      txt_print_shape(txt, node_get_value(popped));

      llist_insertat_end(barriers, popped);
    }

    llist_destroy(new_barriers);

    shapes_removed++;
  }
}

static void command_d(llist_t *shapes, llist_t *barriers, double x, double y, char *sfx, FILE *txt, svg_t *qrysvg, char sort_type, size_t threshold) {
  svg_t *svg_sfx = qrysvg_sfx(qrysvg, sfx);
  
  point_t *origin = point_init(x, y);
  polygon_t *py = generate_visibility_polygon(barriers, origin, sort_type, threshold);

  svg_write_polygon(svg_sfx, py);

  llist_t *inside_py = shapes_inside_polygon(shapes, py);
  node_t *current = llist_get_head(inside_py);
  size_t ipy_len = llist_get_length(inside_py);

  for (size_t i = 0; i < ipy_len; i++) {
    fprintf(txt, "\n\n FORMA DESTRUÍDA: \n");
    txt_print_shape(txt, node_get_value(current));

    current = node_get_rpt(current);
  }

  llist_destroy(inside_py);

  inside_py = shapes_inside_polygon(barriers, py);
  current = llist_get_head(inside_py);
  ipy_len = llist_get_length(inside_py);

  for (size_t i = 0; i < ipy_len; i++) {
    fprintf(txt, "\n\n FORMA DESTRUÍDA: \n");
    txt_print_shape(txt, node_get_value(current));

    current = node_get_rpt(current);
  }

  llist_destroy(inside_py);

  py_destroy(py);
  point_destroy(origin);

  if (svg_get_path(svg_sfx) != svg_get_path(qrysvg)) svg_close(svg_sfx);
}

static void command_p(llist_t *shapes, llist_t *barriers, double x, double y, char *color, char *sfx, FILE *txt, svg_t *qrysvg, char sort_type, size_t threshold) {
  svg_t *svg_sfx = qrysvg_sfx(qrysvg, sfx);
  
  point_t *origin = point_init(x, y);
  polygon_t *py = generate_visibility_polygon(barriers, origin, sort_type, threshold);

  svg_write_polygon(svg_sfx, py);

  llist_t *inside_py = shapes_inside_polygon(shapes, py);
  node_t *current = llist_get_head(inside_py);
  size_t ipy_len = llist_get_length(inside_py);

  for (size_t i = 0; i < ipy_len; i++) {
    fprintf(txt, "\n\n FORMA PINTADA: \n");
    txt_print_shape(txt, node_get_value(current));

    shape_set_color(node_get_value(current), color);

    current = node_get_rpt(current);
  }

  for (size_t i = 0; i < ipy_len; i++) {
    llist_insertat_end(shapes, llist_popat_start(inside_py));
  }

  llist_destroy(inside_py);

  inside_py = shapes_inside_polygon(barriers, py);
  current = llist_get_head(inside_py);
  ipy_len = llist_get_length(inside_py);
 
  for (size_t i = 0; i < ipy_len; i++) {
    fprintf(txt, "\n\n FORMA PINTADA: \n");
    txt_print_shape(txt, node_get_value(current));

    shape_set_color(node_get_value(current), color);

    current = node_get_rpt(current);
  }

  for (size_t i = 0; i < ipy_len; i++) {
    llist_insertat_end(barriers, llist_popat_start(inside_py));
  }

  llist_destroy(inside_py);

  py_destroy(py);
  point_destroy(origin);

  if (svg_get_path(svg_sfx) != svg_get_path(qrysvg)) svg_close(svg_sfx);
}

static void command_cln(llist_t *shapes, llist_t *barriers, double x, double y, double dx, double dy, char *sfx, size_t *highest_id, FILE *txt, svg_t *qrysvg, char sort_type, size_t threshold) {
  svg_t *svg_sfx = qrysvg_sfx(qrysvg, sfx);
  
  point_t *origin = point_init(x, y);
  polygon_t *py = generate_visibility_polygon(barriers, origin, sort_type, threshold);

  svg_write_polygon(svg_sfx, py);

  llist_t *inside_py = shapes_inside_polygon(shapes, py);
  node_t *current = llist_get_head(inside_py);
  size_t ipy_len = llist_get_length(inside_py);

  for (size_t i = 0; i < ipy_len; i++) {
    fprintf(txt, "\n\n FORMA CLONADA - ORIGINAL: \n");
    txt_print_shape(txt, node_get_value(current));

    shape_t *clone = shape_clone(node_get_value(current), ++(*highest_id));
    shape_move(clone, shape_get_x(clone), shape_get_y(clone), dx, dy);

    fprintf(txt, "\n\n FORMA CLONADA - CLONE: \n");
    txt_print_shape(txt, clone);

    llist_insertat_end(shapes, shape_as_node(clone));

    current = node_get_rpt(current);
  }

  for (size_t i = 0; i < ipy_len; i++) {
    llist_insertat_end(shapes, llist_popat_start(inside_py));
  }

  llist_destroy(inside_py);
 
  inside_py = shapes_inside_polygon(barriers, py);
  current = llist_get_head(inside_py);
  ipy_len = llist_get_length(inside_py);
 
  for (size_t i = 0; i < ipy_len; i++) {
    fprintf(txt, "\n\n FORMA CLONADA - ORIGINAL: \n");
    txt_print_shape(txt, node_get_value(current));

    shape_t *clone = shape_clone(node_get_value(current), ++(*highest_id));
    shape_move(clone, shape_get_x(clone), shape_get_y(clone), dx, dy);

    fprintf(txt, "\n\n FORMA CLONADA - CLONE: \n");
    txt_print_shape(txt, clone);

    llist_insertat_end(barriers, shape_as_node(clone));

    current = node_get_rpt(current);
  }

  for (size_t i = 0; i < ipy_len; i++) {
    llist_insertat_end(barriers, llist_popat_start(inside_py));
  }

  llist_destroy(inside_py);

  py_destroy(py);
  point_destroy(origin);

  if (svg_get_path(svg_sfx) != svg_get_path(qrysvg)) svg_close(svg_sfx);
}

void qry_processing(char *qrypath, char *txtpath, llist_t *shapes, size_t highest_id, svg_t *qrysvg, char *sort_type, size_t threshold) {
  FILE *qry = fopen(qrypath, "r");
  if (qry == NULL) {
    printf("Erro na leitura do arquivo .qry.\n");
    exit(1);
  }

  FILE *txt = fopen(txtpath, "w");
  llist_t *barriers = llist_init();

  char st = sort_type[0];

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

      command_d(shapes, barriers, x, y, sfx, txt, qrysvg, st, threshold);

      free(sfx);
    }

    // Bomba de pintura
    if (strncmp(str, "p", 1) == 0) {
      double x, y;
      char *color = malloc(8);
      char *sfx = malloc(24);

      sscanf(str, "%*s %lf %lf %s %s", &x, &y, color, sfx);
      fprintf(txt, "p %lf %lf %s %s\n", x, y, color, sfx);

      command_p(shapes, barriers, x, y, color, sfx, txt, qrysvg, st, threshold);

      free(color);
      free(sfx);
    }

    // Bomba de clonagem
    if (strncmp(str, "cln", 3) == 0) {
      double x, y, dx, dy;
      char *sfx = malloc(24);

      sscanf(str, "%*s %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sfx);
      fprintf(txt, "cln %lf %lf %lf %lf %s\n", x, y, dx, dy, sfx);

      command_cln(shapes, barriers, x, y, dx, dy, sfx, &highest_id, txt, qrysvg, st, threshold);

      free(sfx);
    }
  }

  size_t barriers_len = llist_get_length(barriers);
  for (size_t i = 0; i < barriers_len; i++) {
    llist_insertat_end(shapes, llist_popat_start(barriers));
  }

  llist_destroy(barriers);

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
