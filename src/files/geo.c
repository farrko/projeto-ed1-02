#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shapes/shapes.h"

#include "geo.h"

size_t geo_processing(char *path, llist_t *shapes_list) {
  FILE *geo = fopen(path, "r");
  if (geo == NULL) {
    printf("Erro na leitura do arquivo .geo.\n");
    exit(1);
  }

  size_t highest_id = 0;
  char ffam[20] = "sans-serif";
  char fweight[20] = "normal";
  char fsize[20] = "medium";

  // Leitura linha-a-linha do arquivo .geo
  char str[256];
  while(fgets(str, 256, geo)) {
    // Leitura de círculo
    if (strncmp(str, "c", 1) == 0) {
      size_t id;
      double x, y, radius;
      char *border_color = malloc(8);
      char *color = malloc(8);

      sscanf(str, "%*s %zu %lf %lf %lf %s %s", &id, &x, &y, &radius, border_color, color);
      circle_t *circle = circle_init(id, x, y, radius, color, border_color);
      shape_t *shape = shape_init(CIRCLE, circle);
      queue_enqueue(queue, shape_as_node(shape));

      if (id > highest_id) highest_id = id;
    }

    // Leitura de retângulo
    if (strncmp(str, "r", 1) == 0) {
      size_t id;
      double x, y, w, h;
      char *border_color = malloc(8);
      char *color = malloc(8);

      sscanf(str, "%*s %zu %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, border_color, color);
      rectangle_t *rect = rect_init(id, x, y, w, h, color, border_color);
      shape_t *shape = shape_init(RECTANGLE, rect);
      queue_enqueue(queue, shape_as_node(shape));

      if (id > highest_id) highest_id = id;
    }

    // Leitura de linha
    if (strncmp(str, "l", 1) == 0) {
      size_t id;
      double x1, y1, x2, y2;
      char *color = malloc(8);

      sscanf(str, "%*s %zu %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, color);
      line_t *line = line_init(id, x1, y1, x2, y2, color);
      shape_t *shape = shape_init(LINE, line);
      queue_enqueue(queue, shape_as_node(shape));

      if (id > highest_id) highest_id = id;
    }

    // Leitura de texto
    if (strncmp(str, "t ", 2) == 0) {
      size_t id;
      double x, y;
      char *border_color = malloc(8);
      char *color = malloc(8);
      char a;
      char *content = malloc(128);

      sscanf(str, "%*s %zu %lf %lf %s %s %c %s", &id, &x, &y, border_color, color, &a, content);

      // Formatação do anchor para ser inserido diretamente no .svg
      char *anchor;
      switch(a) {
        case 'i':
          anchor = malloc(6);
          strcpy(anchor, "start");
          break;
        case 'm':
          anchor = malloc(7);
          strcpy(anchor, "middle");
          break;
        case 'f':
          anchor = malloc(4);
          strcpy(anchor, "end");
          break;
      }

      char *font_fam = malloc(strlen(ffam) + 1);
      strcpy(font_fam, ffam);
      char *font_size = malloc(strlen(fsize) + 1);
      strcpy(font_size, fsize);
      char *font_weight = malloc(strlen(fweight) + 1);
      strcpy(font_weight, fweight);

      text_t *text = text_init(id, x, y, anchor, color, border_color, font_fam, font_weight, font_size, content);
      shape_t *shape = shape_init(TEXT, text);
      queue_enqueue(queue, shape_as_node(shape));
      
      if (id > highest_id) highest_id = id;
    }

    // Leitura de text style
    if (strncmp(str, "ts", 2) == 0) {
      sscanf(str, "%*s %s %s %s", ffam, fweight, fsize);

      // Reescrever o conteúdo de ffam para deixar formatado para o SVG
      if (strcmp(ffam, "sans") == 0) strcpy(ffam, "sans-serif");

      // Reescrever o conteúdo de fweight para deixar formatado para o SVG
      if (strcmp(fweight, "n") == 0) strcpy(fweight, "normal");
      if (strcmp(fweight, "b") == 0) strcpy(fweight, "bold");
      if (strcmp(fweight, "b+") == 0) strcpy(fweight, "bolder");
      if (strcmp(fweight, "l") == 0) strcpy(fweight, "lighter");
    }
  }

  fclose(geo);

  return highest_id;
}
