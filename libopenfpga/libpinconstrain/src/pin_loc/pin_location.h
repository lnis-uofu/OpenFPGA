#ifndef PIN_LOCATION_H
#define PIN_LOCATION_H
#include "cmd_line.h"

// number of arguments for "pin_c", inlcuding the "pin_c" command itself
#define PIN_C_ARGUMENT_NUMBER 11
class pin_location {
private:
    // error messages to be printed out with std::cerr 
    enum {
        MISSING_IN_OUT_FILES = 0,  
        PIN_LOC_XML_PARSE_ERROR,
        PIN_MAP_CSV_PARSE_ERROR, 
        PIN_CONSTRAINT_PARSE_ERROR, 
        INPUT_DESIGN_PARSE_ERROR,
        CONSTRAINED_PORT_NOT_FOUND,
        CONSTRAINED_PIN_NOT_FOUND,
        RE_CONSTRAINED_PORT,
        OVERLAP_PIN_IN_CONSTRAINT,
        MAX_MESSAGE_ID
    };
    std::string error_messages[MAX_MESSAGE_ID] = {
       "Missing input or output file arguments",    // MISSING_IN_OUT_FILES
       "Pin location file parse error",             // PIN_LOC_XML_PARSE_ERROR 
       "Pin map file parse error",                  // PIN_MAP_CSV_PARSE_ERROR 
       "Pin constraint file parse error",           // PIN_CONSTRAINT_PARSE_ERROR
       "Input design parse error",                  // INPUT_DESIGN_PARSE_ERROR
       "Constrained port not found in design",      // CONSTRAINED_PORT_NOT_FOUND
       "Constrained pin not found in device",       // CONSTRAINED_PIN_NOT_FOUND
       "Re-constrained port",                       // RE_CONSTRAINED_PORT
       "Overlap pin found in constraint"            // OVERLAP_PIN_IN_CONSTRAINT
    };

    cmd_line cl_;

public:
    pin_location(cmd_line& cl): cl_(cl){

    }
    const cmd_line& get_cmd()const;
    bool reader_and_writer(); 

};


#endif
