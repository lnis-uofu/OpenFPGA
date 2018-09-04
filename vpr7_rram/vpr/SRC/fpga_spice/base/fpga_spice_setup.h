
void init_check_arch_spice_models(t_arch* arch,
                                  t_det_routing_arch* routing_arch);

void check_keywords_conflict(t_arch Arch);

void fpga_spice_free(t_arch* Arch);

void fpga_spice_setup(t_vpr_setup vpr_setup,
                      t_arch* Arch);
