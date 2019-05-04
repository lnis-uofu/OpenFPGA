CC = g++
CFLAGS = -g -O0 -w
LFLAGS = -g -O0 
INC_FLAGS = -fpermissive

OBJ_DIR = OBJ
SRC_DIR = SRC
LIB_DIR = -L.
LIB = -lm 

EXE = ace

#ABC_DIR = ../abc_with_bb_support
ABC_DIR = ../abc
ABC_LIB = libabc.a
ABC_LIB_PATH = $(ABC_DIR)/$(ABC_LIB)

ABC_HEADERS = -I$(ABC_DIR)/src
			
LIBS = -lm -ldl -lreadline -pthread 
DEFINES = -DLIN

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
OBJ_DIRS=$(sort $(dir $(OBJ)))
DEP := $(OBJ:.o=.d)

$(EXE): $(OBJ) Makefile $(ABC_LIB_PATH)
	$(CC) $(OBJ) $(CFLAGS) $(INC_FLAGS) $(ABC_HEADERS) $(ABC_LIB_PATH) $(LIBS) -o $(EXE)
	
clean:
	rm -f $(EXE) $(OBJ) $(DEP) $(TARGET) $(EXE)

# Enable a second round of expansion so that we may include
# the target directory as a prerequisite of the object file.
.SECONDEXPANSION:

# The directory follows a "|" to use an existence check instead of the usual
# timestamp check.  Every write to the directory updates the timestamp thus
# without this, all but the last file written to a directory would appear
# to be out of date.
$(OBJ): $(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $$(dir $$@D)
	$(CC) -c $< $(CFLAGS) $(INC_FLAGS) -o $@ $(ABC_HEADERS) $(DEFINES) $(LIBS)

# Silently create target directories as need
$(OBJ_DIRS):
	@ mkdir -p $@

-include $(DEPS)

