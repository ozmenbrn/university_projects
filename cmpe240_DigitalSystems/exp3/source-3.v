`timescale 1ns / 1ns

module source(
	output reg [1:0] y,
	input x,
	input rst,
	input clk
);

parameter S0 = 3'b000,S1 = 3'b001, S2 = 3'b010, S3 = 3'b011;
parameter S4 = 3'b100, S5 = 3'b101 , S6 = 3'b110 , S7= 3'b111;

reg [2:0] s;

initial begin
	y <= 2'b00;
end

always @(s) begin
	case(s)
		S1: y <= 2'b00;
		S3: y <= 2'b11;
		S5: y <= 2'b10;
		S7: y <= 2'b01;
		default: y <= 2'b00;
	endcase
end

always @(posedge clk) begin
	if(rst == 1'b1) begin
		s <= S0;
	end
	else begin
		if(s == S0) begin
			if(x == 1'b0) begin
				s <= S0;
			end
			else begin
				s <= S1;
			end
		end
		else if(s == S1) begin
			if(x == 1'b0) begin
				s <= S2;
			end
			else begin
				s <= S1;
			end
		end
		else if(s == S2) begin
			if(x == 1'b0) begin
				s <= S4;
			end
			else begin
				s <= S3;
			end
		end
		else if(s == S3) begin
			if(x == 1'b0) begin
				s <= S2;
			end
			else begin
				s <= S1;
			end
		end
		else if(s == S4) begin
			if(x == 1'b0) begin
				s <= S6;
			end
			else begin
				s <= S5;
			end
		end
		else if(s == S5) begin
			if(x == 1'b0) begin
				s <= S2;
			end
			else begin
				s <= S1;
			end
		end
		else if(s == S6) begin
			if(x == 1'b0) begin
				s <= S6;
			end
			else begin
				s <= S7;
			end
		end
		else begin
			if(x == 1'b0) begin
				s <= S2;
			end
			else begin
				s <= S1;
			end
		end
	end
end

endmodule