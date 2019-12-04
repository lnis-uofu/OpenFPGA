/********************************************************************
 * Most utilized functions in FPGA X2P framework
 *******************************************************************/
#include <sys/stat.h>
#include <string>
#include <algorithm>

#include "vtr_assert.h"
#include "fpga_x2p_utils.h"

/********************************************************************
 * Format a directory path:
 * 1. Replace "\" with "/" 
 * 2. add a "/" if the string does not end with a "/"
 *******************************************************************/
std::string format_dir_path(const std::string& dir_path_to_format) {
  std::string formatted_dir_path = dir_path_to_format;

  char illegal_back_slash = '\\';
  char legal_back_slash = '/';

#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char illegal_back_slash = '/';
  char legal_back_slash = '\\';
#endif

  /* Replace "\" with "/" */
  std::replace(formatted_dir_path.begin(), formatted_dir_path.end(), illegal_back_slash, legal_back_slash); 

  /* Add a back slash the string is not ended like this! */
  if (legal_back_slash != formatted_dir_path.back()) {
    formatted_dir_path.push_back(legal_back_slash);
  }
 
  return formatted_dir_path; 
}

/******************************************************************** 
 * Extract full file name from a full path of file 
 * For example: <dir_path>/<file_name>
 * This function will return <file_name>
 ********************************************************************/
std::string find_path_file_name(const std::string& file_name) {

  char back_slash = '/';

#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char back_slash = '\\';
#endif 

  /* Find the last '/' in the string and return the left part */
  size_t found = file_name.rfind(back_slash);
  if (found != std::string::npos) {
    return file_name.substr(found + 1);
  }
  /* Not found. The input is the file name! Return the original string */
  return file_name;
}

/******************************************************************** 
 * Extract full directory path from a full path of file
 * For example: <dir_path>/<file_name>
 * This function will return <dir_path>
 ********************************************************************/
std::string find_path_dir_name(const std::string& file_name) {

  char back_slash = '/';

#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char back_slash = '\\';
#endif 

  /* Find the last '/' in the string and return the left part */
  size_t found = file_name.rfind(back_slash);
  if (found != std::string::npos) {
    return file_name.substr(0, found);
  }
  /* Not found, return an empty string */
  return std::string();
}

/******************************************************************** 
 * Check if the file stream is valid 
 ********************************************************************/
void check_file_handler(std::fstream& fp) {
  /* Make sure we have a valid file handler*/
  /* Print out debugging information for if the file is not opened/created properly */
  if (!fp.is_open() || !fp.good()) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create file!\n",
               __FILE__, __LINE__); 
    exit(1);
  }
}

/******************************************************************** 
 * Convert an integer to an one-hot encoding integer array
 ********************************************************************/
std::vector<size_t> my_ito1hot_vec(const size_t& in_int, const size_t& bin_len) {
  /* Make sure we do not have any overflow! */
  VTR_ASSERT ( (in_int <= bin_len) );

  /* Initialize */
  std::vector<size_t> ret(bin_len, 0);

  if (bin_len == in_int) {
    return ret; /* all zero case */
  }
  ret[in_int] = 1; /* Keep a good sequence of bits */
 
  return ret;
}

/******************************************************************** 
 * Converter an integer to a binary vector 
 ********************************************************************/
std::vector<size_t> my_itobin_vec(const size_t& in_int, const size_t& bin_len) {
  std::vector<size_t> ret(bin_len, 0);

  /* Make sure we do not have any overflow! */
  VTR_ASSERT ( (in_int < pow(2., bin_len)) );
  
  size_t temp = in_int;
  for (size_t i = 0; i < bin_len; i++) {
    if (1 == temp % 2) { 
      ret[i] = 1; /* Keep a good sequence of bits */
    }
    temp = temp / 2;
  }
 
  return ret;
}

/******************************************************************** 
 * Create a directory with a given path
 ********************************************************************/
bool create_dir_path(const char* dir_path) {
   /* Give up if the path is empty */
   if (NULL == dir_path) {
     vpr_printf(TIO_MESSAGE_INFO,
                "dir_path is empty and nothing is created.\n");
     return false;
   }

   /* Try to create a directory */
   int ret = mkdir(dir_path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);

   /* Analyze the return flag and output status */
   switch (ret) {
   case 0:
     vpr_printf(TIO_MESSAGE_INFO,
                "Create directory(%s)...successfully.\n",
                dir_path);
     return true;
   case -1:
     if (EEXIST == errno) {
       vpr_printf(TIO_MESSAGE_WARNING,
                  "Directory(%s) already exists. Will overwrite contents\n",
                  dir_path);
       return true;
     }
     break;
   default:
     vpr_printf(TIO_MESSAGE_ERROR,
                "Create directory(%s)...Failed!\n",
                dir_path);
     exit(1);
     return false;
  }

  return false;
}

