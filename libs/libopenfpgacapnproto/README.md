Capnproto usage in Openfpga
======================

Capnproto is a data serialization framework designed for portabliity and speed.
In Openfpga, capnproto is used to provide binary formats for internal data
structures that can be computed once, and used many times.  Specific examples:
 - preload unique blocks

What is capnproto?
==================

capnproto can be broken down into 3 parts:
 - A schema language
 - A code generator
 - A library

The schema language is used to define messages.  Each message must have an
explcit capnproto schema, which are stored in files suffixed with ".capnp".
The capnproto documentation for how to write these schema files can be found
here: https://capnproto.org/language.html

The schema by itself is not especially useful.  In order to read and write
messages defined by the schema in a target language (e.g. C++), a code
generation step is required.  Capnproto provides a cmake function for this
purpose, `capnp_generate_cpp`.  This generates C++ source and header files.
These source and header files combined with the capnproto C++ library, enables
C++ code to read and write the messages matching a particular schema.  The C++
library API can be found here: https://capnproto.org/cxx.html

Contents of libopenfpgacapnproto
===========================

libopenfpgacapnproto should contain two elements:
 - Utilities for working capnproto messages in Openfpga
 - Generate source and header files of all capnproto messages used in Openfpga

I/O Utilities
-------------

Capnproto does not provide IO support, instead it works from arrays (or file
descriptors).  To avoid re-writing this code, libopenfpgacapnproto provides two
utilities that should be used whenever reading or writing capnproto message to
disk. These two files are copied :
 - `serdes_utils.h` provides the writeMessageToFile function - Writes a
   capnproto message to disk.
 - `mmap_file.h` provides MmapFile object - Maps a capnproto message from the
   disk as a flat array.

Capnproto schemas
-----------------

libopenfpgacapnproto should contain all capnproto schema definitions used within
Openfpga.  To add a new schema:
1. Add the schema to git in `libs/libopenfpgacapnproto/`
2. Add the schema file name to `capnp_generate_cpp` invocation in
   `libs/libopenfpgacapnproto/CMakeLists.txt`.

The schema will be available in the header file `schema filename>.h`.  The
actual header file will appear in the CMake build directory
`libs/libopenfpgacapnproto` after `libopenfpgacapnproto` has been rebuilt.

Writing capnproto binary files to text
======================================

The `capnp` tool (found in the CMake build directiory
`/vtr-verilog-to-routing/libs/EXTERNAL/capnproto/c++/src/capnp`) can be used to convert from a binary
capnp message to a textual form.

Example converting UniqueBlockCompactInfo from binary to text:

```
capnp convert binary:text unique_blocks_uxsdcxx.capnp UniqueBlockCompactInfo \
  < test.bin > test.txt
```
