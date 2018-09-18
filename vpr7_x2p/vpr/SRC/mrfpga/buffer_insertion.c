#include <stdio.h>
#include "util.h"
/* Xifan TANG */
#include "physical_types.h"
/* END */
#include "vpr_types.h"
#include "globals.h"
#include "buffer_insertion.h"
/* Xifan TANG */
#include "mrfpga_util.h"
#include "mrfpga_globals.h"
#include "net_delay_types.h"
#include "net_delay_local_void.h"
/* END */

/* memristor */
typedef struct s_buffer_plan {t_linked_int* inode_head; t_linked_int* sink_head; float* sink_delay; float C_downstream; float Tdel;} t_buffer_plan;
typedef struct s_buffer_plan_node { t_buffer_plan value; struct s_buffer_plan_node* next;} t_buffer_plan_node;
typedef struct s_buffer_plan_list { t_buffer_plan_node* front; } t_buffer_plan_list;

/* memristor */
static t_buffer_plan_list get_empty_buffer_plan_list( );
static t_buffer_plan_list join_left_plan_list_into_whole( t_buffer_plan_list left, t_buffer_plan_list* whole, int num_whole, t_buffer_plan_list current, int num_pins );
static void free_buffer_list( t_buffer_plan_list list );
static t_buffer_plan_list insert_buffer( t_buffer_plan_list list, int inode, float C, float R, float Tdel, int num_pins );
static t_buffer_plan insert_wire_to_buffer_plan( t_buffer_plan plan, float C, float R);
static void insert_wire_to_buffer_list( t_buffer_plan_list list, float C,float R);
static void insert_switch_to_buffer_list( t_buffer_plan_list list, struct s_switch_inf switch_inf_local);
static t_buffer_plan insert_switch_to_buffer_plan( t_buffer_plan plan, struct s_switch_inf switch_inf_local);
static t_buffer_plan_list insert_buffer_plan_to_list( t_buffer_plan plan, t_buffer_plan_list list );
static void save_best_buffer_list( t_linked_int* best_list );
static void free_best_buffer_list( );
static int get_int_list_length( t_linked_int* list );
static void save_best_timing( float* sink_delay, t_linked_int* index, float* net_delay );
static int alloc_isink_to_inode( int inet, int** isink_to_inode_ptr );
static t_buffer_plan_list try_buffer_rc_tree (t_rc_node* rc_node, int num_pins, int* isink_to_inode);
static t_buffer_plan get_init_buffer_plan( int inode, int num_pins, int* isink_to_inode );
static t_buffer_plan_list get_init_buffer_plan_list( int inode, int num_pins, int* isink_to_inode );
static t_buffer_plan combine_buffer_plan( t_buffer_plan slow_branch, t_buffer_plan* plan_whole, int num_whole, int num_pins );
static void copy_delay( float* base, float* source, t_linked_int* index );
static void add_delay_to_array( float* sink_delay, t_linked_int* index, float delay_addition );
static float* copy_from_float_array( float* source, int num );
static t_buffer_plan add_delay_to_buffer_plan( t_buffer_plan plan, float Tdel );


void try_buffer_for_net( int inet, t_rc_node** rc_node_free_list, t_linked_rc_edge** rc_edge_free_list, t_linked_rc_ptr* rr_node_to_rc_node, float* net_delay );
/* memristor */

void try_buffer_for_routing ( float** net_delay ) {

    t_rc_node *rc_node_free_list;
    t_linked_rc_edge *rc_edge_free_list;
    int inet;
    t_linked_rc_ptr *rr_node_to_rc_node; /* [0..num_rr_nodes-1]  */

    rr_node_to_rc_node = (t_linked_rc_ptr *) my_calloc (num_rr_nodes, 
            sizeof (t_linked_rc_ptr));

    rc_node_free_list = NULL;
    rc_edge_free_list = NULL;

    free_best_buffer_list( );
    for (inet=0;inet<num_nets;inet++) {
        try_buffer_for_net( inet, &rc_node_free_list, &rc_edge_free_list, rr_node_to_rc_node, net_delay[inet] );
    }
    /* printf( " critical delay after buffer insertion: %g\n", crit_delay ); */
    free_rc_node_free_list (rc_node_free_list);
    free_rc_edge_free_list (rc_edge_free_list);
    free (rr_node_to_rc_node);
}

void try_buffer_for_net( int inet, t_rc_node** rc_node_free_list, t_linked_rc_edge** rc_edge_free_list, t_linked_rc_ptr* rr_node_to_rc_node, float* net_delay ) {
    t_rc_node *rc_root;
    t_buffer_plan_list plan_list;
    t_buffer_plan_node* traverse;
    t_buffer_plan best;
    float crit_delay = 0;
    int* isink_to_inode;
    int num_pins;
 
    /* Xifan TANG: VPR7 use clb_net */
    if (clb_net[inet].is_global) {
      return;
    }
    else {
       num_pins = alloc_isink_to_inode( inet, &isink_to_inode );
       rc_root = alloc_and_load_rc_tree (inet, rc_node_free_list, 
                  rc_edge_free_list, rr_node_to_rc_node);
       plan_list = try_buffer_rc_tree(rc_root, num_pins, isink_to_inode);
       best.Tdel = HUGE_VAL;
       traverse = plan_list.front;
       while ( traverse != NULL )
       {
           if ( traverse->value.Tdel < best.Tdel || ( traverse->value.Tdel == best.Tdel && get_int_list_length( traverse->value.inode_head ) < get_int_list_length( best.inode_head ) ) )
           {
               best = traverse->value;;
           }
           traverse = traverse->next;
       }
       crit_delay = max( crit_delay, best.Tdel );
       save_best_buffer_list( best.inode_head );
       save_best_timing( best.sink_delay, best.sink_head, net_delay );
       free_buffer_list( plan_list );
       free_rc_tree (rc_root, rc_node_free_list, rc_edge_free_list);
       reset_rr_node_to_rc_node (rr_node_to_rc_node, inet );
       free( isink_to_inode );
    }
}

static void save_best_timing( float* sink_delay, t_linked_int* index, float* net_delay )
{
    t_linked_int* traverse;
    traverse = index;
    while( traverse != NULL )
    {
        net_delay[traverse->data] = sink_delay[traverse->data];
        traverse = traverse->next;
    }
}

static int alloc_isink_to_inode( int inet, int** isink_to_inode_ptr )
{
    int i;
    int num_pins = clb_net[inet].num_sinks + 1;
    *isink_to_inode_ptr = (int*)my_malloc( sizeof(int) * num_pins );
    for( i = 1; i < num_pins; i++ )
    {
        (*isink_to_inode_ptr)[i] = net_rr_terminals[inet][i];
    }
    return num_pins;
}

static int get_int_list_length( t_linked_int* list )
{
    t_linked_int* traverse = list;
    int counter = 0;
    while ( traverse != NULL )
    {
        counter++;
        traverse = traverse->next;
    }
    return counter;
}

static void free_best_buffer_list( )
{
    t_linked_int* traverse = main_best_buffer_list;
    t_linked_int* current;
    while ( traverse != NULL )
    {
        current = traverse;
        traverse = current->next;
        free( current );
    }
    main_best_buffer_list = NULL;
}

static void save_best_buffer_list( t_linked_int* best_list )
{
    t_linked_int* current;
    t_linked_int* traverse = best_list;
    while ( traverse != NULL )
    {
        current = ( t_linked_int* ) my_malloc( sizeof( t_linked_int ) );
        current->data = traverse->data;
        current->next = main_best_buffer_list;
        main_best_buffer_list = current;
        traverse = traverse->next;
    }
}

void load_best_buffer_list( )
{
    t_linked_int* traverse = main_best_buffer_list;
    clear_buffer( );
    while ( traverse != NULL )
    {
        rr_node[ traverse->data ].buffered = 1;
        traverse = traverse->next;
    }
}

int print_stat_memristor_buffer( char* fname, float buffer_size )
{
    char fnamex[100];
    char fnamey[100];
    FILE* fpx;
    FILE* fpy;
    int** freqx;
    int** freqy;
    int counter;
    int i, j;
    t_linked_int* traverse;
    int max_number;
    sprintf( fnamex, "x_%s", fname );
    sprintf( fnamey, "y_%s", fname );
    fpx = my_fopen( fnamex, "w" ,0);
    fpy = my_fopen( fnamey, "w" ,0);
    freqx = (int**) alloc_matrix( 1, nx, 0, ny, sizeof(int) );
    freqy = (int**) alloc_matrix( 0, nx, 1, ny, sizeof(int) );
    counter = 0;
    for( i = 1; i <= nx; i++ )
    {
        for( j = 0; j <= ny; j++ )
        {
            freqx[i][j]=0;
        }
    }
    for( i = 0; i <= nx; i++ )
    {
        for( j = 1; j <= ny; j++ )
        {
            freqy[i][j]=0;
        }
    }
    traverse = main_best_buffer_list;
    while ( traverse != NULL )
    {
        counter++;
        if ( rr_node[traverse->data].type == CHANX )
        {
            freqx[rr_node[traverse->data].xlow][( rr_node[traverse->data].ylow + rr_node[traverse->data].yhigh ) / 2]++;
        }
        else if ( rr_node[traverse->data].type == CHANY )
        {
            freqy[(rr_node[traverse->data].xlow+rr_node[traverse->data].xhigh)/2][rr_node[traverse->data].ylow]++;
        }
        traverse = traverse->next;
    }
    max_number = 0;
    for( i = 1; i <= nx; i++ )
    {
        for( j = 0; j <= ny; j++ )
        {
            max_number = max( max_number, freqx[i][j] );
            fprintf( fpx, "%d ", freqx[i][j] );
        }
        fprintf( fpx, "\n" );
    }
    for( i = 0; i <= nx; i++ )
    {
        for( j = 1; j <= ny; j++ )
        {
            max_number = max( max_number, freqy[i][j] );
            fprintf( fpy, "%d ", freqy[i][j] );
        }
        fprintf( fpy, "\n" );
    }
    free_matrix( freqx, 1, nx, 0, sizeof(int) );
    free_matrix( freqy, 0, nx, 1, sizeof(int) );
    fclose( fpx );
    fclose( fpy );
    printf( "%d buffers ( size: %g ) inserted, total active trans: %g\n", counter, buffer_size, counter*buffer_size );
    printf( "maximum # of buffers in one channel: %d\n", max_number );
    return max_number;
}

void clear_buffer( )
{
    int i;
    for( i = 0; i < num_rr_nodes; i++ )
    {
        rr_node[ i ].buffered = 0;
    }
}

static t_buffer_plan_list try_buffer_rc_tree (t_rc_node *rc_node, int num_pins, int* isink_to_inode) {
 t_linked_rc_edge *linked_rc_edge;
 t_rc_node *child_node;
 short iswitch;
 int inode;
 int num_whole;
 int i;
 t_rr_node rrnode;
 t_buffer_plan_list result;
 t_buffer_plan_list* plan_list_from_branch;

 inode = rc_node->inode;  
 rrnode = rr_node[ inode ];

 if ( rrnode.type == SINK )
 {
     result = get_init_buffer_plan_list( inode, num_pins, isink_to_inode );
 }
 else  
 {
     result = get_empty_buffer_plan_list( );
     num_whole = 0;
     linked_rc_edge = rc_node->u.child_list;
     while (linked_rc_edge != NULL) {          /* For all children */
         num_whole++;
         linked_rc_edge = linked_rc_edge->next;
     }
     plan_list_from_branch = ( t_buffer_plan_list* )my_malloc( num_whole*sizeof(t_buffer_plan_list) );

     i = 0;
     linked_rc_edge = rc_node->u.child_list;
     while (linked_rc_edge != NULL) {          /* For all children */
         iswitch = linked_rc_edge->iswitch;
         child_node = linked_rc_edge->child;
         plan_list_from_branch[i] = try_buffer_rc_tree( child_node, num_pins, isink_to_inode );
         insert_switch_to_buffer_list( plan_list_from_branch[i], switch_inf[ iswitch ] );
         linked_rc_edge = linked_rc_edge->next;
         i++;
     }
     if ( num_whole > 1 )
     {
         for( i = 0; i < num_whole; i++ )
         {
             result = join_left_plan_list_into_whole( plan_list_from_branch[ i ], plan_list_from_branch, num_whole, result, num_pins );
         }
         for( i = 0; i < num_whole; i++ )
         {
             free_buffer_list( plan_list_from_branch[ i ] );
         }
     }
     else
     {
         result = plan_list_from_branch[ 0 ];
     }
     free( plan_list_from_branch );
     insert_wire_to_buffer_list( result, rrnode.C, 0.5 * rrnode.R );
     if (rr_node[ inode ].type == CHANX || rr_node[ inode ].type == CHANY)
     {
         result = insert_buffer( result, inode, wire_buffer_inf.C, wire_buffer_inf.R, wire_buffer_inf.Tdel, num_pins );
     }
 }
 return result;
}

static void add_delay_to_buffer_list( t_buffer_plan_list list, float Tdel , boolean skip_first )
{
    t_buffer_plan_node* traverse = list.front;
    if ( skip_first && traverse != NULL )
    {
        traverse = traverse->next;
    }
    while ( traverse != NULL )
    {
        traverse->value = add_delay_to_buffer_plan( traverse->value, Tdel );
        traverse = traverse->next;
    }
}

static t_buffer_plan add_delay_to_buffer_plan( t_buffer_plan plan, float Tdel )
{
    add_delay_to_array( plan.sink_delay, plan.sink_head, Tdel );
    plan.Tdel += Tdel;
    return plan;
}

static t_buffer_plan_list get_empty_buffer_plan_list( )
{
    t_buffer_plan_list list;
    list.front = NULL;
    return list;
}

static t_buffer_plan get_init_buffer_plan( int inode, int num_pins, int* isink_to_inode )
{
    t_buffer_plan plan;
    int i;
    plan.inode_head = NULL;
    plan.sink_head = (t_linked_int*) my_malloc( sizeof( t_linked_int ) );
    plan.sink_head->next = 0;
    plan.sink_delay = (float*) my_malloc( sizeof(float) * num_pins );
    for( i = 1; i < num_pins; i++ )
    {
        if( isink_to_inode[ i ] == inode )
        {
            plan.sink_head->data = i;
        }
        plan.sink_delay[i] = 0;
    }
    plan.C_downstream = 0.;
    plan.Tdel = 0.;
    return plan;
}

static t_buffer_plan_list get_init_buffer_plan_list( int inode, int num_pins, int* isink_to_inode )
{
    t_buffer_plan_list list;
    list.front = ( t_buffer_plan_node* ) my_malloc( sizeof( t_buffer_plan_node ) );
    list.front->value = get_init_buffer_plan( inode, num_pins, isink_to_inode );
    list.front->next = NULL;
    return list;
}

static t_buffer_plan_list join_left_plan_list_into_whole( t_buffer_plan_list left, t_buffer_plan_list* whole, int num_whole, t_buffer_plan_list current, int num_pins )
{
    int i;
    t_buffer_plan_node* left_traverse;
    t_buffer_plan_node* whole_traverse;
    t_buffer_plan* best_plans = (t_buffer_plan*) my_malloc(num_whole*sizeof(t_buffer_plan));
    left_traverse = left.front;
    while( left_traverse != NULL )
    {
        for( i=0; i< num_whole; i++ )
        {
            best_plans[i].C_downstream = HUGE_VAL;
            if ( left.front == whole[ i ].front )
            {
                continue;
            }
            whole_traverse = whole[i].front;
            while ( whole_traverse != NULL )
            {
                if ( whole_traverse->value.Tdel <= left_traverse->value.Tdel 
                        && whole_traverse->value.C_downstream < best_plans[i].C_downstream )
                {
                    best_plans[i] = whole_traverse->value;
                }
                whole_traverse = whole_traverse->next;
            }
            if ( best_plans[i].C_downstream == HUGE_VAL )
            {
                break;
            }
        }
        if ( i == num_whole )
        {
            current = insert_buffer_plan_to_list( combine_buffer_plan( left_traverse->value, best_plans, num_whole, num_pins ), current );
        }
        left_traverse = left_traverse->next;
    }
    free( best_plans );
    return current;
}

static t_buffer_plan combine_buffer_plan( t_buffer_plan slow_branch, t_buffer_plan* plan_whole, int num_whole, int num_pins )
{
    int i;
    t_buffer_plan new_plan = slow_branch;
    new_plan.inode_head = NULL;
    new_plan.sink_head = NULL;
    new_plan.sink_delay = (float*) my_malloc( sizeof(float) * num_pins );
    new_plan.inode_head = copy_from_list( new_plan.inode_head, slow_branch.inode_head );
    new_plan.sink_head = copy_from_list( new_plan.sink_head, slow_branch.sink_head );
    copy_delay( new_plan.sink_delay, slow_branch.sink_delay, slow_branch.sink_head );
    for( i = 0; i < num_whole; i++ )
    {
        if ( plan_whole[ i ].C_downstream == HUGE_VAL )
        {
            continue;
        }
        new_plan.C_downstream += plan_whole[i].C_downstream;
        new_plan.inode_head = copy_from_list( new_plan.inode_head, plan_whole[i].inode_head );
        new_plan.sink_head = copy_from_list( new_plan.sink_head, plan_whole[i].sink_head );
        copy_delay( new_plan.sink_delay, plan_whole[i].sink_delay, plan_whole[i].sink_head );
    }
    return new_plan;
}

static void copy_delay( float* base, float* source, t_linked_int* index )
{
    t_linked_int* traverse = index;
    while( traverse != NULL )
    {
        base[traverse->data] = source[traverse->data];
        traverse = traverse->next;
    }
}

static void free_buffer_list( t_buffer_plan_list list )
{
    t_buffer_plan_node* traverse = list.front;
    t_buffer_plan_node* temp;
    while( traverse != NULL )
    {
        free_int_list( &( traverse->value.inode_head ) );
        free_int_list( &( traverse->value.sink_head ) );
        free( traverse->value.sink_delay );
        temp = traverse->next;
        free( traverse );
        traverse = temp;
    }
}

static t_buffer_plan_list insert_buffer( t_buffer_plan_list list, int inode, float C, float R, float Tdel, int num_pins )
{
    t_buffer_plan best;
    //int i;
    float Tdel_temp;
    float delay_addition;
    float Tdel_best = HUGE_VAL;
    t_buffer_plan_node* traverse = list.front;
    while ( traverse != NULL )
    {
        Tdel_temp = traverse->value.Tdel + traverse->value.C_downstream * R;
        if ( Tdel_temp < Tdel_best )
        {
            Tdel_best = Tdel_temp;
            best = traverse->value;
        }
        traverse = traverse->next;
    }
    best.inode_head = copy_from_list( NULL, best.inode_head );
    best.inode_head = insert_in_int_list2( best.inode_head, inode );
    best.sink_head = copy_from_list( NULL, best.sink_head );
    best.sink_delay = copy_from_float_array( best.sink_delay, num_pins );
    best.C_downstream = C;
    delay_addition = Tdel_best + Tdel - best.Tdel;
    add_delay_to_array( best.sink_delay, best.sink_head, delay_addition );
    best.Tdel += delay_addition;
    return insert_buffer_plan_to_list( best, list );
}

static float* copy_from_float_array( float* source, int num )
{
    int i;
    float* result = (float*)my_malloc( sizeof(float) * num );
    for( i = 0; i < num; i++ )
    {
        result[i] = source[i];
    }
    return result;
}

static t_buffer_plan insert_wire_to_buffer_plan( t_buffer_plan plan, float C, float R )
{
    float delay_addition;
    plan.C_downstream += C;
    delay_addition = R * plan.C_downstream;
    add_delay_to_array( plan.sink_delay, plan.sink_head, delay_addition );
    plan.Tdel += delay_addition;
    return plan;
}

static void add_delay_to_array( float* sink_delay, t_linked_int* index, float delay_addition )
{
    t_linked_int* traverse = index;
    while( traverse != NULL )
    {
        sink_delay[traverse->data] += delay_addition;
        traverse = traverse->next;
    }
}

static t_buffer_plan insert_switch_to_buffer_plan( t_buffer_plan plan, struct s_switch_inf switch_inf_local)
{
    float delay_addition;
    delay_addition = switch_inf_local.R * plan.C_downstream + switch_inf_local.Tdel;
    add_delay_to_array( plan.sink_delay, plan.sink_head, delay_addition );
    plan.Tdel += delay_addition;
    if ( switch_inf_local.buffered )
    {
        plan.C_downstream = switch_inf_local.Cin;
    }
    else
    {
        plan.C_downstream += switch_inf_local.Cout + switch_inf_local.Cin;
    }
    return plan;
}

static void insert_switch_to_buffer_list( t_buffer_plan_list list, struct s_switch_inf switch_inf_local)
{
    t_buffer_plan_node* traverse = list.front;
    while ( traverse != NULL )
    {
        traverse->value = insert_switch_to_buffer_plan( traverse->value, switch_inf_local);
        traverse = traverse->next;
    }
}

static void insert_wire_to_buffer_list( t_buffer_plan_list list, float C,float R )
{
    t_buffer_plan_node* traverse = list.front;
    while ( traverse != NULL )
    {
        traverse->value = insert_wire_to_buffer_plan( traverse->value, C, R );
        traverse = traverse->next;
    }
}

static t_buffer_plan_list insert_buffer_plan_to_list( t_buffer_plan plan, t_buffer_plan_list list ) {
    /* original name : new is a resevered word in C++!!! */
    t_buffer_plan_node* new_buffer_plan_node = ( t_buffer_plan_node* )my_malloc( sizeof( t_buffer_plan_node ) );
    new_buffer_plan_node->value = plan;
    new_buffer_plan_node->next = list.front;
    list.front = new_buffer_plan_node;
    return list;
}

void count_routing_memristor_buffer( int num_per_channel, float buffer_size )
{
    float total_area = num_per_channel*((1+nx)*ny+(1+ny)*nx)*buffer_size;
    printf("\nRouting area due to inserted buffers: Total: %g Per clb: %g\n", total_area, total_area / nx / ny );
    printf("tile area: %#g\n", total_area / nx / ny + grid_logic_tile_area );
}

/* memristor end */

