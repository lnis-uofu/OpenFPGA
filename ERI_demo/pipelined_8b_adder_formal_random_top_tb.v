`timescale 1 ns/ 100 ps

`include "OPENFPGAPATHKEYWORD/ERI_demo/pipelined_8b_adder.v"

module pipelined_8b_adder_top_formal_verification_random_tb();
    reg clk;
    reg[5:0] raddr;
    reg[5:0] waddr;
    reg ren;
    reg wen;
    reg[6:0] a;
    reg[6:0] b;
    wire[7:0] q_gfpga;
    wire[7:0] q_bench;
    reg[7:0] q_flag;

    pipelined_8b_adder_top_formal_verification DUT(
        .clk_fm     (clk),
        .raddr_0__fm (raddr[0]),
        .raddr_1__fm (raddr[1]),
        .raddr_2__fm (raddr[2]),
        .raddr_3__fm (raddr[3]),
        .raddr_4__fm (raddr[4]),
        .raddr_5__fm (raddr[5]),
        .waddr_0__fm (waddr[0]),
        .waddr_1__fm (waddr[1]),
        .waddr_2__fm (waddr[2]),
        .waddr_3__fm (waddr[3]),
        .waddr_4__fm (waddr[4]),
        .waddr_5__fm (waddr[5]),
        .ren_fm     (ren),
        .wen_fm     (wen),
        .a_0__fm     (a[0]),
        .a_1__fm     (a[1]),
        .a_2__fm     (a[2]),
        .a_3__fm     (a[3]),
        .a_4__fm     (a[4]),
        .a_5__fm     (a[5]),
        .a_6__fm     (a[6]),
        .b_0__fm     (b[0]),
        .b_1__fm     (b[1]),
        .b_2__fm     (b[2]),
        .b_3__fm     (b[3]),
        .b_4__fm     (b[4]),
        .b_5__fm     (b[5]),
        .b_6__fm     (b[6]),
        .out_q_0__fm     (q_gfpga[0]),
        .out_q_1__fm     (q_gfpga[1]),
        .out_q_2__fm     (q_gfpga[2]),
        .out_q_3__fm     (q_gfpga[3]),
        .out_q_4__fm     (q_gfpga[4]),
        .out_q_5__fm     (q_gfpga[5]),
        .out_q_6__fm     (q_gfpga[6]),
        .out_q_7__fm     (q_gfpga[7])
    );

    pipelined_8b_adder ref0(
        .clk    (clk),
        .raddr  (raddr),
        .waddr  (waddr),
        .ren    (ren),
        .wen    (wen),
        .a      (a),
        .b      (b),
        .q      (q_bench)
    );

	integer nb_error = 0;
	integer count = 0;
	integer lim_max = 64 - 1;
	integer write_complete = 0;

//----- Initialization
  initial begin
    clk <= 1'b0;
    a <= 7'h00;
    b <= 7'h00;
    wen <= 1'b0;
    ren <= 1'b0;
    waddr <= 9'h000;
    raddr <= 9'h000;
    while(1) begin
      #2.5
      clk <= !clk;
    end
  end

//----- Input Stimulis
  always@(negedge clk) begin
	if(write_complete == 0) begin
	  wen <= 1'b1;
	  ren <= 1'b0;
	  count <= count + 1;
	  waddr <= waddr + 1;
	  if(count == lim_max) begin
		write_complete = 1;
	  end
	end else begin
	  wen <= $random;
	  ren <= $random;
	  waddr <= $random;
	  raddr <= $random;
	end
	a <= $random;
	b <= $random;
  end


  always@(negedge clk) begin
    if(!(q_gfpga[0] === q_bench[0]) && !(q_bench[0] === 1'bx)) begin
      q_flag[0] <= 1'b1;
    end else begin
      q_flag[0] <= 1'b0;
    end
    if(!(q_gfpga[1] === q_bench[1]) && !(q_bench[1] === 1'bx)) begin
      q_flag[1] <= 1'b1;
    end else begin
      q_flag[1] <= 1'b0;
    end
    if(!(q_gfpga[2] === q_bench[2]) && !(q_bench[2] === 1'bx)) begin
      q_flag[2] <= 1'b1;
    end else begin
      q_flag[2] <= 1'b0;
    end
    if(!(q_gfpga[3] === q_bench[3]) && !(q_bench[3] === 1'bx)) begin
      q_flag[3] <= 1'b1;
    end else begin
      q_flag[3] <= 1'b0;
    end
    if(!(q_gfpga[4] === q_bench[4]) && !(q_bench[4] === 1'bx)) begin
      q_flag[4] <= 1'b1;
    end else begin
      q_flag[4] <= 1'b0;
    end
    if(!(q_gfpga[5] === q_bench[5]) && !(q_bench[5] === 1'bx)) begin
      q_flag[5] <= 1'b1;
    end else begin
      q_flag[5] <= 1'b0;
    end
    if(!(q_gfpga[6] === q_bench[6]) && !(q_bench[6] === 1'bx)) begin
      q_flag[6] <= 1'b1;
    end else begin
      q_flag[6] <= 1'b0;
    end
    if(!(q_gfpga[7] === q_bench[7]) && !(q_bench[7] === 1'bx)) begin
      q_flag[7] <= 1'b1;
    end else begin
      q_flag[7] <= 1'b0;
    end
  end


  always@(posedge q_flag[0]) begin
      if(q_flag[0]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[0] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[1]) begin
      if(q_flag[1]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[1] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[2]) begin
      if(q_flag[2]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[2] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[3]) begin
      if(q_flag[3]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[3] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[4]) begin
      if(q_flag[4]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[4] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[5]) begin
      if(q_flag[5]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[5] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[6]) begin
      if(q_flag[6]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[6] at time = %t", $realtime);
      end
  end
  always@(posedge q_flag[7]) begin
      if(q_flag[7]) begin
        nb_error = nb_error + 1;
        $display("Mismatch on q_gfpga[7] at time = %t", $realtime);
      end
  end

  initial begin
    $dumpfile("pipelined_8b_adder_formal.vcd");
    $dumpvars(1, pipelined_8b_adder_top_formal_verification_random_tb);
  end

  initial begin
    $timeformat(-9, 2, "ns", 20);
    $display("Simulation start");
    #1500 // Can be changed by the user for his need
    if(nb_error == 0) begin
      $display("Simulation Succeed");
    end else begin
      $display("Simulation Failed with %d error(s)", nb_error);
    end
    $finish;
  end

endmodule
