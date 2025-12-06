#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "visibility.h"

#include "../shapes/shapes.h"

#include "../datast/binary_tree.h"

#define EPSILON 1e-12
#define PI 3.1415926535

typedef struct {
  line_t *barrier;
  double static_distance_to_barrier;
  bool is_start;
  polar_coords_t *polar;
} barrier_angle_aux_t;

typedef struct {
    line_t *barrier;
    double distance;
} active_barrier_t;

static double static_distance_to_barrier(line_t *barrier, point_t *origin) {
  double lx1 = line_get_x1(barrier);
  double lx2 = line_get_x2(barrier);
  double ly1 = line_get_y1(barrier);
  double ly2 = line_get_y2(barrier);

  double ox = point_get_x(origin);
  double oy = point_get_y(origin );

  double lxd = lx2 - lx1;
  double lyd = ly2 - ly1;

  double plx1dx = ox - lx1;
  double ply1dy = oy - ly1;

  double l_len2 = pow(lxd, 2) + pow(lyd, 2);

  if (!l_len2) {
    double dx = ox - lx1;
    double dy = oy - ly1;
    return sqrt(pow(dx, 2) + pow(dy, 2));
  }

  double t = (plx1dx * lxd + ply1dy * lyd) / l_len2;

  if (t < 0) {
    t = 0;
  } else if (t > 1) t = 1;

  double projx = lx1 + t * lxd;
  double projy = ly1 + t * lyd;

  double dx = ox - projx;
  double dy = oy - projy;

  return sqrt(pow(dx, 2) + pow(dy, 2));
}

static barrier_angle_aux_t *barriers_to_angles(llist_t *barriers, point_t *origin, size_t *bta_len) {
  size_t barriers_len = llist_get_length(barriers);
  if (!barriers_len) return NULL;

  barrier_angle_aux_t *angles = calloc(barriers_len * 4, sizeof(barrier_angle_aux_t));
  size_t angles_i = 0;

  node_t *current_barrier = llist_get_head(barriers);
  for (size_t i = 0; i < barriers_len; i++) {
    line_t *barrier = node_get_value(current_barrier);

    polar_coords_t *b_p1_p = polar_from_cartesian(origin,  line_get_p1(barrier));
    polar_coords_t *b_p2_p = polar_from_cartesian(origin, line_get_p2(barrier));

    double diff = polar_get_angle(b_p2_p) - polar_get_angle(b_p1_p);
    if (diff < 0) diff += 2.0 * PI;
    if (diff > PI) {
      polar_coords_t *aux = b_p1_p;
      b_p1_p = b_p2_p;
      b_p2_p = aux;
    }
    
    double distance = static_distance_to_barrier(barrier, origin);

    if (polar_get_angle(b_p1_p) > polar_get_angle(b_p2_p)) {
      polar_coords_t *to_pi = polar_init(PI, distance, origin);
      polar_coords_t *from_zero = polar_init(0, distance, origin);

      angles[angles_i++] = (barrier_angle_aux_t) { barrier, distance, true, b_p1_p};
      angles[angles_i++] = (barrier_angle_aux_t) { barrier, distance, false, to_pi};

      angles[angles_i++] = (barrier_angle_aux_t) { barrier, distance, true, from_zero};
      angles[angles_i++] = (barrier_angle_aux_t) { barrier, distance, false, b_p2_p};
    } else {
      angles[angles_i++] = (barrier_angle_aux_t) { barrier, distance, true, b_p1_p };
      angles[angles_i++] = (barrier_angle_aux_t) { barrier, distance, false, b_p2_p };
    }

    current_barrier = node_get_rpt(current_barrier);
  }

  *bta_len = angles_i;
  return angles;
}

static int cmp_baa(const void *a, const void *b) {
  barrier_angle_aux_t *baa_a = (barrier_angle_aux_t *) a;
  barrier_angle_aux_t *baa_b = (barrier_angle_aux_t *) b;

  double baa_a_angle = polar_get_angle(baa_a->polar);
  double baa_b_angle = polar_get_angle(baa_b->polar);

  if (fabs(baa_a_angle - baa_b_angle) < EPSILON) {
    if (baa_a->is_start != baa_b->is_start) return baa_a->is_start ? -1 : 1;

    if (baa_a->static_distance_to_barrier < baa_b->static_distance_to_barrier - EPSILON) return -1;
    if (baa_a->static_distance_to_barrier > baa_b->static_distance_to_barrier + EPSILON) return 1;

    return 0;
  }

  if (baa_a_angle < baa_b_angle) return -1;
  return 1;
}

static int cmp_active_barriers(const void *a, const void *b) {
  active_barrier_t *ba = (active_barrier_t *) a;
  active_barrier_t *bb = (active_barrier_t *) b;

  if (fabs(ba->distance - bb->distance) > EPSILON) return ba->distance < bb->distance ? -1 : 1;

  if (line_get_id(ba->barrier) < line_get_id(bb->barrier)) return -1;
  if (line_get_id(ba->barrier) > line_get_id(bb->barrier)) return 1;

  return 0;
}

static void active_barriers_update(bitree_t *active_barriers, barrier_angle_aux_t *baa) {
  if (baa->is_start) {
    active_barrier_t *new_ab = malloc(sizeof(active_barrier_t));
    if (new_ab == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    new_ab->barrier = baa->barrier;
    new_ab->distance = baa->static_distance_to_barrier;
    bt_insert_node(active_barriers, node_init(new_ab, free));
  } else {
    active_barrier_t b = { baa->barrier, baa->static_distance_to_barrier };
    void *removed = bt_remove_node(active_barriers, &b);
    if (removed) free(removed);
  }
}

static double dynamic_distance_to_barrier(line_t *barrier, point_t *origin, double angle) {
  double ox = point_get_x(origin);
  double oy = point_get_y(origin);

  // 16384 é um tamanho arbitrário para criar uma linha e calcular a distância até a interseção
  const double R = 16384;

  double rx = ox + R * cos(angle);
  double ry = oy + R * sin(angle);

  double lx1 = line_get_x1(barrier);
  double lx2 = line_get_x2(barrier);
  double ly1 = line_get_y1(barrier);
  double ly2 = line_get_y2(barrier);

  double denom = (ly2 - ly1) * (rx - ox) - (lx2 - lx1) * (ry - oy);

  if (fabs(denom) < EPSILON) return INFINITY;

  double ua = ((lx2 - lx1) * (oy - ly1) - (ly2 - ly1) * (ox - lx1)) / denom;
  double ub = ((rx - ox) * (oy - ly1) - (ry - oy) * (ox - lx1)) / denom;

  if (ua >= 0 - EPSILON && ua <= 1 + EPSILON && ub >= 0 - EPSILON && ub <= 1 + EPSILON) {
    double dist = ub * R;

    if (dist < EPSILON) return 0;
    return dist;
  }

  return INFINITY;
}

static void find_intersection_rec(node_t *node, point_t *origin, double angle, double *distance_to_barrier) {
  if (node == NULL) return;

  find_intersection_rec(node_get_lpt(node), origin, angle, distance_to_barrier);

  active_barrier_t *ab = node_get_value(node);
  double distance = dynamic_distance_to_barrier(ab->barrier, origin, angle);
  if (distance < *distance_to_barrier) *distance_to_barrier = distance;

  find_intersection_rec(node_get_rpt(node), origin, angle, distance_to_barrier);
}

static polar_coords_t *raycast(bitree_t *active_barriers, point_t *origin, double angle) {
  double distance_to_barrier = INFINITY;
  find_intersection_rec(bt_get_root(active_barriers), origin, angle, &distance_to_barrier);

  if (distance_to_barrier == INFINITY) return polar_init(angle, 16384, origin);

  return polar_init(angle, distance_to_barrier, origin);
}

polygon_t *generate_visibility_polygon(llist_t *shapes, llist_t *barriers, point_t *origin) {
  polygon_t *visibility = py_init();

  node_t *current_shape = llist_get_head(shapes);

  double min_x = shape_get_x(node_get_value(current_shape));
  double max_x = shape_get_x(node_get_value(current_shape));
  double min_y = shape_get_y(node_get_value(current_shape));
  double max_y = shape_get_y(node_get_value(current_shape));

  // Cálculo de bounding box
  size_t shapes_len = llist_get_length(shapes);
  for (size_t i = 0; i < shapes_len; i++) {
    shape_t *shape = node_get_value(current_shape);

    double shape_x = shape_get_x(shape);
    double shape_y = shape_get_y(shape);

    if (shape_x < min_x) min_x = shape_x;
    if (shape_x > max_x) max_x = shape_x;
    if (shape_y < min_y) min_y = shape_y;
    if (shape_y > max_y) max_y = shape_y;

    current_shape = node_get_rpt(current_shape);
  }

  min_x -= 5;
  max_x += 5;
  min_y -= 5;
  max_y += 5;

  // Inserção da bounding box na lista de anteparos
  llist_insertat_end(barriers, node_init(line_init(0, min_x, min_y, max_x, min_y, NULL), line_destroy));
  llist_insertat_end(barriers, node_init(line_init(0, min_x, min_y, min_x, max_y, NULL), line_destroy));
  llist_insertat_end(barriers, node_init(line_init(0, max_x, max_y, max_x, min_y, NULL), line_destroy));
  llist_insertat_end(barriers, node_init(line_init(0, max_x, max_y, min_x, max_y, NULL), line_destroy));

  size_t angles_len;
  barrier_angle_aux_t *angles = barriers_to_angles(barriers, origin, &angles_len);

  qsort(angles, angles_len, sizeof(barrier_angle_aux_t), cmp_baa);

  bitree_t *active_barriers = bt_init(NULL, cmp_active_barriers);

  // Pre-load
  for (size_t i = 0; i < angles_len; i++) {
    if (angles[i].is_start && polar_get_angle(angles[i].polar) < EPSILON) active_barriers_update(active_barriers, &angles[i]);
  }

  for (size_t i = 0; i < angles_len; i++) {
    barrier_angle_aux_t *current = &angles[i];

    if (angles[i].is_start && polar_get_angle(angles[i].polar) < EPSILON) continue;

    polar_coords_t *pre_update_point = raycast(active_barriers, origin, polar_get_angle(current->polar));

    active_barriers_update(active_barriers, &angles[i]);

    polar_coords_t *post_update_point = raycast(active_barriers, origin, polar_get_angle(current->polar));

    if (fabs(polar_get_distance(pre_update_point) - polar_get_distance(post_update_point)) < EPSILON) {
      point_t *single_point = cartesian_from_polar(pre_update_point);
      py_add_vertex(visibility, single_point);

      point_destroy(single_point);
    } else {
      point_t *pre_point = cartesian_from_polar(pre_update_point);
      point_t *post_point = cartesian_from_polar(post_update_point);

      py_add_vertex(visibility, pre_point);
      py_add_vertex(visibility, post_point);

      point_destroy(pre_point);
      point_destroy(post_point);
    }

    polar_destroy(pre_update_point);
    polar_destroy(post_update_point);
  }

  // Cleanup
  for (size_t i = 0; i < 4; i++) {
    node_destroy(llist_popat_end(barriers));
  }

  for (size_t i = 0; i < angles_len; i++) {
    polar_destroy(angles[i].polar);
  }
  free(angles);

  bt_destroy(active_barriers);

  return visibility;
}
