//mux.v

module mux_8_1(out,w1,w2,w3,w4,w5,w6,w7,w8,sel);
	input w1,w2,w3,w4,w5,w6,w7,w8;
	input wire [2:0] sel;
	output reg out;
	always@(sel)
	begin
		case(sel)
			3'b000:out=w1;
			3'b001:out=w2;
			3'b010:out=w3;
			3'b011:out=w4;
			3'b100:out=w5;
			3'b101:out=w6;
			3'b110:out=w7;
			3'b111:out=w8;			
		endcase
	end
endmodule