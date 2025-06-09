
*****************XOR2X1**************
.SUBCKT XOR2X1 A B Y VDD GND
M0 VDD A a_2_6# VDD pmos w=1u l=0.05u
M1 a_18_54# a_13_43# VDD VDD pmos w=1u l=0.05u
M2 Y A a_18_54# VDD pmos w=1u l=0.05u
M3 a_35_54# a_2_6# Y VDD pmos w=1u l=0.05u
M4 VDD B a_35_54# VDD pmos w=1u l=0.05u
M5 a_13_43# B VDD VDD pmos w=1u l=0.05u
M6 GND A a_2_6# GND nmos w=0.5u l=0.05u
M7 a_18_6# a_13_43# GND GND nmos w=0.5u l=0.05u
M8 Y a_2_6# a_18_6# GND nmos w=0.5u l=0.05u
M9 a_35_6# A Y GND nmos w=0.5u l=0.05u
M10 GND B a_35_6# GND nmos w=0.5u l=0.05u
M11 a_13_43# B GND GND nmos w=0.5u l=0.05u
.ENDS 
********************************************************
*****************XNOR2X1**************
.SUBCKT XNOR2X1 A B Y VDD GND
M0 VDD A a_2_6# VDD pmos w=1u l=0.05u
M1 a_18_54# a_12_41# VDD VDD pmos w=1u l=0.05u
M2 Y a_2_6# a_18_54# VDD pmos w=1u l=0.05u 
M3 a_35_54# A Y VDD pmos w=1u l=0.05u 
M4 VDD B a_35_54# VDD pmos w=1u l=0.05u 
M5 a_12_41# B VDD VDD pmos w=1u l=0.05u
M6 GND A a_2_6# GND nmos w=0.5u l=0.05u
M7 a_18_6# a_12_41# GND GND nmos w=0.5u l=0.05u 
M8 Y A a_18_6# GND nmos w=0.5u l=0.05u
M9 a_35_6# a_2_6# Y GND nmos w=0.5u l=0.05u
M10 GND B a_35_6# GND nmos w=0.5u l=0.05u
M11 a_12_41# B GND GND nmos w=0.5u l=0.05u
.ENDS
*******************************************************
*****************NOR3X1**************
.SUBCKT NOR3X1 VDD GND A B C Y
M0 VDD A a_2_64# VDD pmos w=0.75u l=0.05u
M1 a_2_64# A VDD VDD pmos w=0.75u l=0.05u
M2 a_25_64# B a_2_64# VDD pmos w=0.75u l=0.05u
M3 a_2_64# B a_25_64# VDD pmos w=0.75u l=0.05u
M4 Y C a_25_64# VDD pmos w=0.75u l=0.05u
M5 a_25_64# C Y VDD pmos w=0.75u l=0.05u
M6 Y A GND GND nmos w=0.25u l=0.05u
M7 GND B Y GND nmos w=0.25u l=0.05u
M8 Y C GND GND nmos w=0.25u l=0.05u
.ENDS
*******************************************************
*****************NAND3X1**************
.SUBCKT NAND3X1 VDD GND A B C Y
M0 Y A VDD VDD pmos w=0.5u l=0.05u
M1 VDD B Y VDD pmos w=0.5u l=0.05u
M2 Y C VDD VDD pmos w=0.5u l=0.05u
M3 a_9_6# A GND GND nmos w=0.75u l=0.05u
M4 a_14_6# B a_9_6# GND nmos w=0.75u l=0.05u
M5 Y C a_14_6# GND nmos w=0.75u l=0.05u
.ENDS
*******************************************************
*****************OR2X1**************
.SUBCKT OR2X1 A B Y VDD GND
M0 a_9_54# A a_2_54# VDD pmos w=1u l=0.05u
M1 VDD B a_9_54# VDD pmos w=1u l=0.05u
M2 Y a_2_54# VDD VDD pmos w=0.5u l=0.05u
M3 a_2_54# A GND GND nmos w=0.25u l=0.05u
M4 GND B a_2_54# GND nmos w=0.25u l=0.05u
M5 Y a_2_54# GND GND nmos w=0.25u l=0.05u
.ENDS
*******************************************************
*****************AND2X1**************
.SUBCKT AND2X1 A B Y VDD GND
M0 a_2_6# A VDD VDD pmos w=0.5u l=0.05u
M1 VDD B a_2_6# VDD pmos w=0.5u l=0.05u
M2 Y a_2_6# VDD VDD pmos w=0.5u l=0.05u
M3 a_9_6# A a_2_6# GND nmos w=0.5u l=0.05u
M4 GND B a_9_6# GND nmos w=0.5u l=0.05u
M5 Y a_2_6# GND GND nmos w=0.25u l=0.05u
.ENDS
*******************************************************
**************NOR2X1************************
.SUBCKT NOR2X1 VDD B GND Y A
M0 a_9_54# A VDD VDD pmos w=1u l=0.05u
M1 Y B a_9_54# VDD pmos w=1u l=0.05u
M2 Y A GND GND nmos w=0.25u l=0.05u
M3 GND B Y GND nmos w=0.25u l=0.05u
.ENDS
***************************************************
*************NAND2X1***********************
.SUBCKT NAND2X1 VDD GND A B Y
M0 Y A VDD VDD pmos w=0.5u l=0.05u
M1 VDD B Y VDD pmos w=0.5u l=0.05u
M2 a_9_6# A GND GND nmos w=0.5u l=0.05u
M3 Y B a_9_6# GND nmos w=0.5u l=0.05u
.ENDS
***********************************************
*****************BUFX2**************
.SUBCKT BUFX2 VDD GND A Y
M0 VDD A a_2_6# VDD pmos w=0.5u l=0.05u
M1 Y a_2_6# VDD VDD pmos w=1u l=0.05u
M2 GND A a_2_6# GND nmos w=0.25u l=0.05u
M3 Y a_2_6# GND GND nmos w=0.5u l=0.05u
.ENDS
*******************************************************
*****************INVX1**************
.SUBCKT INVX1 VDD GND A Y
M0 Y A VDD VDD pmos w=0.5u l=0.05u
M1 Y A GND GND nmos w=0.25u l=0.05u
.ENDS
*******************************************************
