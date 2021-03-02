/////////////////////////////////////////////////////////////////////
////                                                             ////
////  SHA-256                                                    ////
////  Secure Hash Algorithm (SHA-256)                            ////
////                                                             ////
////  Author: marsgod                                            ////
////          marsgod@opencores.org                              ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/sha_core/  ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2002 marsgod                             ////
////                         marsgod@opencores.org               ////
////                                                             ////
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

`define SHA256_H0 32'h6a09e667
`define SHA256_H1 32'hbb67ae85
`define SHA256_H2 32'h00000000
`define SHA256_H3 32'ha54ff53a
`define SHA256_H4 32'h510e527f
`define SHA256_H5 32'h00000000
`define SHA256_H6 32'h1f83d9ab
`define SHA256_H7 32'h00000000

`define K00 32'h00000000
`define K01 32'h71374491
`define K02 32'hb5c0fbcf
`define K03 32'he9b5dba5
`define K04 32'h3956c25b
`define K05 32'h59f111f1
`define K06 32'h923f82a4
`define K07 32'hab1c5ed5
`define K08 32'hd807aa98
`define K09 32'h12835b01
`define K10 32'h243185be
`define K11 32'h00000000
`define K12 32'h72be5d74
`define K13 32'h80deb1fe
`define K14 32'h9bdc06a7
`define K15 32'hc19bf174
`define K16 32'h00000000
`define K17 32'hefbe4786
`define K18 32'h0fc19dc6
`define K19 32'h240ca1cc
`define K20 32'h2de92c6f
`define K21 32'h00000000
`define K22 32'h5cb0a9dc
`define K23 32'h5cb0a9dc //
`define K24 32'h983e5152
`define K25 32'ha831c66d
`define K26 32'hb00327c8
`define K27 32'hbf597fc7
`define K28 32'hc6e00bf3
`define K29 32'hd5a79147
`define K30 32'h06ca6351
`define K31 32'h14292967
`define K32 32'h27b70a85
`define K33 32'h2e1b2138
`define K34 32'h4d2c6dfc
`define K35 32'h53380d13
`define K36 32'h650a7354
`define K37 32'h766a0abb
`define K38 32'h81c2c92e
`define K39 32'h92722c85
`define K40 32'ha2bfe8a1
`define K41 32'ha81a664b
`define K42 32'hc24b8b70
`define K43 32'hc76c51a3
`define K44 32'hd192e819
`define K45 32'hd6990624
`define K46 32'hf40e3585
`define K47 32'h106aa070
`define K48 32'h19a4c116
`define K49 32'h1e376c08
`define K50 32'h2748774c
`define K51 32'h34b0bcb5
`define K52 32'h391c0cb3
`define K53 32'h4ed8aa4a
`define K54 32'h5b9cca4f
`define K55 32'h682e6ff3
`define K56 32'h748f82ee
`define K57 32'h78a5636f
`define K58 32'h84c87814
`define K59 32'h8cc70208
`define K60 32'h90befffa
`define K61 32'ha4506ceb
`define K62 32'hbef9a3f7
`define K63 32'hc67178f2

module sha256 (clk_i, rst_i, text_i, text_o, cmd_i, cmd_w_i, cmd_o);

        input           clk_i;  // global clock input
        input           rst_i;  // global reset input , active high
        
        input   [9:0]  text_i; // text input 32bit
        output  [9:0]  text_o; // text output 32bit
        
        input   [2:0]   cmd_i;  // command input
        input           cmd_w_i;// command input write enable
        output  [3:0]   cmd_o;  // command output(status)

        /*
                cmd
                Busy Round W R

                bit3 bit2  bit1 bit0
                Busy Round W    R
                
                Busy:
                0       idle
                1       busy
                
                Round:
                0       first round
                1       internal round
                
                W:
                0       No-op
                1       write data
                
                R:
                0       No-op
                1       read data
                        
        */
        

        reg     [3:0]   cmd;
        wire    [3:0]   cmd_o;
        
        reg     [9:0]  text_o;
        
        reg     [6:0]   round;
        wire    [6:0]   round_plus_1;
        
        reg     [2:0]   read_counter;
        
        reg     [31:0]  H0,H1,H2,H3,H4,H5,H6,H7;
        reg     [31:0]  W0,W1,W2,W3,W4,W5,W6,W7,W8,W9,W10,W11,W12,W13,W14;
        reg     [31:0]  Wt,Kt;
        reg     [31:0]  A,B,C,D,E,F,G,H;

        reg             busy;
        
        assign cmd_o = cmd;
        always @ (posedge clk_i)
        begin
                if (rst_i)
                        cmd <= 'b0;
                else
                if (cmd_w_i)
                        cmd[2:0] <= cmd_i[2:0];         // busy bit can't write
                else
                begin
                        cmd[3] <= busy;                 // update busy bit
                        if (~busy)
                                cmd[1:0] <= 2'b00;      // hardware auto clean R/W bits
                end
        end
        
        wire [31:0] f1_EFG_32,f2_ABC_32,f3_A_32,f4_E_32,f5_W1_32,f6_W14_32,T1_32,T2_32;
        wire [31:0] next_Wt,next_E,next_A;
        wire [255:0] SHA256_result;
        
        assign f1_EFG_32 = (E & F);

        assign f2_ABC_32 = (A & B);
        
        assign f3_A_32 = {A[1:0],A[31:2]} ^ {A[12:0],A[31:13]} ^ {A[21:0],A[31:22]};
        
        assign f4_E_32 = {E[5:0],E[31:6]} ^ {E[10:0],E[31:11]} ^ {E[24:0],E[31:25]};
        
        assign f5_W1_32 = 0;// {W1[6:0],W1[31:7]} ^ {W1[17:0],W1[31:18]} ^ {3'b000,W1[31:3]};
        
        assign f6_W14_32 = 0; //{W14[16:0],W14[31:17]} ^ {10'b00_0000_0000,W14[31:10]};
        
        
        assign T1_32 = f4_E_32 + f1_EFG_32 + Kt + Wt;
        
        assign T2_32 = f3_A_32 + f2_ABC_32;
        
        assign next_Wt = f6_W14_32 + f5_W1_32;
        assign next_E = D[31:0] + T1_32;
        assign next_A = T1_32 + T2_32;
        

        assign SHA256_result = {A,B,C,D,E,F,G,H};

        assign round_plus_1 = round + 1;
        
        //------------------------------------------------------------------    
        // SHA round
        //------------------------------------------------------------------
        always @(posedge clk_i)
        begin
                if (rst_i)
                begin
                        round <= 'd0;
                        busy <= 'b0;

                        W0  <= 'b0;
                        W1  <= 'b0;
                        W2  <= 'b0;
                        W3  <= 'b0;
                        W4  <= 'b0;
                        W5  <= 'b0;
                        W6  <= 'b0;
                        W7  <= 'b0;
                        W8  <= 'b0;
                        W9  <= 'b0;
                        W10 <= 'b0;
                        W11 <= 'b0;
                        W12 <= 'b0;
                        W13 <= 'b0;
                        W14 <= 'b0;
                        Wt  <= 'b0;
                        
                        A <= 'b0;
                        B <= 'b0;
                        C <= 'b0;
                        D <= 'b0;
                        E <= 'b0;
                        F <= 'b0;
                        G <= 'b0;
                        H <= 'b0;

                        H0 <= 'b0;
                        H1 <= 'b0;
                        H2 <= 'b0;
                        H3 <= 'b0;
                        H4 <= 'b0;
                        H5 <= 'b0;
                        H6 <= 'b0;
                        H7 <= 'b0;
                end
                else
                begin
                        case (round)
                        
                        'd0:
                                begin
                                        if (cmd[1])
                                        begin
                                                W0 <= text_i;
                                                Wt <= text_i;
                                                busy <= 'b1;
                                                round <= round_plus_1;
                                                
                                                case (cmd[2])
                                                        1'b0:   // sha-256 first message
                                                                begin
                                                                        A <= `SHA256_H0;
                                                                        B <= `SHA256_H1;
                                                                        C <= `SHA256_H2;
                                                                        D <= `SHA256_H3;
                                                                        E <= `SHA256_H4;
                                                                        F <= `SHA256_H5;
                                                                        G <= `SHA256_H6;
                                                                        H <= `SHA256_H7;

                                                                        H0 <= `SHA256_H0;
                                                                        H1 <= `SHA256_H1;
                                                                        H2 <= `SHA256_H2;
                                                                        H3 <= `SHA256_H3;
                                                                        H4 <= `SHA256_H4;
                                                                        H5 <= `SHA256_H5;
                                                                        H6 <= `SHA256_H6;
                                                                        H7 <= `SHA256_H7;
                                                                end
                                                        1'b1:   // sha-256 internal message
                                                                begin
                                                                        H0 <= A;
                                                                        H1 <= B;
                                                                        H2 <= C;
                                                                        H3 <= D;
                                                                        H4 <= E;
                                                                        H5 <= F;
                                                                        H6 <= G;
                                                                        H7 <= H;
                                                                end
                                                endcase
                                        end
                                        else
                                        begin   // IDLE
                                                round <= 'd0;
                                        end
                                end
                        'd1:
                                begin
                                        W1 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd2:
                                begin
                                        W2 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd3:
                                begin
                                        W3 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd4:
                                begin
                                        W4 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd5:
                                begin
                                        W5 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd6:
                                begin
                                        W6 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd7:
                                begin
                                        W7 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd8:
                                begin
                                        W8 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd9:
                                begin
                                        W9 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd10:
                                begin
                                        W10 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd11:
                                begin
                                        W11 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd12:
                                begin
                                        W12 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd13:
                                begin
                                        W13 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd14:
                                begin
                                        W14 <= text_i;
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd15:
                                begin
                                        Wt <= text_i;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd16,
                        'd17,
                        'd18,
                        'd19,
                        'd20,
                        'd21,
                        'd22,
                        'd23,
                        'd24,
                        'd25,
                        'd26,
                        'd27,
                        'd28,
                        'd29,
                        'd30,
                        'd31,
                        'd32,
                        'd33,
                        'd34,
                        'd35,
                        'd36,
                        'd37,
                        'd38,
                        'd39,
                        'd40,
                        'd41,
                        'd42,
                        'd43,
                        'd44,
                        'd45,
                        'd46,
                        'd47,
                        'd48,
                        'd49,
                        'd50,
                        'd51,
                        'd52,
                        'd53,
                        'd54,
                        'd55,
                        'd56,
                        'd57,
                        'd58,
                        'd59,
                        'd60,
                        'd61,
                        'd62,
                        'd63:
                                begin
                                        W0  <= W1;
                                        W1  <= W2;
                                        W2  <= W3;
                                        W3  <= W4;
                                        W4  <= W5;
                                        W5  <= W6;
                                        W6  <= W7;
                                        W7  <= W8;
                                        W8  <= W9;
                                        W9  <= W10;
                                        W10 <= W11;
                                        W11 <= W12;
                                        W12 <= W13;
                                        W13 <= W14;
                                        W14 <= Wt;
                                        Wt  <= next_Wt;
                                        
                                        H <= G;
                                        G <= F;
                                        F <= E;
                                        E <= next_E;
                                        D <= C;
                                        C <= B;
                                        B <= A;
                                        A <= next_A;
                                                
                                        round <= round_plus_1;
                                end
                        'd64:
                                begin
                                        A <= next_A + H0;
                                        B <= A + H1;
                                        C <= B + H2;
                                        D <= C + H3;
                                        E <= next_E + H4;
                                        F <= E + H5;
                                        G <= F + H6;
                                        H <= G + H7;
                                        round <= 'd0;
                                        busy <= 'b0;
                                end
                        default:
                                begin
                                        round <= 'd0;
                                        busy <= 'b0;
                                end
                        endcase
                end     
        end 
        
        
        //------------------------------------------------------------------    
        // Kt generator
        //------------------------------------------------------------------    
        always @ (posedge clk_i)
        begin
                if (rst_i)
                begin
                        Kt <= 'b0;
                end
                else
                begin
                        case (round)
                                'd00:   Kt <= `K00;
                                'd01:   Kt <= `K01;
                                'd02:   Kt <= `K02;
                                'd03:   Kt <= `K03;
                                'd04:   Kt <= `K04;
                                'd05:   Kt <= `K05;
                                'd06:   Kt <= `K06;
                                'd07:   Kt <= `K07;
                                'd08:   Kt <= `K08;
                                'd09:   Kt <= `K09;
                                'd10:   Kt <= `K10;
                                'd11:   Kt <= `K11;
                                'd12:   Kt <= `K12;
                                'd13:   Kt <= `K13;
                                'd14:   Kt <= `K14;
                                'd15:   Kt <= `K15;
                                'd16:   Kt <= `K16;
                                'd17:   Kt <= `K17;
                                'd18:   Kt <= `K18;
                                'd19:   Kt <= `K19;
                                'd20:   Kt <= `K20;
                                'd21:   Kt <= `K21;
                                'd22:   Kt <= `K22;
                                'd23:   Kt <= `K23;
                                'd24:   Kt <= `K24;
                                'd25:   Kt <= `K25;
                                'd26:   Kt <= `K26;
                                'd27:   Kt <= `K27;
                                'd28:   Kt <= `K28;
                                'd29:   Kt <= `K29;
                                'd30:   Kt <= `K30;
                                'd31:   Kt <= `K31;
                                'd32:   Kt <= `K32;
                                'd33:   Kt <= `K33;
                                'd34:   Kt <= `K34;
                                'd35:   Kt <= `K35;
                                'd36:   Kt <= `K36;
                                'd37:   Kt <= `K37;
                                'd38:   Kt <= `K38;
                                'd39:   Kt <= `K39;
                                'd40:   Kt <= `K40;
                                'd41:   Kt <= `K41;
                                'd42:   Kt <= `K42;
                                'd43:   Kt <= `K43;
                                'd44:   Kt <= `K44;
                                'd45:   Kt <= `K45;
                                'd46:   Kt <= `K46;
                                'd47:   Kt <= `K47;
                                'd48:   Kt <= `K48;
                                'd49:   Kt <= `K49;
                                'd50:   Kt <= `K50;
                                'd51:   Kt <= `K51;
                                'd52:   Kt <= `K52;
                                'd53:   Kt <= `K53;
                                'd54:   Kt <= `K54;
                                'd55:   Kt <= `K55;
                                'd56:   Kt <= `K56;
                                'd57:   Kt <= `K57;
                                'd58:   Kt <= `K58;
                                'd59:   Kt <= `K59;
                                'd60:   Kt <= `K60;
                                'd61:   Kt <= `K61;
                                'd62:   Kt <= `K62;
                                'd63:   Kt <= `K63;
                                default:Kt <= 'd0;
                        endcase
                end
        end

        //------------------------------------------------------------------    
        // read result 
        //------------------------------------------------------------------    
        always @ (posedge clk_i)
        begin
                if (rst_i)
                begin
                        text_o <= 'b0;
                        read_counter <= 'b0;
                end
                else
                begin
                        if (cmd[0])
                        begin
                                read_counter <= 'd7;    // sha-256      256/32=8
                        end
                        else
                        begin
                        if (~busy)
                        begin
                                case (read_counter)
                                        'd7:    text_o <= SHA256_result[8*32-1:7*32];
                                        'd6:    text_o <= SHA256_result[7*32-1:6*32];
                                        'd5:    text_o <= SHA256_result[6*32-1:5*32];
                                        'd4:    text_o <= SHA256_result[5*32-1:4*32];
                                        'd3:    text_o <= SHA256_result[4*32-1:3*32];
                                        'd2:    text_o <= SHA256_result[3*32-1:2*32];
                                        'd1:    text_o <= SHA256_result[2*32-1:1*32];
                                        'd0:    text_o <= SHA256_result[1*32-1:0*32];
                                        default:text_o <= 'b0;
                                endcase
                                if (|read_counter)
                                        read_counter <= read_counter - 'd1;
                        end
                        else
                        begin
                                text_o <= 'b0;
                        end
                        end
                end
        end
        
endmodule
 
