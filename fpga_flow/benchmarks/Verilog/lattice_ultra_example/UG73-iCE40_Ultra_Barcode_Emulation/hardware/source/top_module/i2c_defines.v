//==================================================================
//   >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
//   ------------------------------------------------------------------
//   Copyright (c) 2014 by Lattice Semiconductor Corporation
// 					ALL RIGHTS RESERVED 
//   ------------------------------------------------------------------
//
//   Permission:
//
//      Lattice SG Pte. Ltd. grants permission to use this code for use
//   in synthesis for any Lattice programmable logic product.  Other
//   use of this code, including the selling or duplication of any
//   portion is strictly prohibited.
  
//
//   Disclaimer:
//
//   This VHDL or Verilog source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Lattice provides no warranty
//   regarding the use or functionality of this code.
//
//   --------------------------------------------------------------------
//
//                  Lattice SG Pte. Ltd.
//                  101 Thomson Road, United Square #07-02 
// 					Singapore 307591
//	
//
//                  TEL: 1-800-Lattice (USA and Canada)
//	+65-6631-2000 (Singapore)
//                       +1-503-268-8001 (other locations)
//
//                  web: http://www.latticesemi.com/
//                  email: techsupport@latticesemi.com
//
//   --------------------------------------------------------------------
//



/***********************************************************************
 *                                                                     *
 * EFB REGISTER SET                                                    *
 *                                                                     *
 ***********************************************************************/

 
`define MICO_EFB_I2C_CR                                    8'h08
`define MICO_EFB_I2C_CMDR                                  8'h09
`define MICO_EFB_I2C_BLOR                                  8'h0a
`define MICO_EFB_I2C_BHIR                                  8'h0b
`define MICO_EFB_I2C_TXDR                                  8'h0d
`define MICO_EFB_I2C_SR                                    8'h0c
`define MICO_EFB_I2C_GCDR                                  8'h0f
`define MICO_EFB_I2C_RXDR                                  8'h0e
`define MICO_EFB_I2C_IRQSR                                 8'h06
`define MICO_EFB_I2C_IRQENR                                8'h09

/***********************************************************************
 *                                                                     *
 * EFB I2C CONTROLLER PHYSICAL DEVICE SPECIFIC INFORMATION             *
 *                                                                     *
 ***********************************************************************/



// Control Register Bit Masks
`define MICO_EFB_I2C_CR_I2CEN				   8'h80 
`define MICO_EFB_I2C_CR_GCEN				   8'h40 
`define MICO_EFB_I2C_CR_WKUPEN			   8'h20 
// Status Register Bit Masks
`define MICO_EFB_I2C_SR_TIP				   8'h80 
`define MICO_EFB_I2C_SR_BUSY				   8'h40 
`define MICO_EFB_I2C_SR_RARC				   8'h20 
`define MICO_EFB_I2C_SR_SRW				   8'h10 
`define MICO_EFB_I2C_SR_ARBL				   8'h08 
`define MICO_EFB_I2C_SR_TRRDY				   8'h04 
`define MICO_EFB_I2C_SR_TROE				   8'h02 
`define MICO_EFB_I2C_SR_HGC				   8'h01 
// Command Register Bit Masks 
`define MICO_EFB_I2C_CMDR_STA				   8'h80 
`define MICO_EFB_I2C_CMDR_STO				   8'h40 
`define MICO_EFB_I2C_CMDR_RD				   8'h20 
`define MICO_EFB_I2C_CMDR_WR				   8'h10 
`define MICO_EFB_I2C_CMDR_NACK			   8'h08 
`define MICO_EFB_I2C_CMDR_CKSDIS			   8'h04 


/***********************************************************************
 *                                                                     *
 * CODE SPECIFIC                                                       *
 *                                                                     *
 ***********************************************************************/
 
 `define ALL_ZERO   8'h00 
 `define READ       1'b0  
 `define READ       1'b0  
 `define HIGH       1'b1  
 `define WRITE      1'b1  
 `define LOW        1'b0  
 `define READ_STATUS     1'b0  
 `define READ_DATA       1'b0  
 
/***********************************************************************
 *                                                                     *
 * State Machine Variables                                             *
 *                                                                     *
 ***********************************************************************/ 		
		
`define	state0	8'd00
`define	state1	8'd01
`define	state2	8'd02
`define	state3	8'd03
`define	state4	8'd04
`define	state5	8'd05
`define	state6	8'd06
`define	state7	8'd07
`define	state8	8'd08
`define	state9	8'd09
`define	state10	8'd10
`define	state11	8'd11
`define	state12	8'd12
`define	state13	8'd13
`define	state14	8'd14
`define	state15	8'd15
`define	state16	8'd16
`define	state17	8'd17
`define	state18	8'd18
`define	state19	8'd19
`define	state20	8'd20
`define	state21	8'd21
`define	state22	8'd22
`define	state23	8'd23
`define	state24	8'd24
`define	state25	8'd25
`define	state26	8'd26
`define	state27	8'd27
`define	state28	8'd28
`define	state29	8'd29
`define	state30	8'd30
`define	state31	8'd31
`define	state32	8'd32
`define	state33	8'd33
`define	state34	8'd34
`define	state35	8'd35
`define	state36	8'd36
`define	state37	8'd37
`define	state38	8'd38
`define	state39	8'd39
`define	state40	8'd40
`define	state41	8'd41
`define	state42	8'd42
`define	state43	8'd43
`define	state44	8'd44
`define	state45	8'd45
`define	state46	8'd46
`define	state47	8'd47
`define	state48	8'd48
`define	state49	8'd49
`define	state50	8'd50
`define	state51	8'd51
`define	state52	8'd52
`define	state53	8'd53
`define	state54	8'd54
`define	state55	8'd55
`define	state56	8'd56
`define	state57	8'd57
`define	state58	8'd58
`define	state59	8'd59
`define	state60	8'd60
`define	state61	8'd61
`define	state62	8'd62
`define	state63	8'd63
`define	state64	8'd64
`define	state65	8'd65
`define	state66	8'd66
`define	state67	8'd67
`define	state68	8'd68
`define	state69	8'd69
`define	state70	8'd70
`define	state71	8'd71
`define	state72	8'd72
`define	state73	8'd73
`define	state74	8'd74
`define	state75	8'd75
`define	state76	8'd76
`define	state77	8'd77
`define	state78	8'd78
`define	state79	8'd79
`define	state80	8'd80
`define	state81	8'd81
`define	state82	8'd82
`define	state83	8'd83
`define	state84	8'd84
`define	state85	8'd85
`define	state86	8'd86
`define	state87	8'd87
`define	state88	8'd88
`define	state89	8'd89
`define	state90	8'd90
`define	state91	8'd91
`define	state92	8'd92
`define	state93	8'd93
`define	state94	8'd94
`define	state95	8'd95
`define	state96	8'd96
`define	state97	8'd97
`define	state98	8'd98
`define	state99	8'd99
`define	state100	8'd100
`define	state101	8'd101
`define	state102	8'd102
`define	state103	8'd103
`define	state104	8'd104
`define	state105	8'd105
`define	state106	8'd106
`define	state107	8'd107
`define	state108	8'd108
`define	state109	8'd109
`define	state110	8'd110
`define	state111	8'd111
`define	state112	8'd112
`define	state113	8'd113
`define	state114	8'd114
`define	state115	8'd115
`define	state116	8'd116
`define	state117	8'd117
`define	state118	8'd118
`define	state119	8'd119
`define	state120	8'd120
`define	state121	8'd121
`define	state122	8'd122
`define	state123	8'd123
`define	state124	8'd124
`define	state125	8'd125
`define	state126	8'd126
`define	state127	8'd127
`define	state128	8'd128
`define	state129	8'd129
`define	state130	8'd130
`define	state131	8'd131
`define	state132	8'd132
`define	state133	8'd133
`define	state134	8'd134
`define	state135	8'd135
`define	state136	8'd136
`define	state137	8'd137
`define	state138	8'd138
`define	state139	8'd139
`define	state140	8'd140
`define	state141	8'd141
`define	state142	8'd142
`define	state143	8'd143
`define	state144	8'd144
`define	state145	8'd145
`define	state146	8'd146
`define	state147	8'd147
`define	state148	8'd148
`define	state149	8'd149
`define	state150	8'd150
`define	state151	8'd151
`define	state152	8'd152
`define	state153	8'd153
`define	state154	8'd154
`define	state155	8'd155
`define	state156	8'd156
`define	state157	8'd157
`define	state158	8'd158
`define	state159	8'd159
`define	state160	8'd160
`define	state161	8'd161
`define	state162	8'd162
`define	state163	8'd163
`define	state164	8'd164
`define	state165	8'd165
`define	state166	8'd166
`define	state167	8'd167
`define	state168	8'd168
`define	state169	8'd169
`define	state170	8'd170
`define	state171	8'd171
`define	state172	8'd172
`define	state173	8'd173
`define	state174	8'd174
`define	state175	8'd175
`define	state176	8'd176
`define	state177	8'd177
`define	state178	8'd178
`define	state179	8'd179
`define	state180	8'd180
`define	state181	8'd181
`define	state182	8'd182
`define	state183	8'd183
`define	state184	8'd184
`define	state185	8'd185
`define	state186	8'd186
`define	state187	8'd187
`define	state188	8'd188
`define	state189	8'd189
`define	state190	8'd190
`define	state191	8'd191
`define	state192	8'd192
`define	state193	8'd193
`define	state194	8'd194
`define	state195	8'd195
`define	state196	8'd196
`define	state197	8'd197
`define	state198	8'd198
`define	state199	8'd199
`define	state200	8'd200
`define	state201	8'd201
`define	state202	8'd202
`define	state203	8'd203
`define	state204	8'd204
`define	state205	8'd205
`define	state206	8'd206
`define	state207	8'd207
`define	state208	8'd208
`define	state209	8'd209
`define	state210	8'd210
`define	state211	8'd211
`define	state212	8'd212
`define	state213	8'd213
`define	state214	8'd214
`define	state215	8'd215
`define	state216	8'd216
`define	state217	8'd217
`define	state218	8'd218
`define	state219	8'd219
`define	state220	8'd220
`define	state221	8'd221
`define	state222	8'd222
`define	state223	8'd223
`define	state224	8'd224
`define	state225	8'd225
`define	state226	8'd226
`define	state227	8'd227
`define	state228	8'd228
`define	state229	8'd229
`define	state230	8'd230
`define	state231	8'd231
`define	state232	8'd232
`define	state233	8'd233
`define	state234	8'd234
`define	state235	8'd235
`define	state236	8'd236
`define	state237	8'd237
`define	state238	8'd238
`define	state239	8'd239
`define	state240	8'd240
`define	state241	8'd241
`define	state242	8'd242
`define	state243	8'd243
`define	state244	8'd244
`define	state245	8'd245
`define	state246	8'd246
`define	state247	8'd247
`define	state248	8'd248
`define	state249	8'd249
`define	state250	8'd250
`define	state251	8'd251
`define	state252	8'd252
`define	state253	8'd253
`define	state254	8'd254
`define	state255	8'd255

