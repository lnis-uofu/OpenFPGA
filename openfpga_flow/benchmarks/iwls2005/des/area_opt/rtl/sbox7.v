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

module  sbox7(addr, dout);
input	[1:6] addr;
output	[1:4] dout;
reg	[1:4] dout;

always @(addr) begin
    case ({addr[1], addr[6], addr[2:5]})	//synopsys full_case parallel_case
         0:  dout =  4;
         1:  dout = 11;
         2:  dout =  2;
         3:  dout = 14;
         4:  dout = 15;
         5:  dout =  0;
         6:  dout =  8;
         7:  dout = 13;
         8:  dout =  3;
         9:  dout = 12;
        10:  dout =  9;
        11:  dout =  7;
        12:  dout =  5;
        13:  dout = 10;
        14:  dout =  6;
        15:  dout =  1;

        16:  dout = 13;
        17:  dout =  0;
        18:  dout = 11;
        19:  dout =  7;
        20:  dout =  4;
        21:  dout =  9;
        22:  dout =  1;
        23:  dout = 10;
        24:  dout = 14;
        25:  dout =  3;
        26:  dout =  5;
        27:  dout = 12;
        28:  dout =  2;
        29:  dout = 15;
        30:  dout =  8;
        31:  dout =  6;

        32:  dout =  1;
        33:  dout =  4;
        34:  dout = 11;
        35:  dout = 13;
        36:  dout = 12;
        37:  dout =  3;
        38:  dout =  7;
        39:  dout = 14;
        40:  dout = 10;
        41:  dout = 15;
        42:  dout =  6;
        43:  dout =  8;
        44:  dout =  0;
        45:  dout =  5;
        46:  dout =  9;
        47:  dout =  2;

        48:  dout =  6;
        49:  dout = 11;
        50:  dout = 13;
        51:  dout =  8;
        52:  dout =  1;
        53:  dout =  4;
        54:  dout = 10;
        55:  dout =  7;
        56:  dout =  9;
        57:  dout =  5;
        58:  dout =  0;
        59:  dout = 15;
        60:  dout = 14;
        61:  dout =  2;
        62:  dout =  3;
        63:  dout = 12;

    endcase
    end

endmodule
