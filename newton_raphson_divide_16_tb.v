`define assert(signal, value) if (signal !== value) begin $display("ASSERTION FAILED in %m: signal != value"); $finish; end

module test();

   reg [15:0] D;
   reg [15:0] N;
   reg        clk;
   
   wire [15:0] Q;

   reg [15:0]  max_cycles;
   reg [15:0]  num_cycles;

   initial begin
      #1 max_cycles = 200;
      #1 num_cycles = 0;
      
      #1 clk = 0;
      
      #1 D = 2;
      #1 N = 8;
      #1 `assert(Q, 4);

      #1 N = -8;
      #1 D = 2;
      #1 `assert(Q, 16'b1111111111111100);

      #1 N = 239;
      #1 D = 17;
      #1 `assert(Q, 14);

      #1 N = 20;
      #1 D = 5;
      #1 `assert(Q, 4);
      
   end

   always #1 clk = ~clk;

   always @(posedge clk) begin
      //$display("Q = %b", Q);
      num_cycles <= num_cycles + 1;
      if (num_cycles >= max_cycles) begin
         $display("Passed");
         $finish();
      end
   end
   
   newton_raphson_divide_16 div(.D(D), .N(N), .Q(Q));
   
endmodule
