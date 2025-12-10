#ifndef LINE_H
#define LINE_H

#include <stddef.h>
#include "point.h"

typedef struct line_t line_t;

/** @brief    Inicialização de uma linha.
  *
  * @param    id      Identificador do objeto.
  * @param    x1      Coordenada X1.
  * @param    y1      Coordenada Y1.
  * @param    x2      Coordenada X2.
  * @param    y2      Coordenada Y2.
  * @param    color   String com o código HEX da cor de preenchimento.
  *
  * @return   Uma instância de linha.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
line_t *line_init(size_t id, double x1, double y1, double x2, double y2, char *color);



/** @brief    Destrói uma linha.
  *
  * @param    line    Uma instância de linha.
  */
void line_destroy(void *line);



/** @brief    Define o id associado a uma linha.
  *
  * @param    line Pointer para uma linha.
  * @param    id   Identificador a ser atribuído.
  */
void line_set_id(line_t *line, size_t id);

/** @brief    Define o X1 de uma linha.
  *
  * @param    line    Uma instância de linha.
  * @param    x1      Coordenada X1.
  */
void line_set_x1(line_t *line, double x1);



/** @brief    Define o Y1 de uma linha.
  *
  * @param    line    Uma instância de linha.
  * @param    y1      Coordenada Y1.
  */
void line_set_y1(line_t *line, double y1);



/** @brief    Define o X2 de uma linha.
  *
  * @param    line    Uma instância de linha.
  * @param    x2      Coordenada X2.
  */

void line_set_x2(line_t *line, double x2);



/** @brief    Define o Y2 de uma linha.
  *
  * @param    line    Uma instância de linha.
  * @param    y2      Coordenada Y2.
  */

void line_set_y2(line_t *line, double y2);



/** @brief    Define a cor de preenchimento de uma linha.
  *
  * @param    line    Uma instância de linha.
  * @param    color   String com o código HEX da cor de preenchimento.
  */
void line_set_color(line_t *line, char *color);



/** @brief    Retorna o ID de uma linha.
  *
  * @param    line    Uma instância de linha.
  *
  * @return   O ID de uma linha.
  */
size_t line_get_id(line_t *line);



/** @brief    Obtém o primeiro ponto de uma linha.
  *
  * @param    line Pointer para uma linha.
  *
  * @return   O primeiro ponto (p1) associado à linha.
  */
point_t *line_get_p1(line_t *line);



/** @brief    Obtém o segundo ponto de uma linha.
  *
  * @param    line Pointer para uma linha.
  *
  * @return   O segundo ponto (p2) associado à linha.
  */
point_t *line_get_p2(line_t *line);



/** @brief    Retorna o X1 de uma linha.
  *
  * @param    line    Uma instância de linha.
  *
  * @return   O X1 de uma linha.
  */
double line_get_x1(line_t *line);



/** @brief    Retorna o Y1 de uma linha.
  *
  * @param    line    Uma instância de linha.
  *
  * @return   O Y1 de uma linha.
  */
double line_get_y1(line_t *line);



/** @brief    Retorna o X2 de uma linha.
  *
  * @param    line    Uma instância de linha.
  *
  * @return   O X2 de uma linha.
  */
double line_get_x2(line_t *line);



/** @brief    Retorna o Y2 de uma linha.
  *
  * @param    line    Uma instância de linha.
  *
  * @return   O Y2 de uma linha.
  */
double line_get_y2(line_t *line);



/** @brief    Retorna a cor de preenchimento de uma linha.
  *
  * @param    line    Uma instância de linha.
  *
  * @return   Uma string com a cor de preenchimento de uma linha.
  */
char *line_get_color(line_t *line);



/** @brief    Retorna a área de uma linha
  *
  * @param    line    Uma instância de linha.
  *
  * @return   A área de uma linha.
  */
double line_get_area(line_t *line);



/** @brief    Cria uma cópia de uma linha com um novo identificador.
  *
  * @param    line    Uma instância de linha a ser clonada.
  * @param    id      Novo identificador para a linha clonada.
  *
  * @return   Uma nova instância de linha, idêntica à original, mas com o novo ID.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
line_t *line_clone(line_t *line, size_t id);

#endif
