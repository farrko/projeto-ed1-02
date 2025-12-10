#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>

void insertion_sort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *));
void mergesort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *));
void mixed_mergesort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *), size_t threshold);

#endif
