#ifndef WRITE_RR_BLOCKS_H
#define WRITE_RR_BLOCKS_H

void write_rr_switch_block_to_xml(std::string fname_prefix, RRGSB& rr_gsb);

void write_device_rr_gsb_to_xml(char* sb_xml_dir, DeviceRRGSB& LL_device_rr_gsb);

#endif
