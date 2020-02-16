/********************************************************************
 * This file includes functions that handles the file outputting
 * in OpenFPGA framework
 *******************************************************************/
#include <sys/stat.h>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h" 

namespace openfpga {

/********************************************************************
 * A most utilized function to validate the file stream
 * This function will return true or false for a valid/invalid file stream 
 *******************************************************************/
bool valid_file_stream(std::fstream& fp) {
  /* Validate the file stream */
  if (!fp.is_open() || !fp.good()) {
    return false;
  }

  return true;
}

/********************************************************************
 * A most utilized function to validate the file stream
 * This function will error out for a valid/invalid file stream 
 *******************************************************************/
void check_file_stream(const char* fname, 
                       std::fstream& fp) {

  if (false == valid_file_stream(fp)) {
    VTR_LOG("Invalid file stream for file: %s\n",
            fname);
    exit(1);
  }
}

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
 * Create a directory with a given path
 ********************************************************************/
bool create_dir_path(const char* dir_path) {
   /* Give up if the path is empty */
   if (nullptr == dir_path) {
     VTR_LOG_ERROR("dir_path is empty and nothing is created.\n");
     return false;
   }

   /* Try to create a directory */
   int ret = mkdir(dir_path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);

   /* Analyze the return flag and output status */
   switch (ret) {
   case 0:
     VTR_LOG("Succeed to create directory '%s'\n",
             dir_path);
     return true;
   case -1:
     if (EEXIST == errno) {
       VTR_LOG_WARN("Directory '%s' already exists. Will overwrite contents\n",
                     dir_path);
       return true;
     }
     break;
   default:
     VTR_LOG_ERROR("Create directory '%s'...Failed!\n",
                 dir_path);
     exit(1);
     return false;
  }

  return false;
}

} /* namespace openfpga ends */
