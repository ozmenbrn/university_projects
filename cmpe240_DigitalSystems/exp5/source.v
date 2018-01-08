`timescale 1ns/1ns
module source(
	input [4:0] X,
	input [4:0] Y,
	input [1:0] S,
	output reg [4:0] F,
	output reg Overflow,
	output reg Cout
);

always @(S) begin
	if(S == 2'b00) begin
		Overflow <= 1'b0;
		F<=X[3:1]*Y[2:0];
		if(X[3:1]*Y[2:0]>31)begin
			Cout <= 1'b1;
		end
		else begin
			Cout <= 1'b0;
		end
	end
	else if(S == 2'b01) begin
		Overflow <= 1'b0;
		F <= 5'b00000;
		if(X>Y)begin
			Cout <= 1'b1;
		end
		else begin
			Cout <= 1'b0;
		end
	end
	else if(S == 2'b10) begin
		F <= X + Y;
		if(X+Y>31)begin
			Cout<=1'b1;
		end
		else begin
			Cout<=1'b0;
		end
		if((X[4]==0&Y[4]==0& X+Y>15) | (X[4]==1&Y[4]==1&X+Y<16))begin
			Overflow <=1'b1;
		end
		else begin
			Overflow <=1'b0;
		end
	end
	else begin
	    Cout <= 1'b0;
		F <= X - (4*Y[2:0]);
		if((X[4]==0&Y[4]==0&X-(4*Y[2:0])>15) | (X[4]==1&Y[4]==1&X-(4*Y[2:0])<16))begin
			Overflow <= 1'b1;
		end
		else begin
			Overflow <=1'b0;
		end
	end
end

endmodule

