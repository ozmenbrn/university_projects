`timescale 1ns / 1ns

module testbench();

reg b;
reg rst;
reg clk;
wire [1:0] y;

source s(.y(y),.b(b),.rst(rst),.clk(clk));
initial begin
    $dumpfile("TimingDiagram.vcd");
    $dumpvars(0, y, b, rst, clk);
	rst=0;
	b = 0;
	#70;
	b = 1;
	#40;
	b = 0;
	rst=1;
	#40;
	rst=0;
	b=1;
	#40;
	b = 1;
	#40;
	b = 0;
	rst=1;
	#40;
	rst=0;
	b = 1;
	#40;
	b = 1;
	#40;
	b = 1;
	#40;
	b = 0;
	rst=1;
	#40;
	rst=0;
	b=1;
	#40;
	b = 1;
	#40;
	b = 1;
	#40;
	b = 0;
	#40;
	b = 1;
	#40;
	b = 0;
	#40
    $finish;
end

always begin
	clk = 0;
	#20;
	clk = 1;
	#20;
end

endmodule