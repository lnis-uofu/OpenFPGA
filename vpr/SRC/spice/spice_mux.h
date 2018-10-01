
void generate_spice_muxes(char* subckt_dir,
                          int num_switch,
                          t_switch_inf* switches,
                          t_spice* spice,
                          t_det_routing_arch* routing_arch);

t_llist* search_mux_linked_list(t_llist* mux_head,
                                int mux_size,
                                t_spice_model* spice_model);

void check_and_add_mux_to_linked_list(t_llist** muxes_head,
                                      int mux_size,
                                      t_spice_model* spice_model);

void free_muxes_llist(t_llist* muxes_head);

