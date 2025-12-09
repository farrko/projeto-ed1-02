#include <stdio.h>
#include <stdlib.h>

// Assumindo que você tem os headers:
#include "../datast/nodes.h"
#include "../datast/binary_tree.h"

// --- Funções Auxiliares (Inteiros) ---

// Função de comparação para inteiros
int compare_int(const void *a, const void *b) {
  int val_a = *(const int *)a;
  int val_b = *(const int *)b;

  if (val_a < val_b) return -1;
  if (val_a > val_b) return 1;
  return 0;
}

// Função destrutora para inteiros alocados dinamicamente
void free_int(void *data) {
  if (data != NULL) {
    free(data);
  }
}

// Aloca um novo inteiro e retorna o ponteiro
int *new_int(int val) {
  int *ptr = malloc(sizeof(int));
  if (ptr == NULL) {
    perror("Erro ao alocar inteiro");
    exit(1);
  }
  *ptr = val;
  return ptr;
}

// Função para imprimir a árvore (In-Order Traversal)
static void print_in_order_rec(node_t *node) {
    if (node == NULL) return;
    print_in_order_rec(node_get_lpt(node));
    printf("%d ", *(int *)node_get_value(node));
    print_in_order_rec(node_get_rpt(node));
}

void print_tree(bitree_t *bt, const char *msg) {
    printf("--- %s ---\n", msg);
    if (bt_get_root(bt) == NULL) {
        printf("Árvore vazia.\n");
        return;
    }
    printf("Elementos (In-Order): ");
    print_in_order_rec(bt_get_root(bt));
    printf("\n");
}

// --- Casos de Teste ---

void test_initialization_and_insertion() {
  printf("\n## 1. Teste de Inicialização e Inserção\n");
  
  // 1. Inicializa a raiz com 50
  node_t *root_node = node_init(new_int(50), free_int);
  bitree_t *bt = bt_init(root_node, compare_int);
  
  // 2. Inserção de vários elementos para formar uma BST
  int values[] = {25, 75, 12, 37, 60, 80, 40};
  for (int i = 0; i < 7; i++) {
    node_t *new = node_init(new_int(values[i]), free_int);
    bt_insert_node(bt, new);
  }
  
  print_tree(bt, "Árvore Após Inserção");
  
  // 3. Teste de Inserção de valor duplicado (deve falhar e imprimir erro)
  printf("Tentativa de inserir 50 (duplicado):\n");
  node_t *dup = node_init(new_int(50), free_int);
  bt_insert_node(bt, dup); // Deve imprimir "Erro na inserção..."
  node_destroy(dup); // O nó duplicado não foi inserido, então deve ser destruído.

  bt_destroy(bt);
  printf("Teste de Inserção concluído.\n");
}

void test_search() {
  printf("\n## 2. Teste de Busca\n");
  
  // Setup: Reconstroi a árvore
  node_t *root_node = node_init(new_int(50), free_int);
  bitree_t *bt = bt_init(root_node, compare_int);
  int values[] = {25, 75, 12, 37, 60, 80, 40};
  for (int i = 0; i < 7; i++) {
    bt_insert_node(bt, node_init(new_int(values[i]), free_int));
  }
  
  // 1. Busca de valor existente (75)
  int search_val_exist = 75;
  node_t *found_exist = bt_search_node(bt, &search_val_exist);
  if (found_exist && *(int *)node_get_value(found_exist) == 75) {
    printf("Busca por 75: SUCESSO.\n");
  } else {
    printf("Busca por 75: FALHA.\n");
  }

  // 2. Busca de valor inexistente (99)
  int search_val_non_exist = 99;
  node_t *found_non_exist = bt_search_node(bt, &search_val_non_exist);
  if (found_non_exist == NULL) {
    printf("Busca por 99: SUCESSO (NULL retornado).\n");
  } else {
    printf("Busca por 99: FALHA.\n");
  }

  bt_destroy(bt);
  printf("Teste de Busca concluído.\n");
}


void test_removal() {
  printf("\n## 3. Teste de Remoção (Todos os Casos)\n");
  
  // Setup: Árvore base: 50, 25, 75, 12, 37, 60, 80, 40
  node_t *root_node = node_init(new_int(50), free_int);
  bitree_t *bt = bt_init(root_node, compare_int);
  int values[] = {25, 75, 12, 37, 60, 80, 40};
  for (int i = 0; i < 7; i++) {
    bt_insert_node(bt, node_init(new_int(values[i]), free_int));
  }
  
  print_tree(bt, "Árvore Inicial");

  // --- Caso 1: Remoção de Nó Folha (0 filhos) ---
  int remove_0_child = 12;
  printf("\nRemovendo 12 (Leaf Node)...\n");
  bt_remove_node(bt, &remove_0_child); // O valor é destruído internamente
  if (bt_search_node(bt, &remove_0_child) == NULL) {
      printf("Remoção de 12: SUCESSO.\n");
  } else {
      printf("Remoção de 12: FALHA.\n");
  }
  
  // --- Caso 2: Remoção de Nó com 1 Filho ---
  // A. Remover 75 (com 1 filho, 80)
  int remove_1_child_a = 75;
  printf("\nRemovendo 75 (1 Filho, 80, que se torna o sucessor da raiz direita)... \n");
  bt_remove_node(bt, &remove_1_child_a); // A raiz direita agora deve ser 80
  if (bt_search_node(bt, &remove_1_child_a) == NULL && *(int *)node_get_value(node_get_rpt(bt_get_root(bt))) == 80) {
      printf("Remoção de 75: SUCESSO.\n");
  } else {
      printf("Remoção de 75: FALHA.\n");
  }
  
  // B. Remover 25 (com 1 filho, 37)
  int remove_1_child_b = 25;
  printf("\nRemovendo 25 (1 Filho, 37)... \n");
  bt_remove_node(bt, &remove_1_child_b); // A raiz esquerda agora deve ser 37
  if (bt_search_node(bt, &remove_1_child_b) == NULL && *(int *)node_get_value(node_get_lpt(bt_get_root(bt))) == 37) {
      printf("Remoção de 25: SUCESSO.\n");
  } else {
      printf("Remoção de 25: FALHA.\n");
  }

  // --- Caso 3: Remoção de Raiz (2 filhos) ---
  // A árvore atual é: 50(R), 37(L), 60(R.L), 80(R.R), 40(L.R)
  // Sucessor In-Order de 50 é 60.
  int remove_2_children = 50;
  printf("\nRemovendo 50 (Raiz, 2 Filhos)...\n");
  bt_remove_node(bt, &remove_2_children); // A nova raiz deve ser 60
  
  int new_root_val = 60;
  if (*(int *)node_get_value(bt_get_root(bt)) == new_root_val && bt_search_node(bt, &remove_2_children) == NULL) {
      printf("Remoção de 50: SUCESSO (Nova Raiz é %d).\n", new_root_val);
  } else {
      printf("Remoção de 50: FALHA.\n");
  }

  print_tree(bt, "Árvore Após Todas as Remoções");
  
  bt_destroy(bt);
  printf("Teste de Remoção concluído.\n");
}


// --- Função Principal ---
/*
int main() {
  printf("==========================================\n");
  printf(" TESTES DE ÁRVORE BINÁRIA DE BUSCA (BST) \n");
  printf("==========================================\n");

  test_initialization_and_insertion();
  
  printf("\n------------------------------------------\n");
  
  test_search();

  printf("\n------------------------------------------\n");

  test_removal();

  printf("\n------------------------------------------\n");
  
  printf("Todos os testes concluídos.\n");
  
  return 0;
}
*/
