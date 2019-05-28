#ifndef WRITE_RR_BLOCKS_H
#define WRITE_RR_BLOCKS_H

void write_rr_switch_block_to_xml(std::string fname_prefix, RRSwitchBlock& rr_sb);

void write_device_rr_switch_block_to_xml(char* sb_xml_dir, DeviceRRSwitchBlock& LL_device_rr_switch_block);

#endif
