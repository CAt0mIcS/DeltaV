`timescale 1ns / 10ps

`include "params.vh"

module h_bridge_controller_tb;

    // 27MHz clock
    reg clk;
    localparam CLK_HALF_PERIOD = 18.518519;
    always #CLK_HALF_PERIOD clk = ~clk;

    wire mos_left_high;
    wire mos_left_low;
    wire mos_right_high;
    wire mos_right_low;

    reg [1:0] drive_state;
    reg decay_state;
    reg [6:0] duty_cycle;

    h_bridge_controller dut(
                            .clk(clk),
                            .i_drive_state(drive_state),
                            .i_decay_state(decay_state),
                            .i_duty_cycle(duty_cycle),
                            .o_mos_left_high(mos_left_high),
                            .o_mos_left_low(mos_left_low),
                            .o_mos_right_high(mos_right_high),
                            .o_mos_right_low(mos_right_low)
                        );

    initial begin
        drive_state = `H_BRIDGE_DRIVE_POS;
        decay_state = `H_BRIDGE_SLOW_DECAY;
        duty_cycle = 30;
        clk = 0;

        $dumpfile("waveform.vcd");
        $dumpvars(0, h_bridge_controller_tb);
        #500000
         $finish;
    end

    always begin
        #10000
         drive_state <= ~drive_state;
    end

endmodule
