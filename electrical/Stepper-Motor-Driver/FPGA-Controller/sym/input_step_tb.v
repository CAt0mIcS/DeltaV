// `timescale 1850ps / 10ps
`timescale 10ns / 100ps

module input_step_tb;

    reg step;
    wire [7:0] target_step;
    input_step dut(.step(step), .target_step(target_step));

    initial begin
        step = 0;

        $dumpfile("input_step_tb.vcd");
        $dumpvars(0, input_step_tb);
        #5000
         $finish;
    end

    always begin
        #10
         step = ~step;
    end

endmodule
