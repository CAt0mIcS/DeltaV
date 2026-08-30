`timescale 1ns / 1ps

module current_sensor_tb;
    reg clk;
    reg spi_data;
    wire spi_clk;
    wire spi_cs;

    current_sensor dut(
                       .clk(clk),
                       .spi_data(spi_data),
                       .spi_clk(spi_clk),
                       .spi_cs(spi_cs)
                   );

    initial begin
        clk = 0;
        spi_data = 0;

        $dumpfile("current_sensor_tb.vcd");
        $dumpvars(0, current_sensor_tb);

        #5000
         $finish;
    end

    // 27MHz clock
    localparam CLK_HALF_PERIOD = 18.518519;
    always #CLK_HALF_PERIOD clk = ~clk;


endmodule
