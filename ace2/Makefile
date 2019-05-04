CC = g++
CFLAGS = -g -O0 -w
LFLAGS = -g -O0

OBJ_DIR = OBJ
SRC_DIR = SRC
LIB_DIR = -L.
LIB = -lm 

EXE = ace

ABC_DIR = ../abc_with_bb_support
#ABC_DIR = ../abc
ABC_LIB = libabc.a
ABC_LIB_PATH = $(ABC_DIR)/$(ABC_LIB)

ABC_HEADERS = -I $(ABC_DIR)/src/base/io \
			 -I $(ABC_DIR)/src/base/abc \
			-I $(ABC_DIR)/src/base/main \
			-I $(ABC_DIR)/src/base/cmd \
			-I $(ABC_DIR)/src/misc/vec \
			-I $(ABC_DIR)/src/misc/util \
			-I $(ABC_DIR)/src/misc/st \
			-I $(ABC_DIR)/src/misc/extra \
			-I $(ABC_DIR)/src/misc/ext \
			-I $(ABC_DIR)/src/misc/espresso \
			-I $(ABC_DIR)/src/misc/avl \
			-I $(ABC_DIR)/src/misc/nm \
			-I $(ABC_DIR)/src/aig/aig \
			-I $(ABC_DIR)/src/aig/hop \
			-I $(ABC_DIR)/src/aig/mem \
			-I $(ABC_DIR)/src/aig/tim \
			-I $(ABC_DIR)/src/aig/ssw \
			-I $(ABC_DIR)/src/aig/fra \
			-I $(ABC_DIR)/src/aig/nwk \
			-I $(ABC_DIR)/src/aig/ntl \
			-I $(ABC_DIR)/src/aig/gia \
			-I $(ABC_DIR)/src/aig/dar \
			-I $(ABC_DIR)/src/aig/ioa \
			-I $(ABC_DIR)/src/bdd/cudd \
			-I $(ABC_DIR)/src/bdd/mtr \
			-I $(ABC_DIR)/src/bdd/epd \
			-I $(ABC_DIR)/src/map/if \
			-I $(ABC_DIR)/src/sat/bsat
			
LIBS = -lm -ldl
DEFINES = -DLIN

$(EXE): $(OBJ) Makefile $(ABC_LIB_PATH)
	$(CC) $(OBJ) $(CFLAGS) $(ABC_HEADERS) $(ABC_LIB_PATH) $(LIBS) -o $(EXE)
	
clean:
	rm -f $(EXE) $(OBJ) $(DEP) $(TARGET) $(EXE)

# Enable a second round of expansion so that we may include
# the target directory as a prerequisite of the object file.
.SECONDEXPANSION:

# The directory follows a "|" to use an existence check instead of the usual
# timestamp check.  Every write to the directory updates the timestamp thus
# without this, all but the last file written to a directory would appear
# to be out of date.
$(OBJ): OBJ/%.o:$(SRC)/%.c | $$(dir $$@D)
	$(CC) -c $< $(CFLAGS) -o $@ $(ABC_HEADERS) $(DEFINES) $(LIBS)

# Silently create target directories as need
$(OBJ_DIRS):
	@ mkdir -p $@

-include $(DEPS)

