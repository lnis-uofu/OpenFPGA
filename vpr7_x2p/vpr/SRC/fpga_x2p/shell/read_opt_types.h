/**
 * Filename : Options.h
 * Author : Xifan TANG, EPFL
 * Description : Header file contains structs and enumeration 
 *               types for option reading purpose.
 */

/*Determine whether it is a mandatory option*/
enum opt_manda {
  OPT_REQ,
  OPT_OPT
};

/*The option has been appeared in the command line */
enum opt_default {
  OPT_DEF,
  OPT_NONDEF
};

/*Determine whether the option contains a value*/
enum opt_with_val {
  OPT_NONVAL,
  OPT_WITHVAL
};

/*Determine the date type of value*/
enum opt_val_type {
  OPT_INT,
  OPT_FLOAT,
  OPT_CHAR,
  OPT_DOUBLE
};

/*Basic struct stores option information*/
typedef struct s_opt_info t_opt_info;
struct s_opt_info {
  char* tag; /* tag of option */
  char* name; /*The name of option*/
  char* val;  /*The value*/
  enum opt_with_val with_val;
  enum opt_val_type val_type; 
  enum opt_manda mandatory;
  enum opt_default opt_def;
  char* description;
};

typedef struct s_cmd_info t_cmd_info;
struct s_cmd_info {
  char* name;
  t_opt_info opts[];
};

#define HELP_OPT_TAG "help"
#define HELP_OPT_NAME "-h,--help"

#define LAST_OPT_TAG NULL
#define LAST_OPT_NAME NULL

/* Return flag */
#define SHELL_SUCCESS 0
#define SHELL_FAIL 1
#define SHELL_ERROR 2

