/* list.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef LIST_H
#define LIST_H

typedef struct generic_list_s {
  struct generic_node_s *l_head;
  struct generic_node_s *l_tail;
} generic_list;

typedef struct generic_node_s {
  struct generic_node_s *n_next;
  struct generic_node_s *n_prev;
} generic_node;

void generic_list_init(generic_list *l);
int generic_list_is_empty(generic_list *l);
void generic_list_append(generic_list *l, generic_node *n);
void generic_list_prepend(generic_list *l, generic_node *n);
void generic_list_remove(generic_list *l, generic_node *n);
void generic_list_rotate(generic_list *l);

#define List_Make(type)                                                                         \
  typedef struct type##_list_s {                                                                \
    struct type##_node_s *l_head; struct type##_node_s *l_tail;                                 \
  } type##_list;                                                                                \
  typedef struct type##_node_s {                                                                \
    struct type##_node_s *n_next;                                                               \
    struct type##_node_s *n_prev;                                                               \
    type type;                                                                                  \
  } type##_node;                                                                                \
static inline void type##_list_init(type##_list *l) { l->l_head = 0; l->l_tail = 0; }           \
static inline void type##_node_init(type##_node *n) { n->n_prev = 0; n->n_next = 0; }           \
static inline int type##_list_is_empty(type##_list *l) { return !l->l_head; }                   \
static inline void type##_list_append(type##_list *l, type##_node *n) {                         \
  generic_list_append((generic_list *) l, (generic_node *) n);                                  \
}                                                                                               \
static inline void type##_list_prepend(type##_list *l, type##_node *n) {                        \
  generic_list_prepend((generic_list *) l, (generic_node *) n);                                 \
}                                                                                               \
static inline void type##_list_remove(type##_list *l, type##_node *n) {                         \
  generic_list_remove((generic_list *) l, (generic_node *) n);                                  \
}                                                                                               \
static inline type##_node *type##_list_head(type##_list *l) { return l->l_head; }               \
static inline type##_node *type##_list_tail(type##_list *l) { return l->l_tail; }               \
static inline type##_node *type##_node_next(type##_node *n) { return n->n_next; }               \
static inline type##_node *type##_node_prev(type##_node *n) { return n->n_prev; }               \
static inline type* type##_node_contents(type##_node *n) { return &n->type; }                   \
static inline void type##_list_rotate(type##_list *l)                                           \
  { generic_list_rotate((generic_list *) l); }                                                  \
;

#endif
