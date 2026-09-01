`timescale 1ns / 1ps

module ADS7042_tb;
    reg clk;
    reg spi_data;
    reg rst_n;
    wire spi_clk;
    wire spi_cs;
    wire[13:0] current;
    wire data_available;

    reg [13:0] final_serial_output;
    reg data_bit;

    ADS7042 dut(
                .clk(clk),
                .rst_n(rst_n),
                .i_spi_data(spi_data),
                .o_spi_clk(spi_clk),
                .o_spi_cs_n(spi_cs),
                .o_current(current),
                .o_data_available(data_available)
            );

    initial begin
        rst_n = 1;
        clk = 0;
        data_bit = 1;
        spi_data = data_bit;
        final_serial_output = 0;

        $dumpfile("waveform.vcd");
        $dumpvars(0, ADS7042_tb);

        #50000
         $finish;
    end

    // 27MHz clock
    localparam CLK_HALF_PERIOD = 18.518519;
    always #CLK_HALF_PERIOD clk = ~clk;

    always @(negedge spi_clk) begin
        if(!spi_cs) begin
            spi_data <= data_bit;
        end
    end

    always @(posedge data_available) begin
        final_serial_output <= current;
        data_bit <= ~data_bit;
    end

endmodule
