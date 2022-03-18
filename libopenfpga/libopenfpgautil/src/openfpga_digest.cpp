/********************************************************************
 * This file includes functions that handles the file outputting
 * in OpenFPGA framework
 *******************************************************************/
#include <sys/stat.h>
#include <vector>
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


#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char illegal_back_slash = '/';
  char legal_back_slash = '\\';
#else
  char illegal_back_slash = '\\';
  char legal_back_slash = '/';
#endif

  /* Return an empty string if the input is empty */
  if (true == formatted_dir_path.empty()) {
    return formatted_dir_path;
  }

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


#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char back_slash = '\\';
#else
  char back_slash = '/';
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


#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char back_slash = '\\';
#else
  char back_slash = '/';
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
static 
bool create_dir_path(const std::string& dir_path,
                     const bool& verbose) {
  /* Give up if the path is empty */
  if (true == dir_path.empty()) {
    VTR_LOG_WARN("Directory path is empty and nothing will be created.\n");
    return true;
  }

  /* Try to create a directory */
#ifdef _WIN32
  int ret = mkdir(dir_path.c_str());
#else  
  int ret = mkdir(dir_path.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
#endif

  /* Analyze the return flag and output status */
  switch (ret) {
  case 0:
    VTR_LOGV(verbose,
             "Succeed to create directory '%s'\n",
             dir_path.c_str());
    return true;
  case -1:
    if (EEXIST == errno) {
      VTR_LOGV_WARN(verbose,
                    "Directory '%s' already exists. Will overwrite contents\n",
                    dir_path.c_str());
      return true;
    }
    VTR_LOG_ERROR("Create directory '%s'...Failed!\n",
                  dir_path.c_str());
    exit(1);
    break;
  default:
    VTR_LOG_ERROR("Create directory '%s'...Failed!\n",
                  dir_path.c_str());
    exit(1);
    return false;
  }

  return false;
}

/******************************************************************** 
 * Recursively create a directory with a given path
 * The create_dir_path() function will only try to create a directory
 * in the last level. If any parent directory is not created, it will
 * always fail.
 * This function will try to create all the parent directory before 
 * creating the last level. 
 ********************************************************************/
static 
bool rec_create_dir_path(const std::string& dir_path) {
  /* Give up if the path is empty */
  if (true == dir_path.empty()) {
    VTR_LOG_WARN("Directory path is empty and nothing will be created.\n");
    return true;
  }

  /* Try to find the positions of all the slashes
   * which are the splitter between directories
   */

#ifdef _WIN32
/* For windows OS, replace any '/' with '\' */
  char back_slash = '\\';
#else
  char back_slash = '/';
#endif 

  std::vector<size_t> slash_pos; 
 
  /* Keep searching until we reach the end of the string */
  for (size_t pos = 0; pos < dir_path.size(); ++pos) {
    /* Skip the pos = 0, we should avoid creating any root directory */
    if ( (back_slash == dir_path.at(pos))
      && (0 != pos))  {
      slash_pos.push_back(pos);
    }
  }

  /* Create directory by following the position of back slash
   * For each back slash, create a sub string from the beginning 
   * and try to create directory
   */
  for (const size_t& pos : slash_pos) {
    std::string sub_dir = dir_path.substr(0, pos);

    /* Turn on verbose output only for the last position: the leaf directory */
    if (false == create_dir_path(sub_dir, &pos == &slash_pos.back())) {
      return false;
    } 
  }

  return true;
}

/******************************************************************** 
 * Top function to create a directory with a given path
 * Allow users to select if use the recursive way or not
 *
 * Strongly recommend to use the recursive way, as it can maximum
 * guarantee the success in creation of directories
 ********************************************************************/
void create_directory(const std::string& dir_path, const bool& recursive) { 
  std::string formatted_dir_path = format_dir_path(dir_path);
  bool status = false;

  if (true == recursive) {
    status = rec_create_dir_path(formatted_dir_path); 
  } else {
    status = create_dir_path(formatted_dir_path, true);
  }

  if (false == status) {
    VTR_LOG_ERROR("Fail to create directory '%s'\n",
                  formatted_dir_path.c_str());
    exit(1);
  }
}

/******************************************************************** 
 * Write a number of space to a file 
 ********************************************************************/
bool write_space_to_file(std::fstream& fp,
                         const size_t& num_space) {
  if (false == valid_file_stream(fp)) {
    return false;
  }

  for (size_t i = 0; i < num_space; ++i) {
    fp << " ";
  }

  return true;
}

/******************************************************************** 
 * Write a number of tab to a file 
 ********************************************************************/
bool write_tab_to_file(std::fstream& fp,
                       const size_t& num_tab) {
  if (false == valid_file_stream(fp)) {
    return false;
  }

  for (size_t i = 0; i < num_tab; ++i) {
    fp << "\t";
  }

  return true;
}

} /* namespace openfpga ends */
