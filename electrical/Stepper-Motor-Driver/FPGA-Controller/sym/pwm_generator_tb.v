`timescale 1ns / 10ps

module pwm_generator_tb;

    reg clk;
    reg rst_n;
    reg [8:0] duty_cycle;
    wire gate;

    pwm_generator #(.PWM_FREQ(20000)) dut(
                      .clk(clk),
                      .rst_n(rst_n),
                      .i_duty_cycle(duty_cycle),
                      .o_pwm(gate)
                  );

    initial begin
        duty_cycle = 77;
        clk = 0;
        rst_n = 0;

        $dumpfile("waveform.vcd");
        $dumpvars(0, pwm_generator_tb);

        #500000
         $finish;
    end

    // 27MHz clock
    localparam CLK_HALF_PERIOD = 18.518519;
    always #CLK_HALF_PERIOD clk = ~clk;

    always @(posedge clk) begin
        rst_n <= 1;
        // #10000

        //  if(duty_cycle == 7'd100) begin
        //      duty_cycle <= 0;
        //  end
        //  else begin
        //      duty_cycle <= duty_cycle + 10;
        //  end
    end


endmodule
