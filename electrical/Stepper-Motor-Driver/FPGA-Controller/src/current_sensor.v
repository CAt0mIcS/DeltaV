module current_sensor (
        input clk,
        input spi_data, // from current sensor
        output reg spi_clk,
        output reg spi_cs
    );

    reg clock_divider = 0;

    initial begin
        assign spi_cs = 1;
        assign spi_clk = 0;
    end

    always @(posedge clk) begin
        clock_divider <= clock_divider + 'd1;
        assign spi_clk = clock_divider;
    end

endmodule
