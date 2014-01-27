#ifndef __LIST_H_
#define __LIST_H_

typedef struct llist_t {
	struct llist_t *link;
	char *data;
} llist_t;
void llist_add_to(llist_t **old_head, void *data);
void llist_add_to_end(llist_t **list_head, void *data);
void *llist_pop(llist_t **elm) ;
void llist_unlink(llist_t **head, llist_t *elm) ;
void llist_free(llist_t *elm, void (*freeit)(void *data));
llist_t *llist_rev(llist_t *list) ;
llist_t *llist_find_str(llist_t *first, const char *str);

#endif