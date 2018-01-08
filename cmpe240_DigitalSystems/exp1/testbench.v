`timescale 1ns / 1ns
module TestBench();

reg [2:0] x;
wire [0:0] y;

source s(y, x);

initial begin
    $dumpfile("TimingDiagram.vcd");
    $dumpvars(0, y, x);
    
    x=3'b000;
	#20 x=3'b001;
	#20 x=3'b010;
	#20 x=3'b011;
	#20 x=3'b100;
	#20 x=3'b101;
	#20 x=3'b110;
	#20 x=3'b111;
	#20;
    
    $finish;
end

endmodule