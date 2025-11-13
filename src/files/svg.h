#ifndef SVG_H
#define SVG_H

#include "../shapes/circle.h"
#include "../shapes/rectangle.h"
#include "../shapes/line.h"
#include "../shapes/text.h"
#include "../datast/queue.h"

typedef struct svg_t svg_t;

/** @brief    Inicializa um arquivo SVG para escrita.
  *
  * @param    path   Caminho do arquivo SVG a ser criado.
  *
  * @return   Uma instância de svg pronta para escrita.
  */
svg_t *svg_init(char *path);

/** @brief    Fecha o arquivo SVG e libera os recursos associados.
  *
  * @param    svg    Uma instância de svg.
  */
void svg_close(svg_t *svg);

/** @brief    Escreve um círculo no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    circle    A forma de círculo a ser escrita.
  */
void svg_write_circle(svg_t *svg, circle_t *circle);

/** @brief    Escreve um retângulo no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    rect      A forma de retângulo a ser escrita.
  */
void svg_write_rectangle(svg_t *svg, rectangle_t *rect);

/** @brief    Escreve uma linha no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    line      A forma de linha a ser escrita.
  */
void svg_write_line(svg_t *svg, line_t *line);

/** @brief    Escreve um texto no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    text      A forma de texto a ser escrita.
  */
void svg_write_text(svg_t *svg, text_t *text);

/** @brief    Escreve todas as formas contidas em uma fila no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    queue     Fila contendo as formas a serem escritas.
  */
void svg_write_queue(svg_t *svg, queue_t *queue);

#endif
