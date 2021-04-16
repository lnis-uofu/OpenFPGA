/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Tripple DES                                                ////
////  Tripple DES Top Level module                               ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2002 Rudolf Usselmann                         ////
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

module des3(desOut, desIn, key1, key2, key3, decrypt, clk); 
output	[63:0]	desOut;
input	[63:0]	desIn;
input	[55:0]	key1;
input	[55:0]	key2;
input	[55:0]	key3;
input		decrypt;
input		clk;

wire	[55:0]	key_a;
wire	[55:0]	key_b;
wire	[55:0]	key_c;
wire	[63:0]	stage1_out, stage2_out;
reg	[55:0]	key_b_r [16:0];
reg	[55:0]	key_c_r [33:0];
integer		i;

assign key_a = decrypt ? key3 : key1;
assign key_b = key2;
assign key_c = decrypt ? key1 : key3;

always @(posedge clk)
	key_b_r[0] <= #1 key_b;

always @(posedge clk)
	for(i=0;i<16;i=i+1)
		key_b_r[i+1] <= #1 key_b_r[i];


always @(posedge clk)
	key_c_r[0] <= #1 key_c;

always @(posedge clk)
	for(i=0;i<33;i=i+1)
		key_c_r[i+1] <= #1 key_c_r[i];

des u0(	.desOut(stage1_out),	.desIn(desIn),		.key(key_a), .decrypt(decrypt), .clk(clk) );

des u1(	.desOut(stage2_out),	.desIn(stage1_out),	.key(key_b_r[16]), .decrypt(!decrypt), .clk(clk) );

des u2(	.desOut(desOut),	.desIn(stage2_out),	.key(key_c_r[33]), .decrypt(decrypt),	.clk(clk) );

endmodule


