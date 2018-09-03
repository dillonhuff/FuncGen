`define assert(signal, value) \
        if (signal !== value) begin \
            $display("ASSERTION FAILED in %m: signal != value"); \
            $finish; \
              end

module test();

   reg [15:0] D;
   reg [15:0] N;
   wire [15:0] Q;

   initial begin
      #1 D = 2;
      #1 N = 8;
      #1 `assert(Q, 4);
      
   end

   newton_raphson_divide_16 div(.D(D), .N(N), .Q(Q));
   
endmodule
