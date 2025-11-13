#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datast/linkedlist.h"
#include "datast/queue.h"
#include "files/geo.h"
#include "files/svg.h"
#include "files/qry.h"

int main(int argc, char **argv) {
  char *base_dir = NULL;
  char *geopath = NULL;
  char *out_dir = NULL;
  char *qrypath = NULL;

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-e") == 0) {
      base_dir = argv[i + 1];
    }

    if (strcmp(argv[i], "-f") == 0) {
      geopath = argv[i + 1];
    }

    if (strcmp(argv[i], "-o") == 0) {
      out_dir = argv[i + 1];
    }

    if (strcmp(argv[i], "-q") == 0) {
      qrypath = argv[i + 1];
    }
  }

  if (base_dir != NULL) {
    if (base_dir[strlen(base_dir) - 1] == '/') base_dir[strlen(base_dir) - 1] = '\0';
  }

  if (out_dir != NULL) {
    if (out_dir[strlen(out_dir) - 1] == '/') out_dir[strlen(out_dir) - 1] = '\0';
  }

  char *geo_name = malloc(strlen(geopath) - 3);
  strncpy(geo_name, geopath, strlen(geopath) - 4);
  geo_name[strlen(geopath) - 4] = '\0';

  char *geo_slashes = strchr(geo_name, '/');
  while(geo_slashes != NULL) {
    *geo_slashes = '_';
    geo_slashes = strchr(geo_name, '/');
  }

  char *full_geopath = malloc(128);
  if (base_dir != NULL) {
    sprintf(full_geopath, "%s/%s", base_dir, geopath);
  } else strcpy(full_geopath, geopath);

  queue_t *ground = queue_init();
  size_t highest_id = geo_processing(full_geopath, ground);
  
  char *geosvg_path = malloc(128);
  sprintf(geosvg_path, "%s/%s.svg", out_dir, geo_name);

  svg_t *geosvg = svg_init(geosvg_path);
  svg_write_queue(geosvg, ground);
  svg_close(geosvg);

  if (qrypath == NULL) {
    free(geo_name);
    free(full_geopath);
    free(geosvg_path);

    queue_destroy(ground);
    exit(0);
  }

  char *full_qrypath = malloc(128);
  if (base_dir != NULL) {
    sprintf(full_qrypath, "%s/%s", base_dir, qrypath);
  } else strcpy(full_qrypath, qrypath);

  char *qry_name = malloc(strlen(qrypath) - 3);
  strncpy(qry_name, qrypath, strlen(qrypath) - 4);
  qry_name[strlen(qrypath) - 4] = '\0';

  char *qry_slashes = strchr(qry_name, '/');
  while(qry_slashes != NULL) {
    *qry_slashes = '_';
    qry_slashes = strchr(qry_name, '/');
  }

  char *outpath_svg = malloc(128);
  char *outpath_txt = malloc(128);
  sprintf(outpath_svg, "%s/%s-%s.svg", out_dir, geo_name, qry_name);
  sprintf(outpath_txt, "%s/%s-%s.txt", out_dir, geo_name, qry_name);

  qry_processing(full_qrypath, outpath_txt, ground, highest_id);

  svg_t *qrysvg = svg_init(outpath_svg);
  svg_write_queue(qrysvg, ground);
  svg_close(qrysvg);

  free(geo_name);
  free(full_geopath);
  free(geosvg_path);
  free(full_qrypath);
  free(qry_name);
  free(outpath_svg);
  free(outpath_txt);

  queue_destroy(ground);
}
