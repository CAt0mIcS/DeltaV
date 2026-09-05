// `timescale 1850ps / 10ps
`timescale 10ns / 100ps

`include "params.vh"

module input_step_tb;

    reg step;
    wire [9:0] target_step;
    reg [2:0] microstepping_interval;
    input_step dut(.i_step(step), .i_microstepping_interval(microstepping_interval), .o_target_step(target_step));

    initial begin
        step = 0;
        microstepping_interval = `MICROSTEPPING_INTERVAL_1;

        $dumpfile("waveform.vcd");
        $dumpvars(0, input_step_tb);
        #500000
         $finish;
    end

    always begin
        #7
         step = ~step;

        #17
         microstepping_interval <= microstepping_interval + 1;
    end

endmodule
