#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shapes/shapes.h"
#include "../datast/queue.h"
#include "../launchers/launcher.h"

void txt_print_shape(FILE *txt, shape_t *shape);
launcher_t *get_ln_from_ll_by_id(llist_t *ll, size_t id);
loader_t *get_ld_from_ll_by_id(llist_t *ll, size_t id);

void qry_processing(char *qrypath, char *txtpath, queue_t *ground, size_t highest_id) {
  FILE *qry = fopen(qrypath, "r");
  if (qry == NULL) {
    printf("Erro na leitura do arquivo .qry.\n");
    exit(1);
  }

  FILE *txt = fopen(txtpath, "w");

  llist_t *launchers = llist_init();
  llist_t *loaders = llist_init();
  queue_t *arena = queue_init();
  queue_t *extra_shapes = queue_init();

  double total_score = 0.0;
  int instructions = 0;
  int shots = 0;
  int smashed_shapes = 0;
  int cloned_shapes = 0;

  char str[256];
  while(fgets(str, 256, qry)) {
    // Criação de disparador
    if (strncmp(str, "pd", 2) == 0) {
      size_t id;
      double x, y;

      sscanf(str, "%*s %zu %lf %lf", &id, &x, &y);
      fprintf(txt, "pd %zu %lf %lf\n", id, x, y);

      launcher_t *l = ln_init(id, x, y);
      llist_insertat_end(launchers, node_init(l, ln_destroy));

      instructions++;
    }

    // Criação de carregador
    if (strncmp(str, "lc", 2) == 0) {
      size_t id, amount;

      sscanf(str, "%*s %zu %zu", &id, &amount);
      fprintf(txt, "lc %zu %zu\n", id, amount);

      loader_t *loader = get_ld_from_ll_by_id(loaders, id);

      for (size_t i = 0; i < amount; i++) {
        node_t *node = queue_dequeue(ground);
        txt_print_shape(txt, (shape_t *) node_getvalue(node));
        ld_push(loader, node);
      }

      instructions++;
    }

    // Encaixar carregadores
    if (strncmp(str, "atch", 4) == 0) {
      size_t launcher_id, leftl_id, rightl_id;

      sscanf(str, "%*s %zu %zu %zu", &launcher_id, &leftl_id, &rightl_id);
      fprintf(txt, "atch %zu %zu %zu\n", launcher_id, leftl_id, rightl_id);

      launcher_t *launcher = get_ln_from_ll_by_id(launchers, launcher_id);
      ln_attach_loader(launcher, 0, get_ld_from_ll_by_id(loaders, leftl_id));
      ln_attach_loader(launcher, 1, get_ld_from_ll_by_id(loaders, rightl_id));

      instructions++;
    }

    // Shift
    if (strncmp(str, "shft", 4) == 0) {
      size_t launcher_id, amount;
      char side;

      sscanf(str, "%*s %zu %c %zu", &launcher_id, &side, &amount);
      fprintf(txt, "shft %zu %c %zu\n", launcher_id, side, amount);

      launcher_t *launcher = get_ln_from_ll_by_id(launchers, launcher_id);

      if (side == 'e') ln_shift_loaders(launcher, 0, amount);
      if (side == 'd') ln_shift_loaders(launcher, 1, amount);

      txt_print_shape(txt, (shape_t *) node_getvalue(ln_get_loaded(launcher)));
      
      instructions++;
    }

    // Disparo
    if (strncmp(str, "dsp", 3) == 0) {
      size_t launcher_id;
      double dx, dy;
      char vi;

      sscanf(str, "%*s %zu %lf %lf %c", &launcher_id, &dx, &dy, &vi);
      fprintf(txt, "dsp %zu %lf %lf %c\n", launcher_id, dx, dy, vi);

      launcher_t *launcher = get_ln_from_ll_by_id(launchers, launcher_id);
      double lx = ln_get_x(launcher);
      double ly = ln_get_y(launcher);

      node_t *loaded = ln_get_loaded(launcher);
      shape_t *shape = (shape_t *) node_getvalue(loaded);

      shape_move(shape, lx, ly, dx, dy);

      queue_enqueue(arena, loaded);
      ln_set_loaded(launcher, NULL);

      if (vi == 'v') {
        // todo: anotação de disparo.
        char *color_x = malloc(8);
        strcpy(color_x, "#FF0000");
        line_t *line_x = line_init(0, lx, ly + dy, lx + dx, ly + dy, color_x);

        char *color_y = malloc(8);
        strcpy(color_y, "#FF0000");
        line_t *line_y = line_init(0, lx + dx, ly, lx + dx, ly + dy, color_y);

        queue_enqueue(extra_shapes, shape_as_node(shape_init(LINE, line_x)));
        queue_enqueue(extra_shapes, shape_as_node(shape_init(LINE, line_y)));
      }
      
      shots++;
      instructions++;
    }

    // Rajada
    if (strncmp(str, "rjd", 3) == 0) {
      size_t launcher_id;
      char side;
      double dx, dy, ix, iy;

      sscanf(str, "%*s %zu %c %lf %lf %lf %lf", &launcher_id, &side, &dx, &dy, &ix, &iy);
      fprintf(txt, "rjd %zu %c %lf %lf %lf %lf\n", launcher_id, side, dx, dy, ix, iy);

      launcher_t *launcher = get_ln_from_ll_by_id(launchers, launcher_id);
      loader_t *loader;
      size_t len;

      if (side == 'e') {
        loader = ln_get_right_ld(launcher);
        len = ld_get_length(loader);

        if (ln_get_loaded(launcher) != NULL) {
          ld_push(ln_get_left_ld(launcher), ln_get_loaded(launcher));
          ln_set_loaded(launcher, NULL);
        }
      } else {
        loader = ln_get_left_ld(launcher);
        len = ld_get_length(loader);

        if (ln_get_loaded(launcher) != NULL) {
          ld_push(ln_get_right_ld(launcher), ln_get_loaded(launcher));
          ln_set_loaded(launcher, NULL);
        }
      }

      for (size_t i = 0; i < len; i++) {
        node_t *current = ld_pop(loader);
        shape_t *shape = (shape_t *) node_getvalue(current);

        shape_move(shape, ln_get_x(launcher), ln_get_y(launcher), dx + (i * ix), dy + (i * iy));

        queue_enqueue(arena, current);
        shots++;
      }
      instructions++;
    }

    // Calc
    if (strncmp(str, "calc", 4) == 0) {
      double round_score = 0;

      while(queue_get_length(arena) >= 2) {
        node_t *ni = queue_dequeue(arena);
        node_t *nj = queue_dequeue(arena);

        shape_t *i = (shape_t *) node_getvalue(ni);
        shape_t *j = (shape_t *) node_getvalue(nj);

        size_t i_id = shape_get_id(i);
        size_t j_id = shape_get_id(j);

        if (shape_overlap(i, j)) {
          // Caso: sobreposição entre I e J
          double ai = shape_get_area(i);
          double aj = shape_get_area(j);


          if (ai < aj) {
            // Caso: I < J

            // Criação do asterisco pro SVG
            char *anchor = malloc(6);
            char *color = malloc(8);
            char *border_color = malloc(8);
            char *content = malloc(2);
            char *ffam = malloc(6);
            char *fweight = malloc(7);
            char *fsize = malloc(4);

            if (!anchor || !color || !border_color || !content || !ffam || !fweight || !fsize) {
              printf("Erro na alocação de memória.\n");
              exit(1);
            }

            strcpy(anchor, "start");
            strcpy(color, "#ff0000");
            strcpy(border_color, "#ff0000");
            strcpy(content, "*");
            strcpy(ffam, "serif");
            strcpy(fweight, "normal");
            strcpy(fsize, "2em");

            text_t *asterisk = text_init(0, shape_get_x(i), shape_get_y(i), anchor, color, border_color, ffam, fweight, fsize, content);
            queue_enqueue(extra_shapes, shape_as_node(shape_init(TEXT, asterisk)));

            node_destroy(ni);
            queue_enqueue(ground, nj);

            round_score += ai;
            smashed_shapes++;
            fprintf(txt, "I (id %zu) v J (id %zu) - Sobreposição - I < J\n", i_id, j_id);
          } else if (ai >= aj) {
            // Caso: I >= J
            char *icolor = malloc(8);
            if (icolor == NULL) {
              printf("Erro na alocação de memória.\n");
              exit(1);
            }

            if (shape_get_type(i) == LINE) {
              strcpy(icolor, calc_complementary(shape_get_color(i)));
            } else strcpy(icolor, shape_get_color(i));

            shape_set_border_color(j, icolor);

            shape_t *iclone = shape_clone(i, ++highest_id);
            shape_swap_colors(iclone);

            queue_enqueue(ground, ni);
            queue_enqueue(ground, nj);
            queue_enqueue(ground, shape_as_node(iclone));

            fprintf(txt, "I (id %zu) v J (id %zu) - Sobreposição - I > J\n", i_id, j_id);

            cloned_shapes++;
          }
        } else {
          // Caso: sem sobreposição
          queue_enqueue(ground, ni);
          queue_enqueue(ground, nj);

          fprintf(txt, "I (id %zu) v J (id %zu) - Sem sobreposição\n", i_id, j_id);
        }
      }

      size_t arena_len = queue_get_length(arena);
      for (size_t i = 0; i < arena_len; i++) {
        queue_enqueue(ground, queue_dequeue(arena));
      }

      total_score += round_score;
      fprintf(txt, "Pontuação do round: %lf.\n", round_score);
      fprintf(txt, "Pontuação total: %lf.\n", total_score);

      instructions++;
    }
  }

  fprintf(txt, "- Pontuação final: %lf.\n", total_score);
  fprintf(txt, "- Número total de instruções executadas: %d.\n", instructions);
  fprintf(txt, "- Número total de disparos: %d.\n", shots);
  fprintf(txt, "- Número total de formas esmagadas: %d.\n", smashed_shapes);
  fprintf(txt, "- Número total de formas clonadas: %d.\n", cloned_shapes);

  llist_destroy(launchers);
  llist_destroy(loaders);

  size_t extra_shapes_len = queue_get_length(extra_shapes);
  for (size_t i = 0; i < extra_shapes_len; i++) {
    queue_enqueue(ground, queue_dequeue(extra_shapes));
  }

  queue_destroy(arena);
  queue_destroy(extra_shapes);

  fclose(qry);
  fclose(txt);
}

void txt_print_shape(FILE *txt, shape_t *shape) {
  switch (shape_get_type(shape)) {
    case CIRCLE: {
      circle_t *circle = shape_as_circle(shape);
      fprintf(txt, "- Círculo - id: %zu\n", circle_get_id(circle));
      fprintf(txt, "\tx: %lf\n", circle_get_x(circle));
      fprintf(txt, "\ty: %lf\n", circle_get_y(circle));
      fprintf(txt, "\traio: %lf\n", circle_get_radius(circle));
      fprintf(txt, "\tcor: %s\n", circle_get_color(circle));
      fprintf(txt, "\tcor de borda: %s\n", circle_get_border_color(circle));
      break;
    }

    case RECTANGLE: {
      rectangle_t *rect = shape_as_rectangle(shape);
      fprintf(txt, "- Retângulo - id: %zu\n", rect_get_id(rect));
      fprintf(txt, "\tx: %lf\n", rect_get_x(rect));
      fprintf(txt, "\ty: %lf\n", rect_get_y(rect));
      fprintf(txt, "\tlargura: %lf\n", rect_get_width(rect));
      fprintf(txt, "\taltura: %lf\n", rect_get_height(rect));
      fprintf(txt, "\tcor: %s\n", rect_get_color(rect));
      fprintf(txt, "\tcor de borda: %s\n", rect_get_border_color(rect));
      break;
    }

    case LINE: {
      line_t *line = shape_as_line(shape);
      fprintf(txt, "- Linha - id: %zu\n", line_get_id(line));
      fprintf(txt, "\tx1: %lf\n", line_get_x1(line));
      fprintf(txt, "\ty1: %lf\n", line_get_y1(line));
      fprintf(txt, "\tx2: %lf\n", line_get_x2(line));
      fprintf(txt, "\ty2: %lf\n", line_get_y2(line));
      fprintf(txt, "\tcor: %s\n", line_get_color(line));
      break;
    }

    case TEXT: {
      text_t *text = shape_as_text(shape);
      fprintf(txt, "- Texto - id: %zu\n", text_get_id(text));
      fprintf(txt, "\tx: %lf\n", text_get_x(text));
      fprintf(txt, "\ty: %lf\n", text_get_y(text));
      fprintf(txt, "\tâncora: %s\n", text_get_anchor(text));
      fprintf(txt, "\tcor: %s\n", text_get_color(text));
      fprintf(txt, "\tcor de borda: %s\n", text_get_border_color(text));
      fprintf(txt, "\tfamília: %s\n", text_get_ffam(text));
      fprintf(txt, "\tpeso: %s\n", text_get_fweight(text));
      fprintf(txt, "\ttamanho: %s\n", text_get_fsize(text));
      fprintf(txt, "\tconteúdo: %s\n", text_get_content(text));
      break;
    }
  }
}

launcher_t *get_ln_from_ll_by_id(llist_t *ll, size_t id) {
  size_t len = llist_get_length(ll);

  for (size_t i = 0; i < len; i++) {
    node_t *node = llist_getat_index(ll, i);
    launcher_t *launcher = (launcher_t *) node_getvalue(node);
    if (ln_get_id(launcher) == id) return launcher;
  }

  return NULL;
}

loader_t *get_ld_from_ll_by_id(llist_t *ll, size_t id) {
  size_t len = llist_get_length(ll);

  for (size_t i = 0; i < len; i++) {
    node_t *node = llist_getat_index(ll, i);
    loader_t *loader = (loader_t *) node_getvalue(node);
    if (ld_get_id(loader) == id) return loader;
  }

  loader_t *newld = ld_init(id);
  llist_insertat_end(ll, node_init(newld, ld_destroy));
  return newld;
}
