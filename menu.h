/* menu.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef MENU_H
#define MENU_H

struct selection_s;
struct menu_s;

typedef enum {
  SEL_MENU,
  SEL_ACTION,
  SEL_DISPLAY,
  SEL_INT,
  SEL_CHOICE,
  SEL_NONE
} selection_type_t;

#define SEL_INIT 1
#define SEL_DEFAULT1 2

typedef struct selection_s {
  u8 s_type;
  u8 s_flags;
  union {
    struct {
      char *sa_prompt;
      char * (* const sa_notify)(void *);
      void * const sa_info;
    } s_action;

    struct {
      int sm_count;
      const struct menu_s * const *sm_entries;
      char * (* const sm_notify_enter)(void *);
      char * (* const sm_notify_exit)(void *);
      void * const sm_info;
    } s_menu;

    struct {
      const char *sd_label;
      char * (* const sd_notify)(void  *);
      void * const sd_info;
    } s_display;

    struct {
      int si_low;
      int si_default;
      int si_high;
      int *si_current;
      char * (* const si_notify)(void *);
      void * const si_info;
      int (*si_formatter)(char *, int, int);
    } s_int;

    struct {
      int sc_count;
      int *sc_current;
      const char * const * sc_labels;
      char * (* const sc_notify)(void *);
      void * const sc_info;
    } s_choice;
  } s_content;
} selection_t;


typedef struct menu_s {
  const struct menu_s *m_up;
  char *m_label;
  selection_t m_selection;
} menu_t;

#endif
