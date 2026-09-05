/*
* i_target_step can jump up to 256 spaces (e.g. from 0A target current to 1.8A target current).
* To smooth out large step transitions, 10'dwe will microstep until we reach the target step position
*
* TODO: Only increment current_step when the current current through the motor is sufficiently close to the target current
*/
module current_target(
        input clk,
        input [9:0] i_target_step,
        output reg signed [11:0] o_current_target, // -1800mA to 1800mA
        output reg o_current_increasing
    );

    reg [9:0] current_step;
    reg [10:0] sine_values [0:256];


    initial begin
        $readmemh(`SINE_MEM_PATH, sine_values);
        current_step = 0;
        o_current_target = 0;
        o_current_increasing = 0;
    end


    always @(posedge clk) begin
        if(i_target_step != current_step)
            current_step <= current_step + 1;
    end

    always @(posedge clk) begin
        if(current_step <= 10'd256) begin
            o_current_target <= $signed({1'b0, sine_values[current_step]});
            o_current_increasing <= 1;
        end
        else if (current_step > 10'd256 && current_step <= 10'd512) begin
            o_current_target <= $signed({1'b0, sine_values[10'd512 - current_step]});
            o_current_increasing <= 0;
        end
        else if(current_step > 10'd512 && current_step <= 10'd768) begin
            o_current_target <= -$signed({1'b0, sine_values[current_step - 10'd512]});
            o_current_increasing <= 1;
        end
        else begin
            o_current_target <= -$signed({1'b0, sine_values[11'd1024 - current_step]});
            o_current_increasing <= 0;
        end
    end



endmodule
