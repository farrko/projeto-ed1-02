#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/sorting.h" 

static int g_test_fail_count = 0;

typedef struct {
  int id;
  double value;
} item_t;

// Função de comparação para inteiros
int cmp_int(const void *a, const void *b) {
  int val_a = *(const int *)a;
  int val_b = *(const int *)b;

  if (val_a < val_b) return -1;
  if (val_a > val_b) return 1;
  return 0;
}

// Função de comparação para a struct item_t
int cmp_item(const void *a, const void *b) {
  const item_t *item_a = (const item_t *)a;
  const item_t *item_b = (const item_t *)b;

  if (item_a->id < item_b->id) return -1;
  if (item_a->id > item_b->id) return 1;

  // Em caso de id igual, ordena por value
  if (item_a->value < item_b->value) return -1;
  if (item_a->value > item_b->value) return 1;

  return 0;
}

// Função utilitária para imprimir arrays
void print_int_array(int *arr, size_t len) {
  printf("[");
  for (size_t i = 0; i < len; i++) {
    printf("%d%s", arr[i], (i == len - 1) ? "" : ", ");
  }
  printf("]");
}

void print_item_array(item_t *arr, size_t len) {
  printf("[");
  for (size_t i = 0; i < len; i++) {
    printf("{%d, %.1f}%s", arr[i].id, arr[i].value, (i == len - 1) ? "" : ", ");
  }
  printf("]");
}

// Função de verificação para arrays de int
bool is_sorted_int(int *arr, size_t len) {
  for (size_t i = 0; i < len - 1; i++) {
    if (arr[i] > arr[i+1]) return false;
  }
  return true;
}

// Função de verificação para arrays de item_t
bool is_sorted_item(item_t *arr, size_t len) {
  for (size_t i = 0; i < len - 1; i++) {
    if (cmp_item(&arr[i], &arr[i+1]) > 0) return false;
  }
  return true;
}

typedef void (*GenericSortFunc)(void *, size_t, size_t, int (*)(const void *, const void *));

void run_test(const char *algo_name, GenericSortFunc sort_func, size_t threshold) {
  printf("--- Testes para %s (Threshold: %zu) ---\n", algo_name, threshold);
  int test_count = 0;
  int fail_count = 0;

  // Caso 1: array de ints

  // 1.1: Caso aleatório
  int arr1_original[] = {5, 2, 9, 1, 5, 6};
  int arr1[6];
  size_t len1 = 6;
  memcpy(arr1, arr1_original, sizeof(arr1));

  if ((void *)sort_func == (void *)mixed_mergesort) {
    mixed_mergesort(arr1, len1, sizeof(int), cmp_int, threshold);
  } else {
    sort_func(arr1, len1, sizeof(int), cmp_int);
  }
  
  test_count++;
  if (is_sorted_int(arr1, len1)) {
    printf("  [OK] Int Aleatório: ");
  } else {
    printf("  [FALHA] Int Aleatório. Esperado: [1, 2, 5, 5, 6, 9], Obtido: ");
    fail_count++;
  }
  print_int_array(arr1, len1);
  printf("\n");


  // 1.2: Caso inversamente ordenado
  int arr2_original[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  int arr2[10];
  size_t len2 = 10;
  memcpy(arr2, arr2_original, sizeof(arr2));

  if ((void *)sort_func == (void *)mixed_mergesort) {
    mixed_mergesort(arr2, len2, sizeof(int), cmp_int, threshold);
  } else {
    sort_func(arr2, len2, sizeof(int), cmp_int);
  }

  test_count++;
  if (is_sorted_int(arr2, len2)) {
    printf("  [OK] Int Inverso.\n");
  } else {
    printf("  [FALHA] Int Inverso. Obtido: ");
    print_int_array(arr2, len2);
    printf("\n");
    fail_count++;
  }

  // 1.3: Caso array pequeno (para testar insertion sort em mixed)
  int arr3_original[] = {3, 1, 2};
  int arr3[3];
  size_t len3 = 3;
  memcpy(arr3, arr3_original, sizeof(arr3));
  
  if ((void *)sort_func == (void *)mixed_mergesort) {
    mixed_mergesort(arr3, len3, sizeof(int), cmp_int, threshold);
  } else {
    sort_func(arr3, len3, sizeof(int), cmp_int);
  }

  test_count++;
  if (is_sorted_int(arr3, len3)) {
    printf("  [OK] Int Pequeno.\n");
  } else {
    printf("  [FALHA] Int Pequeno. Obtido: ");
    print_int_array(arr3, len3);
    printf("\n");
    fail_count++;
  }

  // Caso 2: array de structs
  item_t items_original[] = {
    {10, 3.5}, {5, 1.0}, {10, 1.2}, {2, 5.0}, {5, 9.9}
  };

  item_t items[5];
  size_t len_item = 5;
  memcpy(items, items_original, sizeof(items));

  if ((void *)sort_func == (void *)mixed_mergesort) {
    mixed_mergesort(items, len_item, sizeof(item_t), cmp_item, threshold);
  } else {
    sort_func(items, len_item, sizeof(item_t), cmp_item);
  }

  test_count++;
  if (is_sorted_item(items, len_item)) {
    printf("  [OK] Struct item_t: ");
  } else {
    printf("  [FALHA] Struct item_t. Esperado: [{2, 5.0}, {5, 1.0}, {5, 9.9}, {10, 1.2}, {10, 3.5}], Obtido: ");
    fail_count++;
  }
  print_item_array(items, len_item);
  printf("\n");

  printf("Resultados: %d/%d testes passaram (%d falhas).\n\n", test_count - fail_count, test_count, fail_count);
  g_test_fail_count += fail_count;
}


int main() {
  size_t THRESHOLD = 4;

  run_test("Insertion Sort", (void *) insertion_sort, 0);
  run_test("Mergesort", (void *) mergesort, 0);
  run_test("Mixed Mergesort", (void *) mixed_mergesort, THRESHOLD);

  printf("--- Sumário Total ---\n");
  if (g_test_fail_count == 0) {
    printf("SUCESSO: Todos os testes de sorting passaram sem falhas.\n");
    return 0;
  } else {
    printf("FALHA: %d falhas encontradas nos testes de sorting.\n", g_test_fail_count);
    return 1;
  }
}
