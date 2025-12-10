#ifndef POINT_H
#define POINT_H

typedef struct point_t point_t;
typedef struct polar_coords_t polar_coords_t;



/** @brief    Inicializa um ponto em coordenadas cartesianas.
  *
  * @param    x  Coordenada X.
  * @param    y  Coordenada Y.
  *
  * @return   Uma instância de point_t contendo as coordenadas especificadas.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
point_t *point_init(double x, double y);



/** @brief    Inicializa um ponto em coordenadas polares.
  *
  * @param    angle     Ângulo polar em radianos.
  * @param    distance  Distância ao ponto de origem.
  * @param    origin    Ponto de origem das coordenadas polares.
  *
  * @return   Uma instância de polar_coords_t com os valores especificados.
  * @warning  O ponto de origem será clonado para a estrutura.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
polar_coords_t *polar_init(double angle, double distance, point_t *origin);



/** @brief    Destrói uma instância de point_t.
  *
  * @param    point Pointer para o ponto a ser destruído.
  */
void point_destroy(void *point);



/** @brief    Destrói uma instância de polar_coords_t.
  *
  * @param    polar Pointer para as coordenadas polares a serem destruídas.
  */
void polar_destroy(void *polar);



/** @brief    Obtém o valor da coordenada X de um ponto.
  *
  * @param    point Pointer para um ponto.
  *
  * @return   O valor da coordenada X.
  */
double point_get_x(point_t *point);



/** @brief    Obtém o valor da coordenada Y de um ponto.
  *
  * @param    point Pointer para um ponto.
  *
  * @return   O valor da coordenada Y.
  */
double point_get_y(point_t *point);



/** @brief    Define a coordenada X de um ponto.
  *
  * @param    point Pointer para um ponto.
  * @param    x     Novo valor de X.
  */
void point_set_x(point_t *point, double x);



/** @brief    Define a coordenada Y de um ponto.
  *
  * @param    point Pointer para um ponto.
  * @param    y     Novo valor de Y.
  */
void point_set_y(point_t *point, double y);



/** @brief    Obtém o ângulo de uma instância de coordenadas polares.
  *
  * @param    polar Pointer para coordenadas polares.
  *
  * @return   O ângulo em radianos.
  */
double polar_get_angle(polar_coords_t *polar);



/** @brief    Obtém a distância de uma instância de coordenadas polares.
  *
  * @param    polar Pointer para coordenadas polares.
  *
  * @return   A distância ao ponto de origem.
  */
double polar_get_distance(polar_coords_t *polar);



/** @brief    Obtém o ponto de origem associado às coordenadas polares.
  *
  * @param    polar Pointer para coordenadas polares.
  *
  * @return   O ponto de origem.
  */
point_t *polar_get_origin(polar_coords_t *polar);



/** @brief    Define o ângulo de uma estrutura de coordenadas polares.
  *
  * @param    polar Pointer para coordenadas polares.
  * @param    angle Novo ângulo em radianos.
  */
void polar_set_angle(polar_coords_t *polar, double angle);



/** @brief    Define a distância de uma estrutura de coordenadas polares.
  *
  * @param    polar    Pointer para coordenadas polares.
  * @param    distance Nova distância ao ponto de origem.
  */
void polar_set_distance(polar_coords_t *polar, double distance);



/** @brief    Define o ponto de origem de uma estrutura de coordenadas polares.
  *
  * @param    polar  Pointer para coordenadas polares.
  * @param    origin Novo ponto de origem.
  *
  * @warning  O ponto de origem é clonado.
  * @warning  Alterar a origem recalcula os valores de angle e distance, com o objetivo de manter
  *           o mesmo (x, y) absoluto.
  */
void polar_set_origin(polar_coords_t *polar, point_t *origin);



/** @brief    Obtém a coordenada X absoluta representada pelas coordenadas polares.
  *
  * @param    polar Pointer para coordenadas polares.
  *
  * @return   A coordenada X resultante.
  */
double polar_get_absolute_x(polar_coords_t *polar);



/** @brief    Obtém a coordenada Y absoluta representada pelas coordenadas polares.
  *
  * @param    polar Pointer para coordenadas polares.
  *
  * @return   A coordenada Y resultante.
  */
double polar_get_absolute_y(polar_coords_t *polar);



/** @brief    Constrói coordenadas polares a partir de dois pontos cartesianos.
  *
  * @param    origin Ponto inicial utilizado como referência.
  * @param    final  Ponto final utilizado para calcular ângulo e distância.
  *
  * @return   Uma instância de polar_coords_t representando a conversão.
  */
polar_coords_t *polar_from_cartesian(point_t *origin, point_t *final);



/** @brief    Converte coordenadas polares em coordenadas cartesianas.
  *
  * @param    polar Pointer para coordenadas polares.
  *
  * @return   Um novo point_t contendo a posição cartesiana correspondente.
  */
point_t *cartesian_from_polar(polar_coords_t *polar);

#endif
