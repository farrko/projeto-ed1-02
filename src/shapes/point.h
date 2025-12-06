#ifndef POINT_H
#define POINT_H

typedef struct point_t point_t;
typedef struct polar_coords_t polar_coords_t;

point_t *point_init(double x, double y);
polar_coords_t *polar_init(double angle, double distance, point_t *origin);

void point_destroy(void *point);
void polar_destroy(void *polar);

double point_get_x(point_t *point);
double point_get_y(point_t *point);
void point_set_x(point_t *point, double x);
void point_set_y(point_t *point, double y);

double polar_get_angle(polar_coords_t *polar);
double polar_get_distance(polar_coords_t *polar);
point_t *polar_get_origin(polar_coords_t *polar);

void polar_set_angle(polar_coords_t *polar, double angle);
void polar_set_distance(polar_coords_t *polar, double distance);
void polar_set_origin(polar_coords_t *polar, point_t *origin);

double polar_get_absolute_x(polar_coords_t *polar);
double polar_get_absolute_y(polar_coords_t *polar);

polar_coords_t *polar_from_cartesian(point_t *origin, point_t *final);
point_t *cartesian_from_polar(polar_coords_t *polar);

#endif
