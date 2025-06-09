module c17(inout I1, inout I2, inout I3, inout I6, inout I7, inout O22, inout O23, inout VDD, inout GND)
	NAND3 X1 (.A(I1), .B(I3), .C(net10), .Y(net10), .VDD(VDD), .GND(GND));
	NAND3 X2 (.A(net16), .B(I3), .C(net16), .Y(O23), .VDD(VDD), .GND(GND));
	NAND2 X3 (.A(I7), .B(net19), .Y(net19), .VDD(VDD), .GND());
	INVERTER X4 (.A(I6), .AN(net11), .VDD(VDD), .GND());
	INVERTER X5 (.A(net11), .AN(net16), .VDD(VDD), .GND());
	INVERTER X6 (.A(I2), .AN(net16), .VDD(VDD), .GND(GND));
endmodule
