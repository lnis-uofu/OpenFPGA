//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_user_constants.v"                            ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////      - Tadej Markovic (tadej@opencores.org)                  ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Miha Dolenc, mihad@opencores.org          ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: pci_user_constants.v,v $
// Revision 1.15  2004/08/19 15:27:34  mihad
// Changed minimum pci image size to 256 bytes because
// of some PC system problems with size of IO images.
//
// Revision 1.14  2004/07/07 12:45:01  mihad
// Added SubsystemVendorID, SubsystemID, MAXLatency, MinGnt defines.
// Enabled value loading from serial EEPROM for all of the above + VendorID and DeviceID registers.
//
// Revision 1.13  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
// Revision 1.12  2003/12/28 09:54:48  fr2201
// def_wb_imagex_addr_map  defined correctly
//
// Revision 1.11  2003/12/28 09:20:00  fr2201
// Reset values for PCI, WB defined (PCI_TAx,WB_BAx,WB_TAx,WB_AMx,WB_BAx_MEM_IO)
//
// Revision 1.10  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.9  2003/08/03 18:05:06  mihad
// Added limited WISHBONE B3 support for WISHBONE Slave Unit.
// Doesn't support full speed bursts yet.
//
// Revision 1.8  2003/03/14 15:31:57  mihad
// Entered the option to disable no response counter in wb master.
//
// Revision 1.7  2003/01/27 17:05:50  mihad
// Updated.
//
// Revision 1.6  2003/01/27 16:51:19  mihad
// Old files with wrong names removed.
//
// Revision 1.5  2003/01/21 16:06:56  mihad
// Bug fixes, testcases added.
//
// Revision 1.4  2002/09/30 17:22:45  mihad
// Added support for Virtual Silicon two port RAM. Didn't run regression on it yet!
//
// Revision 1.3  2002/08/13 11:03:53  mihad
// Added a few testcases. Repaired wrong reset value for PCI_AM5 register. Repaired Parity Error Detected bit setting. Changed PCI_AM0 to always enabled(regardles of PCI_AM0 define), if image 0 is used as configuration image
//
// Revision 1.2  2002/03/05 11:53:47  mihad
// Added some testcases, removed un-needed fifo signals
//
// Revision 1.1  2002/02/01 14:43:31  mihad
// *** empty log message ***
//
//

// Fifo implementation defines:
// If FPGA and XILINX are defined, Xilinx's BlockSelectRAM+ is instantiated for Fifo storage.
// 16 bit width is used, so 8 bits of address ( 256 ) locations are available. If RAM_DONT_SHARE is not defined (commented out),
// then one block RAM is shared between two FIFOs. That means each Fifo can have a maximum address length of 7 - depth of 128 and only 6 block rams are used
// If RAM_DONT_SHARE is defined ( not commented out ), then 12 block RAMs are used and each Fifo can have a maximum address length of 8 ( 256 locations )
// If FPGA is not defined, then ASIC RAMs are used. Currently there is only one version of ARTISAN RAM supported. User should generate synchronous RAM with
// width of 40 and instantiate it in pci_tpram.v. If RAM_DONT_SHARE is defined, then these can be dual port rams ( write port
// in one clock domain, read in other ), otherwise it must be two port RAM ( read and write ports in both clock domains ).
// If RAM_DONT_SHARE is defined, then all RAM address lengths must be specified accordingly, otherwise there are two relevant lengths - PCI_FIFO_RAM_ADDR_LENGTH and
// WB_FIFO_RAM_ADDR_LENGTH.

`define WBW_ADDR_LENGTH 4
`define WBR_ADDR_LENGTH 4
`define PCIW_ADDR_LENGTH 3
`define PCIR_ADDR_LENGTH 3

//`define FPGA
//`define XILINX

`define WB_RAM_DONT_SHARE
`define PCI_RAM_DONT_SHARE

`ifdef FPGA
    `ifdef XILINX
        `define PCI_FIFO_RAM_ADDR_LENGTH 8      // PCI target unit fifo storage definition
        `define WB_FIFO_RAM_ADDR_LENGTH 8       // WB slave unit fifo storage definition
        `define PCI_XILINX_RAMB4
        `define WB_XILINX_RAMB4
        //`define PCI_XILINX_DIST_RAM
        //`define WB_XILINX_DIST_RAM
    `endif
`else
    `define PCI_FIFO_RAM_ADDR_LENGTH 3      // PCI target unit fifo storage definition when RAM sharing is used ( both pcir and pciw fifo use same instance of RAM )
    `define WB_FIFO_RAM_ADDR_LENGTH 4       // WB slave unit fifo storage definition when RAM sharing is used ( both wbr and wbw fifo use same instance of RAM )
//    `define WB_ARTISAN_SDP
//    `define PCI_ARTISAN_SDP
//    `define PCI_VS_STP
//    `define WB_VS_STP
`endif

// these two defines allow user to select active high or low output enables on PCI bus signals, depending on
// output buffers instantiated. Xilinx FPGAs use active low output enables.
`define ACTIVE_LOW_OE
//`define ACTIVE_HIGH_OE

// HOST/GUEST implementation selection - see design document and specification for description of each implementation
// only one can be defined at same time
//`define HOST
`define GUEST

// if NO_CNF_IMAGE is commented out, then READ-ONLY access to configuration space is ENABLED:
// - ENABLED Read-Only access from WISHBONE for GUEST bridges
// - ENABLED Read-Only access from PCI for HOST bridges
// with defining NO_CNF_IMAGE, one decoder and one multiplexer are saved
`define NO_CNF_IMAGE

// number defined here specifies how many MS bits in PCI address are compared with base address, to decode
// accesses. Maximum number allows for minimum image size ( number = 20, image size = 4KB ), minimum number
// allows for maximum image size ( number = 1, image size = 2GB ). If you intend on using different sizes of PCI images,
// you have to define a number of minimum sized image and enlarge others by specifying different address mask.
// smaller the number here, faster the decoder operation
`define PCI_NUM_OF_DEC_ADDR_LINES 24

// no. of PCI Target IMAGES
// - PCI provides 6 base address registers for image implementation.
// PCI_IMAGE1 definition is not required and has no effect, since PCI image 1 is always implemented
// If GUEST is defined, PCI Image 0 is also always implemented and is used for configuration space
// access.
// If HOST is defined and NO_CNF_IMAGE is not, then PCI Image 0 is used for Read Only access to configuration
// space. If HOST is defined and NO_CNF_IMAGE is defined, then user can define PCI_IMAGE0 as normal image, and there
// is no access to Configuration space possible from PCI bus.
// Implementation of all other PCI images is selected by defining PCI_IMAGE2 through PCI_IMAGE5 regardles of HOST
// or GUEST implementation.
`ifdef HOST
    `ifdef NO_CNF_IMAGE
        //`define PCI_IMAGE0
    `endif
`endif

//`define PCI_IMAGE2
//`define PCI_IMAGE3
//`define PCI_IMAGE4
//`define PCI_IMAGE5

// initial value for PCI image address masks. Address masks can be defined in enabled state,
// to allow device independent software to detect size of image and map base addresses to
// memory space. If initial mask for an image is defined as 0, then device independent software
// won't detect base address implemented and device dependent software will have to configure
// address masks as well as base addresses!
// Don't define PCI_AMx to 24'hffff_ff for memory images! Use that just for I/O images.
`define PCI_AM0 24'hffff_f0
`define PCI_AM1 24'hffff_ff
`define PCI_AM2 24'hffff_f0
`define PCI_AM3 24'hffff_f0
`define PCI_AM4 24'hffff_f0
`define PCI_AM5 24'hffff_f0

// initial value for PCI image maping to MEMORY or IO spaces.  If initial define is set to 0,
// then IMAGE with that base address points to MEMORY space, othervise it points ti IO space. D
// Device independent software sets the base addresses acording to MEMORY or IO maping!
`define PCI_BA0_MEM_IO 1'b0 // considered only when PCI_IMAGE0 is used as general PCI-WB image!
`define PCI_BA1_MEM_IO 1'b1
`define PCI_BA2_MEM_IO 1'b0
`define PCI_BA3_MEM_IO 1'b0
`define PCI_BA4_MEM_IO 1'b0
`define PCI_BA5_MEM_IO 1'b0

// initial value for PCI translation addresses. The  initial values
// are set after reset. When ADDR_TRAN_IMPL is defined then then Images 
// are transleted to this adresses whithout access to pci_ta registers.
`define PCI_TA0 24'h0000_0
`define PCI_TA1 24'h0000_0
`define PCI_TA2 24'h0000_0
`define PCI_TA3 24'h0000_0
`define PCI_TA4 24'h0000_0
`define PCI_TA5 24'h0000_0

`define PCI_AT_EN0 1'b0
`define PCI_AT_EN1 1'b0
`define PCI_AT_EN2 1'b0
`define PCI_AT_EN3 1'b0
`define PCI_AT_EN4 1'b0
`define PCI_AT_EN5 1'b0

// number defined here specifies how many MS bits in WB address are compared with base address, to decode
// accesses. Maximum number allows for minimum image size ( number = 20, image size = 4KB ), minimum number
// allows for maximum image size ( number = 1, image size = 2GB ). If you intend on using different sizes of WB images,
// you have to define a number of minimum sized image and enlarge others by specifying different address mask.
// smaller the number here, faster the decoder operation
`define WB_NUM_OF_DEC_ADDR_LINES 1

// no. of WISHBONE Slave IMAGES
// WB image 0 is always used for access to configuration space. In case configuration space access is not implemented,
// ( both GUEST and NO_CNF_IMAGE defined ), then WB image 0 is not implemented. User doesn't need to define image 0.
// WB Image 1 is always implemented and user doesnt need to specify its definition
// WB images' 2 through 5 implementation by defining each one.
`define WB_IMAGE2
//`define WB_IMAGE3
//`define WB_IMAGE4
//`define WB_IMAGE5

//Address bar register defines the base address for each image.
//To asccess bus without Software configuration.
`define  WB_BA1	20'h0000_0
`define  WB_BA2	20'h8000_0
`define  WB_BA3	20'h0000_0
`define  WB_BA4	20'h0000_0
`define  WB_BA5	20'h0000_0

// initial value for WB image maping to MEMORY or IO spaces.  If initial define is set to 0,
// then IMAGE with that base address points to MEMORY space, othervise it points ti IO space.
`define  WB_BA1_MEM_IO  1'b0
`define  WB_BA2_MEM_IO  1'b0
`define  WB_BA3_MEM_IO	1'b0
`define  WB_BA4_MEM_IO	1'b0
`define  WB_BA5_MEM_IO	1'b0  

// initial value for WB image address masks. 
`define  WB_AM1 20'h8000_0
`define  WB_AM2 20'h8000_0
`define  WB_AM3 20'h0000_0
`define  WB_AM4 20'h0000_0
`define  WB_AM5 20'h0000_0

// initial value for WB translation addresses. The  initial values
// are set after reset. When ADDR_TRAN_IMPL is defined then then Images 
// are transleted to this adresses whithout access to pci_ta registers.
`define WB_TA1 20'h0000_0
`define WB_TA2 20'h0000_0
`define WB_TA3 20'h0000_0
`define WB_TA4 20'h0000_0
`define WB_TA5 20'h0000_0

`define WB_AT_EN1 1'b0
`define WB_AT_EN2 1'b0
`define WB_AT_EN3 1'b0
`define WB_AT_EN4 1'b0
`define WB_AT_EN5 1'b0

// If this define is commented out, then address translation will not be implemented.
// addresses will pass through bridge unchanged, regardles of address translation enable bits.
// Address translation also slows down the decoding
//When  ADDR_TRAN_IMPL this define is present then adress translation is enabled after reset.
//`define ADDR_TRAN_IMPL

// decode speed for WISHBONE definition - initial cycle on WISHBONE bus will take 1 WS for FAST, 2 WSs for MEDIUM and 3 WSs for slow.
// slower decode speed can be used, to provide enough time for address to be decoded.
`define WB_DECODE_FAST
//`define WB_DECODE_MEDIUM
//`define WB_DECODE_SLOW

// Base address for Configuration space access from WB bus. This value cannot be changed during runtime
`define WB_CONFIGURATION_BASE 20'h0000_0

// Turn registered WISHBONE slave outputs on or off
// all outputs from WB Slave state machine are registered, if this is defined - WB bus outputs as well as
// outputs to internals of the core.
//`define REGISTER_WBS_OUTPUTS

/*-----------------------------------------------------------------------------------------------------------
Core speed definition - used for simulation and 66MHz Capable bit value in status register indicating 66MHz
capable device
-----------------------------------------------------------------------------------------------------------*/
`define PCI33
//`define PCI66

/*-----------------------------------------------------------------------------------------------------------
[000h-00Ch] First 4 DWORDs (32-bit) of PCI configuration header - the same regardless of the HEADER type !
	Vendor_ID is an ID for a specific vendor defined by PCI_SIG - 2321h does not belong to anyone (e.g.
	Xilinx's Vendor_ID is 10EEh and Altera's Vendor_ID is 1172h). Device_ID and Revision_ID should be used
	together by application.
-----------------------------------------------------------------------------------------------------------*/
`define HEADER_VENDOR_ID        16'h1895
`define HEADER_DEVICE_ID        16'h0001
`define HEADER_REVISION_ID      8'h01
`define HEADER_SUBSYS_VENDOR_ID 16'h1895
`define HEADER_SUBSYS_ID        16'h0001
`define HEADER_MAX_LAT          8'h1a
`define HEADER_MIN_GNT          8'h08

// MAX Retry counter value for WISHBONE Master state-machine
// 	This value is 8-bit because of 8-bit retry counter !!!
`define WB_RTY_CNT_MAX			8'hff

// define the macro below to disable internal retry generation in the wishbone master interface
// used when wb master accesses extremly slow devices.
`define PCI_WBM_NO_RESPONSE_CNT_DISABLE

`define PCI_WB_REV_B3
//`define PCI_WBS_B3_RTY_DISABLE

`ifdef GUEST
//    `define PCI_CPCI_HS_IMPLEMENT
//    `define PCI_SPOCI
`endif

