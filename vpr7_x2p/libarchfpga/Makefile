################################   MAKEFILE OPTIONS     ####################################

# By default, libarchfpga's build type (debug/release) is inherited from VPR's makefile.  
# However, by uncommenting out the line BUILD_TYPE = debug, you can override this 
# and set libarchfpga's build type independently.

# BUILD_TYPE = release

# (can be debug or release)

#############################################################################################

CC = g++
AR = ar

OBJ_DIR = OBJ
SRC_DIR = SRC
LIB_DIR = -L.
LIB = -lm -lpcre -larchfpga

WARN_FLAGS = -Wall -Wpointer-arith -Wcast-qual -D__USE_FIXED_PROTOTYPES__ -pedantic -Wshadow -Wcast-align -D_POSIX_SOURCE -Wno-write-strings
DEBUG_FLAGS = -g
OPT_FLAGS = -O3
INC_FLAGS = -I$(SRC_DIR)/include -I$(SRC_DIR)/fpga_spice_include -I../libprinthandler/SRC/TIO_InputOutputHandlers
LIB_FLAGS = rcs

FLAGS = $(INC_FLAGS) $(WARN_FLAGS) -MD -MP

ifneq (,$(findstring release, $(BUILD_TYPE)))
  FLAGS := $(FLAGS) $(OPT_FLAGS)
else                              # DEBUG build
  FLAGS := $(FLAGS) $(DEBUG_FLAGS)
endif

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
OBJ_DIRS=$(sort $(dir $(OBJ)))
DEPS = $(OBJ:.o=.d) 

EXE = read_arch

# Standalone executable to test architecture reader
$(EXE): $(OBJ) Makefile libarchfpga.a
	$(CC) $(INC_FLAGS) OBJ/main.o -o $(EXE) $(LIB_DIR) $(LIB) 
 

libarchfpga.a: $(OBJ) ../libpcre/libpcre.a ../libprinthandler/libprinthandler.a
	 cp ../libprinthandler/libprinthandler.a $@
	 ar rcs $@ $(OBJ)

../pcre/libpcre.a:
	@ cd ../libpcre && make

../printhandler/libprinthandler.a:
	@ cd ../libprinthandler && make

# Enable a second round of expansion so that we may include
# the target directory as a prerequisite of the object file.
.SECONDEXPANSION:

# The directory follows a "|" to use an existence check instead of the usual
# timestamp check.  Every write to the directory updates the timestamp thus
# without this, all but the last file written to a directory would appear
# to be out of date.
$(OBJ): OBJ/%.o:$(SRC_DIR)/%.c | $$(dir $$@D)
	$(CC) $(FLAGS) $(INC_FLAGS) -c $< -o $@

# Silently create target directories as need
$(OBJ_DIRS):
	@ mkdir -p $@
 
-include $(DEPS)

clean :
	rm -f $(EXE) $(OBJ) $(DEP)
	@ rm -f libarchfpga.a
	@ rm -f read_arch
	@ rm -f main.o main.d

