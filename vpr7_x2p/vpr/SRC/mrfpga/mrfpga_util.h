/* Xifan TANG : 
 * Create a local copy of max and min exclusively for mrFPGA
 */

#ifndef max
#define max(a,b) (((a) > (b))? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) > (b))? (b) : (a))
#endif

t_linked_int* copy_from_list(t_linked_int* base, t_linked_int* target);

t_linked_int* insert_in_int_list2(t_linked_int *head, int data);
