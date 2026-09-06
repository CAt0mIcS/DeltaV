// `timescale 1850ps / 10ps
`timescale 1ns / 10ps

`include "params.vh"

module input_step_tb;

    // 27MHz clock
    reg clk = 0;
    localparam CLK_HALF_PERIOD = 18.518519;
    always #CLK_HALF_PERIOD clk = ~clk;

    reg step;
    reg rst_n;
    wire [9:0] target_step;
    reg [2:0] microstepping_interval;
    input_step dut(.clk(clk), .rst_n(rst_n), .i_step(step), .i_microstepping_interval(microstepping_interval), .o_target_step(target_step));

    initial begin
        step = 0;
        rst_n = 1;
        microstepping_interval = `MICROSTEPPING_INTERVAL_1;

        $dumpfile("waveform.vcd");
        $dumpvars(0, input_step_tb);
        #500000
         $finish;
    end

    always begin
        #133
         step = ~step;
    end

    always begin
        #1777
         microstepping_interval <= microstepping_interval + 1;
    end

endmodule
