/////////////////////////////////////////////////////////////////////
////                                                             ////
////  DES CORE                                                   ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2001 Rudolf Usselmann                         ////
////                    rudi@asics.ws                            ////
////                                                             ////
//// This source file may be used and distributed without        ////
//// restriction provided that this copyright statement is not   ////
//// removed from the file and that any derivative work contains ////
//// the original copyright notice and the associated disclaimer.////
////                                                             ////
////     THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY     ////
//// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   ////
//// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   ////
//// FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR      ////
//// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         ////
//// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    ////
//// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   ////
//// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        ////
//// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  ////
//// LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT  ////
//// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  ////
//// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         ////
//// POSSIBILITY OF SUCH DAMAGE.                                 ////
////                                                             ////
/////////////////////////////////////////////////////////////////////



Triple DES Core
===============
Attached is a Triple DES core implementation in verilog. It takes
three standard 56 bit keys and 64 bits of data as input and generates
a 64 bit encrypted/decrypted result. Two implementations are provided:

1) Area Optimized (CBC Mode)
   This is a sequential implementation and needs 48 cycles to complete
   a full encryption/decryption cycle.


2) Performance Optimized (EBC Mode)
   This is a pipelined implementation that has a 48 cycle pipeline
   (plus 1 input and 1 output register). It can perform a complete
   encryption/decryption every cycle.

Performance
===========
1) Area Optimized (CBC Mode)
   0.18u UMC ASIC process: 5.5K gates, > 160 Mhz
   Spartan IIe 100-6 : 1450 LUTs (about 60%), 88MHz

2) Performance Optimized (EBC Mode)
   0.18u UMC ASIC process: 55K Gates, 300MHz (19.2 Gbits/sec)
   Virtex-II-1500-6: 79% utilization, 166Mhz (10.6 Gbits/sec)



DES Core
========
Attached is a DES core implementation in verilog. It takes a standard
56 bit key and 64 bits of data as input and generates a 64 bit 
encrypted/decrypted result. Two implementations are provided:

1) Area Optimized (CBC Mode)
   This is a sequential implementation and needs 16 cycles to complete
   a full encryption/decryption cycle.


2) Performance Optimized (EBC Mode)
   This is a pipelined implementation that has a 16 cycle pipeline
   (plus 1 input and 1 output register). It can perform a complete
   encryption/decryption every cycle.


Performance
===========
1) Area Optimized (CBC Mode)
   0.18u UMC ASIC process: >155Mhz 3K Gates
   Altera APEX 20KE-1: 1106 lcells >27MHz
   Altera FLEX 10K50E-1: 1283 lcells >43MHz

2) Performance Optimized (EBC Mode)
   0.18u UMC ASIC process: >290Mhz 28K Gates
   Altera APEX 20KE-1: 6688 lcells >53MHz
   Altera FLEX 10K130E-1: 6485 lcells >76 Mhz



Status
======
31-Oct-2002	Added Triple DES
05-Oct-2001	Added decrypt input (Thanks to Mark Cynar for
                providing the code)
		Reorganized directory structure
		Added Makefile
		Cleaned up test benches
03-Feb-2001	Initial Release

 
Directory Structure
===================
[core_root]
 |
 +-doc                        Documentation
 |
 +-bench--+                   Test Bench
 |        +- verilog          Verilog Sources
 |        +-vhdl              VHDL Sources
 |
 +-rtl----+                   Core RTL Sources
 |        +-verilog           Verilog Sources
 |        +-vhdl              VHDL Sources
 |
 +-sim----+
 |        +-rtl_sim---+       Functional verification Directory
 |        |           +-bin   Makefiles/Run Scripts
 |        |           +-run   Working Directory
 |        |
 |        +-gate_sim--+       Functional & Timing Gate Level
 |                    |       Verification Directory
 |                    +-bin   Makefiles/Run Scripts
 |                    +-run   Working Directory
 |
 +-lint--+                    Lint Directory Tree
 |       +-bin                Makefiles/Run Scripts
 |       +-run                Working Directory
 |       +-log                Linter log & result files
 |
 +-syn---+                    Synthesis Directory Tree
 |       +-bin                Synthesis Scripts
 |       +-run                Working Directory
 |       +-log                Synthesis log files
 |       +-out                Synthesis Output


About the Author
================
To find out more about me (Rudolf Usselmann), please visit:
http://www.asics.ws
