module pwm_generator
    #(
         parameter CLK_FREQ_HZ = 27000000,
         parameter PWM_FREQ = 25000
     )(
         input clk,
         input rst_n,
         input [8:0] i_duty_cycle, // 0 to 255

         output o_pwm
     );

    localparam [31:0] TOTAL_CYCLES = CLK_FREQ_HZ / (PWM_FREQ);

    reg [15:0] counter = 0;
    wire [15:0] duty_threshold;
    assign o_pwm = (counter < duty_threshold);
    assign duty_threshold = (TOTAL_CYCLES * i_duty_cycle) >> 8;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            counter <= 0;
        end
        else if(counter >= TOTAL_CYCLES - 1) begin
            counter <= 0;
        end
        else
            counter <= counter + 1;
    end

endmodule
