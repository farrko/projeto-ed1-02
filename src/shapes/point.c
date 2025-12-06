#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "point.h"

struct point_t {
  double x, y;
};

struct polar_coords_t {
  double angle;
  double distance;
  point_t *origin;
};

point_t *point_init(double x, double y) {
  point_t *point = malloc(sizeof(point_t));
  if (point == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  point->x = x;
  point->y = y;

  return point;
}

polar_coords_t *polar_init(double angle, double distance, point_t *origin) {
  polar_coords_t *polar = malloc(sizeof(polar_coords_t));
  if (polar == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  if (angle < 0) angle += 2.0 * 3.1415926535;

  polar->angle = angle;
  polar->distance = distance;

  point_t *origin_clone = point_init(origin->x, origin->y);
  polar->origin = origin_clone;

  return polar;
}

void point_destroy(void *point) {
  free((point_t *) point);
}

void polar_destroy(void *polar) {
  free(((polar_coords_t *) polar)->origin);
  free((polar_coords_t *) polar);
}

double point_get_x(point_t *point) {
  return point->x;
}

double point_get_y(point_t *point) {
  return point->y;
}

void point_set_x(point_t *point, double x) {
  point->x = x;
}

void point_set_y(point_t *point, double y) {
  point->y = y;
}

double polar_get_angle(polar_coords_t *polar) {
  return polar->angle;
}

double polar_get_distance(polar_coords_t *polar) {
  return polar->distance;
}

point_t *polar_get_origin(polar_coords_t *polar) {
  return polar->origin;
}

void polar_set_angle(polar_coords_t *polar, double angle) {
  polar->angle = angle;
}

void polar_set_distance(polar_coords_t *polar, double distance) {
  polar->distance = distance;
}

void polar_set_origin(polar_coords_t *polar, point_t *origin) {
  double absolute_x = polar_get_absolute_x(polar);
  double absolute_y = polar_get_absolute_y(polar);

  double angle = atan2(origin->y - absolute_y, origin->x - absolute_x);
  double distance = sqrt(pow(origin->x - absolute_x, 2) + pow(origin->y - absolute_y, 2));

  if (angle < 0) angle += 2.0 * 3.1415926535;

  point_t *origin_clone = point_init(origin->x, origin->y);
  free(polar->origin);

  polar->angle = angle;
  polar->distance = distance;
  polar->origin = origin_clone;
}

double polar_get_absolute_x(polar_coords_t *polar) {
  return polar->origin->x + polar->distance * cos(polar->angle);
}

double polar_get_absolute_y(polar_coords_t *polar) {
  return polar->origin->y + polar->distance * sin(polar->angle);
}

polar_coords_t *polar_from_cartesian(point_t *origin, point_t *final) {
  double angle = atan2(origin->y - final->y, origin->x - final->x);
  double distance = sqrt(pow(origin->x - final->x, 2) + pow(origin->y - final->y, 2));

  if (angle < 0) angle += 2.0 * 3.1415926535;

  return polar_init(angle, distance, origin);
}

point_t *cartesian_from_polar(polar_coords_t *polar) {
  return point_init(polar_get_absolute_x(polar), polar_get_absolute_y(polar));
}
