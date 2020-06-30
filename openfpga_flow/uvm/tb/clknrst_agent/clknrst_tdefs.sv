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

`ifndef CLKNRST_TDEFS
`define CLKNRST_TDEFS

struct {
	bit 
	clk,
	clk_stim,
	bs_clock_reg,
	clknrst_uvm,
	Reset,
	pReset,
	Test_en,
	OUT;
} clknrst_signal;

typedef enum {
   STANDARD  ,
   NOVALUE   
} initial_interface_value;


typedef enum {
   CLK_SEQ_ITEM_ACTION_START_CLK   ,
   CLK_SEQ_ITEM_ACTION_STOP_CLK    ,
   CLK_SEQ_ITEM_ACTION_ASSERT_RESET
} clknrst_seq_item_action_enum;

typedef enum {
   CLK_SEQ_ITEM_INITIAL_VALUE_0,
   CLK_SEQ_ITEM_INITIAL_VALUE_1,
   CLK_SEQ_ITEM_INITIAL_VALUE_X
} clknrst_seq_item_initial_value_enum;

typedef enum {
   CLK_MON_TRN_EVENT_CLK_STARTED     ,
   CLK_MON_TRN_EVENT_CLK_STOPPED     ,
   CLK_MON_TRN_EVENT_RESET_ASSERTED  ,
   CLK_MON_TRN_EVENT_RESET_DEASSERTED
} clknrst_mon_trn_event_enum;


`endif // CLKNRST_TDEFS
