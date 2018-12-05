
t_spice_model_port** get_spice_model_delay_info_ports(t_spice_model* cur_spice_model, 
                                                      char* port_list,
                                                      int* num_port);

int get_spice_model_num_tedges_per_pin(t_spice_model* cur_spice_model,
                                       enum PORTS port_type);

t_spice_model_tedge* get_unused_spice_model_port_tedge(t_spice_model_port* cur_port,
                                                      int pin_index);

void alloc_spice_model_num_tedges(t_spice_model* cur_spice_model);

void configure_one_tedge_delay(t_spice_model_tedge* cur_tedge,
                               enum spice_model_delay_type delay_type,
                               float delay);

void configure_tedges_delay_matrix(enum spice_model_delay_type delay_type,
                                   int num_in_port, t_spice_model_port** in_port,
                                   int num_out_port, t_spice_model_port** out_port,
                                   float** delay_matrix);

float** fpga_spice_atof_2D(int num_in_port, int num_out_port, char* str);

void free_2D_matrix(void** delay_matrix,
                    int num_in_port, int num_out_port);

void annotate_spice_model_timing(t_spice_model* cur_spice_model);
