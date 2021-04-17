/////////////////////////////////////////////////////////////////////
////                                                             ////
////  SBOX                                                       ////
////  The SBOX is essentially a 64x4 ROM                         ////
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

module  sbox6(addr, dout);
input	[1:6] addr;
output	[1:4] dout;
reg	[1:4] dout;

always @(addr) begin
    case ({addr[1], addr[6], addr[2:5]})	//synopsys full_case parallel_case
         0:  dout = 12;
         1:  dout =  1;
         2:  dout = 10;
         3:  dout = 15;
         4:  dout =  9;
         5:  dout =  2;
         6:  dout =  6;
         7:  dout =  8;
         8:  dout =  0;
         9:  dout = 13;
        10:  dout =  3;
        11:  dout =  4;
        12:  dout = 14;
        13:  dout =  7;
        14:  dout =  5;
        15:  dout = 11;

        16:  dout = 10;
        17:  dout = 15;
        18:  dout =  4;
        19:  dout =  2;
        20:  dout =  7;
        21:  dout = 12;
        22:  dout =  9;
        23:  dout =  5;
        24:  dout =  6;
        25:  dout =  1;
        26:  dout = 13;
        27:  dout = 14;
        28:  dout =  0;
        29:  dout = 11;
        30:  dout =  3;
        31:  dout =  8;

        32:  dout =  9;
        33:  dout = 14;
        34:  dout = 15;
        35:  dout =  5;
        36:  dout =  2;
        37:  dout =  8;
        38:  dout = 12;
        39:  dout =  3;
        40:  dout =  7;
        41:  dout =  0;
        42:  dout =  4;
        43:  dout = 10;
        44:  dout =  1;
        45:  dout = 13;
        46:  dout = 11;
        47:  dout =  6;

        48:  dout =  4;
        49:  dout =  3;
        50:  dout =  2;
        51:  dout = 12;
        52:  dout =  9;
        53:  dout =  5;
        54:  dout = 15;
        55:  dout = 10;
        56:  dout = 11;
        57:  dout = 14;
        58:  dout =  1;
        59:  dout =  7;
        60:  dout =  6;
        61:  dout =  0;
        62:  dout =  8;
        63:  dout = 13;

    endcase
    end

endmodule
