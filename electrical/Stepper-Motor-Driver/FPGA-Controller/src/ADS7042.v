/*
* ADC ADS7042 Module for current sensor reading
* FPGA clock frequency 27MHz, ~38ns
*/

module ADS7042 (
        input clk,
        input rst_n,
        input i_spi_data, // from current sensor
        output reg o_spi_clk,
        output reg o_spi_cs_n,
        output reg[13:0] o_current,
        output reg o_data_available
    );

    localparam STATE_INITIAL = 2'd0;
    localparam STATE_CALIBRATING = 2'd1;
    localparam STATE_SAMPLE = 2'd2;
    localparam STATE_AQUISITION = 2'd3;

    // Metastability buffers
    reg spi_data_buffer;
    reg spi_data;

    reg [1:0] state;
    reg [7:0] counter;
    reg [3:0] current_bit;

    initial begin
        o_spi_cs_n = 1;
        o_spi_clk = 0;
        o_data_available = 0;

        state = STATE_INITIAL;
        counter = 0;
    end

    // Metastability
    always @(posedge clk) begin
        spi_data_buffer <= i_spi_data;
        spi_data <= spi_data_buffer;
    end

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            state <= STATE_INITIAL;
            o_spi_cs_n <= 1'b1;
        end

        case (state)
            STATE_INITIAL: begin
                o_current <= 0;
                o_spi_cs_n <= 1'b1;
                o_spi_clk <= 1'b0;
                counter <= 0;
                current_bit <= 4'd14;
                o_data_available <= 0;
                state <= STATE_CALIBRATING;
            end

            STATE_CALIBRATING: begin
                o_spi_cs_n <= 1'b0;
                // Wait 16 spi_clk cycles
                if(counter == 8'd32) begin
                    counter <= 0;
                    o_spi_cs_n <= 1'b1;
                    state <= STATE_AQUISITION;
                end
                else begin
                    o_spi_clk <= ~o_spi_clk;
                    counter <= counter + 1;
                end
            end

            STATE_AQUISITION: begin
                // Wait >200ns (6 clk-cycles) while keeping CS high
                o_spi_cs_n <= 1'b1;
                if(counter == 6) begin
                    counter <= 0;
                    o_spi_cs_n <= 1'b0;
                    o_spi_clk <= 1'b1;
                    o_data_available <= 0;
                    state <= STATE_SAMPLE;
                end
                else begin
                    counter <= counter + 1;
                end
            end

            STATE_SAMPLE: begin
                // data gets sampled on negedge o_spi_clk, so it's guaranteed to be valid during o_spi_clk == HIGH

                if(current_bit == 0) begin
                    // Sampling finished
                    current_bit <= 4'd14;
                    state <= STATE_AQUISITION;
                    o_data_available <= 1;
                    o_spi_clk <= 0;
                end
                else if(o_spi_clk) begin
                    o_current[current_bit - 1] <= spi_data;
                    current_bit = current_bit - 1;
                end

                o_spi_clk <= ~o_spi_clk;
            end

            default:
                state <= STATE_CALIBRATING;
        endcase
    end

endmodule
