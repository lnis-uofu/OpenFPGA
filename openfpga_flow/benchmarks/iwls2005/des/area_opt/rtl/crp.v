/////////////////////////////////////////////////////////////////////
////                                                             ////
////  CRP                                                        ////
////  DES Crypt Module                                           ////
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

module  crp(P, R, K_sub);
output	[1:32]	P;
input	[1:32]	R;
input	[1:48]	K_sub;

wire	[1:48] E;
wire	[1:48] X;
wire	[1:32] S;

assign E[1:48] = {	R[32], R[1], R[2], R[3], R[4], R[5], R[4], R[5],
			R[6], R[7], R[8], R[9], R[8], R[9], R[10], R[11],
			R[12], R[13], R[12], R[13], R[14], R[15], R[16],
			R[17], R[16], R[17], R[18], R[19], R[20], R[21],
			R[20], R[21], R[22], R[23], R[24], R[25], R[24],
			R[25], R[26], R[27], R[28], R[29], R[28], R[29],
			R[30], R[31], R[32], R[1]};

assign X = E ^ K_sub;

sbox1 u0( .addr(X[01:06]), .dout(S[01:04]) );
sbox2 u1( .addr(X[07:12]), .dout(S[05:08]) );
sbox3 u2( .addr(X[13:18]), .dout(S[09:12]) );
sbox4 u3( .addr(X[19:24]), .dout(S[13:16]) );
sbox5 u4( .addr(X[25:30]), .dout(S[17:20]) );
sbox6 u5( .addr(X[31:36]), .dout(S[21:24]) );
sbox7 u6( .addr(X[37:42]), .dout(S[25:28]) );
sbox8 u7( .addr(X[43:48]), .dout(S[29:32]) );

assign P[1:32] = {	S[16], S[7], S[20], S[21], S[29], S[12], S[28],
			S[17], S[1], S[15], S[23], S[26], S[5], S[18],
			S[31], S[10], S[2], S[8], S[24], S[14], S[32],
			S[27], S[3], S[9], S[19], S[13], S[30], S[6],
			S[22], S[11], S[4], S[25]};

endmodule
