#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../datast/nodes.h"
#include "../datast/binary_tree.h"

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

int cmp_item(const void *a, const void *b) {
    const item_t *data_a = (const item_t *)a;
    const item_t *data_b = (const item_t *)b;

    if (data_a->key < data_b->key) return -1;
    if (data_a->key > data_b->key) return 1;
    return 0;
}

item_t *item_init(int key, const char *name) {
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

    if (!data->name) exit(1);
    return data;
}

/**
 * Verifica se a árvore está vazia (root == NULL).
 */
bool is_tree_empty(bitree_t *bt) {
    return bt_get_root(bt) == NULL;
}

/**
 * Realiza uma in-order traversal e verifica se a ordem dos elementos está correta.
 */
void check_in_order(node_t *node, int *prev_key) {
  if (node == NULL) return;

  check_in_order(node_get_lpt(node), prev_key);

  item_t *current_data = (item_t *)node_get_value(node);

  if (current_data->key < *prev_key) {
    printf("[ERRO de Ordem] Chave %d (atual) é menor que %d (anterior).\n", current_data->key, *prev_key);
    g_test_fail_count++;
  }
  *prev_key = current_data->key;

  check_in_order(node_get_rpt(node), prev_key);
}

void test_initialization() {
  printf("--- Teste 1: Inicialização e Destruição ---\n");
  bitree_t *bt = bt_init(NULL, cmp_item);

  if (bt != NULL && is_tree_empty(bt)) {
    printf("[OK] Árvore inicializada corretamente.\n");
  } else {
    printf("[FALHA] Falha na inicialização.\n");
    g_test_fail_count++;
  }

  bt_destroy(bt);
  printf("[INFO] bt_destroy chamada.\n\n");
}

void test_insertion_and_search() {
  printf("--- Teste 2: Inserção e Busca ---\n");
  g_destructor_count = 0;

  bitree_t *bt = bt_init(NULL, cmp_item);

  // Dados para inserção (5, 2, 8, 1, 6)
  item_t *d5 = item_init(5, "Root");
  item_t *d2 = item_init(2, "Left Child");
  item_t *d8 = item_init(8, "Right Child");
  item_t *d1 = item_init(1, "Leaf 1");
  item_t *d6 = item_init(6, "Leaf 2");

  // Criação dos nodes
  node_t *n5 = node_init(d5, item_destroy);
  node_t *n2 = node_init(d2, item_destroy);
  node_t *n8 = node_init(d8, item_destroy);
  node_t *n1 = node_init(d1, item_destroy);
  node_t *n6 = node_init(d6, item_destroy);

  // Inserção
  bt_insert_node(bt, n5); // Root
  bt_insert_node(bt, n2);
  bt_insert_node(bt, n8);
  bt_insert_node(bt, n1);
  bt_insert_node(bt, n6);
  
  // Verifica a ordem (in-order traversal)
  int prev_key = -2147483648; // Menor int possível
  check_in_order(bt_get_root(bt), &prev_key);
  if (g_test_fail_count > 0) {
    printf("[FALHA] Falha na verificação de ordem (BST).\n");
    g_test_fail_count++;
  } else {
    printf("[OK] Ordem da BST verificada.\n");
  }

  // Busca (existente)
  item_t search_val_5 = {5, NULL};
  node_t *found_n5 = bt_search_node(bt, &search_val_5);
  if (found_n5 != NULL && ((item_t *)node_get_value(found_n5))->key == 5) {
    printf("[OK] Busca de valor 5 bem-sucedida.\n");
  } else {
    printf("[FALHA] Busca de valor 5 falhou.\n");
    g_test_fail_count++;
  }

  // Busca (não existente)
  item_t search_val_99 = {99, NULL};
  node_t *found_n99 = bt_search_node(bt, &search_val_99);
  if (found_n99 == NULL) {
    printf("[OK] Busca de valor 99 (inexistente) bem-sucedida.\n");
  } else {
    printf("[FALHA] Busca de valor 99 (inexistente) encontrou um node.\n");
    g_test_fail_count++;
  }

  // Limpeza
  bt_destroy(bt);
  if (g_destructor_count == 5) { // 5 nodes inseridos
    printf("[OK] Destrutor chamado 5 vezes na destruição da árvore.\n");
  } else {
    printf("[FALHA] Destrutor chamado %d vezes (Esperado: 5).\n", g_destructor_count);
    g_test_fail_count++;
  }
  printf("\n");
}

void test_removal_scenarios() {
  printf("--- Teste 3: Remoção de Node ---\n");
  g_destructor_count = 0;
  
  // Construção de uma árvore com casos de remoção:
  // Raiz: 50
  // Filho esquerdo (2 filhos): 30
  // Filho direito (1 filho): 70
  // Folha: 10
  
  bitree_t *bt = bt_init(NULL, cmp_item);

  // Inserção de 8 nodes
  int keys[] = {50, 30, 70, 10, 40, 60, 80, 5};
  node_t *nodes[8];

  for (int i = 0; i < 8; i++) {
    item_t *d = item_init(keys[i], "Node");
    nodes[i] = node_init(d, item_destroy);
    bt_insert_node(bt, nodes[i]);
  }
  printf("[INFO] 8 nodes inseridos.\n");

  // 3.1: Remoção de Folha (key 5)
  item_t r_leaf = {5, NULL};
  bt_remove_node(bt, &r_leaf);
  if (bt_search_node(bt, &r_leaf) == NULL) {
    printf("[OK] Remoção de Folha (5) bem-sucedida.\n");
  } else {
    printf("[FALHA] Remoção de Folha (5) falhou.\n");
    g_test_fail_count++;
  }
  if (g_destructor_count == 1) { // 1 node deve ter sido liberado
    printf("[OK] Destrutor chamado após remoção de folha.\n");
  } else {
    printf("[FALHA] Destrutor chamado %d vezes (Esperado: 1).\n", g_destructor_count);
    g_test_fail_count++;
  }

  // 3.2: Remoção de Node com 1 Filho (key 70 -> só tem o 80)
  item_t r_one_child = {70, NULL};
  bt_remove_node(bt, &r_one_child);
  if (bt_search_node(bt, &r_one_child) == NULL) {
    printf("[OK] Remoção de Node com 1 Filho (70) bem-sucedida.\n");
  } else {
    printf("[FALHA] Remoção de Node com 1 Filho (70) falhou.\n");
    g_test_fail_count++;
  }

  // 3.3: Remoção de Node com 2 Filhos (key 30 -> tem 10 e 40)
  item_t r_two_children = {30, NULL};
  bt_remove_node(bt, &r_two_children);
  if (bt_search_node(bt, &r_two_children) == NULL) {
    printf("[OK] Remoção de Node com 2 Filhos (30) bem-sucedida.\n");
  } else {
    printf("[FALHA] Remoção de Node com 2 Filhos (30) falhou.\n");
    g_test_fail_count++;
  }

  // 3.4: Remoção da Raiz (key 50)
  item_t r_root = {50, NULL};
  bt_remove_node(bt, &r_root);
  if (bt_search_node(bt, &r_root) == NULL && !is_tree_empty(bt)) { // A árvore não deve estar vazia
    printf("[OK] Remoção da Raiz (50) bem-sucedida.\n");
  } else {
    printf("[FALHA] Remoção da Raiz (50) falhou ou árvore ficou vazia inesperadamente.\n");
    g_test_fail_count++;
  }

  // 3.5: Remoção de elemento não existente (key 99)
  item_t r_non_existent = {99, NULL};
  bt_remove_node(bt, &r_non_existent); // Não deve fazer nada
  printf("[OK] Remoção de elemento não existente não causou falha.\n");

  // Verifica quantos destrutores foram chamados (4 removidos, 4 para destruir)
  if (g_destructor_count == 4) {
    printf("[OK] Destrutor chamado 4 vezes (1 por folha, 1 por 1-filho, 1 por 2-filhos, 1 por raiz).\n");
  } else {
    printf("[FALHA] Destrutor chamado %d vezes (Esperado: 4).\n", g_destructor_count);
    g_test_fail_count++;
  }

  // Limpeza final (destruir o restante)
  bt_destroy(bt);
  if (g_destructor_count == 8) { // Total de 8 nodes deve ser liberado
    printf("[OK] Destrutor chamado total de 8 vezes após bt_destroy.\n");
  } else {
    printf("[FALHA] Destrutor total chamado %d vezes (Esperado: 8).\n", g_destructor_count);
    g_test_fail_count++;
  }
  printf("\n");
}


int main() {
  printf("--- Iniciando Testes para Binary Tree ---\n");

  test_initialization();
  test_insertion_and_search();
  test_removal_scenarios();

  printf("--- Sumário dos Testes ---\n");
  if (g_test_fail_count == 0) {
    printf("SUCESSO: Todos os testes de árvore binária passaram.\n");
    return SUCCESS;
  } else {
    printf("FALHA: %d falhas encontradas nos testes.\n", g_test_fail_count);
    return FAIL;
  }
}
