`timescale 1ns / 1ns

module DFlipFlop #(parameter N = 0) (
	output reg [N:0] o,
	input [N:0] i,
	input rst,
	input clk
);

always @(posedge clk) begin
	if(rst == 1'b1) begin
		o <= 1'b0;
	end
	else begin
		o <= i;
	end
end

endmodule
