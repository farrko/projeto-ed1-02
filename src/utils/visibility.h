#ifndef VISIBILITY_H
#define VISIBILITY_H

#include "../shapes/polygon.h"
#include "../datast/linkedlist.h"
#include "../shapes/point.h"

polygon_t *generate_visibility_polygon(llist_t *shapes, llist_t *barriers, point_t *origin);

#endif
