#ifndef CIRCLE_H
#define CIRCLE_H

#include <stddef.h>

typedef struct circle_t circle_t;

/** @brief    Inicialização de um círculo.
  *
  * @param    id              Identificador do objeto.
  * @param    x               Coordenada X do centro.
  * @param    y               Coordenada Y do centro.
  * @param    radius          Raio do círculo.
  * @param    color           String com o código HEX da cor de preenchimento.
  * @param    border_color    String com o código HEX da cor da borda.
  *
  * @return   Uma instância de círculo.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
circle_t *circle_init(size_t id, double x, double y, double radius, char *color, char *border_color);

/** @brief    Destrói um círculo.
  *
  * @param    circle  Uma instância de círculo.
  */
void circle_destroy(void *circle);

/** @brief    Define o X de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    x       Coordenada X do centro.
  */
void circle_set_x(circle_t *circle, double x);

/** @brief    Define o Y de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    y       Coordenada Y do centro.
  */
void circle_set_y(circle_t *circle, double y);

/** @brief    Define o raio de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    radius  Raio do círculo.
  */
void circle_set_radius(circle_t *circle, double radius);

/** @brief    Define a cor de preenchimento de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    color   String com o código HEX da cor de preenchimento.
  */
void circle_set_color(circle_t *circle, char *color);

/** @brief    Define a cor da borda de um círculo.
  *
  * @param    circle        Uma instância de círculo.
  * @param    border_color  String com o código HEX da cor da borda.
  */
void circle_set_border_color(circle_t *circle, char *border_color);

/** @brief    Retorna o ID de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O ID de um círculo.
  */
size_t circle_get_id(circle_t *circle);

/** @brief    Retorna o X de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O X do centro do círculo.
  */
double circle_get_x(circle_t *circle);

/** @brief    Retorna o Y de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O Y do centro do círculo.
  */
double circle_get_y(circle_t *circle);

/** @brief    Retorna o raio de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O raio do círculo.
  */
double circle_get_radius(circle_t *circle);

/** @brief    Retorna a cor de preenchimento de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   Uma string com a cor de preenchimento do círculo.
  */
char *circle_get_color(circle_t *circle);

/** @brief    Retorna a cor da borda de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   Uma string com a cor da borda do círculo.
  */
char *circle_get_border_color(circle_t *circle);

/** @brief    Retorna a área de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   A área do círculo.
  */
double circle_get_area(circle_t *circle);

/** @brief    Cria uma cópia de um círculo com um novo identificador.
  *
  * @param    circle   Uma instância de círculo a ser clonada.
  * @param    id       Novo identificador para o círculo clonado.
  *
  * @return   Uma nova instância de círculo, idêntica à original, mas com o novo ID.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
circle_t *circle_clone(circle_t *circle, size_t id);

/** @brief    Troca entre si as cores de preenchimento e de borda de um círculo.
  *
  * @param    circle   Uma instância de círculo.
  */
void circle_swap_colors(circle_t *circle);

#endif
