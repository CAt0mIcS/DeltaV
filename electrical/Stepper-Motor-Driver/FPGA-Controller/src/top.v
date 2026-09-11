`include "params.vh"

module top(
        input clk,
        input rst_n,
        input i_step,
        input i_dir,
        input [2:0] i_microstepping_interval,

        input i_spi_data_current_sensor_a,
        input i_spi_data_current_sensor_b,

        output o_mos_left_high_a,
        output o_mos_left_low_a,
        output o_mos_right_high_a,
        output o_mos_right_low_a,

        output o_mos_left_high_b,
        output o_mos_left_low_b,
        output o_mos_right_high_b,
        output o_mos_right_low_b,

        output o_spi_clk_current_sensor_a,
        output o_spi_cs_n_current_sensor_a,
        output o_spi_clk_current_sensor_b,
        output o_spi_cs_n_current_sensor_b
    );


    wire [9:0] target_step;

    input_step input_step_handler(
                   .clk(clk),
                   .rst_n(rst_n),
                   .i_step(i_step),
                   .i_microstepping_interval(i_microstepping_interval),
                   .o_target_step(target_step)
               );

    reg [1:0] drive_state_a = `H_BRIDGE_OFF;
    reg decay_state_a = `H_BRIDGE_SLOW_DECAY;
    reg [8:0] duty_cycle_a = 0;

    reg [1:0] drive_state_b = `H_BRIDGE_OFF;
    reg decay_state_b = `H_BRIDGE_SLOW_DECAY;
    reg [8:0] duty_cycle_b = 0;

    h_bridge_controller controller_a(
                            .clk(clk),
                            .i_drive_state(drive_state_a),
                            .i_decay_state(decay_state_a),
                            .i_duty_cycle(duty_cycle_a),
                            .o_mos_left_high(o_mos_left_high_a),
                            .o_mos_left_low(o_mos_left_low_a),
                            .o_mos_right_high(o_mos_right_high_a),
                            .o_mos_right_low(o_mos_right_low_a)
                        );

    h_bridge_controller controller_b(
                            .clk(clk),
                            .i_drive_state(drive_state_b),
                            .i_decay_state(decay_state_b),
                            .i_duty_cycle(duty_cycle_b),
                            .o_mos_left_high(o_mos_left_high_b),
                            .o_mos_left_low(o_mos_left_low_b),
                            .o_mos_right_high(o_mos_right_high_b),
                            .o_mos_right_low(o_mos_right_low_b)
                        );

    wire signed [11:0] target_current_a;
    wire current_increasing_a;

    current_target current_target_a(
                       .clk(clk),
                       .i_target_step(target_step),
                       .o_current_target(target_current_a),
                       .o_current_increasing(current_increasing_a)
                   );

    wire signed [11:0] target_current_b;
    wire current_increasing_b;

    current_target current_target_b(
                       .clk(clk),
                       .i_target_step(target_step),
                       .o_current_target(target_current_b),
                       .o_current_increasing(current_increasing_b)
                   );

    wire [13:0] current_current_a;
    wire data_available_current_sensor_a;

    ADS7042 current_sensor_a (
                .clk(clk),
                .rst_n(rst_n),
                .i_spi_data(i_spi_data_current_sensor_a),
                .o_spi_clk(o_spi_clk_current_sensor_a),
                .o_spi_cs_n(o_spi_cs_n_current_sensor_a),
                .o_current(current_current_a),
                .o_data_available(data_available_current_sensor_a)
            );

    wire [13:0] current_current_b;
    wire data_available_current_sensor_b;

    ADS7042 current_sensor_b (
                .clk(clk),
                .rst_n(rst_n),
                .i_spi_data(i_spi_data_current_sensor_b),
                .o_spi_clk(o_spi_clk_current_sensor_b),
                .o_spi_cs_n(o_spi_cs_n_current_sensor_b),
                .o_current(current_current_b),
                .o_data_available(data_available_current_sensor_b)
            );

endmodule
