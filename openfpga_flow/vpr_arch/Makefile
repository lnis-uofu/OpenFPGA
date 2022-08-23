# 
# Arch Makefile
# =============
# 
# This makefile is designed to process architecture files
#
SHELL = bash
PYTHON_EXEC ?= python3
ARCH_UPDATER = ../scripts/arch_file_updater.py

.SILENT:

.ONESHELL:

# Put it first so that "make" without argument is like "make help".
export COMMENT_EXTRACT

# Put it first so that "make" without argument is like "make help".
help:
	@${PYTHON_EXEC} -c "$$COMMENT_EXTRACT"

v1p1_to_v1p2:
# This commands will update all the architecture file from v1.1 to v1.2
	for f in `ls *.xml`; \
	do ${PYTHON_EXEC} ${ARCH_UPDATER} --input_file $${f} --output_file $${f} || exit 1; \
	done;

# Functions to extract comments from Makefiles
define COMMENT_EXTRACT
import re
with open ('Makefile', 'r' ) as f:
    matches = re.finditer('^([a-zA-Z-_]*):.*\n#(.*)', f.read(), flags=re.M)
    for _, match in enumerate(matches, start=1):
        header, content = match[1], match[2]
        print(f"  {header:10} {content}")
endef
