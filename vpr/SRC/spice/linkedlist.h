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

void remove_llist_node(t_llist* cur);

t_llist* cat_llists(t_llist* head1,
                    t_llist* head2);

t_llist* search_llist_tail(t_llist* head);

void free_llist(t_llist* head);
