//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_conf_space.v                                 ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - tadej@opencores.org                                   ////
////      - Tadej Markovic                                        ////
////                                                              ////
////  All additional information is avaliable in the README.txt   ////
////  file.                                                       ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Tadej Markovic, tadej@opencores.org       ////
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
// $Log: pci_conf_space.v,v $
// Revision 1.10  2004/08/19 16:04:53  mihad
// Removed some unused signals.
//
// Revision 1.9  2004/08/19 15:27:34  mihad
// Changed minimum pci image size to 256 bytes because
// of some PC system problems with size of IO images.
//
// Revision 1.8  2004/07/07 12:45:01  mihad
// Added SubsystemVendorID, SubsystemID, MAXLatency, MinGnt defines.
// Enabled value loading from serial EEPROM for all of the above + VendorID and DeviceID registers.
//
// Revision 1.7  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
// Revision 1.6  2003/12/28 09:54:48  fr2201
// def_wb_imagex_addr_map  defined correctly
//
// Revision 1.5  2003/12/28 09:20:00  fr2201
// Reset values for PCI, WB defined (PCI_TAx,WB_BAx,WB_TAx,WB_AMx,WB_BAx_MEM_IO)
//
// Revision 1.4  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.3  2003/08/14 13:06:02  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.2  2003/03/26 13:16:18  mihad
// Added the reset value parameter to the synchronizer flop module.
// Added resets to all synchronizer flop instances.
// Repaired initial sync value in fifos.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.4  2002/08/13 11:03:53  mihad
// Added a few testcases. Repaired wrong reset value for PCI_AM5 register. Repaired Parity Error Detected bit setting. Changed PCI_AM0 to always enabled(regardles of PCI_AM0 define), if image 0 is used as configuration image
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:28  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//
//

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

/*-----------------------------------------------------------------------------------------------------------
	w_ prefix is a sign for Write (and read) side of Dual-Port registers
	r_ prefix is a sign for Read only side of Dual-Port registers
In the first line there are DATA and ADDRESS ports, in the second line there are write enable and read
enable signals with chip-select (conf_hit) for config. space.
In the third line there are output signlas from Command register of the PCI configuration header !!!
In the fourth line there are input signals to Status register of the PCI configuration header !!!
In the fifth line there is output from Latency Timer & r_Interrupt pin registers of the PCI conf. header !!!
Following are IMAGE specific registers, from which PCI_BASE_ADDR registers are the same as base address
registers from the PCI conf. header !!!
-----------------------------------------------------------------------------------------------------------*/
					// normal R/W address, data and control
module pci_conf_space 
                (	w_conf_address_in, w_conf_data_in, w_conf_data_out, r_conf_address_in, r_conf_data_out,
					w_we_i, w_re, r_re, w_byte_en_in, w_clock, reset, pci_clk, wb_clk,
					// outputs from command register of the PCI header
					serr_enable, perr_response, pci_master_enable, memory_space_enable, io_space_enable,
					// inputs to status register of the PCI header
					perr_in, serr_in, master_abort_recv, target_abort_recv, target_abort_set, master_data_par_err,
					// output from cache_line_size, latency timer and r_interrupt_pin register of the PCI header
					cache_line_size_to_pci, cache_line_size_to_wb, cache_lsize_not_zero_to_wb,
					latency_tim,
					// output from all pci IMAGE registers
					pci_base_addr0, pci_base_addr1, pci_base_addr2, pci_base_addr3, pci_base_addr4, pci_base_addr5,
					pci_memory_io0, pci_memory_io1, pci_memory_io2, pci_memory_io3, pci_memory_io4, pci_memory_io5,
					pci_addr_mask0, pci_addr_mask1, pci_addr_mask2, pci_addr_mask3, pci_addr_mask4, pci_addr_mask5,
					pci_tran_addr0, pci_tran_addr1, pci_tran_addr2, pci_tran_addr3, pci_tran_addr4, pci_tran_addr5,
					pci_img_ctrl0,  pci_img_ctrl1,  pci_img_ctrl2,  pci_img_ctrl3,  pci_img_ctrl4,  pci_img_ctrl5,
					// input to pci error control and status register, error address and error data registers
					pci_error_be, pci_error_bc, pci_error_rty_exp, pci_error_es, pci_error_sig, pci_error_addr,
					pci_error_data,
					// output from all wishbone IMAGE registers
					wb_base_addr0, wb_base_addr1, wb_base_addr2, wb_base_addr3, wb_base_addr4, wb_base_addr5,
					wb_memory_io0, wb_memory_io1, wb_memory_io2, wb_memory_io3, wb_memory_io4, wb_memory_io5,
					wb_addr_mask0, wb_addr_mask1, wb_addr_mask2, wb_addr_mask3, wb_addr_mask4, wb_addr_mask5,
					wb_tran_addr0, wb_tran_addr1, wb_tran_addr2, wb_tran_addr3, wb_tran_addr4, wb_tran_addr5,
					wb_img_ctrl0,  wb_img_ctrl1,  wb_img_ctrl2,  wb_img_ctrl3,  wb_img_ctrl4,  wb_img_ctrl5,
					// input to wb error control and status register, error address and error data registers
					wb_error_be, wb_error_bc, wb_error_rty_exp, wb_error_es, wb_error_sig, wb_error_addr, wb_error_data,
					// output from conf. cycle generation register (sddress), int. control register & interrupt output
					config_addr, icr_soft_res, int_out,
					// input to interrupt status register
					isr_sys_err_int, isr_par_err_int, isr_int_prop,

                    pci_init_complete_out, wb_init_complete_out

                `ifdef PCI_CPCI_HS_IMPLEMENT
                    ,
                    pci_cpci_hs_enum_oe_o, pci_cpci_hs_led_oe_o, pci_cpci_hs_es_i
                `endif

                `ifdef PCI_SPOCI
                    ,
                    spoci_scl_oe_o, spoci_sda_i, spoci_sda_oe_o
                `endif
                ) ;


/*###########################################################################################################
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Input and output ports
	======================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
###########################################################################################################*/

// output data
output	[31 : 0]				w_conf_data_out ;
output	[31 : 0]				r_conf_data_out ;
reg		[31 : 0]				w_conf_data_out ;

`ifdef	NO_CNF_IMAGE
`else
reg		[31 : 0]				r_conf_data_out ;
`endif

// input data
input	[31 : 0]				w_conf_data_in ;
wire	[31 : 0]				w_conf_pdata_reduced ; // reduced data written into PCI image registers
wire	[31 : 0]				w_conf_wdata_reduced ; // reduced data written into WB  image registers
// input address
input	[11 : 0]				w_conf_address_in ;
input	[11 : 0]				r_conf_address_in ;
// input control signals
input							w_we_i ;
input							w_re   ;
input							r_re   ;
input	[3 : 0]					w_byte_en_in ;
input							w_clock ;
input							reset ;
input							pci_clk ;
input							wb_clk ;
// PCI header outputs from command register
output							serr_enable ;
output							perr_response ;
output							pci_master_enable ;
output							memory_space_enable ;
output							io_space_enable ;
// PCI header inputs to status register
input							perr_in ;
input							serr_in ;
input							master_abort_recv ;
input							target_abort_recv ;
input							target_abort_set ;
input							master_data_par_err ;
// PCI header output from cache_line_size, latency timer and interrupt pin
output	[7 : 0]					cache_line_size_to_pci ; // sinchronized to PCI clock
output	[7 : 0]					cache_line_size_to_wb ;  // sinchronized to WB clock
output							cache_lsize_not_zero_to_wb ; // used in WBU and PCIU
output	[7 : 0]					latency_tim ;
//output	[2 : 0]					int_pin ; // only 3 LSbits are important!
// PCI output from image registers
`ifdef GUEST
    output	[31:12] pci_base_addr0 ;
`endif

`ifdef HOST
    `ifdef NO_CNF_IMAGE
        output	[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_base_addr0 ;
    `else
        output	[31:12] pci_base_addr0 ;
    `endif
`endif

output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_base_addr1 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_base_addr2 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_base_addr3 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_base_addr4 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_base_addr5 ;
output							pci_memory_io0 ;
output							pci_memory_io1 ;
output							pci_memory_io2 ;
output							pci_memory_io3 ;
output							pci_memory_io4 ;
output							pci_memory_io5 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_addr_mask0 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_addr_mask1 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_addr_mask2 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_addr_mask3 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_addr_mask4 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_addr_mask5 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_tran_addr0 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_tran_addr1 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_tran_addr2 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_tran_addr3 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_tran_addr4 ;
output  [31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] pci_tran_addr5 ;
output  [2 : 1]                 pci_img_ctrl0 ;
output  [2 : 1]                 pci_img_ctrl1 ;
output  [2 : 1]                 pci_img_ctrl2 ;
output  [2 : 1]                 pci_img_ctrl3 ;
output  [2 : 1]                 pci_img_ctrl4 ;
output  [2 : 1]                 pci_img_ctrl5 ;
// PCI input to pci error control and status register, error address and error data registers
input	[3 : 0]					pci_error_be ;
input   [3 : 0]                 pci_error_bc ;
input                           pci_error_rty_exp ;
input							pci_error_es ;
input                           pci_error_sig ;
input   [31 : 0]                pci_error_addr ;
input   [31 : 0]                pci_error_data ;
// WISHBONE output from image registers
output	[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_base_addr0 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_base_addr1 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_base_addr2 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_base_addr3 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_base_addr4 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_base_addr5 ;
output							wb_memory_io0 ;
output							wb_memory_io1 ;
output							wb_memory_io2 ;
output							wb_memory_io3 ;
output							wb_memory_io4 ;
output							wb_memory_io5 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_addr_mask0 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_addr_mask1 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_addr_mask2 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_addr_mask3 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_addr_mask4 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_addr_mask5 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_tran_addr0 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_tran_addr1 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_tran_addr2 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_tran_addr3 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_tran_addr4 ;
output  [31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] wb_tran_addr5 ;
output  [2 : 0]                 wb_img_ctrl0 ;
output  [2 : 0]                 wb_img_ctrl1 ;
output  [2 : 0]                 wb_img_ctrl2 ;
output  [2 : 0]                 wb_img_ctrl3 ;
output  [2 : 0]                 wb_img_ctrl4 ;
output  [2 : 0]                 wb_img_ctrl5 ;
// WISHBONE input to wb error control and status register, error address and error data registers
input	[3 : 0] 				wb_error_be ;
input   [3 : 0]	                wb_error_bc ;
input   		                wb_error_rty_exp ;
input                           wb_error_es ;
input                           wb_error_sig ;
input   [31 : 0]                wb_error_addr ;
input   [31 : 0]                wb_error_data ;
// GENERAL output from conf. cycle generation register & int. control register
output	[23 : 0]				config_addr ;
output                          icr_soft_res ;
output							int_out ;
// GENERAL input to interrupt status register
input                           isr_sys_err_int ;
input                           isr_par_err_int ;
input							isr_int_prop ;

output                          pci_init_complete_out ;
output                          wb_init_complete_out  ;

`ifdef PCI_CPCI_HS_IMPLEMENT
output  pci_cpci_hs_enum_oe_o   ; 
output  pci_cpci_hs_led_oe_o    ; 
input   pci_cpci_hs_es_i        ;

reg pci_cpci_hs_enum_oe_o   ; 
reg pci_cpci_hs_led_oe_o    ; 

// set the hot swap ejector switch debounce counter width
// it is only 4 for simulation purposes
`ifdef PCI_CPCI_SIM

    parameter hs_es_cnt_width = 4  ;

`else

    `ifdef PCI33
    
    parameter hs_es_cnt_width = 16 ;
    
    `endif
    
    `ifdef PCI66
    
    parameter hs_es_cnt_width = 17 ;
    
    `endif
`endif

`endif

`ifdef PCI_SPOCI
output  spoci_scl_oe_o  ;
input   spoci_sda_i     ;
output  spoci_sda_oe_o  ;

reg spoci_cs_nack,
    spoci_cs_write,
    spoci_cs_read;

reg [10: 0] spoci_cs_adr   ; 
reg [ 7: 0] spoci_cs_dat   ;
`endif

/*###########################################################################################################
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	REGISTERS definition
	====================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
###########################################################################################################*/

// Decoded Register Select signals for writting (only one address decoder)
reg		[56 : 0]				w_reg_select_dec ;

/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------
PCI CONFIGURATION SPACE HEADER (type 00h) registers

	BIST and some other registers are not implemented and therefor written in correct
	place with comment line. There are also some registers with NOT all bits implemented and therefor uses
	_bitX or _bitX2_X1 to sign which bit or range of bits are implemented.
	Some special cases and examples are described below!
-------------------------------------------------------------------------------------------------------------
###########################################################################################################*/

/*-----------------------------------------------------------------------------------------------------------
[000h-00Ch] First 4 DWORDs (32-bit) of PCI configuration header - the same regardless of the HEADER type !
			r_ prefix is a sign for read only registers
	Vendor_ID is an ID for a specific vendor defined by PCI_SIG - 2321h does not belong to anyone (e.g.
	Xilinx's Vendor_ID is 10EEh and Altera's Vendor_ID is 1172h). Device_ID and Revision_ID should be used
	together by application. Class_Code has 3 bytes to define BASE class (06h for PCI Bridge), SUB class
	(00h for HOST type, 80h for Other Bridge type) and Interface type (00h for normal).
-----------------------------------------------------------------------------------------------------------*/
            reg [15: 0] r_vendor_id         ;
            reg [15: 0] r_device_id         ;
            reg [15: 0] r_subsys_vendor_id  ;
            reg [15: 0] r_subsys_id         ;

			reg					command_bit8 ;
			reg					command_bit6 ;
			reg		[2 : 0]		command_bit2_0 ;
			reg		[15 : 11]	status_bit15_11 ;
			parameter			r_status_bit10_9 = 2'b01 ;	// 2'b01 means MEDIUM devsel timing !!!
			reg					status_bit8 ;
			parameter			r_status_bit7 = 1'b1 ; // xfast back-to-back capable response !!!
			parameter			r_status_bit5 = `HEADER_66MHz ;   	// 1'b0 indicates 33 MHz capable !!!

`ifdef PCI_CPCI_HS_IMPLEMENT
            wire                r_status_bit4 = 1   ;
            reg                 hs_ins              ;
            reg                 hs_ext              ;
            wire    [ 1: 0]     hs_pi = 2'b00       ;
            reg                 hs_loo              ;
            reg                 hs_eim              ;
            wire    [ 7: 0]     hs_cap_id = 8'h06   ;
            reg                 hs_ins_armed        ;
            reg                 hs_ext_armed        ;
`else
            wire                r_status_bit4 = 0 ;
`endif

            reg     [ 7: 0]     r_revision_id   ;

`ifdef		HOST
			parameter			r_class_code = 24'h06_00_00 ;
`else
			parameter			r_class_code = 24'h06_80_00 ;
`endif
			reg		[7 : 0]		cache_line_size_reg	;
			reg		[7 : 0]		latency_timer ;
			parameter			r_header_type = 8'h00 ;
			// REG				bist							NOT implemented !!!

/*-----------------------------------------------------------------------------------------------------------
[010h-03Ch] all other DWORDs (32-bit) of PCI configuration header - only for HEADER type 00h !
			r_ prefix is a sign for read only registers
	BASE_ADDRESS_REGISTERS are the same as ones in the PCI Target configuration registers section. They
	are duplicated and therefor defined just ones and used with the same name as written below. If
	IMAGEx is NOT defined there is only parameter image_X assigned to '0' and this parameter is used
	elsewhere in the code. This parameter is defined in the INTERNAL SIGNALS part !!!
	Interrupt_Pin value 8'h01 is used for INT_A pin used.
	MIN_GNT and MAX_LAT are used for device's desired values for Latency Timer value. The value in boath
	registers specifies a period of time in units of 1/4 microsecond. ZERO indicates that there are no
	major requirements for the settings of Latency Timer.
	MIN_GNT specifieshow how long a burst period the device needs at 33MHz. MAX_LAT specifies how often
	the device needs to gain access to the PCI bus. Values are choosen assuming that the target does not
	insert any wait states. Follow the expamle of settings for simple display card.
	If we use 64 (32-bit) FIFO locations for one burst then we need 8 x 1/4 microsecond periods at 33MHz
	clock rate => MIN_GNT = 08h ! Resolution is 1024 x 768 (= 786432 pixels for one frame) with 16-bit
	color mode. We can transfere 2 16-bit pixels in one FIFO location. From that we calculate, that for
	one frame we need 6144 burst transferes in 1/25 second. So we need one burst every 6,51 microsecond
	and that is 26 x 1/4 microsecond or 1Ah x 1/4 microsecond => MAX_LAT = 1Ah !
-----------------------------------------------------------------------------------------------------------*/
			// REG x 6		base_address_register_X			IMPLEMENTED as		pci_ba_X !!!
			// REG			r_cardbus_cis_pointer			NOT implemented !!!
			// REG			r_subsystem_vendor_id			NOT implemented !!!
			// REG			r_subsystem_id					NOT implemented !!!
			// REG			r_expansion_rom_base_address	NOT implemented !!!
			// REG			r_cap_list_pointer				NOT implemented !!!
			reg		[7 : 0]	interrupt_line ;
			parameter		r_interrupt_pin = 8'h01 ;
			reg     [7 : 0] r_min_gnt   ;
            reg     [7 : 0] r_max_lat   ;

/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------
PCI Target configuration registers
	There are also some registers with NOT all bits implemented and therefor uses _bitX or _bitX2_X1 to
	sign which bit or range of bits are implemented. Some special cases and examples are described below!
-------------------------------------------------------------------------------------------------------------
###########################################################################################################*/

/*-----------------------------------------------------------------------------------------------------------
[100h-168h]
	Depending on defines (PCI_IMAGE1 or .. or PCI_IMAGE5 or (PCI_IMAGE0 and HOST)) in constants.v file,
	there are registers corresponding to each IMAGE defined to REG and parameter pci_image_X assigned to '1'.
	The maximum number of images is "6". By default there are first two images used and the first (PCI_IMAGE0)
	is assigned to Configuration space! With a 'define' PCI_IMAGEx you choose the number of used PCI IMAGES
	in a bridge without PCI_IMAGE0 (e.g. PCI_IMAGE3 tells, that PCI_IMAGE1, PCI_IMAGE2 and PCI_IMAGE3 are
	used for mapping the space from WB to PCI. Offcourse, PCI_IMAGE0 is assigned to Configuration space).
	That leave us PCI_IMAGE5 as the maximum number of images.
	There is one exeption, when the core is implemented as HOST. If so, then the PCI specification allowes
	the Configuration space NOT to be visible on the PCI bus. With `define PCI_IMAGE0 (and `define HOST), we
	assign PCI_IMAGE0 to normal WB to PCI image and not to configuration space!

	When error occurs, PCI ERR_ADDR and ERR_DATA registers stores address and data on the bus that
	caused error. While ERR_CS register stores Byte Enables and Bus Command in the MSByte. In bits 10
	and 8 it reports Retry Counter Expired (for posted writes), Error Source (Master Abort) and Error
	Report Signal (signals that error has occured) respectively. With bit 0 we enable Error Reporting
	mechanism.
-----------------------------------------------------------------------------------------------------------*/
`ifdef		HOST
	`ifdef	NO_CNF_IMAGE
		`ifdef	PCI_IMAGE0	// if PCI bridge is HOST and IMAGE0 is assigned as general image space
			reg		[31 : 8]	pci_ba0_bit31_8 ;
			reg		[2 : 1]		pci_img_ctrl0_bit2_1 ;
			reg					pci_ba0_bit0 ;
			reg		[31 : 8]	pci_am0 ;
			reg		[31 : 8]	pci_ta0 ;
		`else // if PCI bridge is HOST and IMAGE0 is not used
			wire	[31 : 8]	pci_ba0_bit31_8 = 24'h0000_00 ; // NO base address needed
			wire	[2 : 1]		pci_img_ctrl0_bit2_1 = 2'b00 ; // NO addr.transl. and pre-fetch
			wire				pci_ba0_bit0 = 0 ; // config. space is MEMORY space
			wire	[31 : 8]	pci_am0 = 24'h0000_00 ; // NO address mask needed
			wire	[31 : 8]	pci_ta0 = 24'h0000_00 ; // NO address translation needed
		`endif
	`else // if PCI bridge is HOST and IMAGE0 is assigned to PCI configuration space
			reg		[31 : 8]	pci_ba0_bit31_8 ;
			wire	[2 : 1]		pci_img_ctrl0_bit2_1 = 2'b00 ; // NO pre-fetch and read line support
			wire				pci_ba0_bit0 = 0 ; // config. space is MEMORY space
			wire	[31 : 8]	pci_am0 = 24'hFFFF_F0 ; // address mask for configuration image always 20'hffff_f
			wire	[31 : 8]	pci_ta0 = 24'h0000_00 ; // NO address translation needed
	`endif
`endif

`ifdef GUEST // if PCI bridge is GUEST, then IMAGE0 is assigned to PCI configuration space
			reg		[31 : 8]	pci_ba0_bit31_8 ;
			wire	[2 : 1]		pci_img_ctrl0_bit2_1 = 2'b00 ; // NO addr.transl. and pre-fetch
			wire				pci_ba0_bit0 = 0 ; // config. space is MEMORY space
			wire	[31 : 8]	pci_am0 = 24'hffff_f0 ; // address mask for configuration image always 24'hffff_f0 - 4KB mem image
			wire	[31 : 8]	pci_ta0 = 24'h0000_00 ; // NO address translation needed
`endif

// IMAGE1 is included by default, meanwhile other IMAGEs are optional !!!
			reg		[2 : 1]		pci_img_ctrl1_bit2_1 ;
			reg		[31 : 8]	pci_ba1_bit31_8 ;
	`ifdef	HOST
			reg					pci_ba1_bit0 ;
	`else
			wire				pci_ba1_bit0 = `PCI_BA1_MEM_IO ;
	`endif
			reg		[31 :  8]	pci_am1 ;
			reg		[31 :  8]	pci_ta1 ;
`ifdef		PCI_IMAGE2
			reg		[2 : 1]		pci_img_ctrl2_bit2_1 ;
			reg		[31 : 8]	pci_ba2_bit31_8 ;
	`ifdef	HOST
			reg					pci_ba2_bit0 ;
	`else
			wire				pci_ba2_bit0 = `PCI_BA2_MEM_IO ;
	`endif
			reg		[31 :  8]	pci_am2 ;
			reg		[31 :  8]	pci_ta2 ;
`else
            wire	[2 : 1]		pci_img_ctrl2_bit2_1 = 2'b00 ;
			wire	[31 :  8]	pci_ba2_bit31_8 = 24'h0000_00 ;
            wire				pci_ba2_bit0 = 1'b0 ;
            wire	[31 :  8]	pci_am2 = 24'h0000_00 ;
            wire	[31 :  8]	pci_ta2 = 24'h0000_00 ;
`endif
`ifdef		PCI_IMAGE3
			reg		[2 : 1]		pci_img_ctrl3_bit2_1 ;
			reg		[31 :  8]	pci_ba3_bit31_8 ;
	`ifdef	HOST
			reg					pci_ba3_bit0 ;
	`else
			wire				pci_ba3_bit0 = `PCI_BA3_MEM_IO ;
	`endif
			reg		[31 :  8]	pci_am3 ;
			reg		[31 :  8]	pci_ta3 ;
`else
            wire	[2 : 1]		pci_img_ctrl3_bit2_1 = 2'b00 ;
			wire	[31 :  8]	pci_ba3_bit31_8 = 24'h0000_00 ;
            wire				pci_ba3_bit0 = 1'b0 ;
            wire	[31 :  8]	pci_am3 = 24'h0000_00 ;
            wire	[31 :  8]	pci_ta3 = 24'h0000_00 ;
`endif
`ifdef		PCI_IMAGE4
			reg		[2 : 1]		pci_img_ctrl4_bit2_1 ;
			reg		[31 :  8]	pci_ba4_bit31_8 ;
	`ifdef	HOST
			reg					pci_ba4_bit0 ;
	`else
			wire				pci_ba4_bit0 = `PCI_BA4_MEM_IO ;
	`endif
			reg		[31 :  8]	pci_am4 ;
			reg		[31 :  8]	pci_ta4 ;
`else
            wire	[2 : 1]		pci_img_ctrl4_bit2_1 = 2'b00 ;
			wire	[31 :  8]	pci_ba4_bit31_8 = 24'h0000_00 ;
            wire				pci_ba4_bit0 = 1'b0 ;
            wire	[31 :  8]	pci_am4 = 24'h0000_00 ;
            wire	[31 :  8]	pci_ta4 = 24'h0000_00 ;
`endif
`ifdef		PCI_IMAGE5
			reg		[2 : 1]		pci_img_ctrl5_bit2_1 ;
			reg		[31 :  8]	pci_ba5_bit31_8 ;
	`ifdef	HOST
			reg					pci_ba5_bit0 ;
	`else
			wire				pci_ba5_bit0 = `PCI_BA5_MEM_IO ;
	`endif
			reg		[31 :  8]	pci_am5 ;
			reg		[31 :  8]	pci_ta5 ;
`else
            wire	[2 : 1]		pci_img_ctrl5_bit2_1 = 2'b00 ;
			wire	[31 :  8]	pci_ba5_bit31_8 = 24'h0000_00 ;
            wire				pci_ba5_bit0 = 1'b0 ;
            wire	[31 :  8]	pci_am5 = 24'h0000_00 ;
            wire	[31 :  8]	pci_ta5 = 24'h0000_00 ;
`endif
			reg		[31 : 24]	pci_err_cs_bit31_24 ;
			reg					pci_err_cs_bit10 ;
			reg					pci_err_cs_bit9 ;
			reg					pci_err_cs_bit8 ;
			reg					pci_err_cs_bit0 ;
			reg		[31 : 0]	pci_err_addr ;
			reg		[31 : 0]	pci_err_data ;


/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------
WISHBONE Slave configuration registers
	There are also some registers with NOT all bits implemented and therefor uses _bitX or _bitX2_X1 to
	sign which bit or range of bits are implemented. Some special cases and examples are described below!
-------------------------------------------------------------------------------------------------------------
###########################################################################################################*/

/*-----------------------------------------------------------------------------------------------------------
[800h-85Ch]
	Depending on defines (WB_IMAGE1 or .. or WB_IMAGE4 or WB_IMAGE5) in constants.v file, there are
	registers corresponding to each IMAGE defined to REG and parameter wb_image_X assigned to '1'.
	The maximum number of images is "6". By default there are first two images used and the first (WB_IMAGE0)
	is assigned to Configuration space! With a 'define' WB_IMAGEx you choose the number of used WB IMAGES in
	a bridge without WB_IMAGE0 (e.g. WB_IMAGE3 tells, that WB_IMAGE1, WB_IMAGE2 and WB_IMAGE3 are used for
	mapping the space from PCI to WB. Offcourse, WB_IMAGE0 is assigned to Configuration space). That leave
	us WB_IMAGE5 as the maximum number of images.

	When error occurs, WISHBONE ERR_ADDR and ERR_DATA registers stores address and data on the bus that
	caused error. While ERR_CS register stores Byte Enables and Bus Command in the MSByte. In bits 10, 9
	and 8 it reports Retry Counter Expired (for posted writes), Error Source (Master Abort) and Error
	Report Signal (signals that error has occured) respectively. With bit 0 we enable Error Reporting
	mechanism.
-----------------------------------------------------------------------------------------------------------*/
// WB_IMAGE0 is always assigned to config. space or is not used
			wire	[2 : 0]		wb_img_ctrl0_bit2_0 = 3'b000 ; // NO addr.transl., pre-fetch and read-line
			wire	[31 : 12]	wb_ba0_bit31_12 = `WB_CONFIGURATION_BASE ;
			wire				wb_ba0_bit0 = 0 ; // config. space is MEMORY space
			wire	[31 : 12]	wb_am0 = `WB_AM0 ; // 4KBytes of configuration space is minimum
			wire	[31 : 12]	wb_ta0 = 20'h0000_0 ; // NO address translation needed
// WB_IMAGE1 is included by default meanwhile others are optional !
			reg		[2 : 0]		wb_img_ctrl1_bit2_0 ;
			reg		[31 : 12]	wb_ba1_bit31_12 ;
			reg					wb_ba1_bit0 ;
			reg		[31 : 12]	wb_am1 ;
			reg		[31 : 12]	wb_ta1 ;
`ifdef		WB_IMAGE2
			reg		[2 : 0]		wb_img_ctrl2_bit2_0 ;
			reg		[31 : 12]	wb_ba2_bit31_12 ;
			reg					wb_ba2_bit0 ;
			reg		[31 : 12]	wb_am2 ;
			reg		[31 : 12]	wb_ta2 ;
`else
            wire	[2 : 0]		wb_img_ctrl2_bit2_0 = 3'b000 ;
			wire	[31 : 12]	wb_ba2_bit31_12 = 20'h0000_0 ;
            wire				wb_ba2_bit0 = 1'b0 ;
            wire	[31 : 12]	wb_am2 = 20'h0000_0 ;
            wire	[31 : 12]	wb_ta2 = 20'h0000_0 ;
`endif
`ifdef		WB_IMAGE3
			reg		[2 : 0]		wb_img_ctrl3_bit2_0 ;
			reg		[31 : 12]	wb_ba3_bit31_12 ;
			reg					wb_ba3_bit0 ;
			reg		[31 : 12]	wb_am3 ;
			reg		[31 : 12]	wb_ta3 ;
`else
            wire	[2 : 0]		wb_img_ctrl3_bit2_0 = 3'b000 ;
			wire	[31 : 12]	wb_ba3_bit31_12 = 20'h0000_0 ;
            wire				wb_ba3_bit0 = 1'b0 ;
            wire	[31 : 12]	wb_am3 = 20'h0000_0 ;
            wire	[31 : 12]	wb_ta3 = 20'h0000_0 ;
`endif
`ifdef		WB_IMAGE4
			reg		[2 : 0]		wb_img_ctrl4_bit2_0 ;
			reg		[31 : 12]	wb_ba4_bit31_12 ;
			reg					wb_ba4_bit0 ;
			reg		[31 : 12]	wb_am4 ;
			reg		[31 : 12]	wb_ta4 ;
`else
            wire	[2 : 0]		wb_img_ctrl4_bit2_0 = 3'b000 ;
			wire	[31 : 12]	wb_ba4_bit31_12 = 20'h0000_0 ;
            wire				wb_ba4_bit0 = 1'b0 ;
            wire	[31 : 12]	wb_am4 = 20'h0000_0 ;
            wire	[31 : 12]	wb_ta4 = 20'h0000_0 ;
`endif
`ifdef		WB_IMAGE5
			reg		[2 : 0]		wb_img_ctrl5_bit2_0 ;
			reg		[31 : 12]	wb_ba5_bit31_12 ;
			reg					wb_ba5_bit0 ;
			reg		[31 : 12]	wb_am5 ;
			reg		[31 : 12]	wb_ta5 ;
`else
            wire	[2 : 0]		wb_img_ctrl5_bit2_0 = 3'b000 ;
			wire	[31 : 12]	wb_ba5_bit31_12 = 20'h0000_0 ;
            wire				wb_ba5_bit0 = 1'b0 ;
            wire	[31 : 12]	wb_am5 = 20'h0000_0 ;
            wire	[31 : 12]	wb_ta5 = 20'h0000_0 ;
`endif
			reg		[31 : 24]	wb_err_cs_bit31_24 ;
/*			reg					wb_err_cs_bit10 ;*/
			reg					wb_err_cs_bit9 ;
			reg					wb_err_cs_bit8 ;
			reg					wb_err_cs_bit0 ;
			reg		[31 : 0]	wb_err_addr ;
			reg		[31 : 0]	wb_err_data ;


/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------
Configuration Cycle address register
	There are also some registers with NOT all bits implemented and therefor uses _bitX or _bitX2_X1 to
	sign which bit or range of bits are implemented.
-------------------------------------------------------------------------------------------------------------
###########################################################################################################*/

/*-----------------------------------------------------------------------------------------------------------
[860h-868h]
	PCI bridge must ignore Type 1 configuration cycles (Master Abort) since they are used for PCI to PCI
	bridges. This is single function device, that means responding on configuration cycles to all functions
	(or responding only to function 0). Configuration address register for generating configuration cycles
	is prepared for all options (it includes Bus Number, Device, Function, Offset and Type).
	Interrupt acknowledge register stores interrupt vector data returned during Interrupt Acknowledge cycle.
-----------------------------------------------------------------------------------------------------------*/
`ifdef		HOST
			reg		[23 : 2]	cnf_addr_bit23_2 ;
			reg					cnf_addr_bit0 ;
`else // GUEST
			wire	[23 : 2]	cnf_addr_bit23_2	= 22'h0 ;
			wire				cnf_addr_bit0		= 1'b0 ;
`endif
			// reg	[31 : 0]	cnf_data ;		IMPLEMENTED elsewhere !!!!!
			// reg	[31 : 0]	int_ack ;		IMPLEMENTED elsewhere !!!!!


/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------
General Interrupt registers
	There are also some registers with NOT all bits implemented and therefor uses _bitX or _bitX2_X1 to
	sign which bit or range of bits are implemented.
-------------------------------------------------------------------------------------------------------------
###########################################################################################################*/

/*-----------------------------------------------------------------------------------------------------------
[FF8h-FFCh]
	Bit 31 in the Interrupt Control register is set by software and used to generate SOFT RESET. Other 4
	bits are used to enable interrupt generations.
	5 LSbits in the Interrupt Status register are indicating System Error Int, Parity Error Int, PCI & WB
	Error Int and Inerrupt respecively. System and Parity errors are implented only in HOST bridge
	implementations!
-----------------------------------------------------------------------------------------------------------*/
			reg					icr_bit31 ;
`ifdef		HOST
			reg		[4 : 3]		icr_bit4_3              ;
			reg		[4 : 3]		isr_bit4_3              ;
			reg		[2 : 0]		icr_bit2_0              ;
			reg		[2 : 0]		isr_bit2_0              ;
`else // GUEST
			wire	[4 : 3]		icr_bit4_3 = 2'h0 ;
			wire	[4 : 3]		isr_bit4_3 = 2'h0 ;
			reg		[2 : 0]		icr_bit2_0 ;
			reg		[2 : 0]		isr_bit2_0 ;
`endif

/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------
Initialization complete identifier
    When using I2C or similar initialisation mechanism,
    the bridge must not respond to transaction requests on PCI bus,
    not even to configuration cycles.
    Therefore, only when init_complete is set, the bridge starts
    participating on the PCI bus as an active device.
    Two additional flip flops are also provided for GUEST implementation,
    to synchronize to the pci clock after PCI reset is asynchronously de-asserted.
-------------------------------------------------------------------------------------------------------------
###########################################################################################################*/

`ifdef GUEST

reg rst_inactive_sync ;
reg rst_inactive      ;

`else

wire rst_inactive = 1'b1 ;

`endif

reg init_complete   ;

wire    sync_init_complete ;

`ifdef HOST
assign  wb_init_complete_out = init_complete ;

pci_synchronizer_flop #(1, 0) i_pci_init_complete_sync
(
    .data_in        (   init_complete       ), 
    .clk_out        (   pci_clk             ), 
    .sync_data_out  (   sync_init_complete  ), 
    .async_reset    (   reset               )
);

reg pci_init_complete_out ;

always@(posedge pci_clk or posedge reset)
begin
    if (reset)
        pci_init_complete_out <= 1'b0 ;
    else
        pci_init_complete_out <= sync_init_complete ;
end

`endif

`ifdef GUEST

assign  pci_init_complete_out = init_complete ;

pci_synchronizer_flop #(1, 0) i_wb_init_complete_sync
(
    .data_in        (   init_complete       ), 
    .clk_out        (   wb_clk              ), 
    .sync_data_out  (   sync_init_complete  ), 
    .async_reset    (   reset               )
);

reg wb_init_complete_out ;

always@(posedge wb_clk or posedge reset)
begin
    if (reset)
        wb_init_complete_out <= 1'b0 ;
    else
        wb_init_complete_out <= sync_init_complete ;
end

`endif

/*###########################################################################################################
-------------------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------------*/

`ifdef NO_CNF_IMAGE // if IMAGE0 is assigned as general image space

		assign	r_conf_data_out = 32'h0000_0000 ;

`else

    always@(r_conf_address_in or
    		status_bit15_11 or status_bit8 or r_status_bit4 or command_bit8 or command_bit6 or command_bit2_0 or
    		latency_timer or cache_line_size_reg or r_vendor_id or r_device_id or r_revision_id or
            r_subsys_vendor_id or r_subsys_id or r_max_lat or r_min_gnt or
    		pci_ba0_bit31_8 or
    		pci_img_ctrl0_bit2_1 or pci_am0 or pci_ta0 or pci_ba0_bit0 or
    		pci_img_ctrl1_bit2_1 or pci_am1 or pci_ta1 or pci_ba1_bit31_8  or pci_ba1_bit0 or
    		pci_img_ctrl2_bit2_1 or pci_am2 or pci_ta2 or pci_ba2_bit31_8 or pci_ba2_bit0 or
    		pci_img_ctrl3_bit2_1 or pci_am3 or pci_ta3 or pci_ba3_bit31_8 or pci_ba3_bit0 or
    		pci_img_ctrl4_bit2_1 or pci_am4 or pci_ta4 or pci_ba4_bit31_8 or pci_ba4_bit0 or
    		pci_img_ctrl5_bit2_1 or pci_am5 or pci_ta5 or pci_ba5_bit31_8 or pci_ba5_bit0 or
    		interrupt_line or
    		pci_err_cs_bit31_24 or pci_err_cs_bit10 or pci_err_cs_bit9 or pci_err_cs_bit8 or pci_err_cs_bit0 or
    		pci_err_addr or pci_err_data or
    		wb_ba0_bit31_12 or wb_ba0_bit0 or
    		wb_img_ctrl1_bit2_0 or wb_ba1_bit31_12 or wb_ba1_bit0 or wb_am1 or wb_ta1 or
    		wb_img_ctrl2_bit2_0 or wb_ba2_bit31_12 or wb_ba2_bit0 or wb_am2 or wb_ta2 or
    		wb_img_ctrl3_bit2_0 or wb_ba3_bit31_12 or wb_ba3_bit0 or wb_am3 or wb_ta3 or
    		wb_img_ctrl4_bit2_0 or wb_ba4_bit31_12 or wb_ba4_bit0 or wb_am4 or wb_ta4 or
    		wb_img_ctrl5_bit2_0 or wb_ba5_bit31_12 or wb_ba5_bit0 or wb_am5 or wb_ta5 or
    		wb_err_cs_bit31_24 or /*wb_err_cs_bit10 or*/ wb_err_cs_bit9 or wb_err_cs_bit8 or wb_err_cs_bit0 or
    		wb_err_addr or wb_err_data or
    		cnf_addr_bit23_2 or cnf_addr_bit0 or icr_bit31 or icr_bit4_3 or icr_bit2_0 or isr_bit4_3 or isr_bit2_0

        `ifdef PCI_CPCI_HS_IMPLEMENT
            or hs_ins or hs_ext or hs_pi or hs_loo or hs_eim or hs_cap_id
        `endif

        `ifdef PCI_SPOCI
            or spoci_cs_nack or spoci_cs_write or spoci_cs_read or spoci_cs_adr or spoci_cs_dat
        `endif
    		)
    begin
    	case (r_conf_address_in[9:2])
    	// PCI header - configuration space
    	8'h0: r_conf_data_out = { r_device_id, r_vendor_id } ;
    	8'h1: r_conf_data_out = { status_bit15_11, r_status_bit10_9, status_bit8, r_status_bit7, 1'h0, r_status_bit5, r_status_bit4, 
    								 4'h0, 7'h00, command_bit8, 1'h0, command_bit6, 3'h0, command_bit2_0 } ;
    	8'h2: r_conf_data_out = { r_class_code, r_revision_id } ;
    	8'h3: r_conf_data_out = { 8'h00, r_header_type, latency_timer, cache_line_size_reg } ;
    	8'h4: 
    	begin
        `ifdef HOST
            `ifdef NO_CNF_IMAGE
    		    r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba0_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															      pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		    r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		    r_conf_data_out[0] = pci_ba0_bit0 & pci_am0[31];
            `else
                r_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
                r_conf_data_out[11: 0] = 12'h000 ;
            `endif
        `endif

        `ifdef GUEST
            r_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
            r_conf_data_out[11: 0] = 12'h000 ;
        `endif
    	end
    	8'h5: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba1_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															 pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba1_bit0 & pci_am1[31];
    	end
    	8'h6: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba2_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															 pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba2_bit0 & pci_am2[31];
    	end
    	8'h7: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba3_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															 pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba3_bit0 & pci_am3[31];
    	end
    	8'h8: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba4_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															 pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba4_bit0 & pci_am4[31];
    	end
    	8'h9: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba5_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															 pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba5_bit0 & pci_am5[31];
    	end
        8'hB:
        begin
            r_conf_data_out = {r_subsys_id, r_subsys_vendor_id} ;
        end
    `ifdef PCI_CPCI_HS_IMPLEMENT
        8'hD:
        begin
            r_conf_data_out = {24'h0000_00, `PCI_CAP_PTR_VAL} ;
        end
    `endif
    	8'hf: r_conf_data_out = { r_max_lat, r_min_gnt, r_interrupt_pin, interrupt_line } ;
    `ifdef PCI_CPCI_HS_IMPLEMENT
        (`PCI_CAP_PTR_VAL >> 2):
        begin
            r_conf_data_out  = {8'h00, hs_ins, hs_ext, hs_pi, hs_loo, 1'b0, hs_eim, 1'b0, 8'h00, hs_cap_id} ;
        end
    `endif
  		// PCI target - configuration space
    	{2'b01, `P_IMG_CTRL0_ADDR}: r_conf_data_out = { 29'h00000000, pci_img_ctrl0_bit2_1, 1'h0 } ;
        {2'b01, `P_BA0_ADDR}	  : 
        begin
    	`ifdef HOST
            `ifdef NO_CNF_IMAGE
    		    r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba0_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															      pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		    r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		    r_conf_data_out[0] = pci_ba0_bit0 & pci_am0[31];
            `else
                r_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
                r_conf_data_out[11: 0] = 12'h000 ;
            `endif
        `endif

        `ifdef GUEST
            r_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
            r_conf_data_out[11: 0] = 12'h000 ;
        `endif
    	end
        {2'b01, `P_AM0_ADDR}: 
    	begin
        `ifdef HOST
            `ifdef NO_CNF_IMAGE
           	    r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		    r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
            `else
                r_conf_data_out[31:12] = pci_am0[31:12] ;
                r_conf_data_out[11: 0] = 12'h000        ;
            `endif
        `endif

        `ifdef GUEST
            r_conf_data_out[31:12] = pci_am0[31:12] ;
            r_conf_data_out[11: 0] = 12'h000        ;
        `endif
    	end
        {2'b01, `P_TA0_ADDR}: 
    	begin
            r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_IMG_CTRL1_ADDR}: r_conf_data_out = { 29'h00000000, pci_img_ctrl1_bit2_1, 1'h0 } ;
        {2'b01, `P_BA1_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba1_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    	 														  pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba1_bit0 & pci_am1[31];
    	end
        {2'b01, `P_AM1_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_TA1_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_IMG_CTRL2_ADDR}: r_conf_data_out = { 29'h00000000, pci_img_ctrl2_bit2_1, 1'h0 } ;
        {2'b01, `P_BA2_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba2_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															  pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba2_bit0 & pci_am2[31];
    	end
        {2'b01, `P_AM2_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_TA2_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_IMG_CTRL3_ADDR}: r_conf_data_out = { 29'h00000000, pci_img_ctrl3_bit2_1, 1'h0 } ;
        {2'b01, `P_BA3_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba3_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															  pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba3_bit0 & pci_am3[31];
    	end
        {2'b01, `P_AM3_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_TA3_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_IMG_CTRL4_ADDR}: r_conf_data_out = { 29'h00000000, pci_img_ctrl4_bit2_1, 1'h0 } ;
        {2'b01, `P_BA4_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba4_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															  pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba4_bit0 & pci_am4[31];
    	end
        {2'b01, `P_AM4_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_TA4_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_IMG_CTRL5_ADDR}: r_conf_data_out = { 29'h00000000, pci_img_ctrl5_bit2_1, 1'h0 } ;
        {2'b01, `P_BA5_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba5_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    															  pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    		r_conf_data_out[0] = pci_ba5_bit0 & pci_am5[31];
    	end
        {2'b01, `P_AM5_ADDR}:
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_TA5_ADDR}: 
    	begin
           	r_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
    	end
        {2'b01, `P_ERR_CS_ADDR}: r_conf_data_out = { pci_err_cs_bit31_24, 13'h0000, pci_err_cs_bit10, pci_err_cs_bit9,
            					 				     pci_err_cs_bit8, 7'h00, pci_err_cs_bit0 } ;
        {2'b01, `P_ERR_ADDR_ADDR}: r_conf_data_out = pci_err_addr ;
        {2'b01, `P_ERR_DATA_ADDR}: r_conf_data_out = pci_err_data ;
    		// WB slave - configuration space
    	{2'b01, `WB_CONF_SPC_BAR_ADDR}: r_conf_data_out = { wb_ba0_bit31_12, 11'h000, wb_ba0_bit0 } ;
    	{2'b01, `W_IMG_CTRL1_ADDR}: r_conf_data_out = { 29'h00000000, wb_img_ctrl1_bit2_0 } ;
    	{2'b01, `W_BA1_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba1_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    															 wb_am1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    		r_conf_data_out[0] = wb_ba1_bit0 ;
    	end
    	{2'b01, `W_AM1_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_TA1_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_IMG_CTRL2_ADDR}: r_conf_data_out = { 29'h00000000, wb_img_ctrl2_bit2_0 } ;
    	`W_BA2_ADDR		 : 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba2_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    															 wb_am2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    		r_conf_data_out[0] = wb_ba2_bit0 ;
    	end
    	{2'b01, `W_AM2_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_TA2_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_IMG_CTRL3_ADDR}: r_conf_data_out = { 29'h00000000, wb_img_ctrl3_bit2_0 } ;
    	{2'b01, `W_BA3_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba3_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    															 wb_am3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    		r_conf_data_out[0] = wb_ba3_bit0 ;
    	end
    	{2'b01, `W_AM3_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_TA3_ADDR}: 
    	begin
    	    r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_IMG_CTRL4_ADDR}: r_conf_data_out = { 29'h00000000, wb_img_ctrl4_bit2_0 } ;
    	{2'b01, `W_BA4_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba4_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    															 wb_am4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    		r_conf_data_out[0] = wb_ba4_bit0 ;
    	end
    	{2'b01, `W_AM4_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_TA4_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_IMG_CTRL5_ADDR}: r_conf_data_out = { 29'h00000000, wb_img_ctrl5_bit2_0 } ;
    	{2'b01, `W_BA5_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba5_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    															 wb_am5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    		r_conf_data_out[0] = wb_ba5_bit0 ;
    	end
    	{2'b01, `W_AM5_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_TA5_ADDR}: 
    	begin
    		r_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    		r_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
    	end
    	{2'b01, `W_ERR_CS_ADDR}: r_conf_data_out = { wb_err_cs_bit31_24, /*13*/14'h0000, /*wb_err_cs_bit10,*/
            									     wb_err_cs_bit9, wb_err_cs_bit8, 7'h00, wb_err_cs_bit0 } ;
    	{2'b01, `W_ERR_ADDR_ADDR}: r_conf_data_out = wb_err_addr ;
    	{2'b01, `W_ERR_DATA_ADDR}: r_conf_data_out = wb_err_data ;

    	{2'b01, `CNF_ADDR_ADDR}: r_conf_data_out = { 8'h00, cnf_addr_bit23_2, 1'h0, cnf_addr_bit0 } ;
    		// `CNF_DATA_ADDR: implemented elsewhere !!!
    		// `INT_ACK_ADDR : implemented elsewhere !!!
        {2'b01, `ICR_ADDR}: r_conf_data_out = { icr_bit31, 26'h0000_000, icr_bit4_3, icr_bit2_0 } ;
        {2'b01, `ISR_ADDR}: r_conf_data_out = { 27'h0000_000, isr_bit4_3, isr_bit2_0 } ;

    `ifdef PCI_SPOCI
        8'hff: r_conf_data_out = {spoci_cs_nack, 5'h0, spoci_cs_write, spoci_cs_read,
                                  5'h0, spoci_cs_adr[10:8],
                                  spoci_cs_adr[7:0],
                                  spoci_cs_dat[7:0]} ;
    `endif
    	default	: r_conf_data_out = 32'h0000_0000 ;
    	endcase
    end

`endif

`ifdef PCI_SPOCI
reg [ 7: 0] spoci_reg_num ;
wire [11: 0] w_conf_address = init_complete ? w_conf_address_in : {2'b00, spoci_reg_num, 2'b00} ;
`else
wire [11: 0] w_conf_address = w_conf_address_in ;
wire [ 7: 0] spoci_reg_num = 'hff ;
`endif

always@(w_conf_address or
		status_bit15_11 or status_bit8 or r_status_bit4 or command_bit8 or command_bit6 or command_bit2_0 or
		latency_timer or cache_line_size_reg or r_vendor_id or r_device_id or r_revision_id or
        r_subsys_id or r_subsys_vendor_id or r_max_lat or r_min_gnt or
		pci_ba0_bit31_8 or
		pci_img_ctrl0_bit2_1 or pci_am0 or pci_ta0 or pci_ba0_bit0 or
		pci_img_ctrl1_bit2_1 or pci_am1 or pci_ta1 or pci_ba1_bit31_8  or pci_ba1_bit0 or
		pci_img_ctrl2_bit2_1 or pci_am2 or pci_ta2 or pci_ba2_bit31_8 or pci_ba2_bit0 or
		pci_img_ctrl3_bit2_1 or pci_am3 or pci_ta3 or pci_ba3_bit31_8 or pci_ba3_bit0 or
		pci_img_ctrl4_bit2_1 or pci_am4 or pci_ta4 or pci_ba4_bit31_8 or pci_ba4_bit0 or
		pci_img_ctrl5_bit2_1 or pci_am5 or pci_ta5 or pci_ba5_bit31_8 or pci_ba5_bit0 or
		interrupt_line or
		pci_err_cs_bit31_24 or pci_err_cs_bit10 or pci_err_cs_bit9 or pci_err_cs_bit8 or pci_err_cs_bit0 or
		pci_err_addr or pci_err_data or
		wb_ba0_bit31_12 or wb_ba0_bit0 or
		wb_img_ctrl1_bit2_0 or wb_ba1_bit31_12 or wb_ba1_bit0 or wb_am1 or wb_ta1 or
		wb_img_ctrl2_bit2_0 or wb_ba2_bit31_12 or wb_ba2_bit0 or wb_am2 or wb_ta2 or
		wb_img_ctrl3_bit2_0 or wb_ba3_bit31_12 or wb_ba3_bit0 or wb_am3 or wb_ta3 or
		wb_img_ctrl4_bit2_0 or wb_ba4_bit31_12 or wb_ba4_bit0 or wb_am4 or wb_ta4 or
		wb_img_ctrl5_bit2_0 or wb_ba5_bit31_12 or wb_ba5_bit0 or wb_am5 or wb_ta5 or
		wb_err_cs_bit31_24 or /*wb_err_cs_bit10 or*/ wb_err_cs_bit9 or wb_err_cs_bit8 or wb_err_cs_bit0 or
		wb_err_addr or wb_err_data or
		cnf_addr_bit23_2 or cnf_addr_bit0 or icr_bit31 or icr_bit4_3 or icr_bit2_0 or isr_bit4_3 or isr_bit2_0

    `ifdef PCI_CPCI_HS_IMPLEMENT
        or hs_ins or hs_ext or hs_pi or hs_loo or hs_eim or hs_cap_id
    `endif

    `ifdef PCI_SPOCI
        or spoci_cs_nack or spoci_cs_write or spoci_cs_read or spoci_cs_adr or spoci_cs_dat
    `endif
		)
begin
	case (w_conf_address[9:2])
	8'h0:
	begin
		w_conf_data_out = { r_device_id, r_vendor_id } ;
		w_reg_select_dec = 57'h000_0000_0000_0000 ; // Read-Only register
	end
	8'h1: // w_reg_select_dec bit 0
	begin
		w_conf_data_out = { status_bit15_11, r_status_bit10_9, status_bit8, r_status_bit7, 1'h0, r_status_bit5, r_status_bit4, 
	 					    4'h0, 7'h00, command_bit8, 1'h0, command_bit6, 3'h0, command_bit2_0 } ;
		w_reg_select_dec = 57'h000_0000_0000_0001 ;
	end
	8'h2:
	begin
		w_conf_data_out = { r_class_code, r_revision_id } ;
		w_reg_select_dec = 57'h000_0000_0000_0000 ; // Read-Only register
	end
	8'h3: // w_reg_select_dec bit 1
	begin
		w_conf_data_out = { 8'h00, r_header_type, latency_timer, cache_line_size_reg } ;
		w_reg_select_dec = 57'h000_0000_0000_0002 ;
	end
	8'h4: // w_reg_select_dec bit 4
	begin
    `ifdef HOST
        `ifdef NO_CNF_IMAGE
    	    w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba0_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														      pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	    w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	    w_conf_data_out[0] = pci_ba0_bit0 & pci_am0[31];
        `else
            w_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
            w_conf_data_out[11: 0] = 12'h000 ;
        `endif
    `endif

    `ifdef GUEST
        w_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
        w_conf_data_out[11: 0] = 12'h000 ;
    `endif
		w_reg_select_dec = 57'h000_0000_0000_0010 ; // The same for another address
	end
	8'h5: // w_reg_select_dec bit 8
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba1_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba1_bit0 & pci_am1[31];
		w_reg_select_dec = 57'h000_0000_0000_0100 ; // The same for another address
	end
	8'h6: // w_reg_select_dec bit 12
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba2_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba2_bit0 & pci_am2[31];
		w_reg_select_dec = 57'h000_0000_0000_1000 ; // The same for another address
	end
	8'h7: // w_reg_select_dec bit 16
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba3_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba3_bit0 & pci_am3[31];
		w_reg_select_dec = 57'h000_0000_0001_0000 ; // The same for another address
	end
	8'h8: // w_reg_select_dec bit 20
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba4_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba4_bit0 & pci_am4[31];
		w_reg_select_dec = 57'h000_0000_0010_0000 ; // The same for another address
	end
	8'h9: // w_reg_select_dec bit 24
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba5_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba5_bit0 & pci_am5[31];
		w_reg_select_dec = 57'h000_0000_0100_0000 ; // The same for another address
	end
    8'hB:
    begin
        w_conf_data_out = {r_subsys_id, r_subsys_vendor_id} ;
        w_reg_select_dec = 57'h000_0000_0000_0000 ;
    end

`ifdef PCI_CPCI_HS_IMPLEMENT
    8'hD:
    begin
        w_conf_data_out  = {24'h0000_00, `PCI_CAP_PTR_VAL} ;
        w_reg_select_dec = 57'h000_0000_0000_0000 ; // Read-Only register
    end
`endif
	8'hf: // w_reg_select_dec bit 2
	begin
		w_conf_data_out = { r_max_lat, r_min_gnt, r_interrupt_pin, interrupt_line } ;
		w_reg_select_dec = 57'h000_0000_0000_0004 ;
	end
`ifdef PCI_CPCI_HS_IMPLEMENT
    (`PCI_CAP_PTR_VAL >> 2):
    begin
        w_reg_select_dec = 57'h100_0000_0000_0000 ;
        w_conf_data_out  = {8'h00, hs_ins, hs_ext, hs_pi, hs_loo, 1'b0, hs_eim, 1'b0, 8'h00, hs_cap_id} ;
    end
`endif
	{2'b01, `P_IMG_CTRL0_ADDR}:  // w_reg_select_dec bit 3
	begin
		w_conf_data_out = { 29'h00000000, pci_img_ctrl0_bit2_1, 1'h0 } ;
		w_reg_select_dec = 57'h000_0000_0000_0008 ;
	end
    {2'b01, `P_BA0_ADDR}:   // w_reg_select_dec bit 4
	begin
    `ifdef HOST
        `ifdef NO_CNF_IMAGE
    	    w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba0_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														      pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	    w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	    w_conf_data_out[0] = pci_ba0_bit0 & pci_am0[31];
        `else
            w_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
            w_conf_data_out[11: 0] = 12'h000 ;
        `endif
    `endif

    `ifdef GUEST
        w_conf_data_out[31:12] = pci_ba0_bit31_8[31:12] ;
        w_conf_data_out[11: 0] = 12'h000 ;
    `endif
		w_reg_select_dec = 57'h000_0000_0000_0010 ; // The same for another address
	end
    {2'b01, `P_AM0_ADDR}:   // w_reg_select_dec bit 5
	begin
    `ifdef HOST
        `ifdef NO_CNF_IMAGE
       	    w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	    w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
        `else
            w_conf_data_out[31:12] = pci_am0[31:12] ;
            w_conf_data_out[11: 0] = 12'h000        ;
        `endif
    `endif

    `ifdef GUEST
        w_conf_data_out[31:12] = pci_am0[31:12] ;
        w_conf_data_out[11: 0] = 12'h000        ;
    `endif
		w_reg_select_dec = 57'h000_0000_0000_0020 ;
	end
    {2'b01, `P_TA0_ADDR}:   // w_reg_select_dec bit 6
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
        w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0000_0040 ;
	end
    {2'b01, `P_IMG_CTRL1_ADDR}:   // w_reg_select_dec bit 7
	begin
	    w_conf_data_out = { 29'h00000000, pci_img_ctrl1_bit2_1, 1'h0 } ;
		w_reg_select_dec = 57'h000_0000_0000_0080 ;
	end
    {2'b01, `P_BA1_ADDR}:   // w_reg_select_dec bit 8
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba1_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba1_bit0 & pci_am1[31];
		w_reg_select_dec = 57'h000_0000_0000_0100 ; // The same for another address
	end
    {2'b01, `P_AM1_ADDR}:   // w_reg_select_dec bit 9
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
        w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0000_0200 ;
	end
    {2'b01, `P_TA1_ADDR}:   // w_reg_select_dec bit 10
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
        w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0000_0400 ;
	end
    {2'b01, `P_IMG_CTRL2_ADDR}:   // w_reg_select_dec bit 11
	begin
	    w_conf_data_out = { 29'h00000000, pci_img_ctrl2_bit2_1, 1'h0 } ;
		w_reg_select_dec = 57'h000_0000_0000_0800 ;
	end
    {2'b01, `P_BA2_ADDR}:   // w_reg_select_dec bit 12
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba2_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba2_bit0 & pci_am2[31];
		w_reg_select_dec = 57'h000_0000_0000_1000 ; // The same for another address
	end
    {2'b01, `P_AM2_ADDR}:   // w_reg_select_dec bit 13
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0000_2000 ;
	end
    {2'b01, `P_TA2_ADDR}:   // w_reg_select_dec bit 14
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
        w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0000_4000 ;
	end
    {2'b01, `P_IMG_CTRL3_ADDR}:   // w_reg_select_dec bit 15
	begin
	    w_conf_data_out = { 29'h00000000, pci_img_ctrl3_bit2_1, 1'h0 } ;
		w_reg_select_dec = 57'h000_0000_0000_8000 ;
	end
    {2'b01, `P_BA3_ADDR}:   // w_reg_select_dec bit 16
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba3_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba3_bit0 & pci_am3[31];
		w_reg_select_dec = 57'h000_0000_0001_0000 ; // The same for another address
	end
    {2'b01, `P_AM3_ADDR}:   // w_reg_select_dec bit 17
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0002_0000 ;
	end
    {2'b01, `P_TA3_ADDR}:   // w_reg_select_dec bit 18
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
        w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0004_0000 ;
	end
    {2'b01, `P_IMG_CTRL4_ADDR}:   // w_reg_select_dec bit 19
	begin
	    w_conf_data_out = { 29'h00000000, pci_img_ctrl4_bit2_1, 1'h0 } ;
		w_reg_select_dec = 57'h000_0000_0008_0000 ;
	end
    {2'b01, `P_BA4_ADDR}:   // w_reg_select_dec bit 20
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba4_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba4_bit0 & pci_am4[31];
		w_reg_select_dec = 57'h000_0000_0010_0000 ; // The same for another address
	end
    {2'b01, `P_AM4_ADDR}:   // w_reg_select_dec bit 21
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0020_0000 ;
	end
    {2'b01, `P_TA4_ADDR}:   // w_reg_select_dec bit 22
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0040_0000 ;
	end
    {2'b01, `P_IMG_CTRL5_ADDR}:   // w_reg_select_dec bit 23
	begin
		w_conf_data_out = { 29'h00000000, pci_img_ctrl5_bit2_1, 1'h0 } ;
		w_reg_select_dec = 57'h000_0000_0080_0000 ;
	end
    {2'b01, `P_BA5_ADDR}:   // w_reg_select_dec bit 24
	begin
    	w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ba5_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] & 
    														  pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):1] = 0 ;
    	w_conf_data_out[0] = pci_ba5_bit0 & pci_am5[31];
		w_reg_select_dec = 57'h000_0000_0100_0000 ; // The same for another address
	end
    {2'b01, `P_AM5_ADDR}:   // w_reg_select_dec bit 25
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0200_0000 ;
	end
    {2'b01, `P_TA5_ADDR}:   // w_reg_select_dec bit 26
	begin
        w_conf_data_out[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] = pci_ta5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
        w_conf_data_out[(31-`PCI_NUM_OF_DEC_ADDR_LINES):0] = 0 ;
		w_reg_select_dec = 57'h000_0000_0400_0000 ;
	end
    {2'b01, `P_ERR_CS_ADDR}:   // w_reg_select_dec bit 27
	begin
	    w_conf_data_out = { pci_err_cs_bit31_24, 13'h0000, pci_err_cs_bit10, pci_err_cs_bit9,
        				    pci_err_cs_bit8, 7'h00, pci_err_cs_bit0 } ;
		w_reg_select_dec = 57'h000_0000_0800_0000 ;
	end
    {2'b01, `P_ERR_ADDR_ADDR}:   // w_reg_select_dec bit 28
	begin
	    w_conf_data_out = pci_err_addr ;
		w_reg_select_dec = 57'h000_0000_0000_0000 ; // = 56'h00_0000_1000_0000 ;
	end
    {2'b01, `P_ERR_DATA_ADDR}:   // w_reg_select_dec bit 29
	begin
		w_conf_data_out = pci_err_data ;
		w_reg_select_dec = 57'h000_0000_0000_0000 ; // = 56'h00_0000_2000_0000 ;
	end
	// WB slave - configuration space
	{2'b01, `WB_CONF_SPC_BAR_ADDR}:
	begin
		w_conf_data_out = { wb_ba0_bit31_12, 11'h000, wb_ba0_bit0 } ;
		w_reg_select_dec = 57'h000_0000_0000_0000 ; // Read-Only register
	end
	{2'b01, `W_IMG_CTRL1_ADDR}:   // w_reg_select_dec bit 30
	begin
		w_conf_data_out = { 29'h00000000, wb_img_ctrl1_bit2_0 } ;
		w_reg_select_dec = 57'h000_0000_4000_0000 ;
	end
	{2'b01, `W_BA1_ADDR}:   // w_reg_select_dec bit 31
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba1_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    														 wb_am1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    	w_conf_data_out[0] = wb_ba1_bit0 ;
		w_reg_select_dec = 57'h000_0000_8000_0000 ;
	end
	{2'b01, `W_AM1_ADDR}:   // w_reg_select_dec bit 32
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_0001_0000_0000 ;
	end
    {2'b01, `W_TA1_ADDR}:   // w_reg_select_dec bit 33
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_0002_0000_0000 ;
	end
	{2'b01, `W_IMG_CTRL2_ADDR}:   // w_reg_select_dec bit 34
	begin
		w_conf_data_out = { 29'h00000000, wb_img_ctrl2_bit2_0 } ;
		w_reg_select_dec = 57'h000_0004_0000_0000 ;
	end
	{2'b01, `W_BA2_ADDR}:   // w_reg_select_dec bit 35
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba2_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    														 wb_am2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    	w_conf_data_out[0] = wb_ba2_bit0 ;
		w_reg_select_dec = 57'h000_0008_0000_0000 ;
	end
	{2'b01, `W_AM2_ADDR}:   // w_reg_select_dec bit 36
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_0010_0000_0000 ;
	end
	{2'b01, `W_TA2_ADDR}:   // w_reg_select_dec bit 37
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_0020_0000_0000 ;
	end
	{2'b01, `W_IMG_CTRL3_ADDR}:   // w_reg_select_dec bit 38
	begin
		w_conf_data_out = { 29'h00000000, wb_img_ctrl3_bit2_0 } ;
		w_reg_select_dec = 57'h000_0040_0000_0000 ;
	end
	{2'b01, `W_BA3_ADDR}:   // w_reg_select_dec bit 39
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba3_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    														 wb_am3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    	w_conf_data_out[0] = wb_ba3_bit0 ;
		w_reg_select_dec = 57'h000_0080_0000_0000 ;
	end
	{2'b01, `W_AM3_ADDR}:   // w_reg_select_dec bit 40
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_0100_0000_0000 ;
	end
	{2'b01, `W_TA3_ADDR}:   // w_reg_select_dec bit 41
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_0200_0000_0000 ;
	end
	{2'b01, `W_IMG_CTRL4_ADDR}:   // w_reg_select_dec bit 42
	begin
		w_conf_data_out = { 29'h00000000, wb_img_ctrl4_bit2_0 } ;
		w_reg_select_dec = 57'h000_0400_0000_0000 ;
	end
	{2'b01, `W_BA4_ADDR}:   // w_reg_select_dec bit 43
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba4_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    														 wb_am4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    	w_conf_data_out[0] = wb_ba4_bit0 ;
		w_reg_select_dec = 57'h000_0800_0000_0000 ;
	end
	{2'b01, `W_AM4_ADDR}:   // w_reg_select_dec bit 44
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_1000_0000_0000 ;
	end
	{2'b01, `W_TA4_ADDR}:   // w_reg_select_dec bit 45
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h000_2000_0000_0000 ;
	end
	{2'b01, `W_IMG_CTRL5_ADDR}:   // w_reg_select_dec bit 46
	begin
		w_conf_data_out = { 29'h00000000, wb_img_ctrl5_bit2_0 } ;
		w_reg_select_dec = 57'h000_4000_0000_0000 ;
	end
	{2'b01, `W_BA5_ADDR}:   // w_reg_select_dec bit 47
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ba5_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] & 
    														 wb_am5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):1]  = 0 ;
    	w_conf_data_out[0] = wb_ba5_bit0 ;
		w_reg_select_dec = 57'h000_8000_0000_0000 ;
	end
	{2'b01, `W_AM5_ADDR}:   // w_reg_select_dec bit 48
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_am5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h001_0000_0000_0000 ;
	end
	{2'b01, `W_TA5_ADDR}:   // w_reg_select_dec bit 49
	begin
    	w_conf_data_out[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] = wb_ta5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
    	w_conf_data_out[(31-`WB_NUM_OF_DEC_ADDR_LINES):0]  = 0 ;
		w_reg_select_dec = 57'h002_0000_0000_0000 ;
	end
	{2'b01, `W_ERR_CS_ADDR}:   // w_reg_select_dec bit 50
	begin
		w_conf_data_out = { wb_err_cs_bit31_24, /*13*/14'h0000, /*wb_err_cs_bit10,*/
    					    wb_err_cs_bit9, wb_err_cs_bit8, 7'h00, wb_err_cs_bit0 } ;
		w_reg_select_dec = 57'h004_0000_0000_0000 ;
	end
	{2'b01, `W_ERR_ADDR_ADDR}:   // w_reg_select_dec bit 51
	begin
		w_conf_data_out = wb_err_addr ;
		w_reg_select_dec = 57'h008_0000_0000_0000 ;
	end
	{2'b01, `W_ERR_DATA_ADDR}:   // w_reg_select_dec bit 52
	begin
		w_conf_data_out = wb_err_data ;
		w_reg_select_dec = 57'h010_0000_0000_0000 ;
	end
	{2'b01, `CNF_ADDR_ADDR}:   // w_reg_select_dec bit 53
	begin
		w_conf_data_out = { 8'h00, cnf_addr_bit23_2, 1'h0, cnf_addr_bit0 } ;
		w_reg_select_dec = 57'h020_0000_0000_0000 ;
	end
		// `CNF_DATA_ADDR: implemented elsewhere !!!
		// `INT_ACK_ADDR: implemented elsewhere !!!
    {2'b01, `ICR_ADDR}:   // w_reg_select_dec bit 54
	begin
		w_conf_data_out = { icr_bit31, 26'h0000_000, icr_bit4_3, icr_bit2_0 } ;
		w_reg_select_dec = 57'h040_0000_0000_0000 ;
	end
    {2'b01, `ISR_ADDR}:   // w_reg_select_dec bit 55
	begin
		w_conf_data_out = { 27'h0000_000, isr_bit4_3, isr_bit2_0 } ;
		w_reg_select_dec = 57'h080_0000_0000_0000 ;
	end

`ifdef PCI_SPOCI
    8'hff:
    begin
        w_conf_data_out = {spoci_cs_nack, 5'h0, spoci_cs_write, spoci_cs_read,
                           5'h0, spoci_cs_adr[10:8],
                           spoci_cs_adr[7:0],
                           spoci_cs_dat[7:0]} ;

        // this register is implemented separate from other registers, because
        // it has special features implemented
        w_reg_select_dec = 57'h000_0000_0000_0000 ;
    end
`endif

	default:
	begin
		w_conf_data_out = 32'h0000_0000 ;
		w_reg_select_dec = 57'h000_0000_0000_0000 ;
	end
	endcase
end

`ifdef PCI_SPOCI
reg init_we ;
reg init_cfg_done ;
reg [31: 0] spoci_dat ;
wire [31: 0] w_conf_data = init_cfg_done ? w_conf_data_in : spoci_dat ;
wire [ 3: 0] w_byte_en   = init_cfg_done ? w_byte_en_in   : 4'b0000   ;
`else
wire init_we        = 1'b0  ;
wire init_cfg_done  = 1'b1  ;
wire [31: 0] w_conf_data    = w_conf_data_in ;
wire [ 3: 0] w_byte_en      = w_byte_en_in   ;
wire [31: 0] spoci_dat      = 'h0000_0000    ;
`endif

// Reduced write data for BASE, MASK and TRANSLATION registers of PCI and WB images
assign	w_conf_pdata_reduced[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)]	= w_conf_data[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign	w_conf_pdata_reduced[(31-`PCI_NUM_OF_DEC_ADDR_LINES): 0]	= 0 ;
assign	w_conf_wdata_reduced[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)]	= w_conf_data[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign	w_conf_wdata_reduced[(31-`WB_NUM_OF_DEC_ADDR_LINES): 0]	= 0 ;

wire w_we = w_we_i | init_we ;

always@(posedge w_clock or posedge reset)
begin
	// Here are implemented all registers that are reset with RESET signal otherwise they can be normaly written!!!
	// Registers that are commented are implemented after this alwasy statement, because they are e.g. reset with
	//   RESET signal, set with some status signal and they are erased with writting '1' into them !!!
	if (reset)
	begin
		/*status_bit15_11 ; status_bit8 ;*/ command_bit8 <= 1'h0 ; command_bit6 <= 1'h0 ; command_bit2_0 <= 3'h0 ;
		latency_timer <= 8'h00 ; cache_line_size_reg <= 8'h00 ;
		// ALL pci_base address registers are the same as pci_baX registers !
		interrupt_line <= 8'h00 ;

		`ifdef		HOST
		  `ifdef	NO_CNF_IMAGE	// if PCI bridge is HOST and IMAGE0 is assigned as general image space
		 	`ifdef	PCI_IMAGE0	 
			        pci_img_ctrl0_bit2_1 <= {`PCI_AT_EN0, 1'b0} ;
					pci_ba0_bit31_8 <= 24'h0000_00 ;
					pci_ba0_bit0 <= `PCI_BA0_MEM_IO ;
					pci_am0 <= `PCI_AM0 ; 
					pci_ta0 <= `PCI_TA0 ;//fr2201 translation address 
		 	`endif
		  `else
					pci_ba0_bit31_8 <= 24'h0000_00 ;
		  `endif
	 	`endif

        `ifdef GUEST
					pci_ba0_bit31_8 <= 24'h0000_00 ;
		`endif

		pci_img_ctrl1_bit2_1 <= {`PCI_AT_EN1, 1'b0} ;

		pci_ba1_bit31_8 <= 24'h0000_00 ; 
	`ifdef	HOST
		pci_ba1_bit0 <= `PCI_BA1_MEM_IO ;
	`endif
		pci_am1 <= `PCI_AM1;
		pci_ta1 <=  `PCI_TA1 ;//FR2201 translation address ;
		`ifdef	PCI_IMAGE2
			
			        pci_img_ctrl2_bit2_1 <= {`PCI_AT_EN2, 1'b0} ;

					pci_ba2_bit31_8 <= 24'h0000_00 ; 
			`ifdef	HOST
					pci_ba2_bit0 <= `PCI_BA2_MEM_IO ;
			`endif
					pci_am2 <= `PCI_AM2;
					pci_ta2 <= `PCI_TA2 ;//FR2201 translation address ;
		`endif
		`ifdef	PCI_IMAGE3
			
			        pci_img_ctrl3_bit2_1 <= {`PCI_AT_EN3, 1'b0} ; //FR2201 when defined enabled
            
        			pci_ba3_bit31_8 <= 24'h0000_00 ; 
        	`ifdef	HOST
        			pci_ba3_bit0 <= `PCI_BA3_MEM_IO ;
        	`endif
        			pci_am3 <= `PCI_AM3;
					pci_ta3 <= `PCI_TA3 ;//FR2201 translation address ;
		`endif
		`ifdef	PCI_IMAGE4
			
			        pci_img_ctrl4_bit2_1 <= {`PCI_AT_EN4, 1'b0} ; //FR2201 when defined enabled
        	
					pci_ba4_bit31_8 <= 24'h0000_00 ; 
			`ifdef	HOST
					pci_ba4_bit0 <= `PCI_BA4_MEM_IO ;
			`endif
					pci_am4 <= `PCI_AM4;
					pci_ta4 <= `PCI_TA4 ;//FR2201  translation address ;
		`endif
		`ifdef	PCI_IMAGE5
			
			        pci_img_ctrl5_bit2_1 <= {`PCI_AT_EN5, 1'b0} ; //FR2201 when defined enabled
        	
					pci_ba5_bit31_8 <= 24'h0000_00 ; 
			`ifdef	HOST
					pci_ba5_bit0 <= `PCI_BA5_MEM_IO ;
			`endif
					pci_am5 <= `PCI_AM5; //FR2201  pci_am0 
					pci_ta5 <= `PCI_TA5 ;//FR2201  translation address ;
		`endif
		/*pci_err_cs_bit31_24 ; pci_err_cs_bit10; pci_err_cs_bit9 ; pci_err_cs_bit8 ;*/ pci_err_cs_bit0 <= 1'h0 ;
		/*pci_err_addr ;*/
        /*pci_err_data ;*/
		//
		wb_img_ctrl1_bit2_0 <= {`WB_AT_EN1, 2'b00} ;
        
		wb_ba1_bit31_12 <=`WB_BA1; //FR2201 Address bar 
		wb_ba1_bit0 <=`WB_BA1_MEM_IO;//
		wb_am1 <= `WB_AM1 ;//FR2201 Address mask 
		wb_ta1 <= `WB_TA1 ;//FR2201 20'h0000_0 ;
        `ifdef	WB_IMAGE2
			        wb_img_ctrl2_bit2_0 <= {`WB_AT_EN2, 2'b00} ; 
        	 
					wb_ba2_bit31_12 <=`WB_BA2; //FR2201 Address bar  
					wb_ba2_bit0 <=`WB_BA2_MEM_IO;//
					wb_am2 <=`WB_AM2 ;//FR2201 Address mask
					wb_ta2 <=`WB_TA2 ;//FR2201 translation address ;
		`endif
		`ifdef	WB_IMAGE3
			        wb_img_ctrl3_bit2_0 <= {`WB_AT_EN3, 2'b00} ; 
        	 
					wb_ba3_bit31_12 <=`WB_BA3; //FR2201 Address bar  
					wb_ba3_bit0 <=`WB_BA3_MEM_IO;//
					wb_am3 <=`WB_AM3 ;//FR2201 Address mask
					wb_ta3 <=`WB_TA3 ;//FR2201 translation address ;
		`endif
		`ifdef	WB_IMAGE4
			        wb_img_ctrl4_bit2_0 <= {`WB_AT_EN4, 2'b00} ; 
        	 
					wb_ba4_bit31_12 <=`WB_BA4; //FR2201 Address bar 
					wb_ba4_bit0 <=`WB_BA4_MEM_IO;//
					wb_am4 <=`WB_AM4 ;//FR2201 Address mask
					wb_ta4 <=`WB_TA4 ;//FR2201 translation address ;
		`endif
		`ifdef	WB_IMAGE5
			        wb_img_ctrl5_bit2_0 <= {`WB_AT_EN5, 2'b00} ;
        	 
        			wb_ba5_bit31_12 <=`WB_BA5; //FR2201 Address bar  ;
        			wb_ba5_bit0 <=`WB_BA5_MEM_IO;//FR2201 1'h0 ;
					wb_am5 <=`WB_AM5 ;//FR2201  Address mask
					wb_ta5 <=`WB_TA5 ;//FR2201  translation address ;
		`endif
		/*wb_err_cs_bit31_24 ; wb_err_cs_bit10 ; wb_err_cs_bit9 ; wb_err_cs_bit8 ;*/ wb_err_cs_bit0 <= 1'h0 ;
		/*wb_err_addr ;*/
		/*wb_err_data ;*/

		`ifdef		HOST
        	cnf_addr_bit23_2 <= 22'h0000_00 ; cnf_addr_bit0 <= 1'h0 ;
		`endif

		icr_bit31 <= 1'h0 ;
		`ifdef	HOST
			icr_bit2_0 <= 3'h0 ;
			icr_bit4_3 <= 2'h0 ;
		`else
			icr_bit2_0[2:0] <= 3'h0 ;
		`endif
		/*isr_bit4_3 ; isr_bit2_0 ;*/

        // Not register bit; used only internally after reset!
        init_complete <= 1'b0 ;

    `ifdef GUEST
        rst_inactive_sync <= 1'b0 ;
        rst_inactive      <= 1'b0 ;
    `endif

        `ifdef PCI_CPCI_HS_IMPLEMENT
            /*hs_ins hs_ext*/ hs_loo <= 1'b0; hs_eim <= 1'b0;
            // Not register bits; used only internally after reset!
            /*hs_ins_armed hs_ext_armed*/
        `endif
	end
/* -----------------------------------------------------------------------------------------------------------
Following register bits should have asynchronous RESET & SET! That is why they are IMPLEMENTED separately
after this ALWAYS block!!! (for every register bit, there are two D-FF implemented)
		status_bit15_11[15] <= 1'b1 ;
		status_bit15_11[14] <= 1'b1 ;
		status_bit15_11[13] <= 1'b1 ;
		status_bit15_11[12] <= 1'b1 ;
		status_bit15_11[11] <= 1'b1 ;
		status_bit8 <= 1'b1 ;
		pci_err_cs_bit10 <= 1'b1 ;
		pci_err_cs_bit9 <= 1'b1 ;
		pci_err_cs_bit8 <= 1'b1 ;
		pci_err_cs_bit31_24 <= { pci_error_be, pci_error_bc } ;
		pci_err_addr <= pci_error_addr ;
		pci_err_data <= pci_error_data ;
		wb_err_cs_bit10 <= 1'b1 ;
		wb_err_cs_bit9 <= 1'b1 ;
		wb_err_cs_bit8 <= 1'b1 ;
		wb_err_cs_bit31_24 <= { wb_error_be, wb_error_bc } ;
		wb_err_addr <= wb_error_addr ;
		wb_err_data <= wb_error_data ;
		isr_bit4_0[4] <= 1'b1 & icr_bit4_0[4] ;
		isr_bit4_0[3] <= 1'b1 & icr_bit4_0[3] ;
		isr_bit4_0[2] <= 1'b1 & icr_bit4_0[2] ;
		isr_bit4_0[1] <= 1'b1 & icr_bit4_0[1] ;
		isr_bit4_0[0] <= 1'b1 & icr_bit4_0[0] ;

        hs_ins; hs_ext;
-----------------------------------------------------------------------------------------------------------*/
	// Here follows normal writting to registers (only to their valid bits) !
	else
	begin
		if (w_we)
		begin
				// PCI header - configuration space
				if (w_reg_select_dec[0]) // w_conf_address[5:2] = 4'h1:
				begin
					if (~w_byte_en[1])
						command_bit8 <= w_conf_data[8] ;
					if (~w_byte_en[0])
					begin
						command_bit6 <= w_conf_data[6] ;
						command_bit2_0 <= w_conf_data[2:0] ;
					end
				end
				if (w_reg_select_dec[1]) // w_conf_address[5:2] = 4'h3:
				begin
					if (~w_byte_en[1])
						latency_timer <= w_conf_data[15:8] ;
					if (~w_byte_en[0])
						cache_line_size_reg <= w_conf_data[7:0] ;
				end
//	            if (w_reg_select_dec[4]) // w_conf_address[5:2] = 4'h4:
//				Also used with IMAGE0

//	            if (w_reg_select_dec[8]) // w_conf_address[5:2] = 4'h5:
//				Also used with IMAGE1

//	            if (w_reg_select_dec[12]) // w_conf_address[5:2] = 4'h6:
//				Also used with IMAGE2

//	            if (w_reg_select_dec[16]) // w_conf_address[5:2] = 4'h7:
//				Also used with IMAGE3

//	            if (w_reg_select_dec[20]) // w_conf_address[5:2] = 4'h8:
//				Also used with IMAGE4

//	            if (w_reg_select_dec[24]) // w_conf_address[5:2] = 4'h9:
//				Also used with IMAGE5 and IMAGE6
				if (w_reg_select_dec[2]) // w_conf_address[5:2] = 4'hf:
				begin
					if (~w_byte_en[0])
						interrupt_line <= w_conf_data[7:0] ;
				end
				// PCI target - configuration space
`ifdef		HOST
  `ifdef	NO_CNF_IMAGE
	`ifdef	PCI_IMAGE0	// if PCI bridge is HOST and IMAGE0 is assigned as general image space
				if (w_reg_select_dec[3]) // case (w_conf_address[7:2]) = `P_IMG_CTRL0_ADDR:
				begin
					if (~w_byte_en[0])
						pci_img_ctrl0_bit2_1 <= w_conf_data[2:1] ;
				end
	            if (w_reg_select_dec[4]) // case (w_conf_address[7:2]) = `P_BA0_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba0_bit31_8[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ba0_bit31_8[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ba0_bit31_8[15: 8] <= w_conf_pdata_reduced[15: 8] ;
					if (~w_byte_en[0])
						pci_ba0_bit0 <= w_conf_data[0] ;
				end
	            if (w_reg_select_dec[5]) // case (w_conf_address[7:2]) = `P_AM0_ADDR:
				begin
					if (~w_byte_en[3])
						pci_am0[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_am0[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_am0[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	            if (w_reg_select_dec[6]) // case (w_conf_address[7:2]) = `P_TA0_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ta0[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ta0[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ta0[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	`endif
  `else
	            if (w_reg_select_dec[4]) // case (w_conf_address[7:2]) = `P_BA0_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba0_bit31_8[31:24] <= w_conf_data[31:24] ;
					if (~w_byte_en[2])
						pci_ba0_bit31_8[23:16] <= w_conf_data[23:16] ;
					if (~w_byte_en[1])
						pci_ba0_bit31_8[15:12] <= w_conf_data[15:12] ;
				end
  `endif
`endif

`ifdef GUEST
	            if (w_reg_select_dec[4]) // case (w_conf_address[7:2]) = `P_BA0_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba0_bit31_8[31:24] <= w_conf_data[31:24] ;
					if (~w_byte_en[2])
						pci_ba0_bit31_8[23:16] <= w_conf_data[23:16] ;
					if (~w_byte_en[1])
						pci_ba0_bit31_8[15:12] <= w_conf_data[15:12] ;
				end
`endif
	            if (w_reg_select_dec[7]) // case (w_conf_address[7:2]) = `P_IMG_CTRL1_ADDR:
				begin
					if (~w_byte_en[0])
						pci_img_ctrl1_bit2_1 <= w_conf_data[2:1] ;
				end
	            if (w_reg_select_dec[8]) // case (w_conf_address[7:2]) = `P_BA1_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba1_bit31_8[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ba1_bit31_8[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ba1_bit31_8[15: 8] <= w_conf_pdata_reduced[15: 8] ;
	`ifdef	HOST
					if (~w_byte_en[0])
						pci_ba1_bit0 <= w_conf_data[0] ;
	`endif
				end
	            if (w_reg_select_dec[9]) // case (w_conf_address[7:2]) = `P_AM1_ADDR:
				begin
					if (~w_byte_en[3])
						pci_am1[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_am1[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_am1[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	            if (w_reg_select_dec[10]) // case (w_conf_address[7:2]) = `P_TA1_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ta1[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ta1[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ta1[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
`ifdef		PCI_IMAGE2
	            if (w_reg_select_dec[11]) // case (w_conf_address[7:2]) = `P_IMG_CTRL2_ADDR:
				begin
					if (~w_byte_en[0])
						pci_img_ctrl2_bit2_1 <= w_conf_data[2:1] ;
				end
	            if (w_reg_select_dec[12]) // case (w_conf_address[7:2]) = `P_BA2_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba2_bit31_8[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ba2_bit31_8[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ba2_bit31_8[15: 8] <= w_conf_pdata_reduced[15: 8] ;
	`ifdef	HOST
					if (~w_byte_en[0])
						pci_ba2_bit0 <= w_conf_data[0] ;
	`endif
				end
	            if (w_reg_select_dec[13]) // case (w_conf_address[7:2]) = `P_AM2_ADDR:
				begin
					if (~w_byte_en[3])
						pci_am2[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_am2[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_am2[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	            if (w_reg_select_dec[14]) // case (w_conf_address[7:2]) = `P_TA2_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ta2[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ta2[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ta2[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
`endif
`ifdef		PCI_IMAGE3
	            if (w_reg_select_dec[15]) // case (w_conf_address[7:2]) = `P_IMG_CTRL3_ADDR:
				begin
					if (~w_byte_en[0])
						pci_img_ctrl3_bit2_1 <= w_conf_data[2:1] ;
				end
	            if (w_reg_select_dec[16]) // case (w_conf_address[7:2]) = `P_BA3_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba3_bit31_8[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ba3_bit31_8[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ba3_bit31_8[15: 8] <= w_conf_pdata_reduced[15: 8] ;
	`ifdef	HOST
					if (~w_byte_en[0])
						pci_ba3_bit0 <= w_conf_data[0] ;
	`endif
				end
	            if (w_reg_select_dec[17]) // case (w_conf_address[7:2]) = `P_AM3_ADDR:
				begin
					if (~w_byte_en[3])
						pci_am3[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_am3[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_am3[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	            if (w_reg_select_dec[18]) // case (w_conf_address[7:2]) = `P_TA3_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ta3[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ta3[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ta3[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
`endif
`ifdef		PCI_IMAGE4
	            if (w_reg_select_dec[19]) // case (w_conf_address[7:2]) = `P_IMG_CTRL4_ADDR:
				begin
					if (~w_byte_en[0])
						pci_img_ctrl4_bit2_1 <= w_conf_data[2:1] ;
				end
	            if (w_reg_select_dec[20]) // case (w_conf_address[7:2]) = `P_BA4_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba4_bit31_8[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ba4_bit31_8[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ba4_bit31_8[15: 8] <= w_conf_pdata_reduced[15: 8] ;
	`ifdef	HOST
					if (~w_byte_en[0])
						pci_ba4_bit0 <= w_conf_data[0] ;
	`endif
				end
	            if (w_reg_select_dec[21]) // case (w_conf_address[7:2]) = `P_AM4_ADDR:
				begin
					if (~w_byte_en[3])
						pci_am4[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_am4[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_am4[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	            if (w_reg_select_dec[22]) // case (w_conf_address[7:2]) = `P_TA4_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ta4[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ta4[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ta4[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
`endif
`ifdef		PCI_IMAGE5
	            if (w_reg_select_dec[23]) // case (w_conf_address[7:2]) = `P_IMG_CTRL5_ADDR:
				begin
					if (~w_byte_en[0])
						pci_img_ctrl5_bit2_1 <= w_conf_data[2:1] ;
				end
	            if (w_reg_select_dec[24]) // case (w_conf_address[7:2]) = `P_BA5_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ba5_bit31_8[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ba5_bit31_8[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ba5_bit31_8[15: 8] <= w_conf_pdata_reduced[15: 8] ;
	`ifdef	HOST
					if (~w_byte_en[0])
						pci_ba5_bit0 <= w_conf_data[0] ;
	`endif
				end
	            if (w_reg_select_dec[25]) // case (w_conf_address[7:2]) = `P_AM5_ADDR:
				begin
					if (~w_byte_en[3])
						pci_am5[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_am5[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_am5[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
	            if (w_reg_select_dec[26]) // case (w_conf_address[7:2]) = `P_TA5_ADDR:
				begin
					if (~w_byte_en[3])
						pci_ta5[31:24] <= w_conf_pdata_reduced[31:24] ;
					if (~w_byte_en[2])
						pci_ta5[23:16] <= w_conf_pdata_reduced[23:16] ;
					if (~w_byte_en[1])
						pci_ta5[15: 8] <= w_conf_pdata_reduced[15: 8] ;
				end
`endif
	            if (w_reg_select_dec[27]) // case (w_conf_address[7:2]) = `P_ERR_CS_ADDR:
				begin
					if (~w_byte_en[0])
						pci_err_cs_bit0 <= w_conf_data[0] ;
				end
			// WB slave - configuration space
				if (w_reg_select_dec[30]) // case (w_conf_address[7:2]) = `W_IMG_CTRL1_ADDR:
				begin
					if (~w_byte_en[0])
						wb_img_ctrl1_bit2_0 <= w_conf_data[2:0] ;
				end
				if (w_reg_select_dec[31]) // case (w_conf_address[7:2]) = `W_BA1_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ba1_bit31_12[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ba1_bit31_12[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ba1_bit31_12[15:12] <= w_conf_wdata_reduced[15:12] ;
					if (~w_byte_en[0])
						wb_ba1_bit0 <= w_conf_data[0] ;
				end
				if (w_reg_select_dec[32]) // case (w_conf_address[7:2]) = `W_AM1_ADDR:
				begin
					if (~w_byte_en[3])
						wb_am1[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_am1[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_am1[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
				if (w_reg_select_dec[33]) // case (w_conf_address[7:2]) = `W_TA1_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ta1[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ta1[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ta1[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
`ifdef		WB_IMAGE2
				if (w_reg_select_dec[34]) // case (w_conf_address[7:2]) = `W_IMG_CTRL2_ADDR:
				begin
					if (~w_byte_en[0])
						wb_img_ctrl2_bit2_0 <= w_conf_data[2:0] ;
				end
				if (w_reg_select_dec[35]) // case (w_conf_address[7:2]) = `W_BA2_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ba2_bit31_12[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ba2_bit31_12[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ba2_bit31_12[15:12] <= w_conf_wdata_reduced[15:12] ;
					if (~w_byte_en[0])
						wb_ba2_bit0 <= w_conf_data[0] ;
				end
				if (w_reg_select_dec[36]) // case (w_conf_address[7:2]) = `W_AM2_ADDR:
				begin
					if (~w_byte_en[3])
						wb_am2[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_am2[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_am2[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
				if (w_reg_select_dec[37]) // case (w_conf_address[7:2]) = `W_TA2_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ta2[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ta2[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ta2[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
`endif
`ifdef		WB_IMAGE3
				if (w_reg_select_dec[38]) // case (w_conf_address[7:2]) = `W_IMG_CTRL3_ADDR:
				begin
					if (~w_byte_en[0])
						wb_img_ctrl3_bit2_0 <= w_conf_data[2:0] ;
				end
				if (w_reg_select_dec[39]) // case (w_conf_address[7:2]) = `W_BA3_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ba3_bit31_12[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ba3_bit31_12[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ba3_bit31_12[15:12] <= w_conf_wdata_reduced[15:12] ;
					if (~w_byte_en[0])
						wb_ba3_bit0 <= w_conf_data[0] ;
				end
				if (w_reg_select_dec[40]) // case (w_conf_address[7:2]) = `W_AM3_ADDR:
				begin
					if (~w_byte_en[3])
						wb_am3[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_am3[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_am3[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
				if (w_reg_select_dec[41]) // case (w_conf_address[7:2]) = `W_TA3_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ta3[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ta3[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ta3[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
`endif
`ifdef		WB_IMAGE4
				if (w_reg_select_dec[42]) // case (w_conf_address[7:2]) = `W_IMG_CTRL4_ADDR:
				begin
					if (~w_byte_en[0])
						wb_img_ctrl4_bit2_0 <= w_conf_data[2:0] ;
				end
				if (w_reg_select_dec[43]) // case (w_conf_address[7:2]) = `W_BA4_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ba4_bit31_12[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ba4_bit31_12[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ba4_bit31_12[15:12] <= w_conf_wdata_reduced[15:12] ;
					if (~w_byte_en[0])
						wb_ba4_bit0 <= w_conf_data[0] ;
				end
				if (w_reg_select_dec[44]) // case (w_conf_address[7:2]) = `W_AM4_ADDR:
				begin
					if (~w_byte_en[3])
						wb_am4[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_am4[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_am4[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
				if (w_reg_select_dec[45]) // case (w_conf_address[7:2]) = `W_TA4_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ta4[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ta4[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ta4[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
`endif
`ifdef		WB_IMAGE5
				if (w_reg_select_dec[46]) // case (w_conf_address[7:2]) = `W_IMG_CTRL5_ADDR:
				begin
					if (~w_byte_en[0])
						wb_img_ctrl5_bit2_0 <= w_conf_data[2:0] ;
				end
				if (w_reg_select_dec[47]) // case (w_conf_address[7:2]) = `W_BA5_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ba5_bit31_12[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ba5_bit31_12[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ba5_bit31_12[15:12] <= w_conf_wdata_reduced[15:12] ;
					if (~w_byte_en[0])
						wb_ba5_bit0 <= w_conf_data[0] ;
				end
				if (w_reg_select_dec[48]) // case (w_conf_address[7:2]) = `W_AM5_ADDR:
				begin
					if (~w_byte_en[3])
						wb_am5[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_am5[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_am5[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
				if (w_reg_select_dec[49]) // case (w_conf_address[7:2]) = `W_TA5_ADDR:
				begin
					if (~w_byte_en[3])
						wb_ta5[31:24] <= w_conf_wdata_reduced[31:24] ;
					if (~w_byte_en[2])
						wb_ta5[23:16] <= w_conf_wdata_reduced[23:16] ;
					if (~w_byte_en[1])
						wb_ta5[15:12] <= w_conf_wdata_reduced[15:12] ;
				end
`endif
				if (w_reg_select_dec[50]) // case (w_conf_address[7:2]) = `W_ERR_CS_ADDR:
				begin
					if (~w_byte_en[0])
						wb_err_cs_bit0 <= w_conf_data[0] ;
				end

`ifdef	HOST
				if (w_reg_select_dec[53]) // case (w_conf_address[7:2]) = `CNF_ADDR_ADDR:
				begin
					if (~w_byte_en[2])
						cnf_addr_bit23_2[23:16] <= w_conf_data[23:16] ;
					if (~w_byte_en[1])
						cnf_addr_bit23_2[15:8] <= w_conf_data[15:8] ;
					if (~w_byte_en[0])
					begin
						cnf_addr_bit23_2[7:2] <= w_conf_data[7:2] ;
						cnf_addr_bit0 <= w_conf_data[0] ;
					end
				end
`endif
				// `CNF_DATA_ADDR: implemented elsewhere !!!
				// `INT_ACK_ADDR : implemented elsewhere !!!
	            if (w_reg_select_dec[54]) // case (w_conf_address[7:2]) = `ICR_ADDR:
				begin
					if (~w_byte_en[3])
						icr_bit31 <= w_conf_data[31] ;

					if (~w_byte_en[0])
                    begin
`ifdef	HOST
						icr_bit4_3 <= w_conf_data[4:3] ;
						icr_bit2_0 <= w_conf_data[2:0] ;
`else
						icr_bit2_0[2:0] <= w_conf_data[2:0] ;
`endif
                    end
                end

`ifdef PCI_CPCI_HS_IMPLEMENT
                if (w_reg_select_dec[56])
                begin
                    if (~w_byte_en[2])
                    begin
                        hs_loo <= w_conf_data[19];
                        hs_eim <= w_conf_data[17];
                    end
                end
`endif
		end // end of we

        // Not register bits; used only internally after reset!
    `ifdef GUEST
        rst_inactive_sync <= 1'b1               ;
        rst_inactive      <= rst_inactive_sync  ;
    `endif

        if (rst_inactive & ~init_complete & init_cfg_done)
            init_complete <= 1'b1 ;
	end
end

// implementation of read only device identification registers
always@(posedge w_clock or posedge reset)
begin
    if (reset)
    begin
        r_vendor_id         <= `HEADER_VENDOR_ID        ;
        r_device_id         <= `HEADER_DEVICE_ID        ;
        r_revision_id       <= `HEADER_REVISION_ID      ;
        r_subsys_vendor_id  <= `HEADER_SUBSYS_VENDOR_ID ;
        r_subsys_id         <= `HEADER_SUBSYS_ID        ;
        r_max_lat           <= `HEADER_MAX_LAT          ;
        r_min_gnt           <= `HEADER_MIN_GNT          ;
    end else
    begin
        if (init_we)
        begin
            if (spoci_reg_num == 'h0)
            begin
                r_vendor_id <= spoci_dat[15: 0] ;
                r_device_id <= spoci_dat[31:16] ;
            end

            if (spoci_reg_num == 'hB)
            begin
                r_subsys_vendor_id  <= spoci_dat[15: 0] ;
                r_subsys_id         <= spoci_dat[31:16] ;
            end

            if (spoci_reg_num == 'h2)
            begin
                r_revision_id   <= spoci_dat[ 7: 0] ;
            end

            if (spoci_reg_num == 'hF)
            begin
                r_max_lat <= spoci_dat[31:24] ;
                r_min_gnt <= spoci_dat[23:16] ;
            end
        end        
    end
end

// This signals are synchronous resets for registers, whic occures when asynchronous RESET is '1' or
// data '1' is synchronously written into them!
reg			delete_status_bit15 ;
reg			delete_status_bit14 ;
reg			delete_status_bit13 ;
reg			delete_status_bit12 ;
reg			delete_status_bit11 ;
reg			delete_status_bit8 ;
reg			delete_pci_err_cs_bit8 ;
reg			delete_wb_err_cs_bit8 ;
reg			delete_isr_bit4 ;
reg			delete_isr_bit3 ;
reg			delete_isr_bit2 ;
reg			delete_isr_bit1 ;

// This are aditional register bits, which are resets when their value is '1' !!!
always@(w_we or w_reg_select_dec or w_conf_data or w_byte_en)
begin
// I' is written into, then it also sets signals to '1'
	delete_status_bit15 	= w_conf_data[31] & !w_byte_en[3] & w_we & w_reg_select_dec[0] ;
	delete_status_bit14 	= w_conf_data[30] & !w_byte_en[3] & w_we & w_reg_select_dec[0] ;
	delete_status_bit13 	= w_conf_data[29] & !w_byte_en[3] & w_we & w_reg_select_dec[0] ;
	delete_status_bit12 	= w_conf_data[28] & !w_byte_en[3] & w_we & w_reg_select_dec[0] ;
	delete_status_bit11 	= w_conf_data[27] & !w_byte_en[3] & w_we & w_reg_select_dec[0] ;
	delete_status_bit8  	= w_conf_data[24] & !w_byte_en[3] & w_we & w_reg_select_dec[0] ;
	delete_pci_err_cs_bit8 	= w_conf_data[8]  & !w_byte_en[1] & w_we & w_reg_select_dec[27] ;
	delete_wb_err_cs_bit8 	= w_conf_data[8]  & !w_byte_en[1] & w_we & w_reg_select_dec[50] ;
	delete_isr_bit4 		= w_conf_data[4]  & !w_byte_en[0] & w_we & w_reg_select_dec[55] ;
	delete_isr_bit3 		= w_conf_data[3]  & !w_byte_en[0] & w_we & w_reg_select_dec[55] ;
	delete_isr_bit2 		= w_conf_data[2]  & !w_byte_en[0] & w_we & w_reg_select_dec[55] ;
	delete_isr_bit1 		= w_conf_data[1]  & !w_byte_en[0] & w_we & w_reg_select_dec[55] ;
end

// STATUS BITS of PCI Header status register
`ifdef SYNCHRONEOUS_CLOCK_DOMAINS
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[15] <= 1'b0 ;
		else
		begin
			if (perr_in) // Synchronous set
				status_bit15_11[15] <= 1'b1 ;
			else if (delete_status_bit15) // Synchronous reset
				status_bit15_11[15] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[14] <= 1'b0 ;
		else
		begin
			if (serr_in) // Synchronous set
				status_bit15_11[14] <= 1'b1 ;
			else if (delete_status_bit14) // Synchronous reset
				status_bit15_11[14] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[13] <= 1'b0 ;
		else
		begin
			if (master_abort_recv) // Synchronous set
				status_bit15_11[13] <= 1'b1 ;
			else if (delete_status_bit13) // Synchronous reset
				status_bit15_11[13] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[12] <= 1'b0 ;
		else
		begin
			if (target_abort_recv) // Synchronous set
				status_bit15_11[12] <= 1'b1 ;
			else if (delete_status_bit12) // Synchronous reset
				status_bit15_11[12] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[11] <= 1'b0 ;
		else
		begin
			if (target_abort_set) // Synchronous set
				status_bit15_11[11] <= 1'b1 ;
			else if (delete_status_bit11) // Synchronous reset
				status_bit15_11[11] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit8 <= 1'b0 ;
		else
		begin
			if (master_data_par_err) // Synchronous set
				status_bit8 <= 1'b1 ;
			else if (delete_status_bit8) // Synchronous reset
				status_bit8 <= 1'b0 ;
		end
	end
`else // not SYNCHRONEOUS_CLOCK_DOMAINS
  `ifdef HOST
	reg		[15:11]	set_status_bit15_11;
	reg		set_status_bit8;
	wire	delete_set_status_bit15;
	wire	delete_set_status_bit14;
	wire	delete_set_status_bit13;
	wire	delete_set_status_bit12;
	wire	delete_set_status_bit11;
	wire	delete_set_status_bit8;
	wire	block_set_status_bit15;
	wire	block_set_status_bit14;
	wire	block_set_status_bit13;
	wire	block_set_status_bit12;
	wire	block_set_status_bit11;
	wire	block_set_status_bit8;
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_status_15
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_status_bit15),
		.block_set_out	(block_set_status_bit15),
		.delete_in		(delete_status_bit15)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_status_bit15_11[15] <= 1'b0 ;
		else
		begin
			if (perr_in) // Synchronous set
				set_status_bit15_11[15] <= 1'b1 ;
			else if (delete_set_status_bit15) // Synchronous reset
				set_status_bit15_11[15] <= 1'b0 ;
		end
	end
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_status_14
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_status_bit14),
		.block_set_out	(block_set_status_bit14),
		.delete_in		(delete_status_bit14)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_status_bit15_11[14] <= 1'b0 ;
		else
		begin
			if (serr_in) // Synchronous set
				set_status_bit15_11[14] <= 1'b1 ;
			else if (delete_set_status_bit14) // Synchronous reset
				set_status_bit15_11[14] <= 1'b0 ;
		end
	end
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_status_13
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_status_bit13),
		.block_set_out	(block_set_status_bit13),
		.delete_in		(delete_status_bit13)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_status_bit15_11[13] <= 1'b0 ;
		else
		begin
			if (master_abort_recv) // Synchronous set
				set_status_bit15_11[13] <= 1'b1 ;
			else if (delete_set_status_bit13) // Synchronous reset
				set_status_bit15_11[13] <= 1'b0 ;
		end
	end
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_status_12
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_status_bit12),
		.block_set_out	(block_set_status_bit12),
		.delete_in		(delete_status_bit12)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_status_bit15_11[12] <= 1'b0 ;
		else
		begin
			if (target_abort_recv) // Synchronous set
				set_status_bit15_11[12] <= 1'b1 ;
			else if (delete_set_status_bit12) // Synchronous reset
				set_status_bit15_11[12] <= 1'b0 ;
		end
	end
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_status_11
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_status_bit11),
		.block_set_out	(block_set_status_bit11),
		.delete_in		(delete_status_bit11)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_status_bit15_11[11] <= 1'b0 ;
		else
		begin
			if (target_abort_set) // Synchronous set
				set_status_bit15_11[11] <= 1'b1 ;
			else if (delete_set_status_bit11) // Synchronous reset
				set_status_bit15_11[11] <= 1'b0 ;
		end
	end
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_status_8
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_status_bit8),
		.block_set_out	(block_set_status_bit8),
		.delete_in		(delete_status_bit8)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_status_bit8 <= 1'b0 ;
		else
		begin
			if (master_data_par_err) // Synchronous set
				set_status_bit8 <= 1'b1 ;
			else if (delete_set_status_bit8) // Synchronous reset
				set_status_bit8 <= 1'b0 ;
		end
	end
	wire [5:0] status_bits	=	{set_status_bit15_11[15] && !block_set_status_bit15,
								 set_status_bit15_11[14] && !block_set_status_bit14,
								 set_status_bit15_11[13] && !block_set_status_bit13,
								 set_status_bit15_11[12] && !block_set_status_bit12,
								 set_status_bit15_11[11] && !block_set_status_bit11,
								 set_status_bit8		 && !block_set_status_bit8	} ;
	wire [5:0] meta_status_bits ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(6, 0) status_bits_sync
	(
	    .data_in        (status_bits),
	    .clk_out        (wb_clk),
	    .sync_data_out  (meta_status_bits),
	    .async_reset    (reset)
	) ;
	always@(posedge wb_clk or posedge reset)
	begin
	    if (reset)
	    begin
	        status_bit15_11[15:11]	<= 5'b0 ;
	        status_bit8				<= 1'b0 ;
	    end
	    else
	    begin
	        status_bit15_11[15:11]	<= meta_status_bits[5:1] ;
	        status_bit8				<= meta_status_bits[0] ;
	    end
	end
  `else // GUEST
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[15] <= 1'b0 ;
		else
		begin
			if (perr_in) // Synchronous set
				status_bit15_11[15] <= 1'b1 ;
			else if (delete_status_bit15) // Synchronous reset
				status_bit15_11[15] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[14] <= 1'b0 ;
		else
		begin
			if (serr_in) // Synchronous set
				status_bit15_11[14] <= 1'b1 ;
			else if (delete_status_bit14) // Synchronous reset
				status_bit15_11[14] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[13] <= 1'b0 ;
		else
		begin
			if (master_abort_recv) // Synchronous set
				status_bit15_11[13] <= 1'b1 ;
			else if (delete_status_bit13) // Synchronous reset
				status_bit15_11[13] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[12] <= 1'b0 ;
		else
		begin
			if (target_abort_recv) // Synchronous set
				status_bit15_11[12] <= 1'b1 ;
			else if (delete_status_bit12) // Synchronous reset
				status_bit15_11[12] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit15_11[11] <= 1'b0 ;
		else
		begin
			if (target_abort_set) // Synchronous set
				status_bit15_11[11] <= 1'b1 ;
			else if (delete_status_bit11) // Synchronous reset
				status_bit15_11[11] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			status_bit8 <= 1'b0 ;
		else
		begin
			if (master_data_par_err) // Synchronous set
				status_bit8 <= 1'b1 ;
			else if (delete_status_bit8) // Synchronous reset
				status_bit8 <= 1'b0 ;
		end
	end
  `endif
`endif

// STATUS BITS of P_ERR_CS - PCI error control and status register
`ifdef SYNCHRONEOUS_CLOCK_DOMAINS
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			pci_err_cs_bit8 <= 1'b0 ;
		else
		begin
			if (pci_error_sig && pci_err_cs_bit0) // Synchronous set
				pci_err_cs_bit8 <= 1'b1 ;
			else if (delete_pci_err_cs_bit8) // Synchronous reset
				pci_err_cs_bit8 <= 1'b0 ;
		end
	end
`else // not SYNCHRONEOUS_CLOCK_DOMAINS
  `ifdef HOST
	// Set and clear FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			pci_err_cs_bit8 <= 1'b0 ;
		else
		begin
			if (pci_error_sig && pci_err_cs_bit0) // Synchronous set
				pci_err_cs_bit8 <= 1'b1 ;
			else if (delete_pci_err_cs_bit8) // Synchronous reset
				pci_err_cs_bit8 <= 1'b0 ;
		end
	end
  `else // GUEST
	reg		set_pci_err_cs_bit8;
	wire	delete_set_pci_err_cs_bit8;
	wire	block_set_pci_err_cs_bit8;
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_pci_err_cs_8
	(
		.set_clk_in		(wb_clk),
		.delete_clk_in	(pci_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_pci_err_cs_bit8),
		.block_set_out	(block_set_pci_err_cs_bit8),
		.delete_in		(delete_pci_err_cs_bit8)
	);
	// Setting FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_pci_err_cs_bit8 <= 1'b0 ;
		else
		begin
			if (pci_error_sig && pci_err_cs_bit0) // Synchronous set
				set_pci_err_cs_bit8 <= 1'b1 ;
			else if (delete_set_pci_err_cs_bit8) // Synchronous reset
				set_pci_err_cs_bit8 <= 1'b0 ;
		end
	end
	wire	pci_err_cs_bits = set_pci_err_cs_bit8 && !block_set_pci_err_cs_bit8 ;
	wire	meta_pci_err_cs_bits ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop #(1,0) pci_err_cs_bits_sync
	(
	    .data_in        (pci_err_cs_bits),
	    .clk_out        (pci_clk),
	    .sync_data_out  (meta_pci_err_cs_bits),
	    .async_reset    (reset)
	) ;
	always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
	        pci_err_cs_bit8	<= 1'b0 ;
	    else
	        pci_err_cs_bit8	<= meta_pci_err_cs_bits ;
	end
  `endif
`endif
	// Set and clear FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			pci_err_cs_bit10 <= 1'b0 ;
		else
		begin
			if (pci_error_sig) // Synchronous report
				pci_err_cs_bit10 <= pci_error_rty_exp ;
		end
	end
	// Set and clear FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			pci_err_cs_bit9 <= 1'b0 ;
		else
		begin
			if (pci_error_sig) // Synchronous report
				pci_err_cs_bit9 <= pci_error_es ;
		end
	end
	// Set and clear FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
	    begin
			pci_err_cs_bit31_24 <= 8'h00 ;
			pci_err_addr <= 32'h0000_0000 ;
			pci_err_data <= 32'h0000_0000 ;
	    end
		else
			if (pci_error_sig) // Synchronous report
			begin
				pci_err_cs_bit31_24 <= { pci_error_be, pci_error_bc } ;
				pci_err_addr <= pci_error_addr ;
				pci_err_data <= pci_error_data ;
			end
	end

// STATUS BITS of W_ERR_CS - WB error control and status register
`ifdef SYNCHRONEOUS_CLOCK_DOMAINS
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			wb_err_cs_bit8 <= 1'b0 ;
		else
		begin
			if (wb_error_sig && wb_err_cs_bit0) // Synchronous set
				wb_err_cs_bit8 <= 1'b1 ;
			else if (delete_wb_err_cs_bit8) // Synchronous reset
				wb_err_cs_bit8 <= 1'b0 ;
		end
	end
`else // not SYNCHRONEOUS_CLOCK_DOMAINS
  `ifdef HOST
	reg		set_wb_err_cs_bit8;
	wire	delete_set_wb_err_cs_bit8;
	wire	block_set_wb_err_cs_bit8;
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_wb_err_cs_8
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_wb_err_cs_bit8),
		.block_set_out	(block_set_wb_err_cs_bit8),
		.delete_in		(delete_wb_err_cs_bit8)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_wb_err_cs_bit8 <= 1'b0 ;
		else
		begin
			if (wb_error_sig && wb_err_cs_bit0) // Synchronous set
				set_wb_err_cs_bit8 <= 1'b1 ;
			else if (delete_set_wb_err_cs_bit8) // Synchronous reset
				set_wb_err_cs_bit8 <= 1'b0 ;
		end
	end
	wire	wb_err_cs_bits = set_wb_err_cs_bit8 && !block_set_wb_err_cs_bit8 ;
	wire	meta_wb_err_cs_bits ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop #(1,0) wb_err_cs_bits_sync
	(
	    .data_in        (wb_err_cs_bits),
	    .clk_out        (wb_clk),
	    .sync_data_out  (meta_wb_err_cs_bits),
	    .async_reset    (reset)
	) ;
	always@(posedge wb_clk or posedge reset)
	begin
	    if (reset)
	        wb_err_cs_bit8	<= 1'b0 ;
	    else
	        wb_err_cs_bit8	<= meta_wb_err_cs_bits ;
	end
  `else // GUEST
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			wb_err_cs_bit8 <= 1'b0 ;
		else
		begin
			if (wb_error_sig && wb_err_cs_bit0) // Synchronous set
				wb_err_cs_bit8 <= 1'b1 ;
			else if (delete_wb_err_cs_bit8) // Synchronous reset
				wb_err_cs_bit8 <= 1'b0 ;
		end
	end
  `endif
`endif
/*	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			wb_err_cs_bit10 <= 1'b0 ;
		else
		begin
			if (wb_error_sig) // Synchronous report
				wb_err_cs_bit10 <= wb_error_rty_exp ;
		end
	end */
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			wb_err_cs_bit9 <= 1'b0 ;
		else
		begin
			if (wb_error_sig) // Synchronous report
				wb_err_cs_bit9 <= wb_error_es ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
	    begin
			wb_err_cs_bit31_24 <= 8'h00 ;
			wb_err_addr <= 32'h0000_0000 ;
			wb_err_data <= 32'h0000_0000 ;
	    end
		else
			if (wb_error_sig)
			begin
				wb_err_cs_bit31_24 <= { wb_error_be, wb_error_bc } ;
				wb_err_addr <= wb_error_addr ;
				wb_err_data <= wb_error_data ;
			end
	end

// SERR_INT and PERR_INT STATUS BITS of ISR - interrupt status register
`ifdef SYNCHRONEOUS_CLOCK_DOMAINS
  `ifdef HOST
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			isr_bit4_3[4] <= 1'b0 ;
		else
		begin
			if (isr_sys_err_int && icr_bit4_3[4]) // Synchronous set
				isr_bit4_3[4] <= 1'b1 ;
			else if (delete_isr_bit4) // Synchronous reset
				isr_bit4_3[4] <= 1'b0 ;
		end
	end
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			isr_bit4_3[3] <= 1'b0 ;
		else
		begin
			if (isr_par_err_int && icr_bit4_3[3]) // Synchronous set
				isr_bit4_3[3] <= 1'b1 ;
			else if (delete_isr_bit3) // Synchronous reset
				isr_bit4_3[3] <= 1'b0 ;
		end
	end
  `endif
`else // not SYNCHRONEOUS_CLOCK_DOMAINS
  `ifdef HOST
	reg		[4:3]	set_isr_bit4_3;
	wire	delete_set_isr_bit4;
	wire	delete_set_isr_bit3;
	wire	block_set_isr_bit4;
	wire	block_set_isr_bit3;
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_isr_4
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_isr_bit4),
		.block_set_out	(block_set_isr_bit4),
		.delete_in		(delete_isr_bit4)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_isr_bit4_3[4] <= 1'b0 ;
		else
		begin
			if (isr_sys_err_int && icr_bit4_3[4]) // Synchronous set
				set_isr_bit4_3[4] <= 1'b1 ;
			else if (delete_set_isr_bit4) // Synchronous reset
				set_isr_bit4_3[4] <= 1'b0 ;
		end
	end
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_isr_3
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_isr_bit3),
		.block_set_out	(block_set_isr_bit3),
		.delete_in		(delete_isr_bit3)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_isr_bit4_3[3] <= 1'b0 ;
		else
		begin
			if (isr_par_err_int && icr_bit4_3[3]) // Synchronous set
				set_isr_bit4_3[3] <= 1'b1 ;
			else if (delete_set_isr_bit3) // Synchronous reset
				set_isr_bit4_3[3] <= 1'b0 ;
		end
	end
	wire [4:3] isr_bits4_3	=	{set_isr_bit4_3[4] && !block_set_isr_bit4,
								 set_isr_bit4_3[3] && !block_set_isr_bit3	} ;
	wire [4:3] meta_isr_bits4_3 ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(2, 0) isr_bits_sync
	(
	    .data_in        (isr_bits4_3),
	    .clk_out        (wb_clk),
	    .sync_data_out  (meta_isr_bits4_3),
	    .async_reset    (reset)
	) ;
	always@(posedge wb_clk or posedge reset)
	begin
	    if (reset)
	        isr_bit4_3[4:3]	<= 2'b0 ;
	    else
	        isr_bit4_3[4:3]	<= meta_isr_bits4_3[4:3] ;
	end
  `endif
`endif

// PCI_EINT and WB_EINT STATUS BITS of ISR - interrupt status register
`ifdef SYNCHRONEOUS_CLOCK_DOMAINS
  // WB_EINT STATUS BIT
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			isr_bit2_0[1] <= 1'b0 ;
		else
		begin
			if (wb_error_sig && icr_bit2_0[1] && wb_err_cs_bit0) // Synchronous set
				isr_bit2_0[1] <= 1'b1 ;
			else if (delete_isr_bit1) // Synchronous reset
				isr_bit2_0[1] <= 1'b0 ;
		end
	end
  // PCI_EINT STATUS BIT
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			isr_bit2_0[2] <= 1'b0 ;
		else
		begin
			if (pci_error_sig && icr_bit2_0[2] && pci_err_cs_bit0) // Synchronous set
				isr_bit2_0[2] <= 1'b1 ;
			else if (delete_isr_bit2) // Synchronous reset
				isr_bit2_0[2] <= 1'b0 ;
		end
	end
`else // not SYNCHRONEOUS_CLOCK_DOMAINS
  `ifdef HOST
  // WB_EINT STATUS BIT
	reg		set_isr_bit1;
	wire	delete_set_isr_bit1;
	wire	block_set_isr_bit1;
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_isr_1
	(
		.set_clk_in		(pci_clk),
		.delete_clk_in	(wb_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_isr_bit1),
		.block_set_out	(block_set_isr_bit1),
		.delete_in		(delete_isr_bit1)
	);
	// Setting FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_isr_bit1 <= 1'b0 ;
		else
		begin
			if (wb_error_sig && icr_bit2_0[1] && wb_err_cs_bit0) // Synchronous set
				set_isr_bit1 <= 1'b1 ;
			else if (delete_set_isr_bit1) // Synchronous reset
				set_isr_bit1 <= 1'b0 ;
		end
	end
	wire	isr_bit1	= set_isr_bit1 && !block_set_isr_bit1 ;
	wire	meta_isr_bit1 ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(1, 0) isr_bit1_sync
	(
	    .data_in        (isr_bit1),
	    .clk_out        (wb_clk),
	    .sync_data_out  (meta_isr_bit1),
	    .async_reset    (reset)
	) ;
	always@(posedge wb_clk or posedge reset)
	begin
	    if (reset)
	        isr_bit2_0[1]	<= 1'b0 ;
	    else
	        isr_bit2_0[1]	<= meta_isr_bit1 ;
	end
  // PCI_EINT STATUS BIT
	// Set and clear FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			isr_bit2_0[2] <= 1'b0 ;
		else
		begin
			if (pci_error_sig && icr_bit2_0[2] && pci_err_cs_bit0) // Synchronous set
				isr_bit2_0[2] <= 1'b1 ;
			else if (delete_isr_bit2) // Synchronous reset
				isr_bit2_0[2] <= 1'b0 ;
		end
	end
  `else // GUEST
  // WB_EINT STATUS BIT
	// Set and clear FF
	always@(posedge pci_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			isr_bit2_0[1] <= 1'b0 ;
		else
		begin
			if (wb_error_sig && icr_bit2_0[1] && wb_err_cs_bit0) // Synchronous set
				isr_bit2_0[1] <= 1'b1 ;
			else if (delete_isr_bit1) // Synchronous reset
				isr_bit2_0[1] <= 1'b0 ;
		end
	end
  // PCI_EINT STATUS BIT
	reg		set_isr_bit2;
	wire	delete_set_isr_bit2;
	wire	block_set_isr_bit2;
	// Synchronization module for clearing FF between two clock domains
	pci_sync_module			sync_isr_2
	(
		.set_clk_in		(wb_clk),
		.delete_clk_in	(pci_clk),
		.reset_in		(reset),
		.delete_set_out	(delete_set_isr_bit2),
		.block_set_out	(block_set_isr_bit2),
		.delete_in		(delete_isr_bit2)
	);
	// Setting FF
	always@(posedge wb_clk or posedge reset)
	begin
		if (reset) // Asynchronous reset
			set_isr_bit2 <= 1'b0 ;
		else
		begin
			if (pci_error_sig && icr_bit2_0[2] && pci_err_cs_bit0) // Synchronous set
				set_isr_bit2 <= 1'b1 ;
			else if (delete_set_isr_bit2) // Synchronous reset
				set_isr_bit2 <= 1'b0 ;
		end
	end
	wire	isr_bit2	= set_isr_bit2 && !block_set_isr_bit2 ;
	wire	meta_isr_bit2 ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(1, 0) isr_bit2_sync
	(
	    .data_in        (isr_bit2),
	    .clk_out        (pci_clk),
	    .sync_data_out  (meta_isr_bit2),
	    .async_reset    (reset)
	) ;
	always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
	        isr_bit2_0[2]	<= 1'b0 ;
	    else
	        isr_bit2_0[2]	<= meta_isr_bit2 ;
	end
  `endif
`endif

// INT BIT of ISR - interrupt status register
`ifdef HOST
	wire	isr_int_prop_bit = isr_int_prop && icr_bit2_0[0] ;
	wire	meta_isr_int_prop_bit ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(1, 0) isr_bit0_sync
	(
	    .data_in        (isr_int_prop_bit),
	    .clk_out        (wb_clk),
	    .sync_data_out  (meta_isr_int_prop_bit),
	    .async_reset    (reset)
	) ;
	always@(posedge wb_clk or posedge reset)
	begin
	    if (reset)
	        isr_bit2_0[0]	<= 1'b0 ;
	    else
	        isr_bit2_0[0]	<= meta_isr_int_prop_bit ;
	end
`else // GUEST
  `ifdef SYNCHRONEOUS_CLOCK_DOMAINS
	wire	isr_int_prop_bit = isr_int_prop && icr_bit2_0[0] ;
	always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
	        isr_bit2_0[0]	<= 1'b0 ;
	    else
	        isr_bit2_0[0]	<= isr_int_prop_bit ;
	end
  `else // not SYNCHRONEOUS_CLOCK_DOMAINS
	wire	isr_int_prop_bit = isr_int_prop && icr_bit2_0[0] ;
	wire	meta_isr_int_prop_bit ;
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(1, 0) isr_bit0_sync
	(
	    .data_in        (isr_int_prop_bit),
	    .clk_out        (pci_clk),
	    .sync_data_out  (meta_isr_int_prop_bit),
	    .async_reset    (reset)
	) ;
	always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
	        isr_bit2_0[0]	<= 1'b0 ;
	    else
	        isr_bit2_0[0]	<= meta_isr_int_prop_bit ;
	end
  `endif
`endif

// INT PIN
wire	int_in;
wire	int_meta;
reg		interrupt_out;
`ifdef HOST
 `ifdef SYNCHRONEOUS_CLOCK_DOMAINS
	assign	int_in = isr_int_prop_bit || isr_bit2_0[1] || isr_bit2_0[2] || isr_bit4_3[3]  || isr_bit4_3[4];
 `else // not SYNCHRONEOUS_CLOCK_DOMAINS
	assign	int_in = isr_int_prop_bit || isr_bit1      || isr_bit2_0[2] || isr_bits4_3[3] || isr_bits4_3[4];
 `endif
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(1, 0) int_pin_sync
	(
	    .data_in        (int_in),
	    .clk_out        (wb_clk),
	    .sync_data_out  (int_meta),
	    .async_reset    (reset)
	) ;
	always@(posedge wb_clk or posedge reset)
	begin
	    if (reset)
	        interrupt_out	<= 1'b0 ;
	    else
	        interrupt_out	<= int_meta ;
	end
`else // GUEST
 `ifdef SYNCHRONEOUS_CLOCK_DOMAINS
	assign	int_in = isr_int_prop_bit || isr_bit2_0[1] || isr_bit2_0[2];
 `else // not SYNCHRONEOUS_CLOCK_DOMAINS
	assign	int_in = isr_int_prop_bit || isr_bit2_0[1] || isr_bit2;
 `endif
	// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
	pci_synchronizer_flop   #(1, 0) int_pin_sync
	(
	    .data_in        (int_in),
	    .clk_out        (pci_clk),
	    .sync_data_out  (int_meta),
	    .async_reset    (reset)
	) ;
	always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
	        interrupt_out	<= 1'b0 ;
	    else
	        interrupt_out	<= int_meta ;
	end
`endif


`ifdef PCI_CPCI_HS_IMPLEMENT
    reg [hs_es_cnt_width - 1:0] hs_es_cnt ; // debounce counter
    reg hs_es_in_state,   // current state of ejector switch input - synchronized
        hs_es_sync,       // synchronization flop for ejector switch input
        hs_es_cur_state ; // current valid state of ejector switch

`ifdef ACTIVE_HIGH_OE
    wire oe_active_val = 1'b1 ;
`endif

`ifdef ACTIVE_LOW_OE
    wire oe_active_val = 1'b0 ;
`endif

	always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
        begin
	        hs_ins          <= 1'b0 ;
            hs_ins_armed    <= 1'b1 ;
            hs_ext          <= 1'b0 ;
            hs_ext_armed    <= 1'b0 ;
            hs_es_in_state  <= 1'b0 ;
            hs_es_sync      <= 1'b0 ;
            hs_es_cur_state <= 1'b0 ;
            hs_es_cnt       <= 'h0  ;

        `ifdef ACTIVE_LOW_OE
            pci_cpci_hs_enum_oe_o   <= 1'b1 ;
            pci_cpci_hs_led_oe_o    <= 1'b0 ;
        `endif

        `ifdef ACTIVE_HIGH_OE
            pci_cpci_hs_enum_oe_o   <= 1'b0 ;
            pci_cpci_hs_led_oe_o    <= 1'b1 ;
        `endif

        end
	    else
        begin
            // INS
            if (hs_ins)
            begin
                if (w_conf_data[23] & ~w_byte_en[2] & w_we & w_reg_select_dec[56]) // clear
                    hs_ins <= 1'b0 ;
            end
            else if (hs_ins_armed)  // set
                hs_ins <= init_complete & (hs_es_cur_state == 1'b1) ;

            // INS armed
            if (~hs_ins & hs_ins_armed & init_complete & (hs_es_cur_state == 1'b1)) // clear
                hs_ins_armed <= 1'b0 ;
            else if (hs_ext)  // set
                hs_ins_armed <= w_conf_data[22] & ~w_byte_en[2] & w_we & w_reg_select_dec[56] ;

            // EXT
            if (hs_ext) // clear
            begin
                if (w_conf_data[22] & ~w_byte_en[2] & w_we & w_reg_select_dec[56])
                    hs_ext <= 1'b0 ;
            end
            else if (hs_ext_armed)  // set
                hs_ext <= (hs_es_cur_state == 1'b0) ;

            // EXT armed
            if (~hs_ext & hs_ext_armed & (hs_es_cur_state == 1'b0)) // clear
                hs_ext_armed <= 1'b0 ;
            else if (hs_ins)  // set
                hs_ext_armed <= w_conf_data[23] & !w_byte_en[2] & w_we & w_reg_select_dec[56] ;

            // ejector switch debounce counter logic
            hs_es_sync     <= pci_cpci_hs_es_i  ;
            hs_es_in_state <= hs_es_sync        ;

            if (hs_es_in_state == hs_es_cur_state)
                hs_es_cnt <= 'h0 ;
            else
                hs_es_cnt <= hs_es_cnt + 1'b1 ;

            if (hs_es_cnt == {hs_es_cnt_width{1'b1}})
                hs_es_cur_state <= hs_es_in_state ;

            if ((hs_ins | hs_ext) & ~hs_eim)
                pci_cpci_hs_enum_oe_o   <=  oe_active_val   ;
            else
                pci_cpci_hs_enum_oe_o   <= ~oe_active_val   ;

            if (~init_complete | hs_loo)
                pci_cpci_hs_led_oe_o    <=  oe_active_val   ;
            else
                pci_cpci_hs_led_oe_o    <= ~oe_active_val   ;
        end
	end
`endif

`ifdef PCI_SPOCI

    wire spoci_write_done,
         spoci_dat_rdy   ,
         spoci_no_ack    ;

    wire [ 7: 0] spoci_wdat ;
    wire [ 7: 0] spoci_rdat ;

    // power on configuration control and status register    
    always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
        begin
            spoci_cs_nack   <= 1'b0 ;
            spoci_cs_write  <= 1'b0 ;
            spoci_cs_read   <= 1'b0 ;
            spoci_cs_adr    <= 'h0  ;
            spoci_cs_dat    <= 'h0  ;
        end
        else
        begin
            if (spoci_cs_write)
            begin
                if (spoci_write_done | spoci_no_ack)
                    spoci_cs_write <= 1'b0 ;
            end
            else if ( w_we & (w_conf_address[9:2] == 8'hFF) & ~w_byte_en[3])
                spoci_cs_write <= w_conf_data[25] ;

            if (spoci_cs_read)
            begin
                if (spoci_dat_rdy | spoci_no_ack)
                    spoci_cs_read <= 1'b0          ;
            end
            else if ( w_we & (w_conf_address[9:2] == 8'hFF) & ~w_byte_en[3] )
                spoci_cs_read <= w_conf_data[24] ;

            if (spoci_cs_nack)
            begin
                if ( w_we & (w_conf_address[9:2] == 8'hFF) & ~w_byte_en[3] & w_conf_data[31] )
                    spoci_cs_nack <= 1'b0 ;
            end
            else if (spoci_cs_write | spoci_cs_read | ~init_cfg_done)
            begin
                spoci_cs_nack <= spoci_no_ack ;
            end

            if ( w_we & (w_conf_address[9:2] == 8'hFF) )
            begin
                if (~w_byte_en[2])
                    spoci_cs_adr[10: 8] <= w_conf_data[18:16] ;

                if (~w_byte_en[1])
                    spoci_cs_adr[ 7: 0] <= w_conf_data[15: 8] ;
            end

            if ( w_we & (w_conf_address[9:2] == 8'hFF) & ~w_byte_en[0] )
                spoci_cs_dat <= w_conf_data[ 7: 0] ;
            else if (spoci_cs_read & spoci_dat_rdy)
                spoci_cs_dat <= spoci_rdat ;
            
        end
    end

    reg [ 2 : 0] bytes_received ;

    always@(posedge pci_clk or posedge reset)
	begin
	    if (reset)
        begin
            init_we         <= 1'b0 ;
            init_cfg_done   <= 1'b0 ;
            bytes_received  <= 1'b0 ;
            spoci_dat       <= 'h0  ;
            spoci_reg_num   <= 'h0  ;
        end
        else if (~init_cfg_done)
        begin
            if (spoci_dat_rdy)
            begin
                case (bytes_received)
                'h0:spoci_reg_num       <= spoci_rdat   ;
                'h1:spoci_dat[ 7: 0]    <= spoci_rdat   ;
                'h2:spoci_dat[15: 8]    <= spoci_rdat   ;
                'h3:spoci_dat[23:16]    <= spoci_rdat   ;
                'h4:spoci_dat[31:24]    <= spoci_rdat   ;
                default:
                begin
                    spoci_dat       <= 32'hxxxx_xxxx    ;
                    spoci_reg_num   <= 'hxx             ;
                end
                endcase
            end

            if (init_we)
                bytes_received <= 'h0 ;
            else if (spoci_dat_rdy)
                bytes_received <= bytes_received + 1'b1 ;

            if (init_we)
                init_we <= 1'b0 ;
            else if (bytes_received == 'h5)
                init_we <= 1'b1 ;
            
            if (spoci_no_ack | ((bytes_received == 'h1) & (spoci_reg_num == 'hff)) )
                init_cfg_done <= 1'b1 ;
        end
    end

    assign spoci_wdat = spoci_cs_dat ;

    pci_spoci_ctrl i_pci_spoci_ctrl
    (
        .reset_i            (reset                          ),
        .clk_i              (pci_clk                        ),

        .do_rnd_read_i      (spoci_cs_read                  ),
        .do_seq_read_i      (rst_inactive & ~init_cfg_done  ),
        .do_write_i         (spoci_cs_write                 ),

        .write_done_o       (spoci_write_done               ),
        .dat_rdy_o          (spoci_dat_rdy                  ),
        .no_ack_o           (spoci_no_ack                   ),

        .adr_i              (spoci_cs_adr                   ),
        .dat_i              (spoci_wdat                     ),
        .dat_o              (spoci_rdat                     ),

        .pci_spoci_sda_i    (spoci_sda_i                    ),
        .pci_spoci_sda_oe_o (spoci_sda_oe_o                 ),
        .pci_spoci_scl_oe_o (spoci_scl_oe_o                 )
    );
`endif

/*-----------------------------------------------------------------------------------------------------------
        OUTPUTs from registers !!!
-----------------------------------------------------------------------------------------------------------*/

// if bridge is HOST then write clock is equal to WB clock, and synchronization of outputs has to be done
`ifdef	HOST
  wire [3:0] command_bits = {command_bit8, command_bit6, command_bit2_0[1:0]} ;
  wire [3:0] meta_command_bits ;
  reg  [3:0] sync_command_bits ;
  pci_synchronizer_flop   #(4, 0)  command_bits_sync
  (
      .data_in        (command_bits),
      .clk_out        (pci_clk),
      .sync_data_out  (meta_command_bits),
      .async_reset    (reset)
  ) ;
  always@(posedge pci_clk or posedge reset)
  begin
      if (reset)
          sync_command_bits <= 4'b0 ;
      else
          sync_command_bits <= meta_command_bits ;
  end
  wire  sync_command_bit8 = sync_command_bits[3] ;
  wire  sync_command_bit6 = sync_command_bits[2] ;
  wire  sync_command_bit1 = sync_command_bits[1] ;
  wire  sync_command_bit0 = sync_command_bits[0] ;
  wire  sync_command_bit2 = command_bit2_0[2] ;
`else	// GUEST
  wire       command_bit = command_bit2_0[2] ;
  wire       meta_command_bit ;
  reg        sync_command_bit ;
  pci_synchronizer_flop   #(1, 0) command_bit_sync
  (
      .data_in        (command_bit),
      .clk_out        (pci_clk),
      .sync_data_out  (meta_command_bit),
      .async_reset    (reset)
  ) ;
  always@(posedge pci_clk or posedge reset)
  begin
      if (reset)
          sync_command_bit <= 1'b0 ;
      else
          sync_command_bit <= meta_command_bit ;
  end
  wire  sync_command_bit8 = command_bit8 ;
  wire  sync_command_bit6 = command_bit6 ;
  wire  sync_command_bit1 = command_bit2_0[1] ;
  wire  sync_command_bit0 = command_bit2_0[0] ;
  wire  sync_command_bit2 = sync_command_bit ;
`endif
// PCI header outputs from command register
assign		serr_enable = sync_command_bit8 & pci_init_complete_out ;           // to PCI clock
assign		perr_response = sync_command_bit6 & pci_init_complete_out ;         // to PCI clock
assign		pci_master_enable = sync_command_bit2 & wb_init_complete_out ;      // to WB clock
assign		memory_space_enable = sync_command_bit1 & pci_init_complete_out ;   // to PCI clock
assign		io_space_enable = sync_command_bit0 & pci_init_complete_out     ;   // to PCI clock

// if bridge is HOST then write clock is equal to WB clock, and synchronization of outputs has to be done
	// We don't support cache line sizes smaller that 4 and it must have last two bits zero!!!
wire	cache_lsize_not_zero = ((cache_line_size_reg[7] || cache_line_size_reg[6] || cache_line_size_reg[5] ||
								 cache_line_size_reg[4] || cache_line_size_reg[3] || cache_line_size_reg[2]) &&
								(!cache_line_size_reg[1] && !cache_line_size_reg[0]) );
`ifdef	HOST
  wire [7:2] cache_lsize_to_pci_bits = { cache_line_size_reg[7:2] } ;
  wire [7:2] meta_cache_lsize_to_pci_bits ;
  reg  [7:2] sync_cache_lsize_to_pci_bits ;
  pci_synchronizer_flop   #(6, 0)  cache_lsize_to_pci_bits_sync
  (
      .data_in        (cache_lsize_to_pci_bits),
      .clk_out        (pci_clk),
      .sync_data_out  (meta_cache_lsize_to_pci_bits),
      .async_reset    (reset)
  ) ;
  always@(posedge pci_clk or posedge reset)
  begin
      if (reset)
          sync_cache_lsize_to_pci_bits <= 6'b0 ;
      else
          sync_cache_lsize_to_pci_bits <= meta_cache_lsize_to_pci_bits ;
  end
  wire [7:2] sync_cache_line_size_to_pci_reg	= sync_cache_lsize_to_pci_bits[7:2] ;
  wire [7:2] sync_cache_line_size_to_wb_reg		= cache_line_size_reg[7:2] ;
  wire		 sync_cache_lsize_not_zero_to_wb	= cache_lsize_not_zero ;
// Latency timer is sinchronized only to PCI clock when bridge implementation is HOST
  wire [7:0] latency_timer_bits = latency_timer ;
  wire [7:0] meta_latency_timer_bits ;
  reg  [7:0] sync_latency_timer_bits ;
  pci_synchronizer_flop   #(8, 0)  latency_timer_bits_sync
  (
      .data_in        (latency_timer_bits),
      .clk_out        (pci_clk),
      .sync_data_out  (meta_latency_timer_bits),
      .async_reset    (reset)
  ) ;
  always@(posedge pci_clk or posedge reset)
  begin
      if (reset)
          sync_latency_timer_bits <= 8'b0 ;
      else
          sync_latency_timer_bits <= meta_latency_timer_bits ;
  end
  wire [7:0] sync_latency_timer = sync_latency_timer_bits ;
`else	// GUEST
  wire [8:2] cache_lsize_to_wb_bits = { cache_lsize_not_zero, cache_line_size_reg[7:2] } ;
  wire [8:2] meta_cache_lsize_to_wb_bits ;
  reg  [8:2] sync_cache_lsize_to_wb_bits ;
  pci_synchronizer_flop   #(7, 0)  cache_lsize_to_wb_bits_sync
  (
      .data_in        (cache_lsize_to_wb_bits),
      .clk_out        (wb_clk),
      .sync_data_out  (meta_cache_lsize_to_wb_bits),
      .async_reset    (reset)
  ) ;
  always@(posedge wb_clk or posedge reset)
  begin
      if (reset)
          sync_cache_lsize_to_wb_bits <= 7'b0 ;
      else
          sync_cache_lsize_to_wb_bits <= meta_cache_lsize_to_wb_bits ;
  end
  wire [7:2] sync_cache_line_size_to_pci_reg	= cache_line_size_reg[7:2] ;
  wire [7:2] sync_cache_line_size_to_wb_reg		= sync_cache_lsize_to_wb_bits[7:2] ;
  wire		 sync_cache_lsize_not_zero_to_wb	= sync_cache_lsize_to_wb_bits[8] ;
// Latency timer
  wire [7:0] sync_latency_timer = latency_timer ;
`endif
// PCI header output from cache_line_size, latency timer and interrupt pin
assign		cache_line_size_to_pci		= {sync_cache_line_size_to_pci_reg, 2'h0} ;  // [7 : 0] to PCI clock
assign		cache_line_size_to_wb		= {sync_cache_line_size_to_wb_reg, 2'h0} ;   // [7 : 0] to WB clock
assign		cache_lsize_not_zero_to_wb	= sync_cache_lsize_not_zero_to_wb ;

assign		latency_tim[7 : 0]     = sync_latency_timer ;        		// to PCI clock
//assign		int_pin[2 : 0]         = r_interrupt_pin ;
assign		int_out				   = interrupt_out ;
// PCI output from image registers
//   base address, address mask, translation address and control registers are sinchronized in PCI_DECODER.V module
`ifdef HOST
    `ifdef NO_CNF_IMAGE
        assign		pci_base_addr0 = pci_ba0_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
    `else
        assign      pci_base_addr0 = pci_ba0_bit31_8[31:12] ;
    `endif
`endif

`ifdef GUEST
    assign  pci_base_addr0 = pci_ba0_bit31_8[31:12] ;
`endif

assign		pci_base_addr1 = pci_ba1_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_base_addr2 = pci_ba2_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_base_addr3 = pci_ba3_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_base_addr4 = pci_ba4_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_base_addr5 = pci_ba5_bit31_8[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_memory_io0 = pci_ba0_bit0 ;
assign		pci_memory_io1 = pci_ba1_bit0 ;
assign		pci_memory_io2 = pci_ba2_bit0 ;
assign		pci_memory_io3 = pci_ba3_bit0 ;
assign		pci_memory_io4 = pci_ba4_bit0 ;
assign		pci_memory_io5 = pci_ba5_bit0 ;

assign		pci_addr_mask0 = pci_am0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_addr_mask1 = pci_am1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_addr_mask2 = pci_am2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_addr_mask3 = pci_am3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_addr_mask4 = pci_am4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_addr_mask5 = pci_am5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_tran_addr0 = pci_ta0[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_tran_addr1 = pci_ta1[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_tran_addr2 = pci_ta2[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_tran_addr3 = pci_ta3[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_tran_addr4 = pci_ta4[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_tran_addr5 = pci_ta5[31:(32-`PCI_NUM_OF_DEC_ADDR_LINES)] ;
assign		pci_img_ctrl0[2 : 1] = pci_img_ctrl0_bit2_1 ;
assign		pci_img_ctrl1[2 : 1] = pci_img_ctrl1_bit2_1 ;
assign		pci_img_ctrl2[2 : 1] = pci_img_ctrl2_bit2_1 ;
assign		pci_img_ctrl3[2 : 1] = pci_img_ctrl3_bit2_1 ;
assign		pci_img_ctrl4[2 : 1] = pci_img_ctrl4_bit2_1 ;
assign		pci_img_ctrl5[2 : 1] = pci_img_ctrl5_bit2_1 ;
// WISHBONE output from image registers
//   base address, address mask, translation address and control registers are sinchronized in DECODER.V module
assign		wb_base_addr0 = wb_ba0_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_base_addr1 = wb_ba1_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_base_addr2 = wb_ba2_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_base_addr3 = wb_ba3_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_base_addr4 = wb_ba4_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_base_addr5 = wb_ba5_bit31_12[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_memory_io0 = wb_ba0_bit0 ;
assign		wb_memory_io1 = wb_ba1_bit0 ;
assign		wb_memory_io2 = wb_ba2_bit0 ;
assign		wb_memory_io3 = wb_ba3_bit0 ;
assign		wb_memory_io4 = wb_ba4_bit0 ;
assign		wb_memory_io5 = wb_ba5_bit0 ;
assign		wb_addr_mask0 = wb_am0[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_addr_mask1 = wb_am1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_addr_mask2 = wb_am2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_addr_mask3 = wb_am3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_addr_mask4 = wb_am4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_addr_mask5 = wb_am5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_tran_addr0 = wb_ta0[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_tran_addr1 = wb_ta1[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_tran_addr2 = wb_ta2[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_tran_addr3 = wb_ta3[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_tran_addr4 = wb_ta4[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_tran_addr5 = wb_ta5[31:(32-`WB_NUM_OF_DEC_ADDR_LINES)] ;
assign		wb_img_ctrl0[2 : 0] = wb_img_ctrl0_bit2_0 ;
assign		wb_img_ctrl1[2 : 0] = wb_img_ctrl1_bit2_0 ;
assign		wb_img_ctrl2[2 : 0] = wb_img_ctrl2_bit2_0 ;
assign		wb_img_ctrl3[2 : 0] = wb_img_ctrl3_bit2_0 ;
assign		wb_img_ctrl4[2 : 0] = wb_img_ctrl4_bit2_0 ;
assign		wb_img_ctrl5[2 : 0] = wb_img_ctrl5_bit2_0 ;
// GENERAL output from conf. cycle generation register & int. control register
assign		config_addr[23 : 0] = { cnf_addr_bit23_2, 1'b0, cnf_addr_bit0 } ;
assign		icr_soft_res = icr_bit31 ;

endmodule
			                    
