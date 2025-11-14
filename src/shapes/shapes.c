#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "shapes.h"
#include "circle.h"
#include "line.h"
#include "rectangle.h"
#include "text.h"

struct shape_t {
  E_SHAPETYPE shapetype;
  void *shape;
};

shape_t *shape_init(E_SHAPETYPE type, void *shape) {
  shape_t *s = malloc(sizeof(shape_t));
  if (s == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  s->shapetype = type;
  s->shape = shape;

  return s;
}

void shape_destroy(void *shape) {
  shape_t *s = (shape_t *) shape;

   switch(s->shapetype) {
    case CIRCLE:
      circle_destroy(s->shape);
      break;
    case RECTANGLE:
      rect_destroy(s->shape);
      break;
    case LINE:
      line_destroy(s->shape);
      break;
    case TEXT:
      text_destroy(s->shape);
      break;
  }

  free(shape);
}

E_SHAPETYPE shape_get_type(shape_t *shape) {
  return shape->shapetype;
}

circle_t *shape_as_circle(shape_t *shape) {
  return (circle_t *) shape->shape;
}

rectangle_t *shape_as_rectangle(shape_t *shape) {
  return (rectangle_t *) shape->shape;
}

line_t *shape_as_line(shape_t *shape) {
  return (line_t *) shape->shape;
}

text_t *shape_as_text(shape_t *shape) {
  return (text_t *) shape->shape;
}

node_t *shape_as_node(shape_t *shape) {
  return node_init(shape, shape_destroy);
}

size_t shape_get_id(shape_t *shape) {
  switch (shape->shapetype) {
    case CIRCLE:
      return circle_get_id(shape_as_circle(shape));
    case RECTANGLE:
      return rect_get_id(shape_as_rectangle(shape));
    case LINE:
      return line_get_id(shape_as_line(shape));
    case TEXT:
      return text_get_id(shape_as_text(shape));
    default:
      return 0;
  }
}

double shape_get_x(shape_t *shape) {
  switch (shape->shapetype) {
    case CIRCLE:
      return circle_get_x(shape_as_circle(shape));
    case RECTANGLE:
      return rect_get_x(shape_as_rectangle(shape));
    case LINE:
      return line_get_x1(shape_as_line(shape));
    case TEXT:
      return text_get_x(shape_as_text(shape));
    default:
      return 0;
  }
}

double shape_get_y(shape_t *shape) {
  switch (shape->shapetype) {
    case CIRCLE:
      return circle_get_y(shape_as_circle(shape));
    case RECTANGLE:
      return rect_get_y(shape_as_rectangle(shape));
    case LINE:
      return line_get_y1(shape_as_line(shape));
    case TEXT:
      return text_get_y(shape_as_text(shape));
    default:
      return 0;
  }
}

void shape_move(shape_t *shape, double lx, double ly, double dx, double dy) {
  switch (shape_get_type(shape)) {
    case CIRCLE: {
      circle_t *circle = shape_as_circle(shape);
      circle_set_x(circle, lx + dx);
      circle_set_y(circle, ly + dy);
      break;
    }
    case RECTANGLE: {
      rectangle_t *rect = shape_as_rectangle(shape);
      rect_set_x(rect, lx + dx);
      rect_set_y(rect, ly + dy);
      break;
    }
    case LINE: {
      line_t *line = shape_as_line(shape);
      double x1x2diff = line_get_x2(line) - line_get_x1(line);
      double y1y2diff = line_get_y2(line) - line_get_y1(line);

      line_set_x1(line, lx + dx);
      line_set_y1(line, ly + dy);
      line_set_x2(line, lx + dx + x1x2diff);
      line_set_y2(line, ly + dy + y1y2diff); 
      break;
    }
    case TEXT: {
      text_t *text = shape_as_text(shape);
      text_set_x(text, lx + dx);
      text_set_y(text, ly + dy);
    }
  }
}

void shape_set_color(shape_t *shape, char *color) { 
  switch (shape->shapetype) {
    case CIRCLE:
      circle_set_color(shape_as_circle(shape), color);
      break;
    case RECTANGLE:
      rect_set_color(shape_as_rectangle(shape), color);
      break;
    case TEXT:
      text_set_color(shape_as_text(shape), color);
      break;
    case LINE:
      line_set_color(shape_as_line(shape), color);
      break;
  }
}

shape_t *shape_clone(shape_t *shape, size_t id) {
  switch (shape->shapetype) {
    case CIRCLE: 
      return shape_init(CIRCLE, circle_clone(shape_as_circle(shape), id));
    case RECTANGLE:
      return shape_init(RECTANGLE, rect_clone(shape_as_rectangle(shape), id));
    case LINE:
      return shape_init(LINE, line_clone(shape_as_line(shape), id));
    case TEXT:
      return shape_init(TEXT, text_clone(shape_as_text(shape), id));
    default: 
      return NULL;
  }
}

// Funções para o cálculo de overlap
double clamp(double n, double min, double max) {
  return n < min ? min : (n > max ? max : n);
}

bool ccw(double Ax, double Ay, double Bx, double By, double Cx, double Cy) {
    return (Cy - Ay) * (Bx - Ax) > (By - Ay) * (Cx - Ax);
}

bool circle_circle_overlap(circle_t *c1, circle_t *c2) {
  double dx = circle_get_x(c1) - circle_get_x(c2);
  double dy = circle_get_y(c1) - circle_get_y(c2);

  double distSqrd = pow(dx, 2) + pow(dy, 2);
  double radiusSum = circle_get_radius(c1) + circle_get_radius(c2);

  return distSqrd <= pow(radiusSum, 2);
}

bool rect_rect_overlap(rectangle_t *r1, rectangle_t *r2) {
  double r1x = rect_get_x(r1);
  double r1y = rect_get_y(r1);
  double r1width = rect_get_width(r1);
  double r1height = rect_get_height(r1);

  double r2x = rect_get_x(r2);
  double r2y = rect_get_y(r2);
  double r2width = rect_get_width(r2);
  double r2height = rect_get_height(r2);

  return !(r1x + r1width < r2x || r2x + r2width < r1x || r1y + r1height < r2y || r2y + r2height < r1y);
}

bool line_line_overlap(line_t *l1, line_t *l2) {
  double l1x1 = line_get_x1(l1);
  double l1y1 = line_get_y1(l1);
  double l1x2 = line_get_x2(l1);
  double l1y2 = line_get_y2(l1);

  double l2x1 = line_get_x1(l2);
  double l2y1 = line_get_y1(l2);
  double l2x2 = line_get_x2(l2);
  double l2y2 = line_get_y2(l2);

  bool cond1 = ccw(l1x1, l1y1, l2x1, l2y1, l2x2, l2y2);
  bool cond2 = ccw(l1x2, l1y2, l2x1, l2y1, l2x2, l2y2);
  bool cond3 = ccw(l1x1, l1y1, l1x2, l1y2, l2x1, l2y1);
  bool cond4 = ccw(l1x1, l1y1, l1x2, l1y2, l2x2, l2y2);

  return (cond1 != cond2) && (cond3 != cond4);
}

bool text_text_overlap(text_t *t1, text_t *t2) {
  line_t *t1l = text_line_collision(t1);
  line_t *t2l = text_line_collision(t2);

  bool overlap = line_line_overlap(t1l, t2l);

  line_destroy(t1l);
  line_destroy(t2l);

  return overlap;
}

bool circle_rect_overlap(circle_t *c, rectangle_t *r) {
  double cx = circle_get_x(c);
  double cy = circle_get_y(c);

  double rx = rect_get_x(r);
  double ry = rect_get_y(r);
  double rwidth = rect_get_width(r);
  double rheight = rect_get_height(r);


  double closestX = clamp(cx, rx, rx + rwidth);
  double closestY = clamp(cy, ry, ry + rheight);

  double dx = cx - closestX;
  double dy = cy - closestY;

  return pow(dx, 2) + pow(dy, 2) <= pow(circle_get_radius(c), 2);
}

bool circle_line_overlap(circle_t *c, line_t *l) {
  double lx1 = line_get_x1(l);
  double ly1 = line_get_y1(l);
  double lx2 = line_get_x2(l);
  double ly2 = line_get_y2(l);

  double cx = circle_get_x(c);
  double cy = circle_get_y(c);

  double dx = lx2 - lx1;
  double dy = ly2 - ly1;

  double fx = cx - lx1;
  double fy = cy - ly1;

  double lenSqrd = pow(dx, 2) + pow(dy, 2);
  double t = (fx * dx + fy * dy) / lenSqrd;
  t = clamp(t, 0.0, 1.0);

  double closestX = lx1 + t * dx;
  double closestY = ly1 + t * dy;

  double distX = closestX - cx;
  double distY = closestY - cy;

  return pow(distX, 2) + pow(distY, 2) <= pow(circle_get_radius(c), 2);
}

bool rect_line_overlap(rectangle_t *r, line_t *l) {
  double rx = rect_get_x(r);
  double ry = rect_get_y(r);
  double rwidth = rect_get_width(r);
  double rheight = rect_get_height(r);

  double lx1 = line_get_x1(l);
  double ly1 = line_get_y1(l);
  double lx2 = line_get_x2(l);
  double ly2 = line_get_y2(l);

  // Checa se existe um ponto de L dentro de R
  bool inside1 = (lx1 >= rx && lx1 <= rx + rwidth && ly1 >= ry && ly1 <= ry + rheight);
  bool inside2 = (lx2 >= rx && lx2 <= rx + rwidth && ly2 >= ry && ly2 <= ry + rheight);
  if (inside1 || inside2) return true;

  // Criação de linhas em cada aresta do retângulo, para usar a line_line_overlap para verificar se existe sobreposição
  line_t *top_edge = line_init(0, rx, ry, rx + rwidth, ry, NULL);
  line_t *left_edge = line_init(0, rx, ry, rx, ry + rheight, NULL);
  line_t *right_edge = line_init(0, rx + rwidth, ry, rx + rwidth, ry + rheight, NULL);
  line_t *bottom_edge = line_init(0, rx, ry + rheight, rx + rwidth, ry + rheight, NULL);

  bool tl = line_line_overlap(l, top_edge);
  bool ll = line_line_overlap(l, left_edge);
  bool rl = line_line_overlap(l, right_edge);
  bool bl = line_line_overlap(l, bottom_edge);

  line_destroy(top_edge);
  line_destroy(left_edge);
  line_destroy(right_edge);
  line_destroy(bottom_edge);

  if (tl || ll || rl || bl) return true;

  return false;
}

bool circle_text_overlap(circle_t *c, text_t *t) {
  line_t *tl = text_line_collision(t);

  bool overlap = circle_line_overlap(c, tl);

  line_destroy(tl);

  return overlap;
}

bool rect_text_overlap(rectangle_t *r, text_t *t) {
  line_t *tl = text_line_collision(t);

  bool overlap = rect_line_overlap(r, tl);

  line_destroy(tl);

  return overlap;
}

bool line_text_overlap(line_t *l, text_t *t) {
  line_t *tl = text_line_collision(t);

  bool overlap = line_line_overlap(l, tl);

  line_destroy(tl);

  return overlap;
}

bool shape_overlap(shape_t *i, shape_t *j) {
  switch (i->shapetype) {
    case CIRCLE: {
      switch (j->shapetype) {
        case CIRCLE: return circle_circle_overlap(shape_as_circle(i), shape_as_circle(j));
        case RECTANGLE: return circle_rect_overlap(shape_as_circle(i), shape_as_rectangle(j));
        case LINE: return circle_line_overlap(shape_as_circle(i), shape_as_line(j));
        case TEXT: return circle_text_overlap(shape_as_circle(i), shape_as_text(j));
      }
      break;
    }
    case RECTANGLE: {
      switch (j->shapetype) {
        case CIRCLE: return circle_rect_overlap(shape_as_circle(j), shape_as_rectangle(i));
        case RECTANGLE: return rect_rect_overlap(shape_as_rectangle(i), shape_as_rectangle(j));
        case LINE: return rect_line_overlap(shape_as_rectangle(i), shape_as_line(j));
        case TEXT: return rect_text_overlap(shape_as_rectangle(i), shape_as_text(j));
      }
      break;
    }
    case LINE: {
      switch (j->shapetype) {
        case CIRCLE: return circle_line_overlap(shape_as_circle(j), shape_as_line(i));
        case RECTANGLE: return rect_line_overlap(shape_as_rectangle(j), shape_as_line(i));
        case LINE: return line_line_overlap(shape_as_line(i), shape_as_line(j));
        case TEXT: return line_text_overlap(shape_as_line(i), shape_as_text(j));
      }
      break;
    }
    case TEXT: {
      switch (j->shapetype) {
        case CIRCLE: return circle_text_overlap(shape_as_circle(j), shape_as_text(i));
        case RECTANGLE: return rect_text_overlap(shape_as_rectangle(j), shape_as_text(i));
        case LINE: return line_text_overlap(shape_as_line(j), shape_as_text(i));
        case TEXT: return text_text_overlap(shape_as_text(i), shape_as_text(j));
      }
      break;
    }
  }

  return false;
}
