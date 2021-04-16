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

module  sbox3(addr, dout);
input	[1:6] addr;
output	[1:4] dout;
reg	[1:4] dout;

always @(addr) begin
    case ({addr[1], addr[6], addr[2:5]})	//synopsys full_case parallel_case
         0:  dout = 10;
         1:  dout =  0;
         2:  dout =  9;
         3:  dout = 14;
         4:  dout =  6;
         5:  dout =  3;
         6:  dout = 15;
         7:  dout =  5;
         8:  dout =  1;
         9:  dout = 13;
        10:  dout = 12;
        11:  dout =  7;
        12:  dout = 11;
        13:  dout =  4;
        14:  dout =  2;
        15:  dout =  8;

        16:  dout = 13;
        17:  dout =  7;
        18:  dout =  0;
        19:  dout =  9;
        20:  dout =  3;
        21:  dout =  4;
        22:  dout =  6;
        23:  dout = 10;
        24:  dout =  2;
        25:  dout =  8;
        26:  dout =  5;
        27:  dout = 14;
        28:  dout = 12;
        29:  dout = 11;
        30:  dout = 15;
        31:  dout =  1;

        32:  dout = 13;
        33:  dout =  6;
        34:  dout =  4;
        35:  dout =  9;
        36:  dout =  8;
        37:  dout = 15;
        38:  dout =  3;
        39:  dout =  0;
        40:  dout = 11;
        41:  dout =  1;
        42:  dout =  2;
        43:  dout = 12;
        44:  dout =  5;
        45:  dout = 10;
        46:  dout = 14;
        47:  dout =  7;

        48:  dout =  1;
        49:  dout = 10;
        50:  dout = 13;
        51:  dout =  0;
        52:  dout =  6;
        53:  dout =  9;
        54:  dout =  8;
        55:  dout =  7;
        56:  dout =  4;
        57:  dout = 15;
        58:  dout = 14;
        59:  dout =  3;
        60:  dout = 11;
        61:  dout =  5;
        62:  dout =  2;
        63:  dout = 12;

    endcase
    end

endmodule
