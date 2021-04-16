/////////////////////////////////////////////////////////////////////
////                                                             ////
////  DES                                                        ////
////  DES Top Level module                                       ////
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

module des(desOut, desIn, key, decrypt, clk);
output	[63:0]	desOut;
input	[63:0]	desIn;
input	[55:0]	key;
input		decrypt;
input		clk;

wire	[1:64]	IP, FP;
reg	[63:0]	desIn_r;
reg	[55:0]	key_r;
reg	[63:0]	desOut;
reg	[1:32]	L0, L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15;
reg	[1:32]	R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15;
wire	[1:32]	out0, out1, out2, out3, out4, out5, out6, out7, out8, out9, out10, out11, out12, out13, out14, out15;
wire	[1:48]	K1, K2, K3, K4, K5, K6, K7, K8, K9;
wire	[1:48]	K10, K11, K12, K13, K14, K15, K16;

// Register the 56 bit key
always @(posedge clk)
	key_r <= #1 key;

// Register the 64 bit input
always @(posedge clk)
	desIn_r <= #1 desIn;

// XOR 32 bit out15 with 32 bit L14         ( FP  1:32 )
//    then concatinate the 32 bit R14 value ( FP 33:64 )
//       This value ( FP 1:64 ) is then registered by the desOut[63:0] register 
assign FP = { (out15 ^ L14), R14};

// Key schedule provides a linear means of intermixing the 56 bit key to form a
//   different 48 bit key for each of the 16 bit rounds
key_sel uk(
	.clk(		clk		),
	.K(		key_r		),
	.decrypt(	decrypt		),
	.K1(		K1		),
	.K2(		K2		),
	.K3(		K3		),
	.K4(		K4		),
	.K5(		K5		),
	.K6(		K6		),
	.K7(		K7		),
	.K8(		K8		),
	.K9(		K9		),
	.K10(		K10		),
	.K11(		K11		),
	.K12(		K12		),
	.K13(		K13		),
	.K14(		K14		),
	.K15(		K15		),
	.K16(		K16		)
	);

// 16 CRP blocks 
crp u0( .P(out0), .R(IP[33:64]), .K_sub(K1) );
crp u1( .P(out1), .R(R0), .K_sub(K2) );
crp u2( .P(out2), .R(R1), .K_sub(K3) );
crp u3( .P(out3), .R(R2), .K_sub(K4) );
crp u4( .P(out4), .R(R3), .K_sub(K5) );
crp u5( .P(out5), .R(R4), .K_sub(K6) );
crp u6( .P(out6), .R(R5), .K_sub(K7) );
crp u7( .P(out7), .R(R6), .K_sub(K8) );
crp u8( .P(out8), .R(R7), .K_sub(K9) );
crp u9( .P(out9), .R(R8), .K_sub(K10) );
crp u10( .P(out10), .R(R9), .K_sub(K11) );
crp u11( .P(out11), .R(R10), .K_sub(K12) );
crp u12( .P(out12), .R(R11), .K_sub(K13) );
crp u13( .P(out13), .R(R12), .K_sub(K14) );
crp u14( .P(out14), .R(R13), .K_sub(K15) );
crp u15( .P(out15), .R(R14), .K_sub(K16) );

// 32 bit L0 get upper 32 bits of IP
always @(posedge clk)
        L0 <= #1 IP[33:64];

// 32 bit R0 gets lower 32 bits of IP XOR'd with 32 bit out0
always @(posedge clk)
        R0 <= #1  IP[01:32] ^ out0;

// 32 bit L1 gets 32 bit R0
always @(posedge clk)
        L1 <= #1 R0;

// 32 bit R1 gets 32 bit L0 XOR'd with 32 bit out1
always @(posedge clk)
        R1 <= #1 L0 ^ out1;

// 32 bit L2 gets 32 bit R1
always @(posedge clk)
        L2 <= #1 R1;

// 32 bit R2 gets 32 bit L1 XOR'd with 32 bit out2
always @(posedge clk)
        R2 <= #1 L1 ^ out2;

always @(posedge clk)
        L3 <= #1 R2;

always @(posedge clk)
        R3 <= #1 L2 ^ out3;

always @(posedge clk)
        L4 <= #1 R3;

always @(posedge clk)
        R4 <= #1 L3 ^ out4;

always @(posedge clk)
        L5 <= #1 R4;

always @(posedge clk)
        R5 <= #1 L4 ^ out5;

always @(posedge clk)
        L6 <= #1 R5;

always @(posedge clk)
        R6 <= #1 L5 ^ out6;

always @(posedge clk)
        L7 <= #1 R6;

always @(posedge clk)
        R7 <= #1 L6 ^ out7;

always @(posedge clk)
        L8 <= #1 R7;

always @(posedge clk)
        R8 <= #1 L7 ^ out8;

always @(posedge clk)
        L9 <= #1 R8;

always @(posedge clk)
        R9 <= #1 L8 ^ out9;

always @(posedge clk)
        L10 <= #1 R9;

always @(posedge clk)
        R10 <= #1 L9 ^ out10;

always @(posedge clk)
        L11 <= #1 R10;

always @(posedge clk)
        R11 <= #1 L10 ^ out11;

always @(posedge clk)
        L12 <= #1 R11;

always @(posedge clk)
        R12 <= #1 L11 ^ out12;

always @(posedge clk)
        L13 <= #1 R12;

always @(posedge clk)
        R13 <= #1 L12 ^ out13;

always @(posedge clk)
        L14 <= #1 R13;

always @(posedge clk)
        R14 <= #1 L13 ^ out14;

// 32 bit L15 gets 32 bit R14
always @(posedge clk)
        L15 <= #1 R14;

// 32 bit R15 gets 32 bit L14 XOR'd with 32 bit out15
always @(posedge clk)
        R15 <= #1 L14 ^ out15;

// Perform the initial permutationi with the registerd desIn
assign IP[1:64] = {	desIn_r[06], desIn_r[14], desIn_r[22], desIn_r[30], desIn_r[38], desIn_r[46],
			desIn_r[54], desIn_r[62], desIn_r[04], desIn_r[12], desIn_r[20], desIn_r[28],
			desIn_r[36], desIn_r[44], desIn_r[52], desIn_r[60], desIn_r[02], desIn_r[10], 
			desIn_r[18], desIn_r[26], desIn_r[34], desIn_r[42], desIn_r[50], desIn_r[58], 
			desIn_r[00], desIn_r[08], desIn_r[16], desIn_r[24], desIn_r[32], desIn_r[40], 
			desIn_r[48], desIn_r[56], desIn_r[07], desIn_r[15], desIn_r[23], desIn_r[31], 
			desIn_r[39], desIn_r[47], desIn_r[55], desIn_r[63], desIn_r[05], desIn_r[13],
			desIn_r[21], desIn_r[29], desIn_r[37], desIn_r[45], desIn_r[53], desIn_r[61],
			desIn_r[03], desIn_r[11], desIn_r[19], desIn_r[27], desIn_r[35], desIn_r[43],
			desIn_r[51], desIn_r[59], desIn_r[01], desIn_r[09], desIn_r[17], desIn_r[25],
			desIn_r[33], desIn_r[41], desIn_r[49], desIn_r[57] };

// Perform the final permutation
always @(posedge clk)
	desOut <= #1 {	FP[40], FP[08], FP[48], FP[16], FP[56], FP[24], FP[64], FP[32], 
			FP[39], FP[07], FP[47], FP[15], FP[55], FP[23], FP[63], FP[31], 
			FP[38], FP[06], FP[46], FP[14], FP[54], FP[22], FP[62], FP[30], 
			FP[37], FP[05], FP[45], FP[13], FP[53], FP[21], FP[61], FP[29], 
			FP[36], FP[04], FP[44], FP[12], FP[52], FP[20], FP[60], FP[28], 
			FP[35], FP[03], FP[43], FP[11], FP[51], FP[19], FP[59], FP[27],
			FP[34], FP[02], FP[42], FP[10], FP[50], FP[18], FP[58], FP[26], 
			FP[33], FP[01], FP[41], FP[09], FP[49], FP[17], FP[57], FP[25] };


endmodule
