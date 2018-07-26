#ifndef BUFFER_INSERTION_H
#define BUFFER_INSERTION_H

void try_buffer_for_routing ( float** net_delay );
void load_best_buffer_list( );
int print_stat_memristor_buffer( char* fname, float buffer_size );
void clear_buffer( );
void count_routing_memristor_buffer( int num_per_channel, float buffer_size );

#endif
