#include "sorting.h"
#include <stdlib.h>
#include <string.h>

void insertion_sort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *)) {
  if (array == NULL || length < 2 || compare == NULL) return;

  char *base = (char *) array;
  void *current = malloc(size);

  for (size_t i = 1; i < length; i++) {
    memcpy(current, base + (i * size), size);

    long j = i - 1;
    while (j >= 0 && compare(base + (j * size), current) > 0) {
      memcpy(base + ((j + 1) * size), base + (j * size), size);
      j--;
    }

    memcpy(base + ((j + 1) * size), current, size);
  }

  free(current);
}

static void merge(void *array, size_t left, size_t mid, size_t right, size_t size, int (*compare)(const void *, const void *)) {
  size_t len_left = mid - left + 1;
  size_t len_right = right - mid;

  void *left_array = malloc(len_left * size);
  void *right_array = malloc(len_right * size);

  char *base = (char *) array;
  char *L = (char *) left_array;
  char *R = (char *) right_array;

  memcpy(L, base + (left * size), len_left * size);
  memcpy(R, base + ((mid + 1) * size), len_right * size);

  size_t i = 0;
  size_t j = 0;
  size_t k = left;

  while(i < len_left && j < len_right) {
    if (compare(L + (i * size), R + (j * size)) <= 0) {
      memcpy(base + (k++ * size), L + (i++ * size), size);
    } else memcpy(base + (k++ * size), R + (j++ * size), size);
  }

  if (i < len_left) memcpy(base + (k * size), L + (i * size), (len_left - i) * size);
  if (j < len_right) memcpy(base + (k * size), R + (j * size), (len_right - j) * size);

  free(left_array);
  free(right_array);
}

static void mergesort_rec(void *array, size_t left, size_t right, size_t size, int (*compare)(const void *, const void *)) {
  if (left >= right) return;

  size_t mid = left + (right - left) / 2;
  mergesort_rec(array, left, mid, size, compare);
  mergesort_rec(array, mid + 1, right, size, compare);
  merge(array, left, mid, right, size, compare);
}

void mergesort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *)) {
  if (array == NULL || !length || compare == NULL) return;

  mergesort_rec(array, 0, length - 1, size, compare);
}

static void mixed_mergesort_rec(void *array, size_t left, size_t right, size_t size, int (*compare)(const void *, const void *), size_t threshold) {
  if (left >= right) return;

  size_t len = right - left + 1;
  char *base = (char *) array;

  if (len <= threshold && threshold > 0) return insertion_sort(base + (left * size), len, size, compare);

  size_t mid = left + (right - left) / 2;
  mixed_mergesort_rec(array, left, mid, size, compare, threshold);
  mixed_mergesort_rec(array, mid + 1, right, size, compare, threshold);

  if (compare(base + (mid * size), base + ((mid + 1) * size)) <= 0) return;

  merge(array, left, mid, right, size, compare);
}

void mixed_mergesort(void *array, size_t length, size_t size, int (*compare)(const void *, const void *), size_t threshold) {
  if (array == NULL || !length || compare == NULL) return;

  mixed_mergesort_rec(array, 0, length - 1, size, compare, threshold);
}
