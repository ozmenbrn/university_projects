`timescale 1ns/1ns
module source(y, x);

input wire [4:0] x;
output wire [0:0] y;

// Fill here with gate level verilog code

wire w;
wire [3:0] wd;
wire nx3,nx4,nwd1;
not(nx3,x[3]);
not(nx4,x[4]);

dec2x4 mydecoder(wd[0],wd[1],wd[2],wd[3],x[4],x[3]);

not(nwd1,wd[1]);

or O1(w,wd[1],wd[2]);

mux_8_1 mymux(y,x[3],1'b0,nx4,nx3,wd[1],nx3,w,nwd1,x[2:0]);

endmodule