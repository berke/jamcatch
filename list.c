/* list.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <assert.h>
#include <list.h>

void generic_list_init(generic_list *l)
{
  l->l_head = 0;
  l->l_tail = 0;
}

int generic_list_is_empty(generic_list *l)
{
  return !l->l_head;
}

void generic_list_append(generic_list *l, generic_node *n)
{
  if(l->l_head) {
    generic_node *nt;

    nt = l->l_tail;

    assert(nt);
    assert(!nt->n_next);

    l->l_tail = n;
    n->n_prev = nt;
    n->n_next = 0;
    nt->n_next = n;
  } else {
    assert(!l->l_tail);
    l->l_head = n;
    l->l_tail = n;
    n->n_prev = 0;
    n->n_next = 0;
  }
}

void generic_list_prepend(generic_list *l, generic_node *n)
{
  if(l->l_head) {
    generic_node *nh;

    nh = l->l_head;

    assert(nh);
    assert(!nh->n_prev);

    l->l_head = n;
    n->n_prev = 0;
    n->n_next = nh;
    nh->n_prev = n;
  } else {
    assert(!l->l_tail);
    l->l_head = n;
    l->l_tail = n;
    n->n_prev = 0;
    n->n_next = 0;
  }
}

void generic_list_remove(generic_list *l, generic_node *n)
{
  generic_node *np, *nn;

  np = n->n_prev;
  nn = n->n_next;

  if(nn) {
    nn->n_prev = np;
  } else {
    /* Was at the end of the list */
    l->l_tail = np;
  }

  if(np) {
    np->n_next = nn;
  } else {
    /* Was at the head of the list */
    l->l_head = nn;
  }
  n->n_prev = 0;
  n->n_next = 0;
}

void generic_list_rotate(generic_list *l)
{
  generic_node *n;

  n = l->l_head;
  if(n) {
    generic_list_remove(l, n);
    generic_list_append(l, n);
  }
}
