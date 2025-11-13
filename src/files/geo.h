#ifndef GEO_H
#define GEO_H

#include "../datast/queue.h"

/** @brief    Processa um arquivo .geo, criando as formas e as inserindo em uma fila.
 *
  * @param    path  Path para o arquivo .geo
  * @param    queue Pointer para o queue onde as formas serão inseridas.
  * @warning  Em caso de erro de leitura, a execução do programa será encerrada.
  *
  * @return   O id mais alto registrado no processamento do .geo, para ser utilizado em casos de clonagem no .qry.
  */
size_t geo_processing(char *path, queue_t *queue);

#endif
