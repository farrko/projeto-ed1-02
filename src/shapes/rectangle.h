#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stddef.h>

typedef struct rectangle_t rectangle_t;

/** @brief    Inicialização de um retângulo.
  *
  * @param    id              Identificador do objeto.
  * @param    x               Coordenada X do canto superior esquerdo.
  * @param    y               Coordenada Y do canto superior esquerdo.
  * @param    width           Largura do retângulo.
  * @param    height          Altura do retângulo.
  * @param    color           String com o código HEX da cor de preenchimento.
  * @param    border_color    String com o código HEX da cor da borda.
  *
  * @return   Uma instância de retângulo.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
rectangle_t *rect_init(size_t id, double x, double y, double width, double height, char *color, char *border_color);

/** @brief    Destrói um retângulo.
  *
  * @param    rectangle   Uma instância de retângulo.
  */
void rect_destroy(void *rectangle);

/** @brief    Define o X de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    x       Coordenada X do canto superior esquerdo.
  */
void rect_set_x(rectangle_t *rect, double x);

/** @brief    Define o Y de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    y       Coordenada Y do canto superior esquerdo.
  */
void rect_set_y(rectangle_t *rect, double y);

/** @brief    Define a largura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    width   Largura do retângulo.
  */
void rect_set_width(rectangle_t *rect, double width);

/** @brief    Define a altura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    height  Altura do retângulo.
  */
void rect_set_height(rectangle_t *rect, double height);

/** @brief    Define a cor de preenchimento de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    color   String com o código HEX da cor de preenchimento.
  */
void rect_set_color(rectangle_t *rect, char *color);

/** @brief    Define a cor da borda de um retângulo.
  *
  * @param    rect          Uma instância de retângulo.
  * @param    border_color  String com o código HEX da cor da borda.
  */
void rect_set_border_color(rectangle_t *rect, char *border_color);

/** @brief    Retorna o ID de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   O ID de um retângulo.
  */
size_t rect_get_id(rectangle_t *rect);

/** @brief    Retorna o X de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   O X do canto superior esquerdo do retângulo.
  */
double rect_get_x(rectangle_t *rect);

/** @brief    Retorna o Y de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @return   O Y do canto superior esquerdo do retângulo.
  */
double rect_get_y(rectangle_t *rect);

/** @brief    Retorna a largura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   A largura do retângulo.
  */
double rect_get_width(rectangle_t *rect);

/** @brief    Retorna a altura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   A altura do retângulo.
  */
double rect_get_height(rectangle_t *rect);

/** @brief    Retorna a cor de preenchimento de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   Uma string com a cor de preenchimento do retângulo.
  */
char *rect_get_color(rectangle_t *rect);

/** @brief    Retorna a cor da borda de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   Uma string com a cor da borda do retângulo.
  */
char *rect_get_border_color(rectangle_t *rect);

/** @brief    Retorna a área de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   A área do retângulo.
  */
double rect_get_area(rectangle_t *rect);

/** @brief    Cria uma cópia de um retângulo com um novo identificador.
  *
  * @param    rect   Uma instância de retângulo a ser clonada.
  * @param    id     Novo identificador para o retângulo clonado.
  *
  * @return   Uma nova instância de retângulo, idêntica à original, mas com o novo ID.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
rectangle_t *rect_clone(rectangle_t *rect, size_t id);

/** @brief    Troca entre si as cores de preenchimento e de borda de um retângulo.
  *
  * @param    rect   Uma instância de retângulo.
  */
void rect_swap_colors(rectangle_t *rect);

#endif
