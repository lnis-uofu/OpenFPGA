module DFFR(RST,
            CK,
            D,
            Q,
						QN);
//----- GLOBAL PORTS -----
input [0:0] RST;
//----- GLOBAL PORTS -----
input [0:0] CK;
//----- INPUT PORTS -----
input [0:0] D;
//----- OUTPUT PORTS -----
output reg [0:0] Q;
output reg [0:0] QN;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Internal logic should start here -----
always @(posedge CK) begin
  if(RST) begin
    Q <= 1'b0;
    QN <= 1'b1;
  end else begin
    Q <= D;
    QN <= ~D;
  end
end

// ----- Internal logic should end here -----
endmodule
