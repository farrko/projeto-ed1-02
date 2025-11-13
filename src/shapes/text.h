#ifndef TEXT_H
#define TEXT_H

#include <stddef.h>
#include "line.h"

typedef struct text_t text_t;

/** @brief    Inicialização de um texto.
  *
  * @param    id              Identificador do objeto.
  * @param    x               Coordenada X do texto.
  * @param    y               Coordenada Y do texto.
  * @param    anchor          Ponto de ancoragem do texto.
  * @param    color           String com o código HEX da cor do texto.
  * @param    border_color    String com o código HEX da cor da borda.
  * @param    ffam            String com a família da fonte.
  * @param    fweight         String com o peso da fonte.
  * @param    fsize           String com o tamanho da fonte.
  * @param    content         String com o conteúdo do texto.
  *
  * @return   Uma instância de texto.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
text_t *text_init(size_t id, double x, double y, char *anchor, char *color, char *border_color, char *ffam, char *fweight, char *fsize, char *content);

/** @brief    Destrói um texto.
  *
  * @param    text    Uma instância de texto.
  */
void text_destroy(void *text);

/** @brief    Define o X de um texto.
  *
  * @param    text    Uma instância de texto.
  * @param    x       Coordenada X do texto.
  */
void text_set_x(text_t *text, double x);

/** @brief    Define o Y de um texto.
  *
  * @param    text    Uma instância de texto.
  * @param    y       Coordenada Y do texto.
  */
void text_set_y(text_t *text, double y);

/** @brief    Define a ancoragem de um texto.
  *
  * @param    text    Uma instância de texto.
  * @param    anchor  Ponto de ancoragem do texto.
  */
void text_set_anchor(text_t *text, char *anchor);

/** @brief    Define a cor de um texto.
  *
  * @param    text    Uma instância de texto.
  * @param    color   String com o código HEX da cor do texto.
  */
void text_set_color(text_t *text, char *color);

/** @brief    Define a cor da borda de um texto.
  *
  * @param    text          Uma instância de texto.
  * @param    border_color  String com o código HEX da cor da borda.
  */
void text_set_border_color(text_t *text, char *border_color);

/** @brief    Define a família da fonte de um texto.
  *
  * @param    text    Uma instância de texto.
  * @param    ffam    String com a família da fonte.
  */
void text_set_ffam(text_t *text, char *ffam);

/** @brief    Define o peso da fonte de um texto.
  *
  * @param    text      Uma instância de texto.
  * @param    fweight   String com o peso da fonte.
  */
void text_set_fweight(text_t *text, char *fweight);

/** @brief    Define o tamanho da fonte de um texto.
  *
  * @param    text    Uma instância de texto.
  * @param    fsize   String com o tamanho da fonte.
  */
void text_set_fsize(text_t *text, char *fsize);

/** @brief    Define o conteúdo de um texto.
  *
  * @param    text      Uma instância de texto.
  * @param    content   String com o conteúdo do texto.
  */
void text_set_content(text_t *text, char *content);

/** @brief    Retorna o ID de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   O ID de um texto.
  */
size_t text_get_id(text_t *text);

/** @brief    Retorna o X de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   O X do texto.
  */
double text_get_x(text_t *text);

/** @brief    Retorna o Y de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   O Y do texto.
  */
double text_get_y(text_t *text);

/** @brief    Retorna a ancoragem de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com o ponto de ancoragem do texto.
  */
char *text_get_anchor(text_t *text);

/** @brief    Retorna a cor de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com a cor do texto.
  */
char *text_get_color(text_t *text);

/** @brief    Retorna a cor da borda de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com a cor da borda do texto.
  */
char *text_get_border_color(text_t *text);

/** @brief    Retorna a família da fonte de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com a família da fonte.
  */
char *text_get_ffam(text_t *text);

/** @brief    Retorna o peso da fonte de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com o peso da fonte.
  */
char *text_get_fweight(text_t *text);

/** @brief    Retorna o tamanho da fonte de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com o tamanho da fonte.
  */
char *text_get_fsize(text_t *text);

/** @brief    Retorna o conteúdo de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   Uma string com o conteúdo do texto.
  */
char *text_get_content(text_t *text);

/** @brief    Retorna a área de um texto.
  *
  * @param    text    Uma instância de texto.
  *
  * @return   A área do texto.
  */
double text_get_area(text_t *text);

/** @brief    Cria uma cópia de um texto com um novo identificador.
  *
  * @param    text   Uma instância de texto a ser clonada.
  * @param    id     Novo identificador para o texto clonado.
  *
  * @return   Uma nova instância de texto, idêntica à original, mas com o novo ID.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
text_t *text_clone(text_t *text, size_t id);

/** @brief    Cria uma instância de linha para ser utilizada no cálculo de colisão do texto.
  *
  * @param    text  Uma instância de texto.
  *
  * @return   Uma instância de linha a ser utilizada para cálculo de colisão.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
line_t *text_line_collision(text_t *text);

/** @brief    Troca entre si as cores de preenchimento e de borda de um texto.
  *
  * @param    rect   Uma instância de texto.
  */

void text_swap_colors(text_t *text);

#endif
