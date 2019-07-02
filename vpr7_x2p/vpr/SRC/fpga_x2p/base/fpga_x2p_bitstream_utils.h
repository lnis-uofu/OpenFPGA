
int determine_decoder_size(int num_addr_out);

int count_num_sram_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        int mux_size);

int count_num_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        enum e_sram_orgz cur_sram_orgz_type,
                                        int mux_size);

int count_num_mode_bits_one_spice_model(t_spice_model* cur_spice_model);

int count_num_reserved_conf_bits_one_lut_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type);

int count_num_reserved_conf_bits_one_mux_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type,
                                                     int mux_size);

int count_num_reserved_conf_bits_one_rram_sram_spice_model(t_spice_model* cur_spice_model,
                                                           enum e_sram_orgz cur_sram_orgz_type);

int count_num_reserved_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                                 enum e_sram_orgz cur_sram_orgz_type,
                                                 int mux_size);

void  
add_mux_scff_conf_bits_to_llist(int mux_size,
                           t_sram_orgz_info* cur_sram_orgz_info, 
                           int num_mux_sram_bits, int* mux_sram_bits,
                           t_spice_model* mux_spice_model);

void 
add_mux_membank_conf_bits_to_llist(int mux_size,
                                   t_sram_orgz_info* cur_sram_orgz_info, 
                                   int num_mux_sram_bits, int* mux_sram_bits,
                                   t_spice_model* mux_spice_model);

void  
add_mux_conf_bits_to_llist(int mux_size,
                           t_sram_orgz_info* cur_sram_orgz_info, 
                           int num_mux_sram_bits, int* mux_sram_bits,
                           t_spice_model* mux_spice_model);

void add_sram_membank_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, int mem_index, 
                                         int num_bls, int num_wls, 
                                         int* bl_conf_bits, int* wl_conf_bits);

void  
add_sram_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, int mem_index, 
                            int num_sram_bits, int* sram_bits);


void decode_memory_bank_sram(t_spice_model* cur_sram_spice_model, int sram_bit,
                             int bl_len, int wl_len, int bl_offset, int wl_offset,
                             int* bl_conf_bits, int* wl_conf_bits);

void 
decode_and_add_sram_membank_conf_bit_to_llist(t_sram_orgz_info* cur_sram_orgz_info,
                                              int mem_index,
                                              int num_bl_per_sram, int num_wl_per_sram, 
                                              int cur_sram_bit);

void determine_blwl_decoder_size(INP t_sram_orgz_info* cur_sram_orgz_info,
                                 OUTP int* num_array_bl, OUTP int* num_array_wl,
                                 OUTP int* bl_decoder_size, OUTP int* wl_decoder_size) ;

void init_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                       int num_switch,
                                       t_switch_inf* switches,
                                       t_spice* spice,
                                       t_det_routing_arch* routing_arch);

void add_one_conf_bit_to_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info) ;

void add_sram_conf_bits_to_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                                          t_spice_model* cur_spice_model) ;

void add_mux_conf_bits_to_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                                         t_spice_model* mux_spice_model, int mux_size) ;
