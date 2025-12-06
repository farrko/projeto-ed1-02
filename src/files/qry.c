#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shapes/shapes.h"

static void txt_print_shape(FILE *txt, shape_t *shape);

static void command_a();
static void command_d();
static void command_p();
static void command_cln();

void qry_processing(char *qrypath, char *txtpath, llist_t *shapes, size_t highest_id) {
  FILE *qry = fopen(qrypath, "r");
  if (qry == NULL) {
    printf("Erro na leitura do arquivo .qry.\n");
    exit(1);
  }

  FILE *txt = fopen(txtpath, "w");

  char str[256];
  while(fgets(str, 256, qry)) {
    // Transformação de formas em anteparos
    if (strncmp(str, "a", 1) == 0) {
      size_t i, j;
      char seg_orientation;

      sscanf(str, "%*s %zu %zu %c", &i, &j, &seg_orientation);
      fprintf(txt, "a %zu %zu %c\n", i, j, seg_orientation);

      command_a();
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
