#include <stdlib.h>
#include <stdio.h>

#include "svg.h"
#include "../shapes/shapes.h"

struct svg_t {
  FILE *svgfile;
};

svg_t *svg_init(char *path) {
  svg_t *svg = malloc(sizeof(svg_t));
  if (svg == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  FILE *file = fopen(path, "w");
  if (file == NULL) {
    printf("Erro na criação do arquivo SVG.\n");
    exit(1);
  }

  fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");

  svg->svgfile = file;
  return svg;
}

void svg_close(svg_t *svg) {
  if (svg == NULL) return;

  FILE *file = svg->svgfile;

  fprintf(file, "</svg>\n");
  fclose(file);
  free(svg);
}

void svg_write_circle(svg_t *svg, circle_t *circle) {
  if (svg == NULL || circle == NULL) return;
  fprintf(svg->svgfile, "<circle id=\"%zu\" cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"%s\" stroke=\"%s\" fill-opacity=\"0.5\" />\n", circle_get_id(circle), circle_get_x(circle), circle_get_y(circle), circle_get_radius(circle), circle_get_color(circle), circle_get_border_color(circle));
}

void svg_write_rectangle(svg_t *svg, rectangle_t *rect) {
  if (svg == NULL || rect == NULL) return;
  fprintf(svg->svgfile, "<rect id=\"%zu\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"%s\" stroke=\"%s\" fill-opacity=\"0.5\" />\n", rect_get_id(rect), rect_get_x(rect), rect_get_y(rect), rect_get_width(rect), rect_get_height(rect), rect_get_color(rect), rect_get_border_color(rect));
}

void svg_write_line(svg_t *svg, line_t *line) {
  if (svg == NULL || line == NULL) return;
  fprintf(svg->svgfile, "<line id=\"%zu\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" />\n", line_get_id(line), line_get_x1(line), line_get_y1(line), line_get_x2(line), line_get_y2(line), line_get_color(line));
}

void svg_write_text(svg_t *svg, text_t *text) {
  if (svg == NULL || text == NULL) return;
  fprintf(svg->svgfile, "<text id=\"%zu\" x=\"%f\" y=\"%f\" text-anchor=\"%s\" fill=\"%s\" stroke=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%s\" fill-opacity=\"0.5\">\n", text_get_id(text), text_get_x(text), text_get_y(text), text_get_anchor(text), text_get_color(text), text_get_border_color(text), text_get_ffam(text), text_get_fweight(text), text_get_fsize(text));
  fprintf(svg->svgfile, "%s\n", text_get_content(text));
  fprintf(svg->svgfile, "</text>\n");
}

void svg_write_queue(svg_t *svg, queue_t *queue) {
  size_t len = queue_get_length(queue);

  for (size_t i = 0; i < len; i++) {
    node_t *current = queue_dequeue(queue);
    shape_t *shape = (shape_t *) node_getvalue(current);

    switch (shape_get_type(shape)) {
      case CIRCLE:
        svg_write_circle(svg, shape_as_circle(shape));
        break;
      case RECTANGLE:
        svg_write_rectangle(svg, shape_as_rectangle(shape));
        break;
      case LINE:
        svg_write_line(svg, shape_as_line(shape));
        break;
      case TEXT:
        svg_write_text(svg, shape_as_text(shape));
        break;
    }

    queue_enqueue(queue, current);
  }
}
