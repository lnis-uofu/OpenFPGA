# 
# OpenFPGA cell library Makefile
# ==============================
# 
# Check correctness of the cell library files

SHELL = bash
PYTHON_EXEC ?= python3

# Put it first so that "make" without argument is like "make help".
export COMMENT_EXTRACT

# Put it first so that "make" without argument is like "make help".
help:
	@${PYTHON_EXEC} -c "$$COMMENT_EXTRACT"

compile_verilog:
# This command checks the compile compatibility of Verilog files
	for f in `cat verilog_sources.f`; do iverilog $$f; done 

# Functions to extract comments from Makefiles
define COMMENT_EXTRACT
import re
with open ('Makefile', 'r' ) as f:
    matches = re.finditer('^([a-zA-Z-_]*):.*\n#(.*)', f.read(), flags=re.M)
    for _, match in enumerate(matches, start=1):
        header, content = match[1], match[2]
        print(f"  {header:10} {content}")
endef
