#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "polygon.h"
#include "rectangle.h"
#include "shapes.h"
#include "text.h"

#define EPSILON 1e-12

struct polygon_t {
  llist_t *vertices;
};

polygon_t *py_init() {
  polygon_t *py = malloc(sizeof(polygon_t));
  if (py == NULL) {
    printf("Erro na alocação de memória.");
    exit(1);
  }

  py->vertices = llist_init();

  return py;
}

void py_add_vertex(polygon_t *py, point_t *point) {
  point_t *point_clone = point_init(point_get_x(point), point_get_y(point));
  llist_insertat_end(py->vertices, node_init(point_clone, point_destroy));
}

llist_t *py_get_vertices(polygon_t *py) {
  return py->vertices;
}

// Funções necessárias para o cálculo de sobreposição de polígonos
static double point_segment_distance(point_t *a, point_t *b, point_t *p);
static bool polygon_point_overlap(polygon_t *py, point_t *p);

static double cross(double x1, double y1, double x2, double y2);
static bool on_segment(double x1, double y1, double x2, double y2, double px, double py);
static bool segments_intersect(point_t *a, point_t *b, point_t *c, point_t *d);

static bool circle_point_overlap(circle_t *c, point_t *p);
static bool polygon_circle_overlap(polygon_t *py, circle_t *c);

static bool rectangle_point_overlap(rectangle_t *r, point_t *p);
static bool polygon_rectangle_overlap(polygon_t *py, rectangle_t *r);

static bool polygon_line_overlap(polygon_t *py, line_t *l);

static bool polygon_text_overlap(polygon_t *py, text_t *t);

bool polygon_overlap(polygon_t *py, shape_t *shape) {
  switch(shape_get_type(shape)) {
    case CIRCLE: return polygon_circle_overlap(py, shape_as_circle(shape));
    case RECTANGLE: return polygon_rectangle_overlap(py, shape_as_rectangle(shape));
    case LINE: return polygon_line_overlap(py, shape_as_line(shape));
    case TEXT: return polygon_text_overlap(py, shape_as_text(shape));
    default: return false;
  }
}

static double point_segment_distance(point_t *a, point_t *b, point_t *p) {
  double ax = point_get_x(a);
  double ay = point_get_y(a);
  double bx = point_get_x(b);
  double by = point_get_y(b);
  double px = point_get_x(p);
  double py = point_get_y(p);

  double ab_x = bx - ax;
  double ab_y = by - ay;
  double ap_x = px - ax;
  double ap_y = py - ay;

  double ab_len_sq = pow(ab_x, 2) + pow(ab_y, 2);

  if (ab_len_sq < EPSILON) return sqrt(pow(px - ax, 2) + pow(py - ay, 2));

  double t = (ap_x * ab_x + ap_y * ab_y) / ab_len_sq;

  if (t < 0.0) {
    t = 0.0;
  } else if (t > 1.0) t = 1.0;

  double projx = ax + t * ab_x;
  double projy = ay + t * ab_y;

  return sqrt(pow(px - projx, 2) + pow(py - projy, 2));
}

static bool polygon_point_overlap(polygon_t *py, point_t *p) {
  bool is_inside = false;

  node_t *current = llist_get_head(py->vertices);
  size_t vertices_len = llist_get_length(py->vertices);

  for (size_t i = 0; i < vertices_len; i++) {
    node_t *next = node_get_rpt(current);
    if (next == NULL) next = llist_get_head(py->vertices);

    point_t *point_i = node_get_value(current);
    point_t *point_j = node_get_value(next);

    double xi = point_get_x(point_i);
    double yi = point_get_y(point_i);
    double xj = point_get_x(point_j);
    double yj = point_get_y(point_j);

    double xp = point_get_x(p);
    double yp = point_get_y(p);

    if (fabs(yj - yi) < EPSILON) {
      current = next;
      continue;
    }

    bool has_intersection = ((yi > yp) != (yj > yp)) && (xp < (xj - xi) * (yp - yi) / (yj - yi) + xi);
    if (has_intersection) is_inside = !is_inside;

    current = next;
  }

  return is_inside;
}

static double cross(double x1, double y1, double x2, double y2) {
  return x1 * y2 - y1 * x2;
}

static bool on_segment(double x1, double y1, double x2, double y2, double px, double py) {
  return (px >= fmin(x1, x2) && px <= fmax(x1, x2) && py >= fmin(y1, y2) && py <= fmax(y1, y2));
}

static bool segments_intersect(point_t *a, point_t *b, point_t *c, point_t *d) {
  double ax = point_get_x(a);
  double ay = point_get_y(a);
  double bx = point_get_x(b);
  double by = point_get_y(b);
  double cx = point_get_x(c);
  double cy = point_get_y(c);
  double dx = point_get_x(d);
  double dy = point_get_y(d);

  double ab_x = bx - ax;
  double ab_y = by - ay;
  double ac_x = cx - ax;
  double ac_y = cy - ay;
  double ad_x = dx - ax;
  double ad_y = dy - ay;

  double cd_x = dx - cx;
  double cd_y = dy - cy;
  double ca_x = ax - cx;
  double ca_y = ay - cy;
  double cb_x = bx - cx;
  double cb_y = by - cy;

  double cross1 = cross(ab_x, ab_y, ac_x, ac_y);
  double cross2 = cross(ab_x, ab_y, ad_x, ad_y);
  double cross3 = cross(cd_x, cd_y, ca_x, ca_y);
  double cross4 = cross(cd_x, cd_y, cb_x, cb_y);

  // Teste de interseção própria
  if (((cross1 > 0 && cross2 < 0) || (cross1 < 0 && cross2 > 0)) && ((cross3 > 0 && cross4 < 0) || (cross3 < 0 && cross4 > 0))) return true;

  // Teste de casos colineares
  if (fabs(cross1) < EPSILON && on_segment(ax, ay, bx, by, cx, cy)) return true;
  if (fabs(cross2) < EPSILON && on_segment(ax, ay, bx, by, dx, dy)) return true;
  if (fabs(cross3) < EPSILON && on_segment(cx, cy, dx, dy, ax, ay)) return true;
  if (fabs(cross4) < EPSILON && on_segment(cx, cy, dx, dy, bx, by)) return true;

  return false;
}

static bool circle_point_overlap(circle_t *c, point_t *p) {
  double cx = circle_get_x(c);
  double cy = circle_get_y(c);
  double r  = circle_get_radius(c);

  double px = point_get_x(p);
  double py = point_get_y(p);

  double dx = px - cx;
  double dy = py - cy;

  return (pow(dx, 2) + pow(dy, 2)) <= pow(r, 2);
}

static bool polygon_circle_overlap(polygon_t *py, circle_t *c) {
  llist_t *pyv = py->vertices;
  size_t pyv_len = llist_get_length(pyv);

  point_t *origin = circle_get_origin(c);

  if (polygon_point_overlap(py, origin)) return true;

  node_t *current = llist_get_head(pyv);
  for (size_t i = 0; i < pyv_len; i++) {
    node_t *next = node_get_rpt(current);
    if (next == NULL) next = llist_get_head(pyv);

    point_t *vertex = node_get_value(current);
    point_t *next_vertex = node_get_value(next);

    if (circle_point_overlap(c, vertex)) return true;
    if (point_segment_distance(vertex, next_vertex, origin) <= circle_get_radius(c)) return true;

    current = next;
  }

  return false;
}

static bool rectangle_point_overlap(rectangle_t *r, point_t *p) {
  double px = point_get_x(p);
  double py = point_get_y(p);

  double rx = point_get_x(rect_get_origin(r));
  double ry = point_get_y(rect_get_origin(r));

  return (px >= rx && px <= rx + rect_get_width(r)) && (py >= ry && py <= ry + rect_get_height(r));
}

static bool polygon_rectangle_overlap(polygon_t *py, rectangle_t *r) {
  double rx = point_get_x(rect_get_origin(r));
  double ry = point_get_y(rect_get_origin(r));
  double rw = rect_get_width(r);
  double rh = rect_get_height(r);

  point_t *rect_vertices[4];

  rect_vertices[0] = point_init(rx, ry);
  rect_vertices[1] = point_init(rx + rw, ry);
  rect_vertices[2] = point_init(rx + rw, ry + rh);
  rect_vertices[3] = point_init(rx, ry + rh);

  for (size_t i = 0; i < 4; i++) {
    bool overlap = polygon_point_overlap(py, rect_vertices[i]);

    if (overlap) {
      for (size_t j = 0; j < 4; j++) {
        point_destroy(rect_vertices[j]);
      }

      return true;
    }
  }

  node_t *current = llist_get_head(py->vertices);
  size_t vertices_len = llist_get_length(py->vertices);
  for (size_t i = 0; i < vertices_len; i++) {
    node_t *next = node_get_rpt(current);
    if (!next) next = llist_get_head(py->vertices);

    point_t *vertex = node_get_value(current);
    point_t *next_vertex = node_get_value(next);

    for (size_t j = 0; j < 4; j++) {
      point_t *rv1 = rect_vertices[j];
      point_t *rv2 = rect_vertices[(j + 1) % 4];

      bool overlap = segments_intersect(vertex, next_vertex, rv1, rv2) || rectangle_point_overlap(r, vertex);

      if (overlap) {
        for (size_t k = 0; k < 4; k++) {
          point_destroy(rect_vertices[k]);
        }

        return true;
      }
    }

    current = next;
  }

  for (size_t i = 0; i < 4; i++) {
    point_destroy(rect_vertices[i]);
  }

  return false;
}

static bool polygon_line_overlap(polygon_t *py, line_t *l) {
  point_t *l0 = line_get_p1(l);
  point_t *l1 = line_get_p2(l);

  if (polygon_point_overlap(py, l0) || polygon_point_overlap(py, l1)) return true;

  node_t *current = llist_get_head(py->vertices);
  size_t vertices_len = llist_get_length(py->vertices);

  for (size_t i = 0; i < vertices_len; i++) {
      node_t *next = node_get_rpt(current);
      if (!next) next = llist_get_head(py->vertices);

      point_t *v0 = node_get_value(current);
      point_t *v1 = node_get_value(next);

      if (segments_intersect(v0, v1, l0, l1)) return true;

      current = next;
  }

  return false;
}

static bool polygon_text_overlap(polygon_t *py, text_t *t) {
  line_t *l = text_line_collision(t);
  bool overlap = polygon_line_overlap(py, l);

  line_destroy(l);
  return overlap;
}
