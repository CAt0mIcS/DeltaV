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

        output reg o_mos_left_high,
        output reg o_mos_left_low,
        output reg o_mos_right_high,
        output reg o_mos_right_low
    );

    wire pwm_left;
    wire pwm_right;

    pwm_generator pwm_left_high(
                      .clk(clk),
                      .i_duty_cycle(i_duty_cycle),
                      .o_pwm(pwm_left)
                  );

    pwm_generator pwm_right_high(
                      .clk(clk),
                      .i_duty_cycle(i_duty_cycle),
                      .o_pwm(pwm_right)
                  );

    always @(*) begin
        o_mos_left_high = 0;
        o_mos_right_high = 0;
        o_mos_left_low = 0;
        o_mos_right_low = 0;

        case(i_drive_state)
            `H_BRIDGE_DRIVE_POS: begin
                if(pwm_left) begin
                    o_mos_left_high = 1;
                    o_mos_right_low = 1;
                end
                else begin
                    case (i_decay_state)
                        `H_BRIDGE_SLOW_DECAY: begin
                            o_mos_left_low  = 1;
                            o_mos_right_low = 1;
                        end

                        `H_BRIDGE_FAST_DECAY: begin
                            o_mos_left_low  = 0;
                            o_mos_right_low = 0;
                        end
                    endcase
                end

            end
            `H_BRIDGE_DRIVE_NEG: begin
                if(pwm_right) begin
                    o_mos_right_high = 1;
                    o_mos_left_low = 1;
                end
                else begin
                    case (i_decay_state)
                        `H_BRIDGE_SLOW_DECAY: begin
                            o_mos_left_low  = 1;
                            o_mos_right_low = 1;
                        end

                        `H_BRIDGE_FAST_DECAY: begin
                            o_mos_left_low  = 0;
                            o_mos_right_low = 0;
                        end
                    endcase
                end

            end
            default /*H_BRIDGE_OFF*/: begin
                case (i_decay_state)
                    `H_BRIDGE_SLOW_DECAY: begin
                        o_mos_left_low  = 1;
                        o_mos_right_low = 1;
                    end

                    `H_BRIDGE_FAST_DECAY: begin
                        // TODO: Maybe switch on other set of MOSFETs during decay period as to not rely on body diodes?
                        // So if we were going in pos. direction, switch on MOSFETs for neg. direction until current decays to 0
                        o_mos_left_low  = 0;
                        o_mos_right_low = 0;
                    end
                endcase
            end
        endcase
    end

endmodule
