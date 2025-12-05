/* display.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <menu.h>

enum {/*{{{*/
  DA_DOWN,
  DA_UP,
  DA_LEFT,
  DA_RIGHT,
  DA_LEFT_HEAD,
  DA_RIGHT_HEAD,
  DA_STAR,
  DA_COUNT
};/*}}}*/

typedef enum {
  DK_LEFT,
  DK_RIGHT,
  DK_UP,
  DK_DOWN,
} display_key_t;

typedef struct {/*{{{*/
  const menu_t *d_menu;
  int d_choice;
  int d_depth;
  const char *d_flags;
  char *d_status; /* Override second line with this */
  int d_proposed;
} display_t;/*}}}*/

void display_init(display_t *d, const menu_t *menu, char *flags);
void display_notify(display_t *d, const menu_t *menu);
void display_set_menu(display_t *d, const menu_t *menu);
void display_update(display_t *d, char *line0, char *line1, int n);
bool display_apply(display_t *d, display_key_t key);

#endif
