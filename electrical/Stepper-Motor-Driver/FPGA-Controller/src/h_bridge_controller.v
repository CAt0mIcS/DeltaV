`include "params.vh"

/*
* TODO: In slow decay, o_mos_left_high = ~o_mos_left_low or o_mos_right_high = ~o_mos_right_low depending on pos. or neg. drive respectively
* In fast decay, o_mos_left_high = o_mos_right_low or o_mos_right_high = o_mos_left_low
*/

module h_bridge_controller(
        input clk,
        input [1:0] i_drive_state,
        input i_decay_state,
        input [6:0] i_duty_cycle,

        output o_mos_left_high,
        output reg o_mos_left_low,
        output o_mos_right_high,
        output reg o_mos_right_low
    );

    reg [6:0] duty_cycle_left;
    reg [6:0] duty_cycle_right;

    pwm_generator pwm_left_high(
                      .clk(clk),
                      .i_duty_cycle(duty_cycle_left),
                      .o_pwm(o_mos_left_high)
                  );

    pwm_generator pwm_right_high(
                      .clk(clk),
                      .i_duty_cycle(duty_cycle_right),
                      .o_pwm(o_mos_right_high)
                  );

    always @(negedge o_mos_left_high or negedge o_mos_right_high) begin
        case(i_decay_state)
            `H_BRIDGE_FAST_DECAY: begin
                // TODO: Maybe switch on other set of MOSFETs during decay period as to not rely on body diodes?
                // So if we were going in pos. direction, switch on MOSFETs for neg. direction until current decays to 0
                o_mos_left_low <= 0;
                o_mos_right_low <= 0;
            end
            default /*`H_BRIDGE_SLOW_DECAY*/: begin
                o_mos_left_low <= 1;
                o_mos_right_low <= 1;
            end
        endcase
    end

    always @(posedge o_mos_left_high or posedge o_mos_right_high or i_drive_state) begin
        case (i_drive_state)
            `H_BRIDGE_DRIVE_POS: begin
                duty_cycle_right <= 0;
                o_mos_left_low <= 0;

                duty_cycle_left <= i_duty_cycle;
                o_mos_right_low <= 1;
            end
            `H_BRIDGE_DRIVE_NEG: begin
                duty_cycle_left <= 0;
                o_mos_right_low <= 0;

                duty_cycle_right <= i_duty_cycle;
                o_mos_left_low <= 1;
            end
            default /*H_BRIDGE_OFF*/: begin
                duty_cycle_left <= 0;
                duty_cycle_right <= 0;

                // TODO: Do we need to case(i_decay_state) here again to set slow/fast decay for H-Bridge that still needs to decay?
            end
        endcase
    end


endmodule
