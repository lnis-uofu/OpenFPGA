/* mrFPGA */
void init_buffer_inf(t_buffer_inf* buffer_inf);

void init_memristor_inf(t_memristor_inf* memristor_inf);

void init_arch_mrfpga(t_arch_mrfpga* arch_mrfpga);

void ProcessTechHack(INOUTP ezxml_t Node,
      OUTP struct s_arch *arch);
void ProcessmrFPGA(INOUTP ezxml_t Node,
      OUTP struct s_arch *arch);
void ProcessTechnology(INOUTP ezxml_t Node,
      OUTP struct s_arch *arch);
void ProcessWireBuffer(INOUTP ezxml_t Node,
      OUTP struct s_arch *arch);

void ProcessMrFPGATiming(INOUTP ezxml_t Cur, 
                         OUTP t_arch* arch);
/* end */
