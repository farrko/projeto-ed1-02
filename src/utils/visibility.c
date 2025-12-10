#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "visibility.h"

#include "../datast/linkedlist.h"
#include "../datast/binary_tree.h"
#include "sorting.h"

#define EPSILON 1e-12
#define PI 3.14159265358979323846

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

static double bbox_distance(point_t *origin, double angle, double min_x, double min_y, double max_x, double max_y) {
  double ox = point_get_x(origin);
  double oy = point_get_y(origin);

  double dx = cos(angle);
  double dy = sin(angle);

  double t_min = INFINITY;

  if (fabs(dx) > EPSILON) {
    double t = (min_x - ox) / dx;
    if (t >= 0) t_min = fmin(t_min, t);
  }

  if (fabs(dx) > EPSILON) {
    double t = (max_x - ox) / dx;
    if (t >= 0) t_min = fmin(t_min, t);
  }

  if (fabs(dy) > EPSILON) {
    double t = (min_y - oy) / dy;
    if (t >= 0) t_min = fmin(t_min, t);
  }

  if (fabs(dy) > EPSILON) {
    double t = (max_y - oy) / dy;
    if (t >= 0) t_min = fmin(t_min, t);
  }

  return t_min;
}

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
    line_t *barrier = shape_as_line(node_get_value(current_barrier));

    polar_coords_t *b_p1_p = polar_from_cartesian(origin,  line_get_p1(barrier));
    polar_coords_t *b_p2_p = polar_from_cartesian(origin, line_get_p2(barrier));

    double a1 = polar_get_angle(b_p1_p);
    double a2 = polar_get_angle(b_p2_p);

    double da = a2 - a1;

    while (da > PI) da -= 2 * PI;
    while (da < -PI) da += 2 * PI;

    if (da < 0) {
      polar_coords_t *tmp = b_p1_p;
      b_p1_p = b_p2_p;
      b_p2_p = tmp;
    }

    a1 = polar_get_angle(b_p1_p);
    a2 = polar_get_angle(b_p2_p);

    printf("Barreira %zu - ângulo do primeiro ponto: %.3lf, ângulo do segundo ponto: %.3lf\n", line_get_id(barrier), a1 * 180/PI, a2 * 180/PI);

    double distance = static_distance_to_barrier(barrier, origin);

    if (a1 > a2) {
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

    return (line_get_id(baa_a->barrier) < line_get_id(baa_b->barrier)) ? -1 : 1;
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

    printf("Nova barreira inserida na árvore: id = %zu\n", line_get_id(baa->barrier));

    new_ab->barrier = baa->barrier;
    new_ab->distance = baa->static_distance_to_barrier;
    bt_insert_node(active_barriers, node_init(new_ab, free));
  } else {
    active_barrier_t b = { baa->barrier, baa->static_distance_to_barrier };
    printf("Barreira removida da árvore: id = %zu\n", line_get_id(baa->barrier));
    bt_remove_node(active_barriers, &b);
  }
}

double dynamic_distance_to_barrier(line_t *barrier, point_t *origin, double angle) {
  double ox = point_get_x(origin);
  double oy = point_get_y(origin);

  double x1 = line_get_x1(barrier);
  double y1 = line_get_y1(barrier);
  double x2 = line_get_x2(barrier);
  double y2 = line_get_y2(barrier);

  double rdx = cos(angle);
  double rdy = sin(angle);

  double sdx = x2 - x1;
  double sdy = y2 - y1;

  double qpx = x1 - ox;
  double qpy = y1 - oy;

  double rxs = rdx * sdy - rdy * sdx;

  if (fabs(rxs) < EPSILON) return INFINITY;

  double t = (qpx * sdy - qpy * sdx) / rxs;
  double u = (qpx * rdy - qpy * rdx) / rxs;

  if (fabs(t) < EPSILON) t = 0.0;
  if (fabs(u) < EPSILON) u = 0.0;
  if (fabs(u - 1.0) < EPSILON) u = 1.0;

  if (t >= 0.0 && u >= 0.0 && u <= 1.0) return t;

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

static polar_coords_t *raycast(bitree_t *active_barriers, point_t *origin, double angle, double min_x, double min_y, double max_x, double max_y) {
  printf("Ângulo: %.3lf - cos = %f, sin = %f\n", angle * 180/PI, cos(angle), sin(angle));

  double distance_to_barrier = INFINITY;
  find_intersection_rec(bt_get_root(active_barriers), origin, angle, &distance_to_barrier);

  double distance_to_bbox = bbox_distance(origin, angle, min_x, min_y, max_x, max_y);

  double distance = fmin(distance_to_barrier, distance_to_bbox);

  return polar_init(angle, distance, origin);
}

polygon_t *generate_visibility_polygon(llist_t *barriers, point_t *origin, char sort_type, size_t threshold) {
  polygon_t *visibility = py_init();

  double min_x = 0;
  double min_y = 0;
  double max_x = 1024;
  double max_y = 1024;

  line_t *upper_bb = line_init(100100, min_x, min_y, max_x, min_y, "black");
  line_t *lower_bb = line_init(100101, min_x, max_y, max_x, max_y, "black");
  line_t *left_bb = line_init(100102, min_x, min_y, min_x, max_y, "black");
  line_t *right_bb = line_init(100103, max_x, min_y, max_x, max_y, "black");

  llist_insertat_end(barriers, shape_as_node(shape_init(LINE, upper_bb)));
  llist_insertat_end(barriers, shape_as_node(shape_init(LINE, lower_bb)));
  llist_insertat_end(barriers, shape_as_node(shape_init(LINE, left_bb)));
  llist_insertat_end(barriers, shape_as_node(shape_init(LINE, right_bb)));

  size_t angles_len;
  barrier_angle_aux_t *angles = barriers_to_angles(barriers, origin, &angles_len);

  if (sort_type == 'q') {
    qsort(angles, angles_len, sizeof(barrier_angle_aux_t), cmp_baa);
  } else mixed_mergesort(angles, angles_len, sizeof(barrier_angle_aux_t), cmp_baa, threshold);

  bitree_t *active_barriers = bt_init(NULL, cmp_active_barriers);

  // Pre-load
  for (size_t i = 0; i < angles_len; i++) {
    if (angles[i].is_start && polar_get_angle(angles[i].polar) < EPSILON) active_barriers_update(active_barriers, &angles[i]);
  }

  for (size_t i = 0; i < angles_len; i++) {
    barrier_angle_aux_t *current = &angles[i];

    if (angles[i].is_start && polar_get_angle(angles[i].polar) < EPSILON) continue;

    polar_coords_t *pre_update_point = raycast(active_barriers, origin, polar_get_angle(current->polar), min_x, min_y, max_x, max_y);
    point_t *pre_point = cartesian_from_polar(pre_update_point);
    py_add_vertex(visibility, pre_point);
    printf("Ponto: (%.5lf, %.5lf)\n\n", polar_get_absolute_x(pre_update_point), polar_get_absolute_y(pre_update_point));

    active_barriers_update(active_barriers, &angles[i]);

    polar_coords_t *post_update_point = raycast(active_barriers, origin, polar_get_angle(current->polar), min_x, min_y, max_x, max_y);
    point_t *post_point = cartesian_from_polar(post_update_point);
    py_add_vertex(visibility, post_point);
    printf("Ponto: (%.5lf, %.5lf)\n\n", polar_get_absolute_x(post_update_point), polar_get_absolute_y(post_update_point));

    point_destroy(pre_point);
    point_destroy(post_point);
    polar_destroy(pre_update_point);
    polar_destroy(post_update_point);
  }


  for (size_t i = 0; i < angles_len; i++) {
    polar_destroy(angles[i].polar);
  }
  free(angles);

  bt_destroy(active_barriers);

  for(size_t i = 0; i < 4; i++) {
    node_destroy(llist_popat_end(barriers));
  }

  return visibility;
}
