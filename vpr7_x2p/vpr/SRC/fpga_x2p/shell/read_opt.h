/*Subroutines in read_opts.c*/
boolean read_options(INP int argc,
                     INP char ** argv,
                     t_opt_info* cur);

void print_opt_info_help_desk(t_opt_info* cur_opt_info);

boolean process_arg_opt(INP char** argv,
                        INOUTP int* iarg,
                        INP char* curarg,
                        t_opt_info* cur);

int show_opt_list(t_opt_info* cur);

int process_int_arg(INP char* arg);

float process_float_arg(INP char* arg);

boolean is_opt_set(t_opt_info* opts, char* opt_name, boolean default_val);

char* get_opt_val(t_opt_info* opts, char* opt_name);

int get_opt_int_val(t_opt_info* opts, char* opt_name, int default_val);

float get_opt_float_val(t_opt_info* opts, char* opt_name, float default_val);
