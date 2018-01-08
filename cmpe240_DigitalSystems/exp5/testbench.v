`timescale 1ns/1ns
module testbench();

reg [4:0] X;
reg [4:0] Y;
reg [1:0] S;

wire [4:0] F;
wire Overflow;
wire Cout;

source Sourc(.F(F),.Overflow(Overflow),.Cout(Cout),.S(S),.X(X),.Y(Y));

initial begin
    $dumpfile("TimingDiagram.vcd");
    $dumpvars(0, F, Overflow, Cout, S, X, Y);
	S=2'b00;
    X=5'b00111;
	Y=5'b00111;
	#20 
	S=2'b01;
	X=5'b00011;
	Y=5'b00001;
	#20
	S=2'b00;
    X=5'b01111;
	Y=5'b00111;
	#20
	S=2'b01;
	X=5'b00011;
	Y=5'b01001;
	#20
	S=2'b10;
	X=5'b01111;
	Y=5'b01111;
	#20
	S=2'b11;
	X=5'b00111;
	Y=5'b00001;
	#20
	S=2'b10;
	X=5'b01001;
	Y=5'b00001;
	#20
	S=2'b11;
	X=5'b00011;
	Y=5'b10001;
	#20
    $finish;
end

endmodule

