//-------------------------------------------------------------------
// Function: Testbench for the Binary to Decimal converter
// Source: 
//   https://verilogcodes.blogspot.com/2015/10/verilog-code-for-8-bit-binary-to-bcd.html
module tb_bin2bcd;

    // Input
    reg [7:0] bin;
    // Output
    wire [11:0] bcd;
    // Extra variables
    reg [8:0] i;

    // Instantiate the Unit Under Test (UUT)
    bin2bcd uut (
        .bin(bin), 
        .bcd(bcd)
    );

//Simulation - Apply inputs
    initial begin
    //A for loop for checking all the input combinations.
        for(i=0;i<256;i=i+1)
        begin
            bin = i; 
            #10; //wait for 10 ns.
        end 
        $finish; //system function for stoping the simulation.
    end
      
endmodule
