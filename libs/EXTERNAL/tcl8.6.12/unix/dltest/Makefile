# This Makefile is used to create several test cases for Tcl's load
# command.  It also illustrates how to take advantage of configuration
# exported by Tcl to set up Makefiles for shared libraries.

CC = gcc
LIBS =			-L/home/manadher/work/March/VerificVPR/Raptor/FOEDAG_rs/FOEDAG/third_party/tcl8.6.12/unix -ltclstub8.6 -ldl -lz  -lpthread -lm
AC_FLAGS =		-DPACKAGE_NAME=\"tcl\" -DPACKAGE_TARNAME=\"tcl\" -DPACKAGE_VERSION=\"8.6\" -DPACKAGE_STRING=\"tcl\ 8.6\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_SYS_PARAM_H=1 -DUSE_THREAD_ALLOC=1 -D_REENTRANT=1 -D_THREAD_SAFE=1 -DHAVE_PTHREAD_ATTR_SETSTACKSIZE=1 -DHAVE_PTHREAD_ATFORK=1 -DTCL_THREADS=1 -DTCL_CFGVAL_ENCODING=\"iso8859-1\" -DHAVE_ZLIB=1 -DMODULE_SCOPE=extern\ __attribute__\(\(__visibility__\(\"hidden\"\)\)\) -DHAVE_HIDDEN=1 -DTCL_CFG_DO64BIT=1 -DHAVE_CAST_TO_UNION=1 -DTCL_SHLIB_EXT=\".so\" -DNDEBUG=1 -DTCL_CFG_OPTIMIZED=1 -DTCL_TOMMATH=1 -DMP_PREC=4 -D_LARGEFILE64_SOURCE=1 -DTCL_WIDE_INT_IS_LONG=1 -DHAVE_GETCWD=1 -DHAVE_MKSTEMP=1 -DHAVE_OPENDIR=1 -DHAVE_STRTOL=1 -DHAVE_WAITPID=1 -DHAVE_GETNAMEINFO=1 -DHAVE_GETADDRINFO=1 -DHAVE_FREEADDRINFO=1 -DHAVE_GAI_STRERROR=1 -DHAVE_STRUCT_ADDRINFO=1 -DHAVE_STRUCT_IN6_ADDR=1 -DHAVE_STRUCT_SOCKADDR_IN6=1 -DHAVE_STRUCT_SOCKADDR_STORAGE=1 -DHAVE_GETPWUID_R_5=1 -DHAVE_GETPWUID_R=1 -DHAVE_GETPWNAM_R_5=1 -DHAVE_GETPWNAM_R=1 -DHAVE_GETGRGID_R_5=1 -DHAVE_GETGRGID_R=1 -DHAVE_GETGRNAM_R_5=1 -DHAVE_GETGRNAM_R=1 -DHAVE_DECL_GETHOSTBYNAME_R=1 -DHAVE_GETHOSTBYNAME_R_6=1 -DHAVE_GETHOSTBYNAME_R=1 -DHAVE_DECL_GETHOSTBYADDR_R=1 -DHAVE_GETHOSTBYADDR_R_8=1 -DHAVE_GETHOSTBYADDR_R=1 -DHAVE_TERMIOS_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_TIME_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_GMTIME_R=1 -DHAVE_LOCALTIME_R=1 -DHAVE_MKTIME=1 -DHAVE_TM_GMTOFF=1 -DHAVE_TIMEZONE_VAR=1 -DHAVE_STRUCT_STAT_ST_BLOCKS=1 -DHAVE_STRUCT_STAT_ST_BLKSIZE=1 -DHAVE_BLKCNT_T=1 -DHAVE_INTPTR_T=1 -DHAVE_UINTPTR_T=1 -DNO_UNION_WAIT=1 -DHAVE_SIGNED_CHAR=1 -DHAVE_LANGINFO=1 -DHAVE_MKSTEMPS=1 -DHAVE_FTS=1 -DHAVE_SYS_IOCTL_H=1 -DTCL_UNLOAD_DLLS=1 -DHAVE_CPUID=1
SHLIB_LD =		${CC} ${CFLAGS} ${LDFLAGS} -shared
SHLIB_CFLAGS =		-fPIC
SHLIB_LD_LIBS =		${LIBS}
SHLIB_SUFFIX =		.so
DLTEST_LD =		${SHLIB_LD}
DLTEST_SUFFIX =		
SRC_DIR =		/home/manadher/work/March/VerificVPR/Raptor/FOEDAG_rs/FOEDAG/third_party/tcl8.6.12/unix/dltest
BUILD_DIR =		.
TCL_VERSION=		8.6

CFLAGS_DEBUG		= -g
CFLAGS_OPTIMIZE		= -O2
CFLAGS			= $(CFLAGS_OPTIMIZE)  -pipe -m64 
LDFLAGS_DEBUG		= 
LDFLAGS_OPTIMIZE	= 
LDFLAGS			= $(LDFLAGS_OPTIMIZE)  -Wl,--export-dynamic 

CC_SWITCHES = $(CFLAGS) -I${SRC_DIR}/../../generic -DTCL_MEM_DEBUG \
	${SHLIB_CFLAGS} -DUSE_TCL_STUBS ${AC_FLAGS}

all: pkga${SHLIB_SUFFIX} pkgb${SHLIB_SUFFIX} pkgc${SHLIB_SUFFIX} pkgd${SHLIB_SUFFIX} pkge${SHLIB_SUFFIX} pkgua${SHLIB_SUFFIX} pkgooa${SHLIB_SUFFIX}
	@if test -n "$(DLTEST_SUFFIX)"; then $(MAKE) dltest_suffix; fi
	@touch ../dltest.marker

dltest_suffix: pkga${DLTEST_SUFFIX} pkgb${DLTEST_SUFFIX} pkgc${DLTEST_SUFFIX} pkgd${DLTEST_SUFFIX} pkge${DLTEST_SUFFIX} pkgua${DLTEST_SUFFIX} pkgooa${DLTEST_SUFFIX}
	@touch ../dltest.marker

pkga.o: $(SRC_DIR)/pkga.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkga.c

pkgb.o: $(SRC_DIR)/pkgb.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgb.c

pkgc.o: $(SRC_DIR)/pkgc.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgc.c

pkgd.o: $(SRC_DIR)/pkgd.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgd.c

pkge.o: $(SRC_DIR)/pkge.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkge.c

pkgua.o: $(SRC_DIR)/pkgua.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgua.c

pkgooa.o: $(SRC_DIR)/pkgooa.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgooa.c

pkga${SHLIB_SUFFIX}: pkga.o
	${SHLIB_LD} -o pkga${SHLIB_SUFFIX} pkga.o ${SHLIB_LD_LIBS}

pkgb${SHLIB_SUFFIX}: pkgb.o
	${SHLIB_LD} -o pkgb${SHLIB_SUFFIX} pkgb.o ${SHLIB_LD_LIBS}

pkgc${SHLIB_SUFFIX}: pkgc.o
	${SHLIB_LD} -o pkgc${SHLIB_SUFFIX} pkgc.o ${SHLIB_LD_LIBS}

pkgd${SHLIB_SUFFIX}: pkgd.o
	${SHLIB_LD} -o pkgd${SHLIB_SUFFIX} pkgd.o ${SHLIB_LD_LIBS}

pkge${SHLIB_SUFFIX}: pkge.o
	${SHLIB_LD} -o pkge${SHLIB_SUFFIX} pkge.o ${SHLIB_LD_LIBS}

pkgua${SHLIB_SUFFIX}: pkgua.o
	${SHLIB_LD} -o pkgua${SHLIB_SUFFIX} pkgua.o ${SHLIB_LD_LIBS}

pkgooa${SHLIB_SUFFIX}: pkgooa.o
	${SHLIB_LD} -o pkgooa${SHLIB_SUFFIX} pkgooa.o ${SHLIB_LD_LIBS}

pkga${DLTEST_SUFFIX}: pkga.o
	${DLTEST_LD} -o pkga${DLTEST_SUFFIX} pkga.o ${SHLIB_LD_LIBS}

pkgb${DLTEST_SUFFIX}: pkgb.o
	${DLTEST_LD} -o pkgb${DLTEST_SUFFIX} pkgb.o ${SHLIB_LD_LIBS}

pkgc${DLTEST_SUFFIX}: pkgc.o
	${DLTEST_LD} -o pkgc${DLTEST_SUFFIX} pkgc.o ${SHLIB_LD_LIBS}

pkgd${DLTEST_SUFFIX}: pkgd.o
	${DLTEST_LD} -o pkgd${DLTEST_SUFFIX} pkgd.o ${SHLIB_LD_LIBS}

pkge${DLTEST_SUFFIX}: pkge.o
	${DLTEST_LD} -o pkge${DLTEST_SUFFIX} pkge.o ${SHLIB_LD_LIBS}

pkgua${DLTEST_SUFFIX}: pkgua.o
	${DLTEST_LD} -o pkgua${DLTEST_SUFFIX} pkgua.o ${SHLIB_LD_LIBS}

pkgooa${DLTEST_SUFFIX}: pkgooa.o
	${DLTEST_LD} -o pkgooa${DLTEST_SUFFIX} pkgooa.o ${SHLIB_LD_LIBS}

clean:
	rm -f *.o lib.exp ../dltest.marker
	@if test "$(SHLIB_SUFFIX)" != ""; then \
	    echo "rm -f *${SHLIB_SUFFIX}" ; \
	    rm -f *${SHLIB_SUFFIX} ; \
	fi
	@if test "$(DLTEST_SUFFIX)" != ""; then \
	    echo "rm -f *${DLTEST_SUFFIX}" ; \
	    rm -f *${DLTEST_SUFFIX} ; \
	fi

distclean: clean
	rm -f Makefile
