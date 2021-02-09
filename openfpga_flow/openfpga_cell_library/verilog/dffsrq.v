module DFFSRQ(SET,
            RST,
            CK,
            D,
            Q);
//----- GLOBAL PORTS -----
input [0:0] SET;
//----- GLOBAL PORTS -----
input [0:0] RST;
//----- GLOBAL PORTS -----
input [0:0] CK;
//----- INPUT PORTS -----
input [0:0] D;
//----- OUTPUT PORTS -----
output reg [0:0] Q;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Internal logic should start here -----
always @(posedge CK) begin
  if(RST) begin
    Q <= 1'b0;
  end else if(SET) begin
    Q <= 1'b1;
  end else begin
    Q <= D;
  end
end

// ----- Internal logic should end here -----
endmodule
