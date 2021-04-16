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

module  sbox5(addr, dout);
input	[1:6] addr;
output	[1:4] dout;
reg	[1:4] dout;

always @(addr) begin
    case ({addr[1], addr[6], addr[2:5]})	//synopsys full_case parallel_case
         0:  dout =  2;
         1:  dout = 12;
         2:  dout =  4;
         3:  dout =  1;
         4:  dout =  7;
         5:  dout = 10;
         6:  dout = 11;
         7:  dout =  6;
         8:  dout =  8;
         9:  dout =  5;
        10:  dout =  3;
        11:  dout = 15;
        12:  dout = 13;
        13:  dout =  0;
        14:  dout = 14;
        15:  dout =  9;

        16:  dout = 14;
        17:  dout = 11;
        18:  dout =  2;
        19:  dout = 12;
        20:  dout =  4;
        21:  dout =  7;
        22:  dout = 13;
        23:  dout =  1;
        24:  dout =  5;
        25:  dout =  0;
        26:  dout = 15;
        27:  dout = 10;
        28:  dout =  3;
        29:  dout =  9;
        30:  dout =  8;
        31:  dout =  6;

        32:  dout =  4;
        33:  dout =  2;
        34:  dout =  1;
        35:  dout = 11;
        36:  dout = 10;
        37:  dout = 13;
        38:  dout =  7;
        39:  dout =  8;
        40:  dout = 15;
        41:  dout =  9;
        42:  dout = 12;
        43:  dout =  5;
        44:  dout =  6;
        45:  dout =  3;
        46:  dout =  0;
        47:  dout = 14;

        48:  dout = 11;
        49:  dout =  8;
        50:  dout = 12;
        51:  dout =  7;
        52:  dout =  1;
        53:  dout = 14;
        54:  dout =  2;
        55:  dout = 13;
        56:  dout =  6;
        57:  dout = 15;
        58:  dout =  0;
        59:  dout =  9;
        60:  dout = 10;
        61:  dout =  4;
        62:  dout =  5;
        63:  dout =  3;

    endcase
    end

endmodule
