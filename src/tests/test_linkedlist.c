#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../datast/nodes.h"
#include "../datast/linkedlist.h"

static int g_destructor_count = 0;
static int g_test_fail_count = 0;
static const int SUCCESS = 0;
static const int FAIL = 1;

typedef struct {
    int key;
    char *name;
} item_t;

void item_destroy(void *value) {
  if (value) {
    item_t *data = (item_t *)value;
    free(data->name);
    free(data);
    g_destructor_count++;
  }
}

item_t *int_data_init(int key, const char *name) {
  item_t *data = malloc(sizeof(item_t));
  if (!data) exit(1);
  data->key = key;

  size_t name_len = strlen(name);
  data->name = malloc(name_len + 1);
  if (!data->name) {
      free(data);
      exit(1);
  }
  strcpy(data->name, name);

  return data;
}

void test_initialization() {
  printf("--- Teste 1: Inicialização e Tamanho Vazio ---\n");
  llist_t *llist = llist_init();

  if (llist != NULL && llist_get_length(llist) == 0 &&
    llist_get_head(llist) == NULL && llist_get_tail(llist) == NULL) {
    printf("[OK] Lista inicializada corretamente.\n");
  } else {
    printf("[FALHA] Falha na inicialização ou estados incorretos.\n");
    g_test_fail_count++;
  }

  llist_destroy(llist);
  printf("\n");
}

void test_insertion() {
  printf("--- Teste 2: Inserções (Start, End, Index) ---\n");
  llist_t *llist = llist_init();
  g_destructor_count = 0;

  // Dados para inserção
  item_t *d1 = int_data_init(1, "Primeiro");
  item_t *d2 = int_data_init(2, "Meio");
  item_t *d3 = int_data_init(3, "Ultimo");
  item_t *d4 = int_data_init(4, "Start");

  node_t *n1 = node_init(d1, item_destroy);
  node_t *n2 = node_init(d2, item_destroy);
  node_t *n3 = node_init(d3, item_destroy);
  node_t *n4 = node_init(d4, item_destroy);

  // 2.1: Inserção no final: [1]
  llist_insertat_end(llist, n1);
  if (llist_get_length(llist) == 1 && llist_get_head(llist) == n1 && llist_get_tail(llist) == n1) {
    printf("[OK] Inserção no final (1) bem-sucedida.\n");
  } else {
    printf("[FALHA] Inserção no final (1).\n");
    g_test_fail_count++;
  }

  // 2.2: Inserção no final: [1, 3]
  llist_insertat_end(llist, n3);
  if (llist_get_length(llist) == 2 && llist_get_head(llist) == n1 && llist_get_tail(llist) == n3) {
    printf("[OK] Inserção no final (3) bem-sucedida.\n");
  } else {
    printf("[FALHA] Inserção no final (3).\n");
    g_test_fail_count++;
  }

  // 2.3: Inserção no começo: [4, 1, 3]
  llist_insertat_start(llist, n4);
  if (llist_get_length(llist) == 3 && llist_get_head(llist) == n4 && llist_get_tail(llist) == n3) {
    printf("[OK] Inserção no começo (4) bem-sucedida.\n");
  } else {
    printf("[FALHA] Inserção no começo (4).\n");
    g_test_fail_count++;
  }
  
  // 2.4: Inserção no índice 1: [4, 2, 1, 3]
  llist_insertat_index(llist, n2, 1);
  node_t *n_at_1 = llist_getat_index(llist, 1);
  if (llist_get_length(llist) == 4 && n_at_1 == n2) {
    printf("[OK] Inserção no índice 1 (2) bem-sucedida.\n");
  } else {
    printf("[FALHA] Inserção no índice 1 (2).\n");
    g_test_fail_count++;
  }

  // 2.:5 Inserção em índice inválido (deve ser ignorada)
  size_t initial_len = llist_get_length(llist);
  item_t *d_bad = int_data_init(99, "Bad");
  node_t *n_bad = node_init(d_bad, item_destroy);
  llist_insertat_index(llist, n_bad, initial_len + 1);

  if (llist_get_length(llist) == initial_len) {
    printf("[OK] Inserção em índice inválido ignorada.\n");
  } else {
    printf("[FALHA] Inserção em índice inválido aceita.\n");
    g_test_fail_count++;
  }
  
  // Desalocar o node "Bad" manualmente, pois ele não foi inserido na lista
  node_destroy(n_bad); 

  llist_destroy(llist);
  // 4 nodes inseridos + 1 node "Bad" destruído manualmente = 5 chamadas
  if (g_destructor_count == 5) {
    printf("[OK] Destrutor chamado 5 vezes na destruição final (4 da lista + 1 manual).\n");
  } else {
    printf("[FALHA] Destrutor chamado %d vezes (Esperado: 5).\n", g_destructor_count);
    g_test_fail_count++;
  }
  printf("\n");
}

void test_pop_and_destruction() {
  printf("--- Teste 3: Pop e Destruição (Remove) ---\n");
  llist_t *llist = llist_init();
  g_destructor_count = 0;

  // Inserir 5 nodes: [10, 20, 30, 40, 50]
  for (int i = 0; i < 5; i++) {
    item_t *d = int_data_init((i + 1) * 10, "PopTest");
    llist_insertat_end(llist, node_init(d, item_destroy));
  }
  printf("[INFO] 5 nodes inseridos. Tamanho: %zu\n", llist_get_length(llist));

  // 3.1: Pop do começo: remove 10. Lista: [20, 30, 40, 50]
  node_t *n_start = llist_popat_start(llist);
  if (llist_get_length(llist) == 4 && n_start != NULL && ((item_t *)node_get_value(n_start))->key == 10) {
    printf("[OK] Pop do começo (10) bem-sucedido.\n");
  } else {
    printf("[FALHA] Pop do começo (10).\n");
    g_test_fail_count++;
  }
  node_destroy(n_start); // Desalocar o node retornado (1 chamada ao destrutor)

  // 3.2: Pop do final: remove 50. Lista: [20, 30, 40]
  node_t *n_end = llist_popat_end(llist);
  if (llist_get_length(llist) == 3 && n_end != NULL && ((item_t *)node_get_value(n_end))->key == 50) {
    printf("[OK] Pop do final (50) bem-sucedido.\n");
  } else {
    printf("[FALHA] Pop do final (50).\n");
    g_test_fail_count++;
  }
  node_destroy(n_end); // Desalocar o node retornado (2 chamadas ao destrutor)

  // 3.3: Pop do índice 1: remove 30. Lista: [20, 40]
  node_t *n_index = llist_popat_index(llist, 1);
  if (llist_get_length(llist) == 2 && n_index != NULL && ((item_t *)node_get_value(n_index))->key == 30) {
    printf("[OK] Pop no índice 1 (30) bem-sucedido.\n");
  } else {
    printf("[FALHA] Pop no índice 1 (30).\n");
    g_test_fail_count++;
  }
  node_destroy(n_index); // Desalocar o node retornado (3 chamadas ao destrutor)

  // 3.4: Destruição in-place (llist_destroyat)
  // Destruir no índice 0: remove 20. Lista: [40]
  llist_destroyat(llist, 0);
  if (llist_get_length(llist) == 1 && ((item_t *)node_get_value(llist_get_head(llist)))->key == 40) {
    printf("[OK] llist_destroyat no índice 0 (20) bem-sucedido.\n");
  } else {
    printf("[FALHA] llist_destroyat no índice 0 (20).\n");
    g_test_fail_count++;
  }

  // 3.5: Tentativa de Pop em índice inválido
  node_t *n_invalid = llist_popat_index(llist, 50);
  if (n_invalid == NULL && llist_get_length(llist) == 1) {
    printf("[OK] Pop em índice inválido retorna NULL.\n");
  } else {
    printf("[FALHA] Pop em índice inválido não retornou NULL ou alterou tamanho.\n");
    g_test_fail_count++;
  }

  // Verificação parcial do destrutor (4 nodes removidos/destruídos in-place)
  if (g_destructor_count == 4) {
    printf("[OK] Destrutor chamado 4 vezes após remoções.\n");
  } else {
    printf("[FALHA] Destrutor chamado %d vezes (Esperado: 4).\n", g_destructor_count);
    g_test_fail_count++;
  }

  // 3.6: Limpeza final (1 node restante)
  llist_destroy(llist);
  if (g_destructor_count == 5) { // Total de 5 nodes deve ter sido liberado
    printf("[OK] Destrutor total chamado 5 vezes após llist_destroy.\n");
  } else {
    printf("[FALHA] Destrutor total chamado %d vezes (Esperado: 5).\n", g_destructor_count);
    g_test_fail_count++;
  }
  printf("\n");
}

int main() {
  printf("--- Iniciando Testes para Linked List ---\n");

  test_initialization();
  test_insertion();
  test_pop_and_destruction();

  printf("--- Sumário dos Testes ---\n");
  if (g_test_fail_count == 0) {
    printf("SUCESSO: Todos os testes de linked list passaram.\n");
    return SUCCESS;
  } else {
    printf("FALHA: %d falhas encontradas nos testes.\n", g_test_fail_count);
    return FAIL;
  }
}
