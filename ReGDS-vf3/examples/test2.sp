**********************o211ai_1***************************
.SUBCKT sky130_fd_sc_hd__o211ai_1 VDD GND A1 A2 B1 C1  Y
M0 a_110_297# A2 Y VDD pmos w=1e+06u l=150000u
M1 VDD A1 a_110_297# VDD pmos w=1e+06u l=150000u
M2 a_27_47# A1 GND GND nmos w=650000u l=150000u
M3 a_326_47# C1 Y GND nmos w=650000u l=150000u
M4 VDD C1 Y VDD pmos w=1e+06u l=150000u
M5 GND A2 a_27_47# GND nmos w=650000u l=150000u
M6 a_27_47# B1 a_326_47# GND nmos w=650000u l=150000u
M7 Y B1 VDD VDD pmos w=1e+06u l=150000u
.ENDS
******************************************************
*********************a22oi_1*************************
.SUBCKT sky130_fd_sc_hd__a22oi_1 VDD GND A1 A2 B1 B2  Y
M0 a_109_47# B1 Y GND nmos w=650000u l=150000u
M1 a_109_297# A2 VDD VDD pmos w=1e+06u l=150000u
M2 VDD A1 a_109_297# VDD pmos w=1e+06u l=150000u
M3 a_381_47# A2 GND GND nmos w=650000u l=150000u
M4 GND B2 a_109_47# GND nmos w=650000u l=150000u
M5 Y B2 a_109_297# VDD pmos w=1e+06u l=150000u
M6 Y A1 a_381_47# GND nmos w=650000u l=150000u
M7 a_109_297# B1 Y VDD pmos w=1e+06u l=150000u
.ENDS
************************a222oi_1**************************
.SUBCKT sky130_fd_sc_hd__a222oi_1 VDD GND A1 A2 B1 B2 C1 C2  Y
M0 GND B2 a_393_47# GND nmos w=640000u l=150000u
M1 VDD A2 a_311_297# VDD pmos w=1e+06u l=150000u
M2 a_109_297# B1 a_311_297# VDD pmos w=1e+06u l=150000u
M3 a_311_297# B2 a_109_297# VDD pmos w=1e+06u l=150000u
M4 a_311_297# A1 VDD VDD pmos w=1e+06u l=150000u
M5 a_109_47# C2 GND GND nmos w=640000u l=150000u
M6 Y A1 a_561_47# GND nmos w=640000u l=150000u
M7 a_561_47# A2 GND GND nmos w=640000u l=150000u
M8 a_393_47# B1 Y GND nmos w=640000u l=150000u
M9 Y C1 a_109_47# GND nmos w=640000u l=150000u
M10 Y C1 a_109_297# VDD pmos w=1e+06u l=150000u
M11 a_109_297# C2 Y VDD pmos w=1e+06u l=150000u
.ENDS
******************************************************
***********************a2bb2oi_1****************************
.SUBCKT sky130_fd_sc_hd__a2bb2oi_1 VDD GND A1_N A2_N B1 B2  Y
M0 a_109_297# A2_N a_109_47# VDD pmos w=1e+06u l=150000u
M1 a_481_47# B1 GND GND nmos w=650000u l=150000u
M2 Y B2 a_481_47# GND nmos w=650000u l=150000u
M3 Y a_109_47# a_397_297# VDD pmos w=1e+06u l=150000u
M4 VDD B1 a_397_297# VDD pmos w=1e+06u l=150000u
M5 a_397_297# B2 VDD VDD pmos w=1e+06u l=150000u
M6 a_109_47# A2_N GND GND nmos w=650000u l=150000u
M7 GND A1_N a_109_47# GND nmos w=650000u l=150000u
M8 VDD A1_N a_109_297# VDD pmos w=1e+06u l=150000u
M9 GND a_109_47# Y GND nmos w=650000u l=150000u
.ENDS
*****************************************************
********************o221ai_1*********************
.SUBCKT sky130_fd_sc_hd__o221ai_1 VDD GND A1 A2 B1 B2 C1  Y
M0 VDD B1 a_295_297# VDD pmos w=1e+06u l=150000u
M1 Y C1 VDD VDD pmos w=1e+06u l=150000u
M2 GND A1 a_213_123# GND nmos w=650000u l=150000u
M3 a_213_123# A2 GND GND nmos w=650000u l=150000u
M4 Y A2 a_493_297# VDD pmos w=1e+06u l=150000u
M5 a_493_297# A1 VDD VDD pmos w=1e+06u l=150000u
M6 a_109_47# B2 a_213_123# GND nmos w=650000u l=150000u
M7 Y C1 a_109_47# GND nmos w=650000u l=150000u
M8 a_295_297# B2 Y VDD pmos w=1e+06u l=150000u
M9 a_213_123# B1 a_109_47# GND nmos w=650000u l=150000u
.ENDS
*************************************************************
**********************xor2_1********************************
.SUBCKT sky130_fd_sc_hd__xor2_1 VDD GND A B Y
M0 a_35_297# A GND GND nmos w=650000u l=150000u
M1 GND B a_35_297# GND nmos w=650000u l=150000u
M2 Y a_35_297# GND GND nmos w=650000u l=150000u
M3 a_285_297# B VDD VDD pmos w=1e+06u l=150000u
M4 VDD A a_285_297# VDD pmos w=1e+06u l=150000u
M5 a_35_297# B a_117_297# VDD pmos w=1e+06u l=150000u
M6 a_117_297# A VDD VDD pmos w=1e+06u l=150000u
M7 a_285_47# B Y GND nmos w=650000u l=150000u
M8 a_285_297# a_35_297# Y VDD pmos w=1e+06u l=150000u
M9 GND A a_285_47# GND nmos w=650000u l=150000u
.ENDS sky130_fd_sc_hd__xor2_1
***********************************************************
***********************022ai_1***********************************
.SUBCKT sky130_fd_sc_hd__o22ai_1 VDD GND A1 A2 B1 B2 Y
M0 Y A2 a_307_297# VDD pmos w=1e+06u l=150000u
M1 a_307_297# A1 VDD VDD pmos w=1e+06u l=150000u
M2 a_109_297# B2 Y VDD pmos w=1e+06u l=150000u
M3 a_27_47# A2 GND GND nmos w=650000u l=150000u
M4 a_27_47# B1 Y GND nmos w=650000u l=150000u
M5 Y B2 a_27_47# GND nmos w=650000u l=150000u
M6 VDD B1 a_109_297# VDD pmos w=1e+06u l=150000u
M7 GND A1 a_27_47# GND nmos w=650000u l=150000u
.ENDS sky130_fd_sc_hd__o22ai_1
******************************mux4_2**************************************
.SUBCKT sky130_fd_sc_hd__mux4_2 A0 A1 A2 A3 S0 S1 GND VDD X
X0 VDD A1 a_1060_369# VDD pmos w=640000u l=150000u
X1 a_193_47# a_27_47# a_288_47# GND nmos w=360000u l=150000u
X2 VDD a_788_316# X VDD pmos w=1e+06u l=150000u
X3 a_1279_413# A0 VDD VDD pmos w=640000u l=150000u
X4 GND a_788_316# X GND nmos w=650000u l=150000u
X5 X a_788_316# VDD VDD pmos w=1e+06u l=150000u
X6 a_288_47# S1 a_788_316# GND nmos w=420000u l=150000u
X7 a_193_369# S0 a_288_47# VDD pmos w=420000u l=150000u
X8 a_372_413# A3 VDD VDD pmos w=640000u l=150000u
X9 a_872_316# a_27_47# a_1281_47# GND nmos w=360000u l=150000u
X10 a_288_47# a_27_47# a_372_413# VDD pmos w=420000u l=150000u
X11 GND A1 a_1064_47# GND nmos w=420000u l=150000u
X12 VDD S1 a_600_345# VDD pmos w=640000u l=150000u
X13 a_872_316# S0 a_1279_413# VDD pmos w=420000u l=150000u
X14 a_27_47# S0 VDD VDD pmos w=640000u l=150000u
X15 VDD A2 a_193_369# VDD pmos w=640000u l=150000u
X16 GND A2 a_193_47# GND nmos w=420000u l=150000u
X17 a_288_47# a_600_345# a_788_316# VDD pmos w=540000u l=150000u
X18 a_397_47# A3 GND GND nmos w=420000u l=150000u
X19 a_27_47# S0 GND GND nmos w=420000u l=150000u
X20 a_1060_369# a_27_47# a_872_316# VDD pmos w=420000u l=150000u
X21 GND S1 a_600_345# GND nmos w=420000u l=150000u
X22 a_1281_47# A0 GND GND nmos w=420000u l=150000u
X23 X a_788_316# GND GND nmos w=650000u l=150000u
X24 a_288_47# S0 a_397_47# GND nmos w=360000u l=150000u
X25 a_788_316# S1 a_872_316# VDD pmos w=540000u l=150000u
X26 a_788_316# a_600_345# a_872_316# GND nmos w=420000u l=150000u
X27 a_1064_47# S0 a_872_316# GND nmos w=360000u l=150000u
.ENDS
**********************************************************
************************xnor2_1**************************
.SUBCKT sky130_fd_sc_hd__xnor2_1 VDD GND A B Y
M0 VDD A a_377_297# VDD pmos w=1e+06u l=150000u
M1 a_377_297# B Y VDD pmos w=1e+06u l=150000u
M2 Y a_47_47# VDD VDD pmos w=1e+06u l=150000u
M3 a_129_47# A GND GND nmos w=650000u l=150000u
M4 GND B a_285_47# GND nmos w=650000u l=150000u
M5 a_47_47# B a_129_47# GND nmos w=650000u l=150000u
M6 VDD B a_47_47# VDD pmos w=1e+06u l=150000u
M7 a_47_47# A VDD VDD pmos w=1e+06u l=150000u
M8 GND A a_285_47# GND nmos w=650000u l=150000u
M9 a_285_47# a_47_47# Y GND nmos w=650000u l=150000u
.ENDS sky130_fd_sc_hd__xnor2_1
************************nor4_1*************************
.SUBCKT sky130_fd_sc_hd__nor4_1 A B C D GND  VDD Y
M0 GND B Y GND nmos w=650000u l=150000u
M1 a_191_297# B a_297_297# VDD pmos w=1e+06u l=150000u
M2 a_297_297# A VDD VDD pmos w=1e+06u l=150000u
M3 Y C GND GND nmos w=650000u l=150000u
M4 GND D Y GND nmos w=650000u l=150000u
M5 a_109_297# C a_191_297# VDD pmos w=1e+06u l=150000u
M6 Y D a_109_297# VDD pmos w=1e+06u l=150000u
M7 Y A GND GND nmos w=650000u l=150000u
.ENDS
*********************************************************
**********************and3_1****************************
.SUBCKT sky130_fd_sc_hd__and3_1 A B C GND VDD Y
M0 a_181_47# C GND GND nmos w=420000u l=150000u
M1 a_27_47# A VDD VDD pmos w=420000u l=150000u
M2 VDD B a_27_47# VDD pmos w=420000u l=150000u
M3 VDD a_27_47# Y VDD pmos w=1e+06u l=150000u
M4 a_109_47# B a_181_47# GND nmos w=420000u l=150000u
M5 a_27_47# A a_109_47# GND nmos w=420000u l=150000u
M6 GND a_27_47# Y GND nmos w=650000u l=150000u
M7 a_27_47# C VDD VDD pmos w=420000u l=150000u
.ENDS
************************nor2_1**********************
.SUBCKT sky130_fd_sc_hd__nor2_1 VDD GND A B Y
M0 a_109_297# A VDD VDD pmos w=1e+06u l=150000u
M1 Y A GND GND nmos w=650000u l=150000u
M2 GND B Y GND nmos w=650000u l=150000u
M3 Y B a_109_297# VDD pmos w=1e+06u l=150000u
.ENDS
************************************************
*****************************a21oi_1*****************************
.SUBCKT sky130_fd_sc_hd__a21oi_1 A1 A2 B1 GND VDD Y
M0 a_199_47# A2 GND GND nmos w=650000u l=150000u
M1 a_113_297# A1 VDD VDD pmos w=1e+06u l=150000u
M2 Y B1 a_113_297# VDD pmos w=1e+06u l=150000u
M3 GND B1 Y GND nmos w=650000u l=150000u
M4 VDD A2 a_113_297# VDD pmos w=1e+06u l=150000u
M5 Y A1 a_199_47# GND nmos w=650000u l=150000u
.ENDS
*********************************************************
*************************inv_1**********************
.SUBCKT sky130_fd_sc_hd__inv_1 VDD GND A Y
M0 GND A Y GND nmos w=650000u l=150000u
M1 VDD A Y VDD pmos w=1e+06u l=150000u
.ENDS sky130_fd_sc_hd__inv_1
*************************************************

********************nand2b_1*********************
.SUBCKT sky130_fd_sc_hd__nand2b_1 A_N B GND VDD Y
M0 a_206_47# a_27_93# Y GND nmos w=650000u l=150000u
M1 a_27_93# A_N GND GND nmos w=420000u l=150000u
M2 GND B a_206_47# GND nmos w=650000u l=150000u
M3 VDD B Y VDD pmos w=1e+06u l=150000u
M4 a_27_93# A_N VDD VDD pmos w=420000u l=150000u
M5 Y a_27_93# VDD VDD pmos w=1e+06u l=150000u
.ENDS
**************************************************
**********************nand2_1*******************
.SUBCKT sky130_fd_sc_hd__nand2_1 VDD GND A B Y
M0 Y A VDD VDD pmos w=1e+06u l=150000u
M1 VDD B Y VDD pmos w=1e+06u l=150000u
M2 GND B a_113_47# GND nmos w=650000u l=150000u
M3 a_113_47# A Y GND nmos w=650000u l=150000u
.ENDS
****************************************************
***********************nor2b_1********************
.SUBCKT sky130_fd_sc_hd__nor2b_1 A B_N GND VDD Y
M0 a_74_47# B_N VDD VDD pmos w=420000u l=150000u
M1 Y a_74_47# GND GND nmos w=650000u l=150000u
M2 GND A Y GND nmos w=650000u l=150000u
M3 a_74_47# B_N GND GND nmos w=420000u l=150000u
M4 VDD A a_265_297# VDD pmos w=1e+06u l=150000u
M5 a_265_297# a_74_47# Y VDD pmos w=1e+06u l=150000u
.ENDS
********************************************************
**********************a31oi_1**************************
.SUBCKT sky130_fd_sc_hd__a31oi_1 A1 A2 A3 B1 GND VDD Y
M0 a_109_297# B1 Y VDD pmos w=1e+06u l=150000u
M1 a_181_47# A1 Y GND nmos w=650000u l=150000u
M2 VDD A1 a_109_297# VDD pmos w=1e+06u l=150000u
M3 a_109_47# A2 a_181_47# GND nmos w=650000u l=150000u
M4 Y B1 GND GND nmos w=650000u l=150000u
M5 GND A3 a_109_47# GND nmos w=650000u l=150000u
M6 VDD A3 a_109_297# VDD pmos w=1e+06u l=150000u
M7 a_109_297# A2 VDD VDD pmos w=1e+06u l=150000u
.ENDS
*************************************************
**********************or4_1**************************
.SUBCKT sky130_fd_sc_hd__or4_1 A B C D GND VDD X
X0 a_27_297# A GND GND nmos w=420000u l=150000u
X1 GND D a_27_297# GND nmos w=420000u l=150000u
X2 GND B a_27_297# GND nmos w=420000u l=150000u
X3 GND a_27_297# X GND nmos w=650000u l=150000u
X4 a_109_297# C a_205_297# VDD pmos w=420000u l=150000u
X5 a_205_297# B a_277_297# VDD pmos w=420000u l=150000u
X6 a_27_297# C GND GND nmos w=420000u l=150000u
X7 a_277_297# A VDD VDD pmos w=420000u l=150000u
X8 a_27_297# D a_109_297# VDD pmos w=420000u l=150000u
X9 VDD a_27_297# X VDD pmos w=1e+06u l=150000u
.ENDS
**********************************************
**********************and2_1****************
.SUBCKT sky130_fd_sc_hd__and2_1 VDD GND A B Y
M0 GND a_59_75# Y GND nmos w=650000u l=150000u
M1 VDD A a_59_75# VDD pmos w=420000u l=150000u
M2 a_59_75# A a_145_75# GND nmos w=420000u l=150000u
M3 a_59_75# B VDD VDD pmos w=420000u l=150000u
M4 a_145_75# B GND GND nmos w=420000u l=150000u
M5 VDD a_59_75# Y VDD pmos w=1e+06u l=150000u
.ENDS sky130_fd_sc_hd__and2_1
*******************************************************
*********************ha_1**********************************
.SUBCKT sky130_fd_sc_hd__ha_1 A B GND VDD COUT SUM
M0 a_250_199# B a_674_47# GND nmos w=420000u l=150000u
M1 a_79_21# a_250_199# a_297_47# GND nmos w=420000u l=150000u
M2 a_250_199# A VDD VDD pmos w=420000u l=150000u
M3 VDD a_250_199# COUT VDD pmos w=1e+06u l=150000u
M4 GND A a_297_47# GND nmos w=420000u l=150000u
M5 a_79_21# B a_376_413# VDD pmos w=420000u l=150000u
M6 a_376_413# A VDD VDD pmos w=420000u l=150000u
M7 SUM a_79_21# GND GND nmos w=650000u l=150000u
M8 a_674_47# A GND GND nmos w=420000u l=150000u
M9 SUM a_79_21# VDD VDD pmos w=1e+06u l=150000u
M10 VDD a_250_199# a_79_21# VDD pmos w=420000u l=150000u
M11 a_297_47# B GND GND nmos w=420000u l=150000u
M12 GND a_250_199# COUT GND nmos w=650000u l=150000u
M13 VDD B a_250_199# VDD pmos w=420000u l=150000u
.ENDS
*******************************************************
************************mux2i_1**********************
.SUBCKT sky130_fd_sc_hd__mux2i_1 A0 A1 S GND VDD Y
X0 a_283_205# S VDD VDD pmos w=1e+06u l=150000u
X1 VDD S a_27_297# VDD pmos w=1e+06u l=150000u
X2 a_27_297# A0 Y VDD pmos w=1e+06u l=150000u
X3 GND S a_193_47# GND nmos w=650000u l=150000u
X4 a_204_297# a_283_205# VDD VDD pmos w=1e+06u l=150000u
X5 a_27_47# a_283_205# GND GND nmos w=650000u l=150000u
X6 a_283_205# S GND GND nmos w=650000u l=150000u
X7 Y A1 a_204_297# VDD pmos w=1e+06u l=150000u
X8 Y A1 a_193_47# GND nmos w=650000u l=150000u
X9 a_27_47# A0 Y GND nmos w=650000u l=150000u
.ENDS
***********************************************************
***********************mux2_2*****************************
.SUBCKT sky130_fd_sc_hd__mux2_2 A0 A1 S GND VDD X
X0 GND a_257_199# a_288_47# GND nmos w=420000u l=150000u
X1 a_306_369# A1 a_79_21# VDD pmos w=640000u l=150000u
X2 a_288_47# A0 a_79_21# GND nmos w=420000u l=150000u
X3 a_79_21# A1 a_578_47# GND nmos w=420000u l=150000u
X4 VDD S a_257_199# VDD pmos w=640000u l=150000u
X5 a_79_21# A0 a_591_369# VDD pmos w=640000u l=150000u
X6 GND S a_257_199# GND nmos w=420000u l=150000u
X7 a_591_369# S VDD VDD pmos w=640000u l=150000u
X8 a_578_47# S GND GND nmos w=420000u l=150000u
X9 X a_79_21# GND GND nmos w=650000u l=150000u
X10 GND a_79_21# X GND nmos w=650000u l=150000u
X11 VDD a_79_21# X VDD pmos w=1e+06u l=150000u
X12 VDD a_257_199# a_306_369# VDD pmos w=640000u l=150000u
X13 X a_79_21# VDD VDD pmos w=1e+06u l=150000u
.ENDS
********************************************************************
****************************and4b_1**************************
.SUBCKT sky130_fd_sc_hd__and4b_1 A_N B C D GND VDD Y
M0 VDD a_193_413# Y VDD pmos w=1e+06u l=150000u
M1 a_193_413# a_27_47# a_297_47# GND nmos w=420000u l=150000u
M2 a_193_413# D VDD VDD pmos w=420000u l=150000u
M3 VDD C a_193_413# VDD pmos w=420000u l=150000u
M4 a_369_47# C a_469_47# GND nmos w=420000u l=150000u
M5 a_27_47# A_N VDD VDD pmos w=420000u l=150000u
M6 a_193_413# B VDD VDD pmos w=420000u l=150000u
M7 GND a_193_413# Y GND nmos w=650000u l=150000u
M8 VDD a_27_47# a_193_413# VDD pmos w=420000u l=150000u
M9 a_27_47# A_N GND GND nmos w=420000u l=150000u
M10 a_297_47# B a_369_47# GND nmos w=420000u l=150000u
M11 a_469_47# D GND GND nmos w=420000u l=150000u
.ENDS
*************************************************************
