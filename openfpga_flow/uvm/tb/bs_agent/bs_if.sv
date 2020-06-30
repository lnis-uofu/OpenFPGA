// 
// Copyright 2020 OpenHW Group
// Copyright 2020 Datum Technology Corporation
// 
// Licensed under the Solderpad Hardware Licence, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     https://solderpad.org/licenses/
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

`ifndef BS_IF
`define BS_IF

`timescale 1ns / 100ps

interface bs_if(input clk_uvm, clk);

   logic [0:0]  data_in; // Configuration signal
   logic [0:ADDR_WIDTH] address; // Configuration signal
   wire  [0:GPIO_WIDTH] gfpga_pad_GPIO; // We use a tri-state buffer assign to control IO's configuration
   logic [0:GPIO_WIDTH] gfpga_pad_GPIO_IN_drv;
   logic [0:GPIO_WIDTH] IE ;

// Assign GPIO[i] to GPIO_IN_drv[i] if GPIO[i] is configured as an input.
   genvar i;
   for (i = 0; i <= GPIO_WIDTH; i++)
	begin
   		assign gfpga_pad_GPIO[i] = IE[i] ? gfpga_pad_GPIO_IN_drv[i] : 1'bZ;
	end

   modport DRIVER (inout gfpga_pad_GPIO); 

endinterface:bs_if

`endif // BS_IF
