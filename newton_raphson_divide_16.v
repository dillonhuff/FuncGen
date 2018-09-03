/* verilator lint_off UNUSED */
module equals_1(input [0:0] in0, input [0:0] in1, output [0:0] out);
	assign out = in0 == in1;
endmodule

module logical_shift_right_variable_32(input [31:0] in0, input [31:0] in1, output [31:0] out);
	assign out = in0 >> in1;
endmodule

module add_32(input [31:0] in0, input [31:0] in1, output [31:0] out);
	assign out = in0 + in1;
endmodule

module subtract_32(input [31:0] in0, input [31:0] in1, output [31:0] out);
	assign out = in0 - in1;
endmodule

module slice_15_0(input [31:0] in, output [15:0] out);
	assign out = in[15:0];
endmodule

module logical_shift_right_15_32(input [31:0] in, output [31:0] out);
	assign out = in >> 15 ;
endmodule

module multiply_32(input [31:0] in0, input [31:0] in1, output [31:0] out);
	assign out = in0 * in1;
endmodule

module zero_extend_32_16_32(input [15:0] in, output [31:0] out);
	assign out = {{(32 - 16){1'b0}}, in};
endmodule

module approximate_reciprocal_16_10(input [15:0] in, output [15:0] out);
	wire [9:0] av_1;
	wire [15:0] slice_15_6_in_0;

	assign slice_15_6_in_0 = in;

	wire [9:0] out_1;
	slice_15_6 slice_15_6_2(.in(slice_15_6_in_0), .out(out_1));

	assign av_1 = out_1;

	wire [15:0] av_2;
	reg [15:0] av_2reg; 
	always @(*) begin
		case (av_1) 
			10'h000: av_2reg = 16'hffff;
			10'h001: av_2reg = 16'h8000;
			10'h002: av_2reg = 16'h8000;
			10'h003: av_2reg = 16'haaaa;
			10'h004: av_2reg = 16'h8000;
			10'h005: av_2reg = 16'hcccc;
			10'h006: av_2reg = 16'haaaa;
			10'h007: av_2reg = 16'h9249;
			10'h008: av_2reg = 16'h8000;
			10'h009: av_2reg = 16'he38e;
			10'h00a: av_2reg = 16'hcccc;
			10'h00b: av_2reg = 16'hba2e;
			10'h00c: av_2reg = 16'haaaa;
			10'h00d: av_2reg = 16'h9d89;
			10'h00e: av_2reg = 16'h9249;
			10'h00f: av_2reg = 16'h8888;
			10'h010: av_2reg = 16'h8000;
			10'h011: av_2reg = 16'hf0f0;
			10'h012: av_2reg = 16'he38e;
			10'h013: av_2reg = 16'hd794;
			10'h014: av_2reg = 16'hcccc;
			10'h015: av_2reg = 16'hc30c;
			10'h016: av_2reg = 16'hba2e;
			10'h017: av_2reg = 16'hb216;
			10'h018: av_2reg = 16'haaaa;
			10'h019: av_2reg = 16'ha3d7;
			10'h01a: av_2reg = 16'h9d89;
			10'h01b: av_2reg = 16'h97b4;
			10'h01c: av_2reg = 16'h9249;
			10'h01d: av_2reg = 16'h8d3d;
			10'h01e: av_2reg = 16'h8888;
			10'h01f: av_2reg = 16'h8421;
			10'h020: av_2reg = 16'h8000;
			10'h021: av_2reg = 16'hf83e;
			10'h022: av_2reg = 16'hf0f0;
			10'h023: av_2reg = 16'hea0e;
			10'h024: av_2reg = 16'he38e;
			10'h025: av_2reg = 16'hdd67;
			10'h026: av_2reg = 16'hd794;
			10'h027: av_2reg = 16'hd20d;
			10'h028: av_2reg = 16'hcccc;
			10'h029: av_2reg = 16'hc7ce;
			10'h02a: av_2reg = 16'hc30c;
			10'h02b: av_2reg = 16'hbe82;
			10'h02c: av_2reg = 16'hba2e;
			10'h02d: av_2reg = 16'hb60b;
			10'h02e: av_2reg = 16'hb216;
			10'h02f: av_2reg = 16'hae4c;
			10'h030: av_2reg = 16'haaaa;
			10'h031: av_2reg = 16'ha72f;
			10'h032: av_2reg = 16'ha3d7;
			10'h033: av_2reg = 16'ha0a0;
			10'h034: av_2reg = 16'h9d89;
			10'h035: av_2reg = 16'h9a90;
			10'h036: av_2reg = 16'h97b4;
			10'h037: av_2reg = 16'h94f2;
			10'h038: av_2reg = 16'h9249;
			10'h039: av_2reg = 16'h8fb8;
			10'h03a: av_2reg = 16'h8d3d;
			10'h03b: av_2reg = 16'h8ad8;
			10'h03c: av_2reg = 16'h8888;
			10'h03d: av_2reg = 16'h864b;
			10'h03e: av_2reg = 16'h8421;
			10'h03f: av_2reg = 16'h8208;
			10'h040: av_2reg = 16'h8000;
			10'h041: av_2reg = 16'hfc0f;
			10'h042: av_2reg = 16'hf83e;
			10'h043: av_2reg = 16'hf489;
			10'h044: av_2reg = 16'hf0f0;
			10'h045: av_2reg = 16'hed73;
			10'h046: av_2reg = 16'hea0e;
			10'h047: av_2reg = 16'he6c2;
			10'h048: av_2reg = 16'he38e;
			10'h049: av_2reg = 16'he070;
			10'h04a: av_2reg = 16'hdd67;
			10'h04b: av_2reg = 16'hda74;
			10'h04c: av_2reg = 16'hd794;
			10'h04d: av_2reg = 16'hd4c7;
			10'h04e: av_2reg = 16'hd20d;
			10'h04f: av_2reg = 16'hcf64;
			10'h050: av_2reg = 16'hcccc;
			10'h051: av_2reg = 16'hca45;
			10'h052: av_2reg = 16'hc7ce;
			10'h053: av_2reg = 16'hc565;
			10'h054: av_2reg = 16'hc30c;
			10'h055: av_2reg = 16'hc0c0;
			10'h056: av_2reg = 16'hbe82;
			10'h057: av_2reg = 16'hbc52;
			10'h058: av_2reg = 16'hba2e;
			10'h059: av_2reg = 16'hb817;
			10'h05a: av_2reg = 16'hb60b;
			10'h05b: av_2reg = 16'hb40b;
			10'h05c: av_2reg = 16'hb216;
			10'h05d: av_2reg = 16'hb02c;
			10'h05e: av_2reg = 16'hae4c;
			10'h05f: av_2reg = 16'hac76;
			10'h060: av_2reg = 16'haaaa;
			10'h061: av_2reg = 16'ha8e8;
			10'h062: av_2reg = 16'ha72f;
			10'h063: av_2reg = 16'ha57e;
			10'h064: av_2reg = 16'ha3d7;
			10'h065: av_2reg = 16'ha237;
			10'h066: av_2reg = 16'ha0a0;
			10'h067: av_2reg = 16'h9f11;
			10'h068: av_2reg = 16'h9d89;
			10'h069: av_2reg = 16'h9c09;
			10'h06a: av_2reg = 16'h9a90;
			10'h06b: av_2reg = 16'h991f;
			10'h06c: av_2reg = 16'h97b4;
			10'h06d: av_2reg = 16'h964f;
			10'h06e: av_2reg = 16'h94f2;
			10'h06f: av_2reg = 16'h939a;
			10'h070: av_2reg = 16'h9249;
			10'h071: av_2reg = 16'h90fd;
			10'h072: av_2reg = 16'h8fb8;
			10'h073: av_2reg = 16'h8e78;
			10'h074: av_2reg = 16'h8d3d;
			10'h075: av_2reg = 16'h8c08;
			10'h076: av_2reg = 16'h8ad8;
			10'h077: av_2reg = 16'h89ae;
			10'h078: av_2reg = 16'h8888;
			10'h079: av_2reg = 16'h8767;
			10'h07a: av_2reg = 16'h864b;
			10'h07b: av_2reg = 16'h8534;
			10'h07c: av_2reg = 16'h8421;
			10'h07d: av_2reg = 16'h8312;
			10'h07e: av_2reg = 16'h8208;
			10'h07f: av_2reg = 16'h8102;
			10'h080: av_2reg = 16'h8000;
			10'h081: av_2reg = 16'hfe03;
			10'h082: av_2reg = 16'hfc0f;
			10'h083: av_2reg = 16'hfa23;
			10'h084: av_2reg = 16'hf83e;
			10'h085: av_2reg = 16'hf660;
			10'h086: av_2reg = 16'hf489;
			10'h087: av_2reg = 16'hf2b9;
			10'h088: av_2reg = 16'hf0f0;
			10'h089: av_2reg = 16'hef2e;
			10'h08a: av_2reg = 16'hed73;
			10'h08b: av_2reg = 16'hebbd;
			10'h08c: av_2reg = 16'hea0e;
			10'h08d: av_2reg = 16'he865;
			10'h08e: av_2reg = 16'he6c2;
			10'h08f: av_2reg = 16'he525;
			10'h090: av_2reg = 16'he38e;
			10'h091: av_2reg = 16'he1fc;
			10'h092: av_2reg = 16'he070;
			10'h093: av_2reg = 16'hdee9;
			10'h094: av_2reg = 16'hdd67;
			10'h095: av_2reg = 16'hdbeb;
			10'h096: av_2reg = 16'hda74;
			10'h097: av_2reg = 16'hd901;
			10'h098: av_2reg = 16'hd794;
			10'h099: av_2reg = 16'hd62b;
			10'h09a: av_2reg = 16'hd4c7;
			10'h09b: av_2reg = 16'hd368;
			10'h09c: av_2reg = 16'hd20d;
			10'h09d: av_2reg = 16'hd0b6;
			10'h09e: av_2reg = 16'hcf64;
			10'h09f: av_2reg = 16'hce16;
			10'h0a0: av_2reg = 16'hcccc;
			10'h0a1: av_2reg = 16'hcb87;
			10'h0a2: av_2reg = 16'hca45;
			10'h0a3: av_2reg = 16'hc907;
			10'h0a4: av_2reg = 16'hc7ce;
			10'h0a5: av_2reg = 16'hc698;
			10'h0a6: av_2reg = 16'hc565;
			10'h0a7: av_2reg = 16'hc437;
			10'h0a8: av_2reg = 16'hc30c;
			10'h0a9: av_2reg = 16'hc1e4;
			10'h0aa: av_2reg = 16'hc0c0;
			10'h0ab: av_2reg = 16'hbfa0;
			10'h0ac: av_2reg = 16'hbe82;
			10'h0ad: av_2reg = 16'hbd69;
			10'h0ae: av_2reg = 16'hbc52;
			10'h0af: av_2reg = 16'hbb3e;
			10'h0b0: av_2reg = 16'hba2e;
			10'h0b1: av_2reg = 16'hb921;
			10'h0b2: av_2reg = 16'hb817;
			10'h0b3: av_2reg = 16'hb70f;
			10'h0b4: av_2reg = 16'hb60b;
			10'h0b5: av_2reg = 16'hb509;
			10'h0b6: av_2reg = 16'hb40b;
			10'h0b7: av_2reg = 16'hb30f;
			10'h0b8: av_2reg = 16'hb216;
			10'h0b9: av_2reg = 16'hb11f;
			10'h0ba: av_2reg = 16'hb02c;
			10'h0bb: av_2reg = 16'haf3a;
			10'h0bc: av_2reg = 16'hae4c;
			10'h0bd: av_2reg = 16'had60;
			10'h0be: av_2reg = 16'hac76;
			10'h0bf: av_2reg = 16'hab8f;
			10'h0c0: av_2reg = 16'haaaa;
			10'h0c1: av_2reg = 16'ha9c8;
			10'h0c2: av_2reg = 16'ha8e8;
			10'h0c3: av_2reg = 16'ha80a;
			10'h0c4: av_2reg = 16'ha72f;
			10'h0c5: av_2reg = 16'ha655;
			10'h0c6: av_2reg = 16'ha57e;
			10'h0c7: av_2reg = 16'ha4a9;
			10'h0c8: av_2reg = 16'ha3d7;
			10'h0c9: av_2reg = 16'ha306;
			10'h0ca: av_2reg = 16'ha237;
			10'h0cb: av_2reg = 16'ha16b;
			10'h0cc: av_2reg = 16'ha0a0;
			10'h0cd: av_2reg = 16'h9fd8;
			10'h0ce: av_2reg = 16'h9f11;
			10'h0cf: av_2reg = 16'h9e4c;
			10'h0d0: av_2reg = 16'h9d89;
			10'h0d1: av_2reg = 16'h9cc8;
			10'h0d2: av_2reg = 16'h9c09;
			10'h0d3: av_2reg = 16'h9b4c;
			10'h0d4: av_2reg = 16'h9a90;
			10'h0d5: av_2reg = 16'h99d7;
			10'h0d6: av_2reg = 16'h991f;
			10'h0d7: av_2reg = 16'h9868;
			10'h0d8: av_2reg = 16'h97b4;
			10'h0d9: av_2reg = 16'h9701;
			10'h0da: av_2reg = 16'h964f;
			10'h0db: av_2reg = 16'h95a0;
			10'h0dc: av_2reg = 16'h94f2;
			10'h0dd: av_2reg = 16'h9445;
			10'h0de: av_2reg = 16'h939a;
			10'h0df: av_2reg = 16'h92f1;
			10'h0e0: av_2reg = 16'h9249;
			10'h0e1: av_2reg = 16'h91a2;
			10'h0e2: av_2reg = 16'h90fd;
			10'h0e3: av_2reg = 16'h905a;
			10'h0e4: av_2reg = 16'h8fb8;
			10'h0e5: av_2reg = 16'h8f17;
			10'h0e6: av_2reg = 16'h8e78;
			10'h0e7: av_2reg = 16'h8dda;
			10'h0e8: av_2reg = 16'h8d3d;
			10'h0e9: av_2reg = 16'h8ca2;
			10'h0ea: av_2reg = 16'h8c08;
			10'h0eb: av_2reg = 16'h8b70;
			10'h0ec: av_2reg = 16'h8ad8;
			10'h0ed: av_2reg = 16'h8a42;
			10'h0ee: av_2reg = 16'h89ae;
			10'h0ef: av_2reg = 16'h891a;
			10'h0f0: av_2reg = 16'h8888;
			10'h0f1: av_2reg = 16'h87f7;
			10'h0f2: av_2reg = 16'h8767;
			10'h0f3: av_2reg = 16'h86d9;
			10'h0f4: av_2reg = 16'h864b;
			10'h0f5: av_2reg = 16'h85bf;
			10'h0f6: av_2reg = 16'h8534;
			10'h0f7: av_2reg = 16'h84a9;
			10'h0f8: av_2reg = 16'h8421;
			10'h0f9: av_2reg = 16'h8399;
			10'h0fa: av_2reg = 16'h8312;
			10'h0fb: av_2reg = 16'h828c;
			10'h0fc: av_2reg = 16'h8208;
			10'h0fd: av_2reg = 16'h8184;
			10'h0fe: av_2reg = 16'h8102;
			10'h0ff: av_2reg = 16'h8080;
			10'h100: av_2reg = 16'h8000;
			10'h101: av_2reg = 16'hff00;
			10'h102: av_2reg = 16'hfe03;
			10'h103: av_2reg = 16'hfd08;
			10'h104: av_2reg = 16'hfc0f;
			10'h105: av_2reg = 16'hfb18;
			10'h106: av_2reg = 16'hfa23;
			10'h107: av_2reg = 16'hf92f;
			10'h108: av_2reg = 16'hf83e;
			10'h109: av_2reg = 16'hf74e;
			10'h10a: av_2reg = 16'hf660;
			10'h10b: av_2reg = 16'hf574;
			10'h10c: av_2reg = 16'hf489;
			10'h10d: av_2reg = 16'hf3a0;
			10'h10e: av_2reg = 16'hf2b9;
			10'h10f: av_2reg = 16'hf1d4;
			10'h110: av_2reg = 16'hf0f0;
			10'h111: av_2reg = 16'hf00f;
			10'h112: av_2reg = 16'hef2e;
			10'h113: av_2reg = 16'hee50;
			10'h114: av_2reg = 16'hed73;
			10'h115: av_2reg = 16'hec97;
			10'h116: av_2reg = 16'hebbd;
			10'h117: av_2reg = 16'heae5;
			10'h118: av_2reg = 16'hea0e;
			10'h119: av_2reg = 16'he939;
			10'h11a: av_2reg = 16'he865;
			10'h11b: av_2reg = 16'he793;
			10'h11c: av_2reg = 16'he6c2;
			10'h11d: av_2reg = 16'he5f3;
			10'h11e: av_2reg = 16'he525;
			10'h11f: av_2reg = 16'he459;
			10'h120: av_2reg = 16'he38e;
			10'h121: av_2reg = 16'he2c4;
			10'h122: av_2reg = 16'he1fc;
			10'h123: av_2reg = 16'he135;
			10'h124: av_2reg = 16'he070;
			10'h125: av_2reg = 16'hdfac;
			10'h126: av_2reg = 16'hdee9;
			10'h127: av_2reg = 16'hde27;
			10'h128: av_2reg = 16'hdd67;
			10'h129: av_2reg = 16'hdca8;
			10'h12a: av_2reg = 16'hdbeb;
			10'h12b: av_2reg = 16'hdb2f;
			10'h12c: av_2reg = 16'hda74;
			10'h12d: av_2reg = 16'hd9ba;
			10'h12e: av_2reg = 16'hd901;
			10'h12f: av_2reg = 16'hd84a;
			10'h130: av_2reg = 16'hd794;
			10'h131: av_2reg = 16'hd6df;
			10'h132: av_2reg = 16'hd62b;
			10'h133: av_2reg = 16'hd578;
			10'h134: av_2reg = 16'hd4c7;
			10'h135: av_2reg = 16'hd417;
			10'h136: av_2reg = 16'hd368;
			10'h137: av_2reg = 16'hd2ba;
			10'h138: av_2reg = 16'hd20d;
			10'h139: av_2reg = 16'hd161;
			10'h13a: av_2reg = 16'hd0b6;
			10'h13b: av_2reg = 16'hd00d;
			10'h13c: av_2reg = 16'hcf64;
			10'h13d: av_2reg = 16'hcebc;
			10'h13e: av_2reg = 16'hce16;
			10'h13f: av_2reg = 16'hcd71;
			10'h140: av_2reg = 16'hcccc;
			10'h141: av_2reg = 16'hcc29;
			10'h142: av_2reg = 16'hcb87;
			10'h143: av_2reg = 16'hcae5;
			10'h144: av_2reg = 16'hca45;
			10'h145: av_2reg = 16'hc9a6;
			10'h146: av_2reg = 16'hc907;
			10'h147: av_2reg = 16'hc86a;
			10'h148: av_2reg = 16'hc7ce;
			10'h149: av_2reg = 16'hc732;
			10'h14a: av_2reg = 16'hc698;
			10'h14b: av_2reg = 16'hc5fe;
			10'h14c: av_2reg = 16'hc565;
			10'h14d: av_2reg = 16'hc4ce;
			10'h14e: av_2reg = 16'hc437;
			10'h14f: av_2reg = 16'hc3a1;
			10'h150: av_2reg = 16'hc30c;
			10'h151: av_2reg = 16'hc278;
			10'h152: av_2reg = 16'hc1e4;
			10'h153: av_2reg = 16'hc152;
			10'h154: av_2reg = 16'hc0c0;
			10'h155: av_2reg = 16'hc030;
			10'h156: av_2reg = 16'hbfa0;
			10'h157: av_2reg = 16'hbf11;
			10'h158: av_2reg = 16'hbe82;
			10'h159: av_2reg = 16'hbdf5;
			10'h15a: av_2reg = 16'hbd69;
			10'h15b: av_2reg = 16'hbcdd;
			10'h15c: av_2reg = 16'hbc52;
			10'h15d: av_2reg = 16'hbbc8;
			10'h15e: av_2reg = 16'hbb3e;
			10'h15f: av_2reg = 16'hbab6;
			10'h160: av_2reg = 16'hba2e;
			10'h161: av_2reg = 16'hb9a7;
			10'h162: av_2reg = 16'hb921;
			10'h163: av_2reg = 16'hb89b;
			10'h164: av_2reg = 16'hb817;
			10'h165: av_2reg = 16'hb793;
			10'h166: av_2reg = 16'hb70f;
			10'h167: av_2reg = 16'hb68d;
			10'h168: av_2reg = 16'hb60b;
			10'h169: av_2reg = 16'hb58a;
			10'h16a: av_2reg = 16'hb509;
			10'h16b: av_2reg = 16'hb48a;
			10'h16c: av_2reg = 16'hb40b;
			10'h16d: av_2reg = 16'hb38c;
			10'h16e: av_2reg = 16'hb30f;
			10'h16f: av_2reg = 16'hb292;
			10'h170: av_2reg = 16'hb216;
			10'h171: av_2reg = 16'hb19a;
			10'h172: av_2reg = 16'hb11f;
			10'h173: av_2reg = 16'hb0a5;
			10'h174: av_2reg = 16'hb02c;
			10'h175: av_2reg = 16'hafb3;
			10'h176: av_2reg = 16'haf3a;
			10'h177: av_2reg = 16'haec3;
			10'h178: av_2reg = 16'hae4c;
			10'h179: av_2reg = 16'hadd5;
			10'h17a: av_2reg = 16'had60;
			10'h17b: av_2reg = 16'haceb;
			10'h17c: av_2reg = 16'hac76;
			10'h17d: av_2reg = 16'hac02;
			10'h17e: av_2reg = 16'hab8f;
			10'h17f: av_2reg = 16'hab1c;
			10'h180: av_2reg = 16'haaaa;
			10'h181: av_2reg = 16'haa39;
			10'h182: av_2reg = 16'ha9c8;
			10'h183: av_2reg = 16'ha957;
			10'h184: av_2reg = 16'ha8e8;
			10'h185: av_2reg = 16'ha879;
			10'h186: av_2reg = 16'ha80a;
			10'h187: av_2reg = 16'ha79c;
			10'h188: av_2reg = 16'ha72f;
			10'h189: av_2reg = 16'ha6c2;
			10'h18a: av_2reg = 16'ha655;
			10'h18b: av_2reg = 16'ha5e9;
			10'h18c: av_2reg = 16'ha57e;
			10'h18d: av_2reg = 16'ha513;
			10'h18e: av_2reg = 16'ha4a9;
			10'h18f: av_2reg = 16'ha440;
			10'h190: av_2reg = 16'ha3d7;
			10'h191: av_2reg = 16'ha36e;
			10'h192: av_2reg = 16'ha306;
			10'h193: av_2reg = 16'ha29e;
			10'h194: av_2reg = 16'ha237;
			10'h195: av_2reg = 16'ha1d1;
			10'h196: av_2reg = 16'ha16b;
			10'h197: av_2reg = 16'ha105;
			10'h198: av_2reg = 16'ha0a0;
			10'h199: av_2reg = 16'ha03c;
			10'h19a: av_2reg = 16'h9fd8;
			10'h19b: av_2reg = 16'h9f74;
			10'h19c: av_2reg = 16'h9f11;
			10'h19d: av_2reg = 16'h9eae;
			10'h19e: av_2reg = 16'h9e4c;
			10'h19f: av_2reg = 16'h9deb;
			10'h1a0: av_2reg = 16'h9d89;
			10'h1a1: av_2reg = 16'h9d29;
			10'h1a2: av_2reg = 16'h9cc8;
			10'h1a3: av_2reg = 16'h9c69;
			10'h1a4: av_2reg = 16'h9c09;
			10'h1a5: av_2reg = 16'h9baa;
			10'h1a6: av_2reg = 16'h9b4c;
			10'h1a7: av_2reg = 16'h9aee;
			10'h1a8: av_2reg = 16'h9a90;
			10'h1a9: av_2reg = 16'h9a33;
			10'h1aa: av_2reg = 16'h99d7;
			10'h1ab: av_2reg = 16'h997a;
			10'h1ac: av_2reg = 16'h991f;
			10'h1ad: av_2reg = 16'h98c3;
			10'h1ae: av_2reg = 16'h9868;
			10'h1af: av_2reg = 16'h980e;
			10'h1b0: av_2reg = 16'h97b4;
			10'h1b1: av_2reg = 16'h975a;
			10'h1b2: av_2reg = 16'h9701;
			10'h1b3: av_2reg = 16'h96a8;
			10'h1b4: av_2reg = 16'h964f;
			10'h1b5: av_2reg = 16'h95f7;
			10'h1b6: av_2reg = 16'h95a0;
			10'h1b7: av_2reg = 16'h9548;
			10'h1b8: av_2reg = 16'h94f2;
			10'h1b9: av_2reg = 16'h949b;
			10'h1ba: av_2reg = 16'h9445;
			10'h1bb: av_2reg = 16'h93ef;
			10'h1bc: av_2reg = 16'h939a;
			10'h1bd: av_2reg = 16'h9345;
			10'h1be: av_2reg = 16'h92f1;
			10'h1bf: av_2reg = 16'h929c;
			10'h1c0: av_2reg = 16'h9249;
			10'h1c1: av_2reg = 16'h91f5;
			10'h1c2: av_2reg = 16'h91a2;
			10'h1c3: av_2reg = 16'h9150;
			10'h1c4: av_2reg = 16'h90fd;
			10'h1c5: av_2reg = 16'h90ab;
			10'h1c6: av_2reg = 16'h905a;
			10'h1c7: av_2reg = 16'h9009;
			10'h1c8: av_2reg = 16'h8fb8;
			10'h1c9: av_2reg = 16'h8f67;
			10'h1ca: av_2reg = 16'h8f17;
			10'h1cb: av_2reg = 16'h8ec7;
			10'h1cc: av_2reg = 16'h8e78;
			10'h1cd: av_2reg = 16'h8e29;
			10'h1ce: av_2reg = 16'h8dda;
			10'h1cf: av_2reg = 16'h8d8b;
			10'h1d0: av_2reg = 16'h8d3d;
			10'h1d1: av_2reg = 16'h8cf0;
			10'h1d2: av_2reg = 16'h8ca2;
			10'h1d3: av_2reg = 16'h8c55;
			10'h1d4: av_2reg = 16'h8c08;
			10'h1d5: av_2reg = 16'h8bbc;
			10'h1d6: av_2reg = 16'h8b70;
			10'h1d7: av_2reg = 16'h8b24;
			10'h1d8: av_2reg = 16'h8ad8;
			10'h1d9: av_2reg = 16'h8a8d;
			10'h1da: av_2reg = 16'h8a42;
			10'h1db: av_2reg = 16'h89f8;
			10'h1dc: av_2reg = 16'h89ae;
			10'h1dd: av_2reg = 16'h8964;
			10'h1de: av_2reg = 16'h891a;
			10'h1df: av_2reg = 16'h88d1;
			10'h1e0: av_2reg = 16'h8888;
			10'h1e1: av_2reg = 16'h883f;
			10'h1e2: av_2reg = 16'h87f7;
			10'h1e3: av_2reg = 16'h87af;
			10'h1e4: av_2reg = 16'h8767;
			10'h1e5: av_2reg = 16'h8720;
			10'h1e6: av_2reg = 16'h86d9;
			10'h1e7: av_2reg = 16'h8692;
			10'h1e8: av_2reg = 16'h864b;
			10'h1e9: av_2reg = 16'h8605;
			10'h1ea: av_2reg = 16'h85bf;
			10'h1eb: av_2reg = 16'h8579;
			10'h1ec: av_2reg = 16'h8534;
			10'h1ed: av_2reg = 16'h84ee;
			10'h1ee: av_2reg = 16'h84a9;
			10'h1ef: av_2reg = 16'h8465;
			10'h1f0: av_2reg = 16'h8421;
			10'h1f1: av_2reg = 16'h83dc;
			10'h1f2: av_2reg = 16'h8399;
			10'h1f3: av_2reg = 16'h8355;
			10'h1f4: av_2reg = 16'h8312;
			10'h1f5: av_2reg = 16'h82cf;
			10'h1f6: av_2reg = 16'h828c;
			10'h1f7: av_2reg = 16'h824a;
			10'h1f8: av_2reg = 16'h8208;
			10'h1f9: av_2reg = 16'h81c6;
			10'h1fa: av_2reg = 16'h8184;
			10'h1fb: av_2reg = 16'h8143;
			10'h1fc: av_2reg = 16'h8102;
			10'h1fd: av_2reg = 16'h80c1;
			10'h1fe: av_2reg = 16'h8080;
			10'h1ff: av_2reg = 16'h8040;
			10'h200: av_2reg = 16'h8000;
			10'h201: av_2reg = 16'hff80;
			10'h202: av_2reg = 16'hff00;
			10'h203: av_2reg = 16'hfe82;
			10'h204: av_2reg = 16'hfe03;
			10'h205: av_2reg = 16'hfd86;
			10'h206: av_2reg = 16'hfd08;
			10'h207: av_2reg = 16'hfc8c;
			10'h208: av_2reg = 16'hfc0f;
			10'h209: av_2reg = 16'hfb93;
			10'h20a: av_2reg = 16'hfb18;
			10'h20b: av_2reg = 16'hfa9d;
			10'h20c: av_2reg = 16'hfa23;
			10'h20d: av_2reg = 16'hf9a9;
			10'h20e: av_2reg = 16'hf92f;
			10'h20f: av_2reg = 16'hf8b6;
			10'h210: av_2reg = 16'hf83e;
			10'h211: av_2reg = 16'hf7c5;
			10'h212: av_2reg = 16'hf74e;
			10'h213: av_2reg = 16'hf6d7;
			10'h214: av_2reg = 16'hf660;
			10'h215: av_2reg = 16'hf5e9;
			10'h216: av_2reg = 16'hf574;
			10'h217: av_2reg = 16'hf4fe;
			10'h218: av_2reg = 16'hf489;
			10'h219: av_2reg = 16'hf414;
			10'h21a: av_2reg = 16'hf3a0;
			10'h21b: av_2reg = 16'hf32d;
			10'h21c: av_2reg = 16'hf2b9;
			10'h21d: av_2reg = 16'hf246;
			10'h21e: av_2reg = 16'hf1d4;
			10'h21f: av_2reg = 16'hf162;
			10'h220: av_2reg = 16'hf0f0;
			10'h221: av_2reg = 16'hf07f;
			10'h222: av_2reg = 16'hf00f;
			10'h223: av_2reg = 16'hef9e;
			10'h224: av_2reg = 16'hef2e;
			10'h225: av_2reg = 16'heebf;
			10'h226: av_2reg = 16'hee50;
			10'h227: av_2reg = 16'hede1;
			10'h228: av_2reg = 16'hed73;
			10'h229: av_2reg = 16'hed05;
			10'h22a: av_2reg = 16'hec97;
			10'h22b: av_2reg = 16'hec2a;
			10'h22c: av_2reg = 16'hebbd;
			10'h22d: av_2reg = 16'heb51;
			10'h22e: av_2reg = 16'heae5;
			10'h22f: av_2reg = 16'hea79;
			10'h230: av_2reg = 16'hea0e;
			10'h231: av_2reg = 16'he9a3;
			10'h232: av_2reg = 16'he939;
			10'h233: av_2reg = 16'he8cf;
			10'h234: av_2reg = 16'he865;
			10'h235: av_2reg = 16'he7fc;
			10'h236: av_2reg = 16'he793;
			10'h237: av_2reg = 16'he72a;
			10'h238: av_2reg = 16'he6c2;
			10'h239: av_2reg = 16'he65a;
			10'h23a: av_2reg = 16'he5f3;
			10'h23b: av_2reg = 16'he58c;
			10'h23c: av_2reg = 16'he525;
			10'h23d: av_2reg = 16'he4bf;
			10'h23e: av_2reg = 16'he459;
			10'h23f: av_2reg = 16'he3f3;
			10'h240: av_2reg = 16'he38e;
			10'h241: av_2reg = 16'he329;
			10'h242: av_2reg = 16'he2c4;
			10'h243: av_2reg = 16'he260;
			10'h244: av_2reg = 16'he1fc;
			10'h245: av_2reg = 16'he198;
			10'h246: av_2reg = 16'he135;
			10'h247: av_2reg = 16'he0d2;
			10'h248: av_2reg = 16'he070;
			10'h249: av_2reg = 16'he00e;
			10'h24a: av_2reg = 16'hdfac;
			10'h24b: av_2reg = 16'hdf4a;
			10'h24c: av_2reg = 16'hdee9;
			10'h24d: av_2reg = 16'hde88;
			10'h24e: av_2reg = 16'hde27;
			10'h24f: av_2reg = 16'hddc7;
			10'h250: av_2reg = 16'hdd67;
			10'h251: av_2reg = 16'hdd08;
			10'h252: av_2reg = 16'hdca8;
			10'h253: av_2reg = 16'hdc4a;
			10'h254: av_2reg = 16'hdbeb;
			10'h255: av_2reg = 16'hdb8d;
			10'h256: av_2reg = 16'hdb2f;
			10'h257: av_2reg = 16'hdad1;
			10'h258: av_2reg = 16'hda74;
			10'h259: av_2reg = 16'hda17;
			10'h25a: av_2reg = 16'hd9ba;
			10'h25b: av_2reg = 16'hd95d;
			10'h25c: av_2reg = 16'hd901;
			10'h25d: av_2reg = 16'hd8a5;
			10'h25e: av_2reg = 16'hd84a;
			10'h25f: av_2reg = 16'hd7ef;
			10'h260: av_2reg = 16'hd794;
			10'h261: av_2reg = 16'hd739;
			10'h262: av_2reg = 16'hd6df;
			10'h263: av_2reg = 16'hd685;
			10'h264: av_2reg = 16'hd62b;
			10'h265: av_2reg = 16'hd5d2;
			10'h266: av_2reg = 16'hd578;
			10'h267: av_2reg = 16'hd520;
			10'h268: av_2reg = 16'hd4c7;
			10'h269: av_2reg = 16'hd46f;
			10'h26a: av_2reg = 16'hd417;
			10'h26b: av_2reg = 16'hd3bf;
			10'h26c: av_2reg = 16'hd368;
			10'h26d: av_2reg = 16'hd310;
			10'h26e: av_2reg = 16'hd2ba;
			10'h26f: av_2reg = 16'hd263;
			10'h270: av_2reg = 16'hd20d;
			10'h271: av_2reg = 16'hd1b7;
			10'h272: av_2reg = 16'hd161;
			10'h273: av_2reg = 16'hd10b;
			10'h274: av_2reg = 16'hd0b6;
			10'h275: av_2reg = 16'hd061;
			10'h276: av_2reg = 16'hd00d;
			10'h277: av_2reg = 16'hcfb8;
			10'h278: av_2reg = 16'hcf64;
			10'h279: av_2reg = 16'hcf10;
			10'h27a: av_2reg = 16'hcebc;
			10'h27b: av_2reg = 16'hce69;
			10'h27c: av_2reg = 16'hce16;
			10'h27d: av_2reg = 16'hcdc3;
			10'h27e: av_2reg = 16'hcd71;
			10'h27f: av_2reg = 16'hcd1e;
			10'h280: av_2reg = 16'hcccc;
			10'h281: av_2reg = 16'hcc7b;
			10'h282: av_2reg = 16'hcc29;
			10'h283: av_2reg = 16'hcbd8;
			10'h284: av_2reg = 16'hcb87;
			10'h285: av_2reg = 16'hcb36;
			10'h286: av_2reg = 16'hcae5;
			10'h287: av_2reg = 16'hca95;
			10'h288: av_2reg = 16'hca45;
			10'h289: av_2reg = 16'hc9f5;
			10'h28a: av_2reg = 16'hc9a6;
			10'h28b: av_2reg = 16'hc956;
			10'h28c: av_2reg = 16'hc907;
			10'h28d: av_2reg = 16'hc8b9;
			10'h28e: av_2reg = 16'hc86a;
			10'h28f: av_2reg = 16'hc81c;
			10'h290: av_2reg = 16'hc7ce;
			10'h291: av_2reg = 16'hc780;
			10'h292: av_2reg = 16'hc732;
			10'h293: av_2reg = 16'hc6e5;
			10'h294: av_2reg = 16'hc698;
			10'h295: av_2reg = 16'hc64b;
			10'h296: av_2reg = 16'hc5fe;
			10'h297: av_2reg = 16'hc5b2;
			10'h298: av_2reg = 16'hc565;
			10'h299: av_2reg = 16'hc519;
			10'h29a: av_2reg = 16'hc4ce;
			10'h29b: av_2reg = 16'hc482;
			10'h29c: av_2reg = 16'hc437;
			10'h29d: av_2reg = 16'hc3ec;
			10'h29e: av_2reg = 16'hc3a1;
			10'h29f: av_2reg = 16'hc356;
			10'h2a0: av_2reg = 16'hc30c;
			10'h2a1: av_2reg = 16'hc2c1;
			10'h2a2: av_2reg = 16'hc278;
			10'h2a3: av_2reg = 16'hc22e;
			10'h2a4: av_2reg = 16'hc1e4;
			10'h2a5: av_2reg = 16'hc19b;
			10'h2a6: av_2reg = 16'hc152;
			10'h2a7: av_2reg = 16'hc109;
			10'h2a8: av_2reg = 16'hc0c0;
			10'h2a9: av_2reg = 16'hc078;
			10'h2aa: av_2reg = 16'hc030;
			10'h2ab: av_2reg = 16'hbfe8;
			10'h2ac: av_2reg = 16'hbfa0;
			10'h2ad: av_2reg = 16'hbf58;
			10'h2ae: av_2reg = 16'hbf11;
			10'h2af: av_2reg = 16'hbec9;
			10'h2b0: av_2reg = 16'hbe82;
			10'h2b1: av_2reg = 16'hbe3c;
			10'h2b2: av_2reg = 16'hbdf5;
			10'h2b3: av_2reg = 16'hbdaf;
			10'h2b4: av_2reg = 16'hbd69;
			10'h2b5: av_2reg = 16'hbd23;
			10'h2b6: av_2reg = 16'hbcdd;
			10'h2b7: av_2reg = 16'hbc97;
			10'h2b8: av_2reg = 16'hbc52;
			10'h2b9: av_2reg = 16'hbc0d;
			10'h2ba: av_2reg = 16'hbbc8;
			10'h2bb: av_2reg = 16'hbb83;
			10'h2bc: av_2reg = 16'hbb3e;
			10'h2bd: av_2reg = 16'hbafa;
			10'h2be: av_2reg = 16'hbab6;
			10'h2bf: av_2reg = 16'hba72;
			10'h2c0: av_2reg = 16'hba2e;
			10'h2c1: av_2reg = 16'hb9ea;
			10'h2c2: av_2reg = 16'hb9a7;
			10'h2c3: av_2reg = 16'hb964;
			10'h2c4: av_2reg = 16'hb921;
			10'h2c5: av_2reg = 16'hb8de;
			10'h2c6: av_2reg = 16'hb89b;
			10'h2c7: av_2reg = 16'hb859;
			10'h2c8: av_2reg = 16'hb817;
			10'h2c9: av_2reg = 16'hb7d4;
			10'h2ca: av_2reg = 16'hb793;
			10'h2cb: av_2reg = 16'hb751;
			10'h2cc: av_2reg = 16'hb70f;
			10'h2cd: av_2reg = 16'hb6ce;
			10'h2ce: av_2reg = 16'hb68d;
			10'h2cf: av_2reg = 16'hb64c;
			10'h2d0: av_2reg = 16'hb60b;
			10'h2d1: av_2reg = 16'hb5ca;
			10'h2d2: av_2reg = 16'hb58a;
			10'h2d3: av_2reg = 16'hb54a;
			10'h2d4: av_2reg = 16'hb509;
			10'h2d5: av_2reg = 16'hb4c9;
			10'h2d6: av_2reg = 16'hb48a;
			10'h2d7: av_2reg = 16'hb44a;
			10'h2d8: av_2reg = 16'hb40b;
			10'h2d9: av_2reg = 16'hb3cc;
			10'h2da: av_2reg = 16'hb38c;
			10'h2db: av_2reg = 16'hb34e;
			10'h2dc: av_2reg = 16'hb30f;
			10'h2dd: av_2reg = 16'hb2d0;
			10'h2de: av_2reg = 16'hb292;
			10'h2df: av_2reg = 16'hb254;
			10'h2e0: av_2reg = 16'hb216;
			10'h2e1: av_2reg = 16'hb1d8;
			10'h2e2: av_2reg = 16'hb19a;
			10'h2e3: av_2reg = 16'hb15d;
			10'h2e4: av_2reg = 16'hb11f;
			10'h2e5: av_2reg = 16'hb0e2;
			10'h2e6: av_2reg = 16'hb0a5;
			10'h2e7: av_2reg = 16'hb068;
			10'h2e8: av_2reg = 16'hb02c;
			10'h2e9: av_2reg = 16'hafef;
			10'h2ea: av_2reg = 16'hafb3;
			10'h2eb: av_2reg = 16'haf76;
			10'h2ec: av_2reg = 16'haf3a;
			10'h2ed: av_2reg = 16'haefe;
			10'h2ee: av_2reg = 16'haec3;
			10'h2ef: av_2reg = 16'hae87;
			10'h2f0: av_2reg = 16'hae4c;
			10'h2f1: av_2reg = 16'hae10;
			10'h2f2: av_2reg = 16'hadd5;
			10'h2f3: av_2reg = 16'had9a;
			10'h2f4: av_2reg = 16'had60;
			10'h2f5: av_2reg = 16'had25;
			10'h2f6: av_2reg = 16'haceb;
			10'h2f7: av_2reg = 16'hacb0;
			10'h2f8: av_2reg = 16'hac76;
			10'h2f9: av_2reg = 16'hac3c;
			10'h2fa: av_2reg = 16'hac02;
			10'h2fb: av_2reg = 16'habc8;
			10'h2fc: av_2reg = 16'hab8f;
			10'h2fd: av_2reg = 16'hab56;
			10'h2fe: av_2reg = 16'hab1c;
			10'h2ff: av_2reg = 16'haae3;
			10'h300: av_2reg = 16'haaaa;
			10'h301: av_2reg = 16'haa71;
			10'h302: av_2reg = 16'haa39;
			10'h303: av_2reg = 16'haa00;
			10'h304: av_2reg = 16'ha9c8;
			10'h305: av_2reg = 16'ha990;
			10'h306: av_2reg = 16'ha957;
			10'h307: av_2reg = 16'ha920;
			10'h308: av_2reg = 16'ha8e8;
			10'h309: av_2reg = 16'ha8b0;
			10'h30a: av_2reg = 16'ha879;
			10'h30b: av_2reg = 16'ha841;
			10'h30c: av_2reg = 16'ha80a;
			10'h30d: av_2reg = 16'ha7d3;
			10'h30e: av_2reg = 16'ha79c;
			10'h30f: av_2reg = 16'ha765;
			10'h310: av_2reg = 16'ha72f;
			10'h311: av_2reg = 16'ha6f8;
			10'h312: av_2reg = 16'ha6c2;
			10'h313: av_2reg = 16'ha68b;
			10'h314: av_2reg = 16'ha655;
			10'h315: av_2reg = 16'ha61f;
			10'h316: av_2reg = 16'ha5e9;
			10'h317: av_2reg = 16'ha5b4;
			10'h318: av_2reg = 16'ha57e;
			10'h319: av_2reg = 16'ha549;
			10'h31a: av_2reg = 16'ha513;
			10'h31b: av_2reg = 16'ha4de;
			10'h31c: av_2reg = 16'ha4a9;
			10'h31d: av_2reg = 16'ha474;
			10'h31e: av_2reg = 16'ha440;
			10'h31f: av_2reg = 16'ha40b;
			10'h320: av_2reg = 16'ha3d7;
			10'h321: av_2reg = 16'ha3a2;
			10'h322: av_2reg = 16'ha36e;
			10'h323: av_2reg = 16'ha33a;
			10'h324: av_2reg = 16'ha306;
			10'h325: av_2reg = 16'ha2d2;
			10'h326: av_2reg = 16'ha29e;
			10'h327: av_2reg = 16'ha26b;
			10'h328: av_2reg = 16'ha237;
			10'h329: av_2reg = 16'ha204;
			10'h32a: av_2reg = 16'ha1d1;
			10'h32b: av_2reg = 16'ha19e;
			10'h32c: av_2reg = 16'ha16b;
			10'h32d: av_2reg = 16'ha138;
			10'h32e: av_2reg = 16'ha105;
			10'h32f: av_2reg = 16'ha0d3;
			10'h330: av_2reg = 16'ha0a0;
			10'h331: av_2reg = 16'ha06e;
			10'h332: av_2reg = 16'ha03c;
			10'h333: av_2reg = 16'ha00a;
			10'h334: av_2reg = 16'h9fd8;
			10'h335: av_2reg = 16'h9fa6;
			10'h336: av_2reg = 16'h9f74;
			10'h337: av_2reg = 16'h9f42;
			10'h338: av_2reg = 16'h9f11;
			10'h339: av_2reg = 16'h9ee0;
			10'h33a: av_2reg = 16'h9eae;
			10'h33b: av_2reg = 16'h9e7d;
			10'h33c: av_2reg = 16'h9e4c;
			10'h33d: av_2reg = 16'h9e1b;
			10'h33e: av_2reg = 16'h9deb;
			10'h33f: av_2reg = 16'h9dba;
			10'h340: av_2reg = 16'h9d89;
			10'h341: av_2reg = 16'h9d59;
			10'h342: av_2reg = 16'h9d29;
			10'h343: av_2reg = 16'h9cf8;
			10'h344: av_2reg = 16'h9cc8;
			10'h345: av_2reg = 16'h9c98;
			10'h346: av_2reg = 16'h9c69;
			10'h347: av_2reg = 16'h9c39;
			10'h348: av_2reg = 16'h9c09;
			10'h349: av_2reg = 16'h9bda;
			10'h34a: av_2reg = 16'h9baa;
			10'h34b: av_2reg = 16'h9b7b;
			10'h34c: av_2reg = 16'h9b4c;
			10'h34d: av_2reg = 16'h9b1d;
			10'h34e: av_2reg = 16'h9aee;
			10'h34f: av_2reg = 16'h9abf;
			10'h350: av_2reg = 16'h9a90;
			10'h351: av_2reg = 16'h9a62;
			10'h352: av_2reg = 16'h9a33;
			10'h353: av_2reg = 16'h9a05;
			10'h354: av_2reg = 16'h99d7;
			10'h355: av_2reg = 16'h99a8;
			10'h356: av_2reg = 16'h997a;
			10'h357: av_2reg = 16'h994c;
			10'h358: av_2reg = 16'h991f;
			10'h359: av_2reg = 16'h98f1;
			10'h35a: av_2reg = 16'h98c3;
			10'h35b: av_2reg = 16'h9896;
			10'h35c: av_2reg = 16'h9868;
			10'h35d: av_2reg = 16'h983b;
			10'h35e: av_2reg = 16'h980e;
			10'h35f: av_2reg = 16'h97e1;
			10'h360: av_2reg = 16'h97b4;
			10'h361: av_2reg = 16'h9787;
			10'h362: av_2reg = 16'h975a;
			10'h363: av_2reg = 16'h972d;
			10'h364: av_2reg = 16'h9701;
			10'h365: av_2reg = 16'h96d4;
			10'h366: av_2reg = 16'h96a8;
			10'h367: av_2reg = 16'h967c;
			10'h368: av_2reg = 16'h964f;
			10'h369: av_2reg = 16'h9623;
			10'h36a: av_2reg = 16'h95f7;
			10'h36b: av_2reg = 16'h95cb;
			10'h36c: av_2reg = 16'h95a0;
			10'h36d: av_2reg = 16'h9574;
			10'h36e: av_2reg = 16'h9548;
			10'h36f: av_2reg = 16'h951d;
			10'h370: av_2reg = 16'h94f2;
			10'h371: av_2reg = 16'h94c6;
			10'h372: av_2reg = 16'h949b;
			10'h373: av_2reg = 16'h9470;
			10'h374: av_2reg = 16'h9445;
			10'h375: av_2reg = 16'h941a;
			10'h376: av_2reg = 16'h93ef;
			10'h377: av_2reg = 16'h93c5;
			10'h378: av_2reg = 16'h939a;
			10'h379: av_2reg = 16'h9370;
			10'h37a: av_2reg = 16'h9345;
			10'h37b: av_2reg = 16'h931b;
			10'h37c: av_2reg = 16'h92f1;
			10'h37d: av_2reg = 16'h92c6;
			10'h37e: av_2reg = 16'h929c;
			10'h37f: av_2reg = 16'h9272;
			10'h380: av_2reg = 16'h9249;
			10'h381: av_2reg = 16'h921f;
			10'h382: av_2reg = 16'h91f5;
			10'h383: av_2reg = 16'h91cc;
			10'h384: av_2reg = 16'h91a2;
			10'h385: av_2reg = 16'h9179;
			10'h386: av_2reg = 16'h9150;
			10'h387: av_2reg = 16'h9126;
			10'h388: av_2reg = 16'h90fd;
			10'h389: av_2reg = 16'h90d4;
			10'h38a: av_2reg = 16'h90ab;
			10'h38b: av_2reg = 16'h9082;
			10'h38c: av_2reg = 16'h905a;
			10'h38d: av_2reg = 16'h9031;
			10'h38e: av_2reg = 16'h9009;
			10'h38f: av_2reg = 16'h8fe0;
			10'h390: av_2reg = 16'h8fb8;
			10'h391: av_2reg = 16'h8f8f;
			10'h392: av_2reg = 16'h8f67;
			10'h393: av_2reg = 16'h8f3f;
			10'h394: av_2reg = 16'h8f17;
			10'h395: av_2reg = 16'h8eef;
			10'h396: av_2reg = 16'h8ec7;
			10'h397: av_2reg = 16'h8e9f;
			10'h398: av_2reg = 16'h8e78;
			10'h399: av_2reg = 16'h8e50;
			10'h39a: av_2reg = 16'h8e29;
			10'h39b: av_2reg = 16'h8e01;
			10'h39c: av_2reg = 16'h8dda;
			10'h39d: av_2reg = 16'h8db3;
			10'h39e: av_2reg = 16'h8d8b;
			10'h39f: av_2reg = 16'h8d64;
			10'h3a0: av_2reg = 16'h8d3d;
			10'h3a1: av_2reg = 16'h8d16;
			10'h3a2: av_2reg = 16'h8cf0;
			10'h3a3: av_2reg = 16'h8cc9;
			10'h3a4: av_2reg = 16'h8ca2;
			10'h3a5: av_2reg = 16'h8c7c;
			10'h3a6: av_2reg = 16'h8c55;
			10'h3a7: av_2reg = 16'h8c2f;
			10'h3a8: av_2reg = 16'h8c08;
			10'h3a9: av_2reg = 16'h8be2;
			10'h3aa: av_2reg = 16'h8bbc;
			10'h3ab: av_2reg = 16'h8b96;
			10'h3ac: av_2reg = 16'h8b70;
			10'h3ad: av_2reg = 16'h8b4a;
			10'h3ae: av_2reg = 16'h8b24;
			10'h3af: av_2reg = 16'h8afe;
			10'h3b0: av_2reg = 16'h8ad8;
			10'h3b1: av_2reg = 16'h8ab3;
			10'h3b2: av_2reg = 16'h8a8d;
			10'h3b3: av_2reg = 16'h8a68;
			10'h3b4: av_2reg = 16'h8a42;
			10'h3b5: av_2reg = 16'h8a1d;
			10'h3b6: av_2reg = 16'h89f8;
			10'h3b7: av_2reg = 16'h89d3;
			10'h3b8: av_2reg = 16'h89ae;
			10'h3b9: av_2reg = 16'h8989;
			10'h3ba: av_2reg = 16'h8964;
			10'h3bb: av_2reg = 16'h893f;
			10'h3bc: av_2reg = 16'h891a;
			10'h3bd: av_2reg = 16'h88f6;
			10'h3be: av_2reg = 16'h88d1;
			10'h3bf: av_2reg = 16'h88ac;
			10'h3c0: av_2reg = 16'h8888;
			10'h3c1: av_2reg = 16'h8864;
			10'h3c2: av_2reg = 16'h883f;
			10'h3c3: av_2reg = 16'h881b;
			10'h3c4: av_2reg = 16'h87f7;
			10'h3c5: av_2reg = 16'h87d3;
			10'h3c6: av_2reg = 16'h87af;
			10'h3c7: av_2reg = 16'h878b;
			10'h3c8: av_2reg = 16'h8767;
			10'h3c9: av_2reg = 16'h8743;
			10'h3ca: av_2reg = 16'h8720;
			10'h3cb: av_2reg = 16'h86fc;
			10'h3cc: av_2reg = 16'h86d9;
			10'h3cd: av_2reg = 16'h86b5;
			10'h3ce: av_2reg = 16'h8692;
			10'h3cf: av_2reg = 16'h866e;
			10'h3d0: av_2reg = 16'h864b;
			10'h3d1: av_2reg = 16'h8628;
			10'h3d2: av_2reg = 16'h8605;
			10'h3d3: av_2reg = 16'h85e2;
			10'h3d4: av_2reg = 16'h85bf;
			10'h3d5: av_2reg = 16'h859c;
			10'h3d6: av_2reg = 16'h8579;
			10'h3d7: av_2reg = 16'h8556;
			10'h3d8: av_2reg = 16'h8534;
			10'h3d9: av_2reg = 16'h8511;
			10'h3da: av_2reg = 16'h84ee;
			10'h3db: av_2reg = 16'h84cc;
			10'h3dc: av_2reg = 16'h84a9;
			10'h3dd: av_2reg = 16'h8487;
			10'h3de: av_2reg = 16'h8465;
			10'h3df: av_2reg = 16'h8443;
			10'h3e0: av_2reg = 16'h8421;
			10'h3e1: av_2reg = 16'h83fe;
			10'h3e2: av_2reg = 16'h83dc;
			10'h3e3: av_2reg = 16'h83bb;
			10'h3e4: av_2reg = 16'h8399;
			10'h3e5: av_2reg = 16'h8377;
			10'h3e6: av_2reg = 16'h8355;
			10'h3e7: av_2reg = 16'h8334;
			10'h3e8: av_2reg = 16'h8312;
			10'h3e9: av_2reg = 16'h82f0;
			10'h3ea: av_2reg = 16'h82cf;
			10'h3eb: av_2reg = 16'h82ae;
			10'h3ec: av_2reg = 16'h828c;
			10'h3ed: av_2reg = 16'h826b;
			10'h3ee: av_2reg = 16'h824a;
			10'h3ef: av_2reg = 16'h8229;
			10'h3f0: av_2reg = 16'h8208;
			10'h3f1: av_2reg = 16'h81e7;
			10'h3f2: av_2reg = 16'h81c6;
			10'h3f3: av_2reg = 16'h81a5;
			10'h3f4: av_2reg = 16'h8184;
			10'h3f5: av_2reg = 16'h8163;
			10'h3f6: av_2reg = 16'h8143;
			10'h3f7: av_2reg = 16'h8122;
			10'h3f8: av_2reg = 16'h8102;
			10'h3f9: av_2reg = 16'h80e1;
			10'h3fa: av_2reg = 16'h80c1;
			10'h3fb: av_2reg = 16'h80a0;
			10'h3fc: av_2reg = 16'h8080;
			10'h3fd: av_2reg = 16'h8060;
			10'h3fe: av_2reg = 16'h8040;
			10'h3ff: av_2reg = 16'h8020;
		endcase
	end

	assign av_2 = av_2reg;


	assign out = av_2;

endmodule

module slice_15_6(input [15:0] in, output [9:0] out);
	assign out = in[15:6];
endmodule

module logical_shift_right_variable_16(input [15:0] in0, input [15:0] in1, output [15:0] out);
	assign out = in0 >> in1;
endmodule

module equals_16(input [15:0] in0, input [15:0] in1, output [0:0] out);
	assign out = in0 == in1;
endmodule

module shift_left_variable_16(input [15:0] in0, input [15:0] in1, output [15:0] out);
	assign out = in0 << in1;
endmodule

module subtract_16(input [15:0] in0, input [15:0] in1, output [15:0] out);
	assign out = in0 - in1;
endmodule

module count_leading_zeros_16(input [15:0] in, output [15:0] out);
	reg [15:0] out_reg;
	always @(*) begin
		casez(in)
			16'b1???????????????: out_reg = 0;
			16'b01??????????????: out_reg = 1;
			16'b001?????????????: out_reg = 2;
			16'b0001????????????: out_reg = 3;
			16'b00001???????????: out_reg = 4;
			16'b000001??????????: out_reg = 5;
			16'b0000001?????????: out_reg = 6;
			16'b00000001????????: out_reg = 7;
			16'b000000001???????: out_reg = 8;
			16'b0000000001??????: out_reg = 9;
			16'b00000000001?????: out_reg = 10;
			16'b000000000001????: out_reg = 11;
			16'b0000000000001???: out_reg = 12;
			16'b00000000000001??: out_reg = 13;
			16'b000000000000001?: out_reg = 14;
			16'b0000000000000001: out_reg = 15;
		endcase
	end
	assign out = out_reg;
endmodule

module twos_complement_absolute_value(input [15:0] in, output [15:0] out);
	wire [0:0] av_1;
	wire [15:0] slice_15_15_in_3;

	assign slice_15_15_in_3 = in;

	wire [0:0] out_4;
	slice_15_15 slice_15_15_5(.in(slice_15_15_in_3), .out(out_4));

	assign av_1 = out_4;

	wire [15:0] av_2;
	reg [15:0] av_2reg; 
	wire [15:0] tc_neg_16_in_6;

	assign tc_neg_16_in_6 = in;

	wire [15:0] out_7;
	tc_neg_16 tc_neg_16_8(.in(tc_neg_16_in_6), .out(out_7));
	always @(*) begin
		case (av_1) 
			1'h0: av_2reg = in;
			1'h1: av_2reg = out_7;
		endcase
	end

	assign av_2 = av_2reg;


	assign out = av_2;

endmodule

module slice_15_15(input [15:0] in, output [0:0] out);
	assign out = in[15:15];
endmodule

module add_16(input [15:0] in0, input [15:0] in1, output [15:0] out);
	assign out = in0 + in1;
endmodule

module invert_16(input [15:0] in, output [15:0] out);
	assign out = ~in;
endmodule

module tc_neg_16(input [15:0] in, output [15:0] out);
	wire [15:0] av_1;
	wire [15:0] invert_16_in_9;

	assign invert_16_in_9 = in;

	wire [15:0] out_10;
	invert_16 invert_16_11(.in(invert_16_in_9), .out(out_10));

	assign av_1 = out_10;

	wire [15:0] av_3;
	wire [15:0] add_16_in0_12;

	assign add_16_in0_12 = av_1;

	wire [15:0] add_16_in1_13;

	assign add_16_in1_13 = 16'h0001;

	wire [15:0] out_14;
	add_16 add_16_15(.in0(add_16_in0_12), .in1(add_16_in1_13), .out(out_14));

	assign av_3 = out_14;


	assign out = av_3;

endmodule

module newton_raphson_divide_16(input [15:0] D, input [15:0] N, output [15:0] Q);
	wire [15:0] absN;
	wire [15:0] twos_complement_absolute_value_in_16;

	assign twos_complement_absolute_value_in_16 = N;

	wire [15:0] out_17;
	twos_complement_absolute_value twos_complement_absolute_value_18(.in(twos_complement_absolute_value_in_16), .out(out_17));

	assign absN = out_17;

	wire [15:0] absD;
	wire [15:0] twos_complement_absolute_value_in_19;

	assign twos_complement_absolute_value_in_19 = D;

	wire [15:0] out_20;
	twos_complement_absolute_value twos_complement_absolute_value_21(.in(twos_complement_absolute_value_in_19), .out(out_20));

	assign absD = out_20;

	wire [0:0] av_3;
	wire [15:0] slice_15_15_in_22;

	assign slice_15_15_in_22 = N;

	wire [0:0] out_23;
	slice_15_15 slice_15_15_24(.in(slice_15_15_in_22), .out(out_23));

	assign av_3 = out_23;

	wire [0:0] av_5;
	wire [15:0] slice_15_15_in_25;

	assign slice_15_15_in_25 = N;

	wire [0:0] out_26;
	slice_15_15 slice_15_15_27(.in(slice_15_15_in_25), .out(out_26));

	assign av_5 = out_26;

	wire [0:0] NisNeg;

	assign NisNeg = av_5;

	wire [0:0] av_6;
	wire [15:0] slice_15_15_in_28;

	assign slice_15_15_in_28 = D;

	wire [0:0] out_29;
	slice_15_15 slice_15_15_30(.in(slice_15_15_in_28), .out(out_29));

	assign av_6 = out_29;

	wire [0:0] av_8;
	wire [15:0] slice_15_15_in_31;

	assign slice_15_15_in_31 = D;

	wire [0:0] out_32;
	slice_15_15 slice_15_15_33(.in(slice_15_15_in_31), .out(out_32));

	assign av_8 = out_32;

	wire [0:0] DisNeg;

	assign DisNeg = av_8;

	wire [15:0] av_9;
	wire [15:0] count_leading_zeros_16_in_34;

	assign count_leading_zeros_16_in_34 = absD;

	wire [15:0] out_35;
	count_leading_zeros_16 count_leading_zeros_16_36(.in(count_leading_zeros_16_in_34), .out(out_35));

	assign av_9 = out_35;

	wire [15:0] av_11;
	wire [15:0] count_leading_zeros_16_in_37;

	assign count_leading_zeros_16_in_37 = absD;

	wire [15:0] out_38;
	count_leading_zeros_16 count_leading_zeros_16_39(.in(count_leading_zeros_16_in_37), .out(out_38));

	assign av_11 = out_38;

	wire [15:0] DleadingZeros;

	assign DleadingZeros = av_11;

	wire [15:0] shiftDistance;
	wire [15:0] subtract_16_in0_40;

	assign subtract_16_in0_40 = DleadingZeros;

	wire [15:0] subtract_16_in1_41;

	assign subtract_16_in1_41 = 16'h0001;

	wire [15:0] out_42;
	subtract_16 subtract_16_43(.in0(subtract_16_in0_40), .in1(subtract_16_in1_41), .out(out_42));

	assign shiftDistance = out_42;

	wire [15:0] av_15;
	wire [15:0] shift_left_variable_16_in0_44;

	assign shift_left_variable_16_in0_44 = absD;

	wire [15:0] shift_left_variable_16_in1_45;

	assign shift_left_variable_16_in1_45 = shiftDistance;

	wire [15:0] out_46;
	shift_left_variable_16 shift_left_variable_16_47(.in0(shift_left_variable_16_in0_44), .in1(shift_left_variable_16_in1_45), .out(out_46));

	assign av_15 = out_46;

	wire [15:0] av_17;
	wire [15:0] shift_left_variable_16_in0_48;

	assign shift_left_variable_16_in0_48 = absD;

	wire [15:0] shift_left_variable_16_in1_49;

	assign shift_left_variable_16_in1_49 = shiftDistance;

	wire [15:0] out_50;
	shift_left_variable_16 shift_left_variable_16_51(.in0(shift_left_variable_16_in0_48), .in1(shift_left_variable_16_in1_49), .out(out_50));

	assign av_17 = out_50;

	wire [15:0] D_;

	assign D_ = av_17;

	wire [15:0] oneConst;

	assign oneConst = 16'h4000;

	wire [0:0] D_isPowOfTwo;
	wire [15:0] equals_16_in0_52;

	assign equals_16_in0_52 = D_;

	wire [15:0] equals_16_in1_53;

	assign equals_16_in1_53 = oneConst;

	wire [0:0] out_54;
	equals_16 equals_16_55(.in0(equals_16_in0_52), .in1(equals_16_in1_53), .out(out_54));

	assign D_isPowOfTwo = out_54;

	wire [15:0] shiftDiv0;
	wire [15:0] subtract_16_in0_56;

	assign subtract_16_in0_56 = 16'h0010;

	wire [15:0] subtract_16_in1_57;

	assign subtract_16_in1_57 = shiftDistance;

	wire [15:0] out_58;
	subtract_16 subtract_16_59(.in0(subtract_16_in0_56), .in1(subtract_16_in1_57), .out(out_58));

	assign shiftDiv0 = out_58;

	wire [15:0] av_26;
	wire [15:0] subtract_16_in0_60;

	assign subtract_16_in0_60 = shiftDiv0;

	wire [15:0] subtract_16_in1_61;

	assign subtract_16_in1_61 = 16'h0002;

	wire [15:0] out_62;
	subtract_16 subtract_16_63(.in0(subtract_16_in0_60), .in1(subtract_16_in1_61), .out(out_62));

	assign av_26 = out_62;

	wire [15:0] av_29;
	wire [15:0] subtract_16_in0_64;

	assign subtract_16_in0_64 = shiftDiv0;

	wire [15:0] subtract_16_in1_65;

	assign subtract_16_in1_65 = 16'h0002;

	wire [15:0] out_66;
	subtract_16 subtract_16_67(.in0(subtract_16_in0_64), .in1(subtract_16_in1_65), .out(out_66));

	assign av_29 = out_66;

	wire [15:0] shiftDiv;

	assign shiftDiv = av_29;

	wire [15:0] av_30;
	wire [15:0] logical_shift_right_variable_16_in0_68;

	assign logical_shift_right_variable_16_in0_68 = absN;

	wire [15:0] logical_shift_right_variable_16_in1_69;

	assign logical_shift_right_variable_16_in1_69 = shiftDiv;

	wire [15:0] out_70;
	logical_shift_right_variable_16 logical_shift_right_variable_16_71(.in0(logical_shift_right_variable_16_in0_68), .in1(logical_shift_right_variable_16_in1_69), .out(out_70));

	assign av_30 = out_70;

	wire [15:0] av_32;
	wire [15:0] logical_shift_right_variable_16_in0_72;

	assign logical_shift_right_variable_16_in0_72 = absN;

	wire [15:0] logical_shift_right_variable_16_in1_73;

	assign logical_shift_right_variable_16_in1_73 = shiftDiv;

	wire [15:0] out_74;
	logical_shift_right_variable_16 logical_shift_right_variable_16_75(.in0(logical_shift_right_variable_16_in0_72), .in1(logical_shift_right_variable_16_in1_73), .out(out_74));

	assign av_32 = out_74;

	wire [15:0] shrD;

	assign shrD = av_32;

	wire [15:0] one;

	assign one = 16'h8000;

	wire [15:0] D_recip0;
	wire [15:0] approximate_reciprocal_16_10_in_76;

	assign approximate_reciprocal_16_10_in_76 = D_;

	wire [15:0] out_77;
	approximate_reciprocal_16_10 approximate_reciprocal_16_10_78(.in(approximate_reciprocal_16_10_in_76), .out(out_77));

	assign D_recip0 = out_77;

	wire [15:0] X;

	assign X = D_recip0;

	// ### Iteration 0
	wire [15:0] add_16_in0_79;

	assign add_16_in0_79 = X;

	wire [15:0] add_16_in1_80;
	wire [31:0] slice_15_0_in_81;
	wire [31:0] logical_shift_right_15_32_in_82;
	wire [31:0] multiply_32_in0_83;
	wire [15:0] zero_extend_32_16_32_in_84;

	assign zero_extend_32_16_32_in_84 = X;

	wire [31:0] out_85;
	zero_extend_32_16_32 zero_extend_32_16_32_86(.in(zero_extend_32_16_32_in_84), .out(out_85));

	assign multiply_32_in0_83 = out_85;

	wire [31:0] multiply_32_in1_87;
	wire [15:0] zero_extend_32_16_32_in_88;
	wire [15:0] subtract_16_in0_89;

	assign subtract_16_in0_89 = one;

	wire [15:0] subtract_16_in1_90;
	wire [31:0] slice_15_0_in_91;
	wire [31:0] logical_shift_right_15_32_in_92;
	wire [31:0] multiply_32_in0_93;
	wire [15:0] zero_extend_32_16_32_in_94;

	assign zero_extend_32_16_32_in_94 = D_;

	wire [31:0] out_95;
	zero_extend_32_16_32 zero_extend_32_16_32_96(.in(zero_extend_32_16_32_in_94), .out(out_95));

	assign multiply_32_in0_93 = out_95;

	wire [31:0] multiply_32_in1_97;
	wire [15:0] zero_extend_32_16_32_in_98;

	assign zero_extend_32_16_32_in_98 = X;

	wire [31:0] out_99;
	zero_extend_32_16_32 zero_extend_32_16_32_100(.in(zero_extend_32_16_32_in_98), .out(out_99));

	assign multiply_32_in1_97 = out_99;

	wire [31:0] out_101;
	multiply_32 multiply_32_102(.in0(multiply_32_in0_93), .in1(multiply_32_in1_97), .out(out_101));

	assign logical_shift_right_15_32_in_92 = out_101;

	wire [31:0] out_103;
	logical_shift_right_15_32 logical_shift_right_15_32_104(.in(logical_shift_right_15_32_in_92), .out(out_103));

	assign slice_15_0_in_91 = out_103;

	wire [15:0] out_105;
	slice_15_0 slice_15_0_106(.in(slice_15_0_in_91), .out(out_105));

	assign subtract_16_in1_90 = out_105;

	wire [15:0] out_107;
	subtract_16 subtract_16_108(.in0(subtract_16_in0_89), .in1(subtract_16_in1_90), .out(out_107));

	assign zero_extend_32_16_32_in_88 = out_107;

	wire [31:0] out_109;
	zero_extend_32_16_32 zero_extend_32_16_32_110(.in(zero_extend_32_16_32_in_88), .out(out_109));

	assign multiply_32_in1_87 = out_109;

	wire [31:0] out_111;
	multiply_32 multiply_32_112(.in0(multiply_32_in0_83), .in1(multiply_32_in1_87), .out(out_111));

	assign logical_shift_right_15_32_in_82 = out_111;

	wire [31:0] out_113;
	logical_shift_right_15_32 logical_shift_right_15_32_114(.in(logical_shift_right_15_32_in_82), .out(out_113));

	assign slice_15_0_in_81 = out_113;

	wire [15:0] out_115;
	slice_15_0 slice_15_0_116(.in(slice_15_0_in_81), .out(out_115));

	assign add_16_in1_80 = out_115;

	wire [15:0] out_117;
	add_16 add_16_118(.in0(add_16_in0_79), .in1(add_16_in1_80), .out(out_117));
	wire [15:0] X_119;

	assign X_119 = out_117;

	// #########################
	wire [31:0] longProd;
	wire [31:0] multiply_32_in0_120;
	wire [15:0] zero_extend_32_16_32_in_121;

	assign zero_extend_32_16_32_in_121 = absN;

	wire [31:0] out_122;
	zero_extend_32_16_32 zero_extend_32_16_32_123(.in(zero_extend_32_16_32_in_121), .out(out_122));

	assign multiply_32_in0_120 = out_122;

	wire [31:0] multiply_32_in1_124;
	wire [15:0] zero_extend_32_16_32_in_125;

	assign zero_extend_32_16_32_in_125 = X_119;

	wire [31:0] out_126;
	zero_extend_32_16_32 zero_extend_32_16_32_127(.in(zero_extend_32_16_32_in_125), .out(out_126));

	assign multiply_32_in1_124 = out_126;

	wire [31:0] out_128;
	multiply_32 multiply_32_129(.in0(multiply_32_in0_120), .in1(multiply_32_in1_124), .out(out_128));

	assign longProd = out_128;

	wire [31:0] wConst;

	assign wConst = 32'h00000010;

	wire [31:0] twoConst;

	assign twoConst = 32'h00000002;

	wire [31:0] resShift;
	wire [31:0] add_32_in0_130;

	assign add_32_in0_130 = wConst;

	wire [31:0] add_32_in1_131;
	wire [31:0] subtract_32_in0_132;
	wire [31:0] subtract_32_in0_133;

	assign subtract_32_in0_133 = wConst;

	wire [31:0] subtract_32_in1_134;
	wire [15:0] zero_extend_32_16_32_in_135;

	assign zero_extend_32_16_32_in_135 = shiftDistance;

	wire [31:0] out_136;
	zero_extend_32_16_32 zero_extend_32_16_32_137(.in(zero_extend_32_16_32_in_135), .out(out_136));

	assign subtract_32_in1_134 = out_136;

	wire [31:0] out_138;
	subtract_32 subtract_32_139(.in0(subtract_32_in0_133), .in1(subtract_32_in1_134), .out(out_138));

	assign subtract_32_in0_132 = out_138;

	wire [31:0] subtract_32_in1_140;

	assign subtract_32_in1_140 = twoConst;

	wire [31:0] out_141;
	subtract_32 subtract_32_142(.in0(subtract_32_in0_132), .in1(subtract_32_in1_140), .out(out_141));

	assign add_32_in1_131 = out_141;

	wire [31:0] out_143;
	add_32 add_32_144(.in0(add_32_in0_130), .in1(add_32_in1_131), .out(out_143));

	assign resShift = out_143;

	//"resShift = %b"
	wire [31:0] av_46;
	wire [31:0] logical_shift_right_variable_32_in0_145;

	assign logical_shift_right_variable_32_in0_145 = longProd;

	wire [31:0] logical_shift_right_variable_32_in1_146;

	assign logical_shift_right_variable_32_in1_146 = resShift;

	wire [31:0] out_147;
	logical_shift_right_variable_32 logical_shift_right_variable_32_148(.in0(logical_shift_right_variable_32_in0_145), .in1(logical_shift_right_variable_32_in1_146), .out(out_147));

	assign av_46 = out_147;

	wire [31:0] av_48;
	wire [31:0] logical_shift_right_variable_32_in0_149;

	assign logical_shift_right_variable_32_in0_149 = longProd;

	wire [31:0] logical_shift_right_variable_32_in1_150;

	assign logical_shift_right_variable_32_in1_150 = resShift;

	wire [31:0] out_151;
	logical_shift_right_variable_32 logical_shift_right_variable_32_152(.in0(logical_shift_right_variable_32_in0_149), .in1(logical_shift_right_variable_32_in1_150), .out(out_151));

	assign av_48 = out_151;

	wire [31:0] a0;

	assign a0 = av_48;

	wire [15:0] tentativeRes;
	wire [31:0] slice_15_0_in_153;

	assign slice_15_0_in_153 = a0;

	wire [15:0] out_154;
	slice_15_0 slice_15_0_155(.in(slice_15_0_in_153), .out(out_154));

	assign tentativeRes = out_154;

	wire [15:0] av_50;
	reg [15:0] av_50reg; 
	always @(*) begin
		case (D_isPowOfTwo) 
			1'h0: av_50reg = tentativeRes;
			1'h1: av_50reg = shrD;
		endcase
	end

	assign av_50 = av_50reg;

	wire [0:0] av_51;
	wire [0:0] equals_1_in0_156;

	assign equals_1_in0_156 = DisNeg;

	wire [0:0] equals_1_in1_157;

	assign equals_1_in1_157 = NisNeg;

	wire [0:0] out_158;
	equals_1 equals_1_159(.in0(equals_1_in0_156), .in1(equals_1_in1_157), .out(out_158));

	assign av_51 = out_158;

	wire [0:0] av_53;
	wire [0:0] equals_1_in0_160;

	assign equals_1_in0_160 = DisNeg;

	wire [0:0] equals_1_in1_161;

	assign equals_1_in1_161 = NisNeg;

	wire [0:0] out_162;
	equals_1 equals_1_163(.in0(equals_1_in0_160), .in1(equals_1_in1_161), .out(out_162));

	assign av_53 = out_162;

	wire [0:0] signsMatch;

	assign signsMatch = av_53;

	wire [15:0] av_54;
	reg [15:0] av_54reg; 
	wire [15:0] tc_neg_16_in_164;

	assign tc_neg_16_in_164 = av_50;

	wire [15:0] out_165;
	tc_neg_16 tc_neg_16_166(.in(tc_neg_16_in_164), .out(out_165));
	always @(*) begin
		case (signsMatch) 
			1'h0: av_54reg = out_165;
			1'h1: av_54reg = av_50;
		endcase
	end

	assign av_54 = av_54reg;


	assign Q = av_54;

endmodule

