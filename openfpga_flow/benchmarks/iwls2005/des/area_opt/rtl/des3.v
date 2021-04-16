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

module des3(desOut, desIn, key1, key2, key3, decrypt, roundSel, clk);
output	[63:0]	desOut;
input	[63:0]	desIn;
input	[55:0]	key1;
input	[55:0]	key2;
input	[55:0]	key3;
input		decrypt;
input	[5:0]	roundSel;
input		clk;

wire	[1:48]	K_sub;
wire	[1:64]	IP, FP, tmp;
reg	[1:64]	FP_R;
reg	[1:32]	L, R;
wire	[1:32]	Xin;
wire	[1:32]	Lout;
wire	[1:32]	Rout;
wire	[1:32]	out;

//assign Lout = (roundSel == 0) ? IP[33:64] : R;
//assign Xin  = (roundSel == 0) ? IP[01:32] : L;


assign Lout =	(roundSel == 0) ? IP[33:64] :
		(	(roundSel == 16) ? FP_R[33:64] :
			(	(roundSel == 32) ? FP_R[33:64] :
					R	));

assign Xin  =	(roundSel == 0) ? IP[01:32] :
		(	(roundSel == 16) ? FP_R[01:32] :
			(	(roundSel == 32) ? FP_R[01:32] :
					L	));



/*
always @(roundSel or IP or tmp or R or FP)
	case(roundSel)
		6'h0:	Lout = IP[33:64];
		6'h10:	Lout = FP[33:64];
		6'h20:	Lout = FP[33:64];
		default: Lout = R;
	endcase

always @(roundSel or IP or tmp or L or FP)
	case(roundSel)
		6'h0:	Xin = IP[01:32];
		6'h10:	Xin = FP[01:32];
		6'h20:	Xin = FP[01:32];
		default: Xin = L;
	endcase
*/

always @(posedge clk)
	FP_R <= #1 FP;

assign Rout = Xin ^ out;
assign FP = { Rout, Lout};

crp u0( .P(out), .R(Lout), .K_sub(K_sub) );

always @(posedge clk)
        L <= #1 Lout;

always @(posedge clk)
        R <= #1 Rout;
        
// Select a subkey from key.
key_sel3 u1(
	.K_sub(		K_sub		),
	.key1(		key1		),
	.key2(		key2		),
	.key3(		key3		),
	.roundSel(	roundSel	),
	.decrypt(	decrypt		)
	);

assign tmp[1:64] = {	desOut[06], desOut[14], desOut[22], desOut[30], desOut[38], desOut[46],
			desOut[54], desOut[62], desOut[04], desOut[12], desOut[20], desOut[28],
			desOut[36], desOut[44], desOut[52], desOut[60], desOut[02], desOut[10], 
			desOut[18], desOut[26], desOut[34], desOut[42], desOut[50], desOut[58], 
			desOut[00], desOut[08], desOut[16], desOut[24], desOut[32], desOut[40], 
			desOut[48], desOut[56], desOut[07], desOut[15], desOut[23], desOut[31], 
			desOut[39], desOut[47], desOut[55], desOut[63], desOut[05], desOut[13],
			desOut[21], desOut[29], desOut[37], desOut[45], desOut[53], desOut[61],
			desOut[03], desOut[11], desOut[19], desOut[27], desOut[35], desOut[43],
			desOut[51], desOut[59], desOut[01], desOut[09], desOut[17], desOut[25],
			desOut[33], desOut[41], desOut[49], desOut[57] };

// Perform initial permutation
assign IP[1:64] = {	desIn[06], desIn[14], desIn[22], desIn[30], desIn[38], desIn[46],
			desIn[54], desIn[62], desIn[04], desIn[12], desIn[20], desIn[28],
			desIn[36], desIn[44], desIn[52], desIn[60], desIn[02], desIn[10], 
			desIn[18], desIn[26], desIn[34], desIn[42], desIn[50], desIn[58], 
			desIn[00], desIn[08], desIn[16], desIn[24], desIn[32], desIn[40], 
			desIn[48], desIn[56], desIn[07], desIn[15], desIn[23], desIn[31], 
			desIn[39], desIn[47], desIn[55], desIn[63], desIn[05], desIn[13],
			desIn[21], desIn[29], desIn[37], desIn[45], desIn[53], desIn[61],
			desIn[03], desIn[11], desIn[19], desIn[27], desIn[35], desIn[43],
			desIn[51], desIn[59], desIn[01], desIn[09], desIn[17], desIn[25],
			desIn[33], desIn[41], desIn[49], desIn[57] };

// Perform final permutation
assign desOut = {	FP[40], FP[08], FP[48], FP[16], FP[56], FP[24], FP[64], FP[32], 
			FP[39], FP[07], FP[47], FP[15], FP[55], FP[23], FP[63], FP[31], 
			FP[38], FP[06], FP[46], FP[14], FP[54], FP[22], FP[62], FP[30], 
			FP[37], FP[05], FP[45], FP[13], FP[53], FP[21], FP[61], FP[29], 
			FP[36], FP[04], FP[44], FP[12], FP[52], FP[20], FP[60], FP[28], 
			FP[35], FP[03], FP[43], FP[11], FP[51], FP[19], FP[59], FP[27],
			FP[34], FP[02], FP[42], FP[10], FP[50], FP[18], FP[58], FP[26], 
			FP[33], FP[01], FP[41], FP[09], FP[49], FP[17], FP[57], FP[25] };


endmodule
