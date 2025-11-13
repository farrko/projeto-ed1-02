#ifndef SHAPES_H
#define SHAPES_H

#include <stdbool.h>

#include "circle.h"
#include "rectangle.h"
#include "line.h"
#include "text.h"
#include "../datast/linkedlist.h"

typedef struct shape_t shape_t;
typedef enum {
  CIRCLE,
  RECTANGLE,
  LINE,
  TEXT,
} E_SHAPETYPE;

/** @brief    Inicialização de um shape.
  *
  * @param    type    O tipo da forma armazenada pelo shape.
  * @param    shape   Um pointer para a forma a ser armazenada.
  *
  * @return   Uma instância de shape.
  */
shape_t *shape_init(E_SHAPETYPE type, void *shape);

/** @brief    Destrói um shape e a sua forma associada.
  *
  * @param    shape   Uma instância de shape.
  */
void shape_destroy(void *shape);

/** @brief    Retorna o tipo da forma armazenada no shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Um membro de E_SHAPETYPE que define o tipo do elemento associado.
  */
E_SHAPETYPE shape_get_type(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como um círculo.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de círculo.
  */
circle_t *shape_as_circle(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como um retângulo.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de retângulo.
  */
rectangle_t *shape_as_rectangle(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como uma linha.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de linha.
  */
line_t *shape_as_line(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como um texto.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de texto.
  */
text_t *shape_as_text(shape_t *shape);


/** @brief    Inicializa um node com um shape associado.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de node.
  */
node_t *shape_as_node(shape_t *shape);

/** @brief    Retorna o identificador associado ao shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   O identificador único do shape.
  */
size_t shape_get_id(shape_t *shape);

/** @brief    Retorna a coordenada X do shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   A posição X do shape.
  */
double shape_get_x(shape_t *shape);

/** @brief    Retorna a coordenada Y do shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   A posição Y do shape.
  */
double shape_get_y(shape_t *shape);

/** @brief    Define a coordenada X do shape.
  *
  * @param    shape   Uma instância de shape.
  */
void shape_set_x(shape_t *shape);

/** @brief    Define a coordenada Y do shape.
  *
  * @param    shape   Uma instância de shape.
  */
void shape_set_y(shape_t *shape);

/** @brief    Retorna a cor de preenchimento do shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma string representando a cor de preenchimento.
  */
char *shape_get_color(shape_t *shape);

/** @brief    Retorna a cor da borda do shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma string representando a cor da borda.
  */
char *shape_get_border_color(shape_t *shape);

/** @brief    Define a cor da borda do shape.
  *
  * @param    shape         Uma instância de shape.
  * @param    border_color  String da cor de borda.
  */
void shape_set_border_color(shape_t *shape, char *border_color);

/** @brief    Move o shape aplicando deslocamentos lineares.
  *
  * @param    shape   Uma instância de shape.
  * @param    lx      Posição X inicial.
  * @param    ly      Posição Y inicial.
  * @param    dx      Deslocamento no eixo X.
  * @param    dy      Deslocamento no eixo Y.
  */
void shape_move(shape_t *shape, double lx, double ly, double dx, double dy);

/** @brief    Calcula a área do shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   A área calculada do shape.
  */
double shape_get_area(shape_t *shape);

/** @brief    Cria uma cópia do shape com um novo identificador.
  *
  * @param    shape   Uma instância de shape a ser clonada.
  * @param    id      Novo identificador para o shape clonado.
  *
  * @return   Uma nova instância de shape.
  */
shape_t *shape_clone(shape_t *shape, size_t id);

/** @brief    Calcula a cor complementar de uma cor hexadecimal.
  *
  * @param    color   Uma string representando a cor no formato hexadecimal.
  *
  * @return   Uma nova string contendo a cor complementar em formato hexadecimal.
  */
char *calc_complementary(char *color);

/** @brief    Troca as cores de preenchimento e borda do shape.
  *
  * @param    shape   Uma instância de shape.
  */
void shape_swap_colors(shape_t *shape);

/** @brief    Verifica se dois shapes se sobrepõem.
  *
  * @param    i   Primeira instância de shape.
  * @param    j   Segunda instância de shape.
  *
  * @return   true se houver sobreposição; caso contrário, false.
  */
bool shape_overlap(shape_t *i, shape_t *j);

#endif
