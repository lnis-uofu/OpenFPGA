#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*General Purpose Linked List*/
typedef struct s_llist t_llist;
struct s_llist
{
  void* dptr;
  t_llist* next;
};

/***** Subroutines *****/
t_llist* create_llist(int len);

t_llist* insert_llist_node(t_llist* cur);

t_llist* insert_llist_node_before_head(t_llist* old_head);

void remove_llist_node(t_llist* cur);

t_llist* cat_llists(t_llist* head1,
                    t_llist* head2);

t_llist* search_llist_tail(t_llist* head);

int find_length_llist(t_llist* head);

void free_llist(t_llist* head);

t_llist* reverse_llist(t_llist* head);

#endif
