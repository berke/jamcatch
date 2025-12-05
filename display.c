/* display.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <assert.h>
#include <format.h>
#include <display.h>
#include <debug.h>

void display_init(display_t *d, const menu_t *menu, char *flags)/*{{{*/
{
  void init_rec(const selection_t *s) {
    assert(s);
    switch(s->s_type) {
      case SEL_MENU:
        {
          int i;

          for(i = 0; i < s->s_content.s_menu.sm_count; i ++) {
            init_rec(&s->s_content.s_menu.sm_entries[i]->m_selection);
          }
        }
        break;

      case SEL_INT:
        *s->s_content.s_int.si_current = s->s_content.s_int.si_default;
        break;

      case SEL_CHOICE:
        *s->s_content.s_choice.sc_current = s->s_flags & SEL_DEFAULT1 ? 1 : 0;
        break;

      default:
        break;
    }
  }

  d->d_menu = menu;
  d->d_choice = 0;
  d->d_depth = 0;
  d->d_flags = flags;
  d->d_status = 0;

  init_rec(&menu->m_selection);
}/*}}}*/
void display_notify(display_t *d, const menu_t *menu)/*{{{*/
{
  void call(const selection_t *s, char *(*f)(void *), void *info) {
    if(s->s_flags & SEL_INIT && f) {
      char *msg = f(info);
      if(msg) {
        DEBUGF("Notify: %s", msg);
      }
    }
  }

  void init_rec(const selection_t *s) {
    assert(s);
    switch(s->s_type) {
      case SEL_MENU:
        {
          int i;

          call(s, s->s_content.s_menu.sm_notify_enter, s->s_content.s_menu.sm_info);
          for(i = 0; i < s->s_content.s_menu.sm_count; i ++) {
            init_rec(&s->s_content.s_menu.sm_entries[i]->m_selection);
          }
          call(s, s->s_content.s_menu.sm_notify_exit, s->s_content.s_menu.sm_info);
        }
        break;

      case SEL_INT:
        if(*s->s_content.s_int.si_current < s->s_content.s_int.si_low ||
           *s->s_content.s_int.si_current > s->s_content.s_int.si_high) {
          *s->s_content.s_int.si_current = s->s_content.s_int.si_low;
        }
        call(s, s->s_content.s_int.si_notify, s->s_content.s_int.si_info);
        break;

      case SEL_CHOICE:
        if(*s->s_content.s_choice.sc_current < 0 ||
           *s->s_content.s_choice.sc_current >= s->s_content.s_choice.sc_count) {
          *s->s_content.s_choice.sc_current = 0;
        }
        call(s, s->s_content.s_choice.sc_notify, s->s_content.s_choice.sc_info);
        break;

      default:
        break;
    }
  }

  init_rec(&menu->m_selection);
}/*}}}*/
void display_set_menu(display_t *d, const menu_t *menu)/*{{{*/
{
  d->d_menu = menu;
  d->d_choice = 0;
  d->d_depth = 0;
  d->d_status = 0;
}/*}}}*/
void display_update(display_t *d, char *line0, char *line1, const int n)/*{{{*/
{
  int i;
  char *label;
  const selection_t *s;
  const menu_t *choice;

  void blank_to(char *u, int i) {
    while(i < n) { u[i] = ' '; i ++; }
  }

  int put_label(char *line, const char *label, int i) {
    while(i < n) {
      if(!*label) break;
      line[i] = *label;
      i ++;
      label ++;
    }

    return i;
  }

  label = d->d_menu->m_label;
  
  /* Fill with blanks */
  blank_to(line0, 0);

  /* Put depth */
  i = 0;
  while(i < n && i < d->d_depth) {
    line0[n - 1 - i] = d->d_flags[DA_UP];
    i ++;
  }

  /* Put label */
  put_label(line0, label, 0);

  /* Second line */
  if(d->d_status) {
    blank_to(line1, 0);
    put_label(line1, d->d_status, 0);
  } else {
    s = &d->d_menu->m_selection;
    switch(s->s_type) {
      case SEL_MENU:
        blank_to(line1, 0);

        if(s->s_content.s_menu.sm_count) {
          choice = s->s_content.s_menu.sm_entries[d->d_choice];
          label = choice->m_label;
          put_label(line1, label, 1);

          line1[0]     = d->d_flags[d->d_choice > 0 ? DA_LEFT : DA_LEFT_HEAD];
          line1[n - 1] = d->d_flags[d->d_choice + 1 < s->s_content.s_menu.sm_count ? DA_RIGHT : DA_RIGHT_HEAD];
        } else {
          put_label(line1, "(Empty)", 1);
        }
        break;

      case SEL_DISPLAY:
        blank_to(line1, 0);
        put_label(line1, s->s_content.s_display.sd_label, 0);
        break;

      case SEL_ACTION:
        blank_to(line1, 0);
        put_label(line1, s->s_content.s_action.sa_prompt, 0);
        break;

      case SEL_INT:
        blank_to(line1, 0);
        line1[0] = '-';
        if(s->s_content.s_int.si_formatter) {
          i = s->s_content.s_int.si_formatter(line1 + 1, n - 2, d->d_proposed);
        } else {
          i = snprintf(line1 + 1, n - 2, "%d", d->d_proposed);
        }
        if(d->d_proposed != *s->s_content.s_int.si_current) {
          if(i < n - 1) line1[i + 1] = d->d_flags[DA_DOWN];
        }
        line1[n-1] = '+';
        break;

      case SEL_CHOICE:
        DEBUGF("Choice, proposed=%d", d->d_proposed);
        blank_to(line1, 0);
        line1[0] = '-';
        assert(0 <= d->d_proposed && d->d_proposed < s->s_content.s_choice.sc_count);
        i = put_label(line1, s->s_content.s_choice.sc_labels[d->d_proposed], 1);
        if(d->d_proposed != *s->s_content.s_choice.sc_current) {
          if(i < n) line1[i] = d->d_flags[DA_DOWN];
        }
        line1[n-1] = '+';
        break;

      default:
        blank_to(line1, 0);
        break;
    }
  }
}/*}}}*/
bool display_apply(display_t *d, display_key_t key)/*{{{*/
{
  const selection_t *s;
  const menu_t *parent;

  void call(char *(*f)(void *), void *info) {
    DEBUGF("Call");
    if(f) d->d_status = f(info);
    else d->d_status = 0;
  }

  void enter() {
    DEBUGF("Enter");
    switch(s->s_type) {
      case SEL_INT:
        d->d_proposed = *s->s_content.s_int.si_current;
        break;

      case SEL_CHOICE:
        DEBUGF("Choice");
        DEBUGF("Current @%p", s->s_content.s_choice.sc_current);
        d->d_proposed = *s->s_content.s_choice.sc_current;
        DEBUGF("Proposed=%d", d->d_proposed);
        break;

      case SEL_MENU:
        call(s->s_content.s_menu.sm_notify_enter, s->s_content.s_menu.sm_info);

      default:
        break;
    }
  }

  s = &d->d_menu->m_selection;
  switch(key) {
    case DK_RIGHT:
      switch(s->s_type) {
        case SEL_MENU:
          if(d->d_choice + 1 < s->s_content.s_menu.sm_count) {
            d->d_choice ++;
          }
          break;

        case SEL_INT:
          if(d->d_proposed < s->s_content.s_int.si_high) {
            d->d_proposed ++;
          }
          break;

        case SEL_CHOICE:
          if(d->d_proposed + 1 < s->s_content.s_choice.sc_count) {
            d->d_proposed ++;
          }
          break;

        default:
          break;
      }
      break;

    case DK_LEFT:
      switch(s->s_type) {
        case SEL_MENU:
          if(d->d_choice) d->d_choice --;
          break;

        case SEL_INT:
          if(d->d_proposed > s->s_content.s_int.si_low) {
            d->d_proposed --;
          }
          break;

        case SEL_CHOICE:
          if(d->d_proposed > 0) {
            d->d_proposed --;
          }
          break;

        default:
          break;
      }
      break;

    case DK_DOWN:
      switch(s->s_type) {
        case SEL_MENU:
          {
            const menu_t *choice;

            DEBUGF("Down menu");
            if(s->s_content.s_menu.sm_count) {
              choice = s->s_content.s_menu.sm_entries[d->d_choice];
              d->d_menu = choice;
              d->d_choice = 0;
              d->d_depth ++;

              s = &d->d_menu->m_selection;
              enter();
            }
          }
          break;

        case SEL_INT:
          *s->s_content.s_int.si_current = d->d_proposed;
          call(s->s_content.s_int.si_notify, s->s_content.s_int.si_info);
          break;

        case SEL_CHOICE:
          *s->s_content.s_choice.sc_current = d->d_proposed;
          call(s->s_content.s_choice.sc_notify, s->s_content.s_choice.sc_info);
          break;

        case SEL_ACTION:
          call(s->s_content.s_action.sa_notify, s->s_content.s_action.sa_info);
          break;

        default:
          break;
      }
      break;

    case DK_UP:
      d->d_status = 0;

      switch(s->s_type) {
        case SEL_MENU:
          call(s->s_content.s_menu.sm_notify_exit, s->s_content.s_menu.sm_info);
          break;
        default:
          break;
      }

      parent = d->d_menu->m_up;
      if(parent) {
        int choice;
        int m;

        choice = 0;
        m = parent->m_selection.s_content.s_menu.sm_count;

        while(choice < m) {
          if(d->d_menu == parent->m_selection.s_content.s_menu.sm_entries[choice]) break;
          choice ++;
        }
        if(choice == m) choice = 0;
        d->d_menu = parent;
        d->d_choice = choice;
        d->d_depth --;
      } else {
        return false;
      }
      break;

    default:
      break;
  }

  return true;
}/*}}}*/
