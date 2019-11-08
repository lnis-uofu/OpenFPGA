#ifndef SDC_WRITER_UTILS_H
#define SDC_WRITER_UTILS_H

#include <fstream>
#include <string>
#include "device_port.h"

void print_sdc_file_header(std::fstream& fp,
                           const std::string& usage);

std::string generate_sdc_port(const BasicPort& port);

#endif
