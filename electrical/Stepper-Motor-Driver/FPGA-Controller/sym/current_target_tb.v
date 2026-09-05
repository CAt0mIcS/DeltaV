`timescale 1ns / 10ps

module current_target_tb;
    // 27MHz clock
    reg clk;
    localparam CLK_HALF_PERIOD = 18.518519;
    always #CLK_HALF_PERIOD clk = ~clk;

    reg [9:0] target_step_sin;
    wire [11:0] current_target_sin;
    wire current_increasing_sin;

    reg [9:0] target_step_cos;
    wire [11:0] current_target_cos;
    wire current_increasing_cos;

    current_target dut_sin(
                       .clk(clk),
                       .i_target_step(target_step_sin),
                       .o_current_target(current_target_sin),
                       .o_current_increasing(current_increasing_sin)
                   );

    current_target dut_cos(
                       .clk(clk),
                       .i_target_step(target_step_cos),
                       .o_current_target(current_target_cos),
                       .o_current_increasing(current_increasing_cos)
                   );

    initial begin
        clk = 0;
        target_step_sin = 0;
        target_step_cos = 10'd256;

        $dumpfile("waveform.vcd");
        $dumpvars(0, current_target_tb);
        #500000
         $finish;
    end

    always @(posedge clk) begin
        target_step_sin <= target_step_sin + 1;
        target_step_cos <= target_step_cos + 1;
    end
endmodule
