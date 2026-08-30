module input_step (
        input step,
        output reg [7:0] target_step
    );

    reg [7:0] target_step_int = 0;

    initial begin
        assign target_step = 0;
    end

    always @(posedge step) begin
        target_step_int <= target_step_int + 8'd1;
        assign target_step = target_step_int;
    end

endmodule
