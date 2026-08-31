module pwm_generator(
        input clk,
        input [6:0] i_duty_cycle, // 0 to 100

        output reg o_pwm
    );

    reg [6:0] counter = 0;

    initial
        o_pwm = 0;

    // This setup gives approx. 267kHz pwm frequency with a 27MHz clk
    always @(posedge clk) begin
        if(counter == 7'd100) begin
            counter <= 0;
        end
        else if(counter >= i_duty_cycle) begin
            counter <= counter + 1;
            o_pwm <= 0;
        end
        else if (counter < i_duty_cycle) begin
            counter <= counter + 1;
            o_pwm <= 1;
        end
    end

endmodule
