/**
 * Filename : linkedlist.c
 * Author : Xifan TANG, EPFL
 * Description : Define most useful functions for a general purpose linked list
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

/* Data structures*/
#include "linkedlist.h"
/**
 * Create, insert, delete, cat 
 * General purpose linked list
 */

/**
 * Create fixed length of Linked list
 * Node struct must have a pointor named after "next"!
 */
t_llist* create_llist(int len) {
  t_llist* head;
  t_llist* tmp_head;
  int ind;

  /* Create a scout, this is a blank node*/
  head = (t_llist*)my_malloc(sizeof(t_llist));
  head->next = NULL;
  
  for (ind=0; ind<(len-1); ind++) {
    /* Create a node ahead of the current*/
    tmp_head = (t_llist*)my_malloc(sizeof(t_llist)); 
    tmp_head->next = head;
    head = tmp_head;
  }
  
  return head; 
}

/**
 * Insert a node inside the linked list
 * Cur is pointer which a new node will be inserted after.
 */
t_llist* insert_llist_node(t_llist* cur) {
  t_llist* cur_next; 
  /* Store the current next*/  
  cur_next = cur->next;
  /* Allocate new node*/
  cur->next = (t_llist*)my_malloc(sizeof(t_llist));
  /* Configure the new node*/
  cur->next->next = cur_next;

  return cur->next;
}

/**
 * Romove a node from linked list
 * cur is the node whose next node is to be removed
 */
void remove_llist_node(t_llist* cur) { 
  t_llist* rm_node = cur->next;
  /* Connect the next next node*/ 
  cur->next = cur->next->next;
  /* free the node*/
  free(rm_node);
}

/**
 * Cat the linked list
 * head2 is connected to the tail of head1
 * return head1
 */
t_llist* cat_llists(t_llist* head1,
                    t_llist* head2) {
  t_llist* tmp = head1;

  /* Reach the tail of head1*/ 
  while(tmp->next != NULL) {
    tmp = tmp->next;
  }
  /* Cat*/
  tmp->next = head2->next;
  /* Free head2*/
  free(head2);
  
  return head1;
}

t_llist* search_llist_tail(t_llist* head) {
  t_llist* temp = head;
  if (NULL == temp) {
    return temp;
  }
  while(temp->next != NULL) {
    temp = temp->next;
  }
  return temp;
}

/* Free a linked list, Make sure before this function,
 * the dptr has been freed! I cannot free them here!!!
 */
void free_llist(t_llist* head) {
  t_llist* temp = head;
  t_llist* node_to_free = NULL;
  /* From the head to tail, 
   * free each linked node and move on to the next
   */
  while(temp) { /*Until we reach the tail, which is a null pointer*/
    if (NULL != temp->dptr) {
      printf("ERROR: The data pointer in linked list is not NULL!\n");
      printf("       It is possible that the data pointer is not freed!\n");
      exit(1);
    }
    /* Store the current node to be freed*/
    node_to_free = temp;
    /* Move on to the next*/
    temp = temp->next; 
    /* Now it is time to free*/
    free(node_to_free);
  }

  return;
}
