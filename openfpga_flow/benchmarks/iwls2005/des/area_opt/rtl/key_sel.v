/////////////////////////////////////////////////////////////////////
////                                                             ////
////  KEY_SEL_Half                                               ////
////  Select one of 16 sub-keys for round                        ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////  Original: Rudolf Usselmann                                 ////
////                                                             ////
////  Modified : 2004/07/10                                      ////
////  Modified: Sakamoto YASUHIRO                                ////
////  Modified: for about Half slices decreased                  ////
////   (XILINX SPARTAN2 Number of SLICEs 546 to 258)             ////
////    Web   : http://hp.vector.co.jp/authors/VA014069          ////
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

module  key_sel(K_sub, K, roundSel, decrypt);
output [1:48] K_sub;
input [55:0] K;
input [3:0] roundSel;
input   decrypt;

reg  [1:48] K_sub;
wire [1:48] K1, K2, K3, K4, K5, K6, K7, K8;

////  Modified: for about Half slices decreased
wire [2:0] roundSelH;  // ADD Sakamoto
wire   decryptH;  // ADD Sakamoto

assign roundSelH[2:0] = roundSel[3] ? (~roundSel[2:0]) : roundSel[2:0];
assign decryptH  = decrypt ^ roundSel[3];

always @(K1 or K2 or K3 or K4 or K5 or K6 or K7 or K8 or roundSelH)
    case (roundSelH)    // synopsys full_case parallel_case
            0:  K_sub = K1;
            1:  K_sub = K2;
            2:  K_sub = K3;
            3:  K_sub = K4;
            4:  K_sub = K5;
            5:  K_sub = K6;
            6:  K_sub = K7;
            7:  K_sub = K8;
    endcase


assign K8[1]  = decryptH ? K[6]  : K[24];
assign K8[2]  = decryptH ? K[27] : K[20];
assign K8[3]  = decryptH ? K[10] : K[3] ;
assign K8[4]  = decryptH ? K[19] : K[12];
assign K8[5]  = decryptH ? K[54] : K[47];
assign K8[6]  = decryptH ? K[25] : K[18];
assign K8[7]  = decryptH ? K[11] : K[4] ;
assign K8[8]  = decryptH ? K[47] : K[40];
assign K8[9]  = decryptH ? K[13] : K[6] ;
assign K8[10] = decryptH ? K[32] : K[25];
assign K8[11] = decryptH ? K[55] : K[48];
assign K8[12] = decryptH ? K[3]  : K[53];
assign K8[13] = decryptH ? K[12] : K[5] ;
assign K8[14] = decryptH ? K[41] : K[34];
assign K8[15] = decryptH ? K[17] : K[10];
assign K8[16] = decryptH ? K[18] : K[11];
assign K8[17] = decryptH ? K[33] : K[26];
assign K8[18] = decryptH ? K[46] : K[39];
assign K8[19] = decryptH ? K[20] : K[13];
assign K8[20] = decryptH ? K[39] : K[32];
assign K8[21] = decryptH ? K[40] : K[33];
assign K8[22] = decryptH ? K[48] : K[41];
assign K8[23] = decryptH ? K[24] : K[17];
assign K8[24] = decryptH ? K[4]  : K[54];
assign K8[25] = decryptH ? K[52] : K[45];
assign K8[26] = decryptH ? K[15] : K[8] ;
assign K8[27] = decryptH ? K[9]  : K[2] ;
assign K8[28] = decryptH ? K[51] : K[44];
assign K8[29] = decryptH ? K[35] : K[28];
assign K8[30] = decryptH ? K[36] : K[29];
assign K8[31] = decryptH ? K[2]  : K[50];
assign K8[32] = decryptH ? K[45] : K[38];
assign K8[33] = decryptH ? K[8]  : K[1] ;
assign K8[34] = decryptH ? K[21] : K[14];
assign K8[35] = decryptH ? K[23] : K[16];
assign K8[36] = decryptH ? K[42] : K[35];
assign K8[37] = decryptH ? K[14] : K[7] ;
assign K8[38] = decryptH ? K[49] : K[42];
assign K8[39] = decryptH ? K[38] : K[31];
assign K8[40] = decryptH ? K[43] : K[36];
assign K8[41] = decryptH ? K[30] : K[23];
assign K8[42] = decryptH ? K[22] : K[15];
assign K8[43] = decryptH ? K[28] : K[21];
assign K8[44] = decryptH ? K[0]  : K[52];
assign K8[45] = decryptH ? K[1]  : K[49];
assign K8[46] = decryptH ? K[44] : K[37];
assign K8[47] = decryptH ? K[50] : K[43];
assign K8[48] = decryptH ? K[16] : K[9] ;
   
assign K7[1]  = decryptH ? K[20] : K[10];
assign K7[2]  = decryptH ? K[41] : K[6] ;
assign K7[3]  = decryptH ? K[24] : K[46];
assign K7[4]  = decryptH ? K[33] : K[55];
assign K7[5]  = decryptH ? K[11] : K[33];
assign K7[6]  = decryptH ? K[39] : K[4] ;
assign K7[7]  = decryptH ? K[25] : K[47];
assign K7[8]  = decryptH ? K[4]  : K[26];
assign K7[9]  = decryptH ? K[27] : K[17];
assign K7[10] = decryptH ? K[46] : K[11];
assign K7[11] = decryptH ? K[12] : K[34];
assign K7[12] = decryptH ? K[17] : K[39];
assign K7[13] = decryptH ? K[26] : K[48];
assign K7[14] = decryptH ? K[55] : K[20];
assign K7[15] = decryptH ? K[6]  : K[53];
assign K7[16] = decryptH ? K[32] : K[54];
assign K7[17] = decryptH ? K[47] : K[12];
assign K7[18] = decryptH ? K[3]  : K[25];
assign K7[19] = decryptH ? K[34] : K[24];
assign K7[20] = decryptH ? K[53] : K[18];
assign K7[21] = decryptH ? K[54] : K[19];
assign K7[22] = decryptH ? K[5]  : K[27];
assign K7[23] = decryptH ? K[13] : K[3] ;
assign K7[24] = decryptH ? K[18] : K[40];
assign K7[25] = decryptH ? K[7]  : K[31];
assign K7[26] = decryptH ? K[29] : K[49];
assign K7[27] = decryptH ? K[23] : K[43];
assign K7[28] = decryptH ? K[38] : K[30];
assign K7[29] = decryptH ? K[49] : K[14];
assign K7[30] = decryptH ? K[50] : K[15];
assign K7[31] = decryptH ? K[16] : K[36];
assign K7[32] = decryptH ? K[0]  : K[51];
assign K7[33] = decryptH ? K[22] : K[42];
assign K7[34] = decryptH ? K[35] : K[0] ;
assign K7[35] = decryptH ? K[37] : K[2] ;
assign K7[36] = decryptH ? K[1]  : K[21];
assign K7[37] = decryptH ? K[28] : K[52];
assign K7[38] = decryptH ? K[8]  : K[28];
assign K7[39] = decryptH ? K[52] : K[44];
assign K7[40] = decryptH ? K[2]  : K[22];
assign K7[41] = decryptH ? K[44] : K[9] ;
assign K7[42] = decryptH ? K[36] : K[1] ;
assign K7[43] = decryptH ? K[42] : K[7] ;
assign K7[44] = decryptH ? K[14] : K[38];
assign K7[45] = decryptH ? K[15] : K[35];
assign K7[46] = decryptH ? K[31] : K[23];
assign K7[47] = decryptH ? K[9]  : K[29];
assign K7[48] = decryptH ? K[30] : K[50];

assign K6[1]  = decryptH ? K[34] : K[53];
assign K6[2]  = decryptH ? K[55] : K[17];
assign K6[3]  = decryptH ? K[13] : K[32];
assign K6[4]  = decryptH ? K[47] : K[41];
assign K6[5]  = decryptH ? K[25] : K[19];
assign K6[6]  = decryptH ? K[53] : K[47];
assign K6[7]  = decryptH ? K[39] : K[33];
assign K6[8]  = decryptH ? K[18] : K[12];
assign K6[9]  = decryptH ? K[41] : K[3] ;
assign K6[10] = decryptH ? K[3]  : K[54];
assign K6[11] = decryptH ? K[26] : K[20];
assign K6[12] = decryptH ? K[6]  : K[25];
assign K6[13] = decryptH ? K[40] : K[34];
assign K6[14] = decryptH ? K[12] : K[6] ;
assign K6[15] = decryptH ? K[20] : K[39];
assign K6[16] = decryptH ? K[46] : K[40];
assign K6[17] = decryptH ? K[4]  : K[55];
assign K6[18] = decryptH ? K[17] : K[11];
assign K6[19] = decryptH ? K[48] : K[10];
assign K6[20] = decryptH ? K[10] : K[4] ;
assign K6[21] = decryptH ? K[11] : K[5] ;
assign K6[22] = decryptH ? K[19] : K[13];
assign K6[23] = decryptH ? K[27] : K[46];
assign K6[24] = decryptH ? K[32] : K[26];
assign K6[25] = decryptH ? K[21] : K[44];
assign K6[26] = decryptH ? K[43] : K[35];
assign K6[27] = decryptH ? K[37] : K[29];
assign K6[28] = decryptH ? K[52] : K[16];
assign K6[29] = decryptH ? K[8]  : K[0] ;
assign K6[30] = decryptH ? K[9]  : K[1] ;
assign K6[31] = decryptH ? K[30] : K[22];
assign K6[32] = decryptH ? K[14] : K[37];
assign K6[33] = decryptH ? K[36] : K[28];
assign K6[34] = decryptH ? K[49] : K[45];
assign K6[35] = decryptH ? K[51] : K[43];
assign K6[36] = decryptH ? K[15] : K[7] ;
assign K6[37] = decryptH ? K[42] : K[38];
assign K6[38] = decryptH ? K[22] : K[14];
assign K6[39] = decryptH ? K[7]  : K[30];
assign K6[40] = decryptH ? K[16] : K[8] ;
assign K6[41] = decryptH ? K[31] : K[50];
assign K6[42] = decryptH ? K[50] : K[42];
assign K6[43] = decryptH ? K[1]  : K[52];
assign K6[44] = decryptH ? K[28] : K[51];
assign K6[45] = decryptH ? K[29] : K[21];
assign K6[46] = decryptH ? K[45] : K[9] ;
assign K6[47] = decryptH ? K[23] : K[15];
assign K6[48] = decryptH ? K[44] : K[36];

assign K5[1]  = decryptH ? K[48] : K[39];
assign K5[2]  = decryptH ? K[12] : K[3] ;
assign K5[3]  = decryptH ? K[27] : K[18];
assign K5[4]  = decryptH ? K[4]  : K[27];
assign K5[5]  = decryptH ? K[39] : K[5] ;
assign K5[6]  = decryptH ? K[10] : K[33];
assign K5[7]  = decryptH ? K[53] : K[19];
assign K5[8]  = decryptH ? K[32] : K[55];
assign K5[9]  = decryptH ? K[55] : K[46];
assign K5[10] = decryptH ? K[17] : K[40];
assign K5[11] = decryptH ? K[40] : K[6] ;
assign K5[12] = decryptH ? K[20] : K[11];
assign K5[13] = decryptH ? K[54] : K[20];
assign K5[14] = decryptH ? K[26] : K[17];
assign K5[15] = decryptH ? K[34] : K[25];
assign K5[16] = decryptH ? K[3]  : K[26];
assign K5[17] = decryptH ? K[18] : K[41];
assign K5[18] = decryptH ? K[6]  : K[54];
assign K5[19] = decryptH ? K[5]  : K[53];
assign K5[20] = decryptH ? K[24] : K[47];
assign K5[21] = decryptH ? K[25] : K[48];
assign K5[22] = decryptH ? K[33] : K[24];
assign K5[23] = decryptH ? K[41] : K[32];
assign K5[24] = decryptH ? K[46] : K[12];
assign K5[25] = decryptH ? K[35] : K[30];
assign K5[26] = decryptH ? K[2]  : K[21];
assign K5[27] = decryptH ? K[51] : K[15];
assign K5[28] = decryptH ? K[7]  : K[2] ;
assign K5[29] = decryptH ? K[22] : K[45];
assign K5[30] = decryptH ? K[23] : K[42];
assign K5[31] = decryptH ? K[44] : K[8] ;
assign K5[32] = decryptH ? K[28] : K[23];
assign K5[33] = decryptH ? K[50] : K[14];
assign K5[34] = decryptH ? K[8]  : K[31];
assign K5[35] = decryptH ? K[38] : K[29];
assign K5[36] = decryptH ? K[29] : K[52];
assign K5[37] = decryptH ? K[1]  : K[51];
assign K5[38] = decryptH ? K[36] : K[0] ;
assign K5[39] = decryptH ? K[21] : K[16];
assign K5[40] = decryptH ? K[30] : K[49];
assign K5[41] = decryptH ? K[45] : K[36];
assign K5[42] = decryptH ? K[9]  : K[28];
assign K5[43] = decryptH ? K[15] : K[38];
assign K5[44] = decryptH ? K[42] : K[37];
assign K5[45] = decryptH ? K[43] : K[7] ;
assign K5[46] = decryptH ? K[0]  : K[50];
assign K5[47] = decryptH ? K[37] : K[1] ;
assign K5[48] = decryptH ? K[31] : K[22];

assign K4[1]  = decryptH ? K[5]  : K[25];
assign K4[2]  = decryptH ? K[26] : K[46];
assign K4[3]  = decryptH ? K[41] : K[4] ;
assign K4[4]  = decryptH ? K[18] : K[13];
assign K4[5]  = decryptH ? K[53] : K[48];
assign K4[6]  = decryptH ? K[24] : K[19];
assign K4[7]  = decryptH ? K[10] : K[5] ;
assign K4[8]  = decryptH ? K[46] : K[41];
assign K4[9]  = decryptH ? K[12] : K[32];
assign K4[10] = decryptH ? K[6]  : K[26];
assign K4[11] = decryptH ? K[54] : K[17];
assign K4[12] = decryptH ? K[34] : K[54];
assign K4[13] = decryptH ? K[11] : K[6] ;
assign K4[14] = decryptH ? K[40] : K[3] ;
assign K4[15] = decryptH ? K[48] : K[11];
assign K4[16] = decryptH ? K[17] : K[12];
assign K4[17] = decryptH ? K[32] : K[27];
assign K4[18] = decryptH ? K[20] : K[40];
assign K4[19] = decryptH ? K[19] : K[39];
assign K4[20] = decryptH ? K[13] : K[33];
assign K4[21] = decryptH ? K[39] : K[34];
assign K4[22] = decryptH ? K[47] : K[10];
assign K4[23] = decryptH ? K[55] : K[18];
assign K4[24] = decryptH ? K[3]  : K[55];
assign K4[25] = decryptH ? K[49] : K[16];
assign K4[26] = decryptH ? K[16] : K[7] ;
assign K4[27] = decryptH ? K[38] : K[1] ;
assign K4[28] = decryptH ? K[21] : K[43];
assign K4[29] = decryptH ? K[36] : K[31];
assign K4[30] = decryptH ? K[37] : K[28];
assign K4[31] = decryptH ? K[31] : K[49];
assign K4[32] = decryptH ? K[42] : K[9] ;
assign K4[33] = decryptH ? K[9]  : K[0] ;
assign K4[34] = decryptH ? K[22] : K[44];
assign K4[35] = decryptH ? K[52] : K[15];
assign K4[36] = decryptH ? K[43] : K[38];
assign K4[37] = decryptH ? K[15] : K[37];
assign K4[38] = decryptH ? K[50] : K[45];
assign K4[39] = decryptH ? K[35] : K[2] ;
assign K4[40] = decryptH ? K[44] : K[35];
assign K4[41] = decryptH ? K[0]  : K[22];
assign K4[42] = decryptH ? K[23] : K[14];
assign K4[43] = decryptH ? K[29] : K[51];
assign K4[44] = decryptH ? K[1]  : K[23];
assign K4[45] = decryptH ? K[2]  : K[52];
assign K4[46] = decryptH ? K[14] : K[36];
assign K4[47] = decryptH ? K[51] : K[42];
assign K4[48] = decryptH ? K[45] : K[8] ;

assign K3[1]  = decryptH ? K[19] : K[11];
assign K3[2]  = decryptH ? K[40] : K[32];
assign K3[3]  = decryptH ? K[55] : K[47];
assign K3[4]  = decryptH ? K[32] : K[24];
assign K3[5]  = decryptH ? K[10] : K[34];
assign K3[6]  = decryptH ? K[13] : K[5] ;
assign K3[7]  = decryptH ? K[24] : K[48];
assign K3[8]  = decryptH ? K[3]  : K[27];
assign K3[9]  = decryptH ? K[26] : K[18];
assign K3[10] = decryptH ? K[20] : K[12];
assign K3[11] = decryptH ? K[11] : K[3] ;
assign K3[12] = decryptH ? K[48] : K[40];
assign K3[13] = decryptH ? K[25] : K[17];
assign K3[14] = decryptH ? K[54] : K[46];
assign K3[15] = decryptH ? K[5]  : K[54];
assign K3[16] = decryptH ? K[6]  : K[55];
assign K3[17] = decryptH ? K[46] : K[13];
assign K3[18] = decryptH ? K[34] : K[26];
assign K3[19] = decryptH ? K[33] : K[25];
assign K3[20] = decryptH ? K[27] : K[19];
assign K3[21] = decryptH ? K[53] : K[20];
assign K3[22] = decryptH ? K[4]  : K[53];
assign K3[23] = decryptH ? K[12] : K[4] ;
assign K3[24] = decryptH ? K[17] : K[41];
assign K3[25] = decryptH ? K[8]  : K[2] ;
assign K3[26] = decryptH ? K[30] : K[52];
assign K3[27] = decryptH ? K[52] : K[42];
assign K3[28] = decryptH ? K[35] : K[29];
assign K3[29] = decryptH ? K[50] : K[44];
assign K3[30] = decryptH ? K[51] : K[14];
assign K3[31] = decryptH ? K[45] : K[35];
assign K3[32] = decryptH ? K[1]  : K[50];
assign K3[33] = decryptH ? K[23] : K[45];
assign K3[34] = decryptH ? K[36] : K[30];
assign K3[35] = decryptH ? K[7]  : K[1] ;
assign K3[36] = decryptH ? K[2]  : K[51];
assign K3[37] = decryptH ? K[29] : K[23];
assign K3[38] = decryptH ? K[9]  : K[31];
assign K3[39] = decryptH ? K[49] : K[43];
assign K3[40] = decryptH ? K[31] : K[21];
assign K3[41] = decryptH ? K[14] : K[8] ;
assign K3[42] = decryptH ? K[37] : K[0] ;
assign K3[43] = decryptH ? K[43] : K[37];
assign K3[44] = decryptH ? K[15] : K[9] ;
assign K3[45] = decryptH ? K[16] : K[38];
assign K3[46] = decryptH ? K[28] : K[22];
assign K3[47] = decryptH ? K[38] : K[28];
assign K3[48] = decryptH ? K[0]  : K[49];

assign K2[1]  = decryptH ? K[33] : K[54];
assign K2[2]  = decryptH ? K[54] : K[18];
assign K2[3]  = decryptH ? K[12] : K[33];
assign K2[4]  = decryptH ? K[46] : K[10];
assign K2[5]  = decryptH ? K[24] : K[20];
assign K2[6]  = decryptH ? K[27] : K[48];
assign K2[7]  = decryptH ? K[13] : K[34];
assign K2[8]  = decryptH ? K[17] : K[13];
assign K2[9]  = decryptH ? K[40] : K[4] ;
assign K2[10] = decryptH ? K[34] : K[55];
assign K2[11] = decryptH ? K[25] : K[46];
assign K2[12] = decryptH ? K[5]  : K[26];
assign K2[13] = decryptH ? K[39] : K[3] ;
assign K2[14] = decryptH ? K[11] : K[32];
assign K2[15] = decryptH ? K[19] : K[40];
assign K2[16] = decryptH ? K[20] : K[41];
assign K2[17] = decryptH ? K[3]  : K[24];
assign K2[18] = decryptH ? K[48] : K[12];
assign K2[19] = decryptH ? K[47] : K[11];
assign K2[20] = decryptH ? K[41] : K[5] ;
assign K2[21] = decryptH ? K[10] : K[6] ;
assign K2[22] = decryptH ? K[18] : K[39];
assign K2[23] = decryptH ? K[26] : K[47];
assign K2[24] = decryptH ? K[6]  : K[27];
assign K2[25] = decryptH ? K[22] : K[43];
assign K2[26] = decryptH ? K[44] : K[38];
assign K2[27] = decryptH ? K[7]  : K[28];
assign K2[28] = decryptH ? K[49] : K[15];
assign K2[29] = decryptH ? K[9]  : K[30];
assign K2[30] = decryptH ? K[38] : K[0] ;
assign K2[31] = decryptH ? K[0]  : K[21];
assign K2[32] = decryptH ? K[15] : K[36];
assign K2[33] = decryptH ? K[37] : K[31];
assign K2[34] = decryptH ? K[50] : K[16];
assign K2[35] = decryptH ? K[21] : K[42];
assign K2[36] = decryptH ? K[16] : K[37];
assign K2[37] = decryptH ? K[43] : K[9] ;
assign K2[38] = decryptH ? K[23] : K[44];
assign K2[39] = decryptH ? K[8]  : K[29];
assign K2[40] = decryptH ? K[45] : K[7] ;
assign K2[41] = decryptH ? K[28] : K[49];
assign K2[42] = decryptH ? K[51] : K[45];
assign K2[43] = decryptH ? K[2]  : K[23];
assign K2[44] = decryptH ? K[29] : K[50];
assign K2[45] = decryptH ? K[30] : K[51];
assign K2[46] = decryptH ? K[42] : K[8] ;
assign K2[47] = decryptH ? K[52] : K[14];
assign K2[48] = decryptH ? K[14] : K[35];

assign K1[1]  = decryptH ? K[40] : K[47];
assign K1[2]  = decryptH ? K[4]  : K[11];
assign K1[3]  = decryptH ? K[19] : K[26];
assign K1[4]  = decryptH ? K[53] : K[3] ;
assign K1[5]  = decryptH ? K[6]  : K[13];
assign K1[6]  = decryptH ? K[34] : K[41];
assign K1[7]  = decryptH ? K[20] : K[27];
assign K1[8]  = decryptH ? K[24] : K[6] ;
assign K1[9]  = decryptH ? K[47] : K[54];
assign K1[10] = decryptH ? K[41] : K[48];
assign K1[11] = decryptH ? K[32] : K[39];
assign K1[12] = decryptH ? K[12] : K[19];
assign K1[13] = decryptH ? K[46] : K[53];
assign K1[14] = decryptH ? K[18] : K[25];
assign K1[15] = decryptH ? K[26] : K[33];
assign K1[16] = decryptH ? K[27] : K[34];
assign K1[17] = decryptH ? K[10] : K[17];
assign K1[18] = decryptH ? K[55] : K[5] ;
assign K1[19] = decryptH ? K[54] : K[4] ;
assign K1[20] = decryptH ? K[48] : K[55];
assign K1[21] = decryptH ? K[17] : K[24];
assign K1[22] = decryptH ? K[25] : K[32];
assign K1[23] = decryptH ? K[33] : K[40];
assign K1[24] = decryptH ? K[13] : K[20];
assign K1[25] = decryptH ? K[29] : K[36];
assign K1[26] = decryptH ? K[51] : K[31];
assign K1[27] = decryptH ? K[14] : K[21];
assign K1[28] = decryptH ? K[1]  : K[8] ;
assign K1[29] = decryptH ? K[16] : K[23];
assign K1[30] = decryptH ? K[45] : K[52];
assign K1[31] = decryptH ? K[7]  : K[14];
assign K1[32] = decryptH ? K[22] : K[29];
assign K1[33] = decryptH ? K[44] : K[51];
assign K1[34] = decryptH ? K[2]  : K[9] ;
assign K1[35] = decryptH ? K[28] : K[35];
assign K1[36] = decryptH ? K[23] : K[30];
assign K1[37] = decryptH ? K[50] : K[2] ;
assign K1[38] = decryptH ? K[30] : K[37];
assign K1[39] = decryptH ? K[15] : K[22];
assign K1[40] = decryptH ? K[52] : K[0] ;
assign K1[41] = decryptH ? K[35] : K[42];
assign K1[42] = decryptH ? K[31] : K[38];
assign K1[43] = decryptH ? K[9]  : K[16];
assign K1[44] = decryptH ? K[36] : K[43];
assign K1[45] = decryptH ? K[37] : K[44];
assign K1[46] = decryptH ? K[49] : K[1] ;
assign K1[47] = decryptH ? K[0]  : K[7] ;
assign K1[48] = decryptH ? K[21] : K[28];

endmodule
