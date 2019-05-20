#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "vpr_types.h"
#include "mrfpga_util.h"

/* mrFPGA */
t_linked_int* insert_in_int_list2 (t_linked_int *head, int data )
{
 t_linked_int *linked_int;
 linked_int = (t_linked_int *) my_malloc (sizeof (t_linked_int));
 linked_int->data = data;
 linked_int->next = head;
 return (linked_int);
}

t_linked_int* copy_from_list( t_linked_int* base, t_linked_int* target )
{
    while ( target != NULL )
    {
        base = insert_in_int_list2( base, target->data );
        target = target->next;
    }
    return base;
}

/* end */
