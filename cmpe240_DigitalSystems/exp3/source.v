`timescale 1ns / 1ns

module source(
	output wire [1:0] y,
	input b,
	input rst,
	input clk
);

wire [1:0] s;
wire [1:0] n;


wire w0,w1,w2,ns0,nb;
not(nb,b);
not(ns0,s[0]);
or O1(w0,ns0,s[1]);
or O2(w1,s[0],s[1]);
xor X1(w2,s[1],s[0]);
and A1(n[0],w0,b);
and A2(n[1],w1,b);
and A3(y[0],nb,s[0]);
and A4(y[1],nb,w2);

DFlipFlop #(1) dFlipFlop(
	.o(s),
	.i(n),
	.rst(rst),
	.clk(clk)
);

endmodule