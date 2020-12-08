# Makefile

ifeq ($(origin CMAKE_COMMAND),undefined)
CMAKE_COMMAND := cmake
else
CMAKE_COMMAND := ${CMAKE_COMMAND}
endif

.PHONY: all env

all: env
	cd build && $(MAKE)

clean:
	rm -rf build

env:
	git submodule init
	git submodule update --init --recursive
	mkdir -p build && cd build && $(CMAKE_COMMAND) ${CMAKE_FLAGS} ..

build/Makefile:
	make env

.PHONY: Makefile

%: build/Makefile
	cd build && $(MAKE) $@
