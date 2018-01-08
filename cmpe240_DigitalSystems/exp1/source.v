`timescale 1ns / 1ns
module source(y, x);

input wire [2:0] x;
output wire [0:0] y;

wire w1, w2;
wire nx2;

or O1(w1,x[0],x[1]); 

not(nx2,x[2]);

or O2(w2,nx2,x[0]);

and A1(y,w1,w2);
	
endmodule