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

module  sbox4(addr, dout);
input	[1:6] addr;
output	[1:4] dout;
reg	[1:4] dout;

always @(addr) begin
    case ({addr[1], addr[6], addr[2:5]})	//synopsys full_case parallel_case
         0:  dout =  7;
         1:  dout = 13;
         2:  dout = 14;
         3:  dout =  3;
         4:  dout =  0;
         5:  dout =  6;
         6:  dout =  9;
         7:  dout = 10;
         8:  dout =  1;
         9:  dout =  2;
        10:  dout =  8;
        11:  dout =  5;
        12:  dout = 11;
        13:  dout = 12;
        14:  dout =  4;
        15:  dout = 15;

        16:  dout = 13;
        17:  dout =  8;
        18:  dout = 11;
        19:  dout =  5;
        20:  dout =  6;
        21:  dout = 15;
        22:  dout =  0;
        23:  dout =  3;
        24:  dout =  4;
        25:  dout =  7;
        26:  dout =  2;
        27:  dout = 12;
        28:  dout =  1;
        29:  dout = 10;
        30:  dout = 14;
        31:  dout =  9;

        32:  dout = 10;
        33:  dout =  6;
        34:  dout =  9;
        35:  dout =  0;
        36:  dout = 12;
        37:  dout = 11;
        38:  dout =  7;
        39:  dout = 13;
        40:  dout = 15;
        41:  dout =  1;
        42:  dout =  3;
        43:  dout = 14;
        44:  dout =  5;
        45:  dout =  2;
        46:  dout =  8;
        47:  dout =  4;

        48:  dout =  3;
        49:  dout = 15;
        50:  dout =  0;
        51:  dout =  6;
        52:  dout = 10;
        53:  dout =  1;
        54:  dout = 13;
        55:  dout =  8;
        56:  dout =  9;
        57:  dout =  4;
        58:  dout =  5;
        59:  dout = 11;
        60:  dout = 12;
        61:  dout =  7;
        62:  dout =  2;
        63:  dout = 14;

    endcase
    end

endmodule
