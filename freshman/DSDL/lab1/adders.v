`ifndef ADDERS
`define ADDERS
`include "gates.v"

// half adder, gate level modeling
module HA(output C, S, input A, B);
	XOR g0(S, A, B);
	AND g1(C, A, B);
endmodule

// full adder, gate level modeling
module FA(output CO, S, input A, B, CI);
	wire c0, s0, c1, s1;
	HA ha0(c0, s0, A, B);
	HA ha1(c1, s1, s0, CI);
	assign S = s1;
	OR or0(CO, c0, c1);
endmodule

// adder without delay, register-transfer level modeling
module adder_rtl(
	output C3,       // carry output
	output[2:0] S,   // sum
	input[2:0] A, B, // operands
	input C0         // carry input
	);
	assign {C3, S} = A+B+C0;
endmodule

//  ripple-carry adder, gate level modeling
//  Do not modify the input/output of module
module rca_gl(
	output C3,       // carry output
	output[2:0] S,   // sum
	input[2:0] A, B, // operands
	input C0         // carry input
	);
	// TODO:: Implement gate-level RCA /////////////////////////////////////////////////////////////////////
	wire C1, C2;
	FA fa0(C1, S[0], A[0], B[0], C0);
	FA fa1(C2, S[1], A[1], B[1], C1);
	FA fa2(C3, S[2], A[2], B[2], C2);	
endmodule

// carry-lookahead adder, gate level modeling
// Do not modify the input/output of module
module cla_gl(
	output C3,       // carry output
	output[2:0] S,   // sum
	input[2:0] A, B, // operands
	input C0         // carry input
	);
	// TODO:: Implement gate-level CLA ////////////////////////////////////////////////////////////////////
	wire p[2:0], g[2:0], C2, C1;
	OR w11(p[0], A[0], B[0]); 
	OR w12(p[1], A[1], B[1]);
	OR w13(p[2], A[2], B[2]);
	AND w21(g[0], A[0], B[0]); 
	AND w22(g[1], A[1], B[1]);
	AND w23(g[2], A[2], B[2]);

	wire t1;
	AND w31(t1, p[0], C0);
	OR  w32(C1, g[0], t1);

	wire temp1, temp2, temp3;
	AND w41(temp1, g[0], p[1]);
	AND w42(temp2, t1, p[1]);
	OR  w43(temp3, g[1], temp1);
	OR  w44(C2, temp3, temp2);

	wire tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	AND w51(tmp2, p[1], p[2]);
	AND w52(tmp3, g[1], p[2]);
	OR  w53(tmp4, g[2], tmp3);
	AND w54(tmp5, g[0], tmp2);
	AND w55(tmp6, t1, tmp2);
	OR  w56(tmp7, tmp5, tmp6);
	OR  w57(C3, tmp4, tmp7);

	wire tem1, tem2, tem3, s1, s2, s3, s4, s5;
	XOR w61(tem1, A[0], B[0]);
	XOR w62(tem2, A[1], B[1]);
	XOR w63(tem3, A[2], B[2]);
	XOR w64(S[0], C0, tem1);
	XOR w65(S[1], C1, tem2);
	XOR w66(S[2], C2, tem3);
endmodule

`endif
