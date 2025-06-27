/////////////////////////////////////////
//  Functionality: 2-input AND 
//  Author:        Xifan Tang
////////////////////////////////////////
module and2 (
    input  logic a,    // First input
    input  logic b,    // Second input
    output logic y     // Output: a AND b
);

    assign y = a & b;

endmodule
