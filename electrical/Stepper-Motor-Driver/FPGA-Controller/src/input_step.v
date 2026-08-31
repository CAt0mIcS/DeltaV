module input_step (
        input i_step,
        output reg [7:0] o_target_step
    );

    initial begin
        o_target_step = 0;
    end

    always @(posedge i_step) begin
        o_target_step <= o_target_step + 1;
    end

endmodule
