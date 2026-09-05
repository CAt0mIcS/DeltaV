`include "params.vh"


module input_step (
        input i_step,
        input [2:0] i_microstepping_interval,
        output reg [9:0] o_target_step
    );

    initial begin
        o_target_step = 0;
    end

    always @(posedge i_step) begin
        case(i_microstepping_interval)
            `MICROSTEPPING_INTERVAL_1:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1;
            `MICROSTEPPING_INTERVAL_1_4:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_4;
            `MICROSTEPPING_INTERVAL_1_8:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_8;
            `MICROSTEPPING_INTERVAL_1_16:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_16;
            `MICROSTEPPING_INTERVAL_1_32:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_32;
            `MICROSTEPPING_INTERVAL_1_64:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_64;
            `MICROSTEPPING_INTERVAL_1_128:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_128;
            `MICROSTEPPING_INTERVAL_1_256:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1_256;
            default:
                o_target_step <= o_target_step + `MICROSTEPPING_INCREMENT_1;
        endcase
    end

endmodule
