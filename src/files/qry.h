#ifndef QRY_H
#define QRY_H

#include "../datast/queue.h"

/** @brief    Processa um arquivo .qry 
  *
  * @param    qrypath     Path para o arquivo .qry a ser processado.
  * @param    txtpath     Path para o arquivo .txt que registrará o processamento do .qry.
  * @param    ground      Uma fila que contém formas que serão utilizadas pelo arquivo .qry.
  * @param    highest_id  O id mais alto registrado no processamento do arquivo .geo, para ser utilizado na clonagem de formas.
  *
  * @warning  A fila "ground" será modificada ao longo da execução da função.
  */
void qry_processing(char *qrypath, char *txtpath, queue_t *ground, size_t highest_id);

#endif
