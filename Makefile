# Makefile

ifeq ($(origin CMAKE_COMMAND),undefined)
CMAKE_COMMAND := cmake
else
CMAKE_COMMAND := ${CMAKE_COMMAND}
endif

.PHONY: all checkout compile

all: checkout
	mkdir -p build && cd build && $(CMAKE_COMMAND) ${CMAKE_FLAGS} ..
	cd build && $(MAKE)

checkout: 
	git submodule init
	git submodule update --init --recursive

compile:
	mkdir -p build && cd build && $(CMAKE_COMMAND) ${CMAKE_FLAGS} ..
	cd build && $(MAKE)

check_tclshrc:
	@touch ~/.tclshrc
	@[[ ! `grep "package require OpenFPGA" ~/.tclshrc` ]] && echo "if { [ file exists ~/.openfpga_tcl_check ] } { package require OpenFPGA }" >> ~/.tclshrc || echo " "

tcl_mode: check_tclshrc
	@touch ~/.openfpga_tcl_check
	@tclsh8.6 $(TCL_FILE)
	@rm ~/.openfpga_tcl_check

clean:
	rm -rf build yosys/install

build/Makefile:
	make checkout

.PHONY: Makefile

%: build/Makefile
	cd build && $(MAKE) $@
