EESchema Schematic File Version 4
LIBS:hw-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CPU:Z80CPU U3
U 1 1 5D29D309
P 4300 2800
F 0 "U3" H 3800 4200 50  0000 C CNN
F 1 "Z80CPU" H 4500 4200 50  0000 C CNN
F 2 "" H 4300 3200 50  0001 C CNN
F 3 "www.zilog.com/manage_directlink.php?filepath=docs/z80/um0080" H 4300 3200 50  0001 C CNN
	1    4300 2800
	1    0    0    -1  
$EndComp
$Comp
L Oscillator:CXO_DIP8 X1
U 1 1 5D29D3C1
P 3650 6800
F 0 "X1" H 3900 6950 50  0000 L CNN
F 1 "CXO_DIP8" H 3700 7050 50  0000 L CNN
F 2 "Oscillator:Oscillator_DIP-8" H 4100 6450 50  0001 C CNN
F 3 "http://cdn-reichelt.de/documents/datenblatt/B400/OSZI.pdf" H 3550 6800 50  0001 C CNN
	1    3650 6800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0101
U 1 1 5D29D46E
P 3650 6400
F 0 "#PWR0101" H 3650 6250 50  0001 C CNN
F 1 "+5V" H 3665 6573 50  0000 C CNN
F 2 "" H 3650 6400 50  0001 C CNN
F 3 "" H 3650 6400 50  0001 C CNN
	1    3650 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 6400 3650 6450
$Comp
L power:GND #PWR0102
U 1 1 5D29D4DC
P 3650 7200
F 0 "#PWR0102" H 3650 6950 50  0001 C CNN
F 1 "GND" H 3655 7027 50  0000 C CNN
F 2 "" H 3650 7200 50  0001 C CNN
F 3 "" H 3650 7200 50  0001 C CNN
	1    3650 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 7200 3650 7100
Wire Wire Line
	3350 6800 3300 6800
Wire Wire Line
	3300 6800 3300 6450
Wire Wire Line
	3300 6450 3650 6450
Connection ~ 3650 6450
Wire Wire Line
	3650 6450 3650 6500
$Comp
L 74xx:74LS161 U4
U 1 1 5D29D60D
P 5050 6500
F 0 "U4" H 4800 7150 50  0000 C CNN
F 1 "74LS161" H 5250 7150 50  0000 C CNN
F 2 "" H 5050 6500 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS161" H 5050 6500 50  0001 C CNN
	1    5050 6500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0103
U 1 1 5D29D7E3
P 5050 5600
F 0 "#PWR0103" H 5050 5450 50  0001 C CNN
F 1 "+5V" H 5065 5773 50  0000 C CNN
F 2 "" H 5050 5600 50  0001 C CNN
F 3 "" H 5050 5600 50  0001 C CNN
	1    5050 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5D29D809
P 5050 7400
F 0 "#PWR0104" H 5050 7150 50  0001 C CNN
F 1 "GND" H 5055 7227 50  0000 C CNN
F 2 "" H 5050 7400 50  0001 C CNN
F 3 "" H 5050 7400 50  0001 C CNN
	1    5050 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 7300 5050 7400
Wire Wire Line
	5050 5600 5050 5700
$Comp
L power:+5V #PWR0105
U 1 1 5D29D9EF
P 4450 6600
F 0 "#PWR0105" H 4450 6450 50  0001 C CNN
F 1 "+5V" V 4465 6728 50  0000 L CNN
F 2 "" H 4450 6600 50  0001 C CNN
F 3 "" H 4450 6600 50  0001 C CNN
	1    4450 6600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4450 6600 4500 6600
Wire Wire Line
	4500 6600 4500 6700
Wire Wire Line
	4500 6700 4550 6700
Connection ~ 4500 6600
Wire Wire Line
	4500 6600 4550 6600
$Comp
L power:+5V #PWR0106
U 1 1 5D29DC0C
P 4450 7000
F 0 "#PWR0106" H 4450 6850 50  0001 C CNN
F 1 "+5V" V 4465 7128 50  0000 L CNN
F 2 "" H 4450 7000 50  0001 C CNN
F 3 "" H 4450 7000 50  0001 C CNN
	1    4450 7000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4550 7000 4450 7000
Text GLabel 4450 6500 0    50   Input ~ 0
CLK_SEL
Wire Wire Line
	4450 6500 4550 6500
Text GLabel 4450 6000 0    50   Input ~ 0
CLK_OVR
Wire Wire Line
	4450 6000 4550 6000
$Comp
L power:GND #PWR0107
U 1 1 5D29DFDE
P 4450 6300
F 0 "#PWR0107" H 4450 6050 50  0001 C CNN
F 1 "GND" V 4455 6172 50  0000 R CNN
F 2 "" H 4450 6300 50  0001 C CNN
F 3 "" H 4450 6300 50  0001 C CNN
	1    4450 6300
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 6300 4500 6300
Wire Wire Line
	4550 6100 4500 6100
Wire Wire Line
	4500 6100 4500 6200
Connection ~ 4500 6300
Wire Wire Line
	4500 6300 4550 6300
Wire Wire Line
	4500 6200 4550 6200
Connection ~ 4500 6200
Wire Wire Line
	4500 6200 4500 6300
NoConn ~ 5550 6500
NoConn ~ 5550 6300
NoConn ~ 5550 6200
NoConn ~ 5550 6100
Text GLabel 5650 6000 2    50   Output ~ 0
CLK_CPU
Wire Wire Line
	5650 6000 5550 6000
Text GLabel 3500 1900 0    50   Input ~ 0
CLK_CPU
Wire Wire Line
	3600 1900 3500 1900
Text GLabel 3500 1600 0    50   Input ~ 0
~RST
Wire Wire Line
	3500 1600 3600 1600
Wire Wire Line
	3500 2200 3600 2200
Wire Wire Line
	3600 2300 3500 2300
$Comp
L power:+5V #PWR0108
U 1 1 5D29FF87
P 3500 2200
F 0 "#PWR0108" H 3500 2050 50  0001 C CNN
F 1 "+5V" V 3515 2328 50  0000 L CNN
F 2 "" H 3500 2200 50  0001 C CNN
F 3 "" H 3500 2200 50  0001 C CNN
	1    3500 2200
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 5D29FF98
P 3500 2300
F 0 "#PWR0109" H 3500 2150 50  0001 C CNN
F 1 "+5V" V 3515 2428 50  0000 L CNN
F 2 "" H 3500 2300 50  0001 C CNN
F 3 "" H 3500 2300 50  0001 C CNN
	1    3500 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3500 2800 3600 2800
$Comp
L power:+5V #PWR0111
U 1 1 5D2A0326
P 3500 3900
F 0 "#PWR0111" H 3500 3750 50  0001 C CNN
F 1 "+5V" V 3515 4028 50  0000 L CNN
F 2 "" H 3500 3900 50  0001 C CNN
F 3 "" H 3500 3900 50  0001 C CNN
	1    3500 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3500 3900 3600 3900
$Comp
L power:GND #PWR0112
U 1 1 5D2A070C
P 4300 4400
F 0 "#PWR0112" H 4300 4150 50  0001 C CNN
F 1 "GND" H 4305 4227 50  0000 C CNN
F 2 "" H 4300 4400 50  0001 C CNN
F 3 "" H 4300 4400 50  0001 C CNN
	1    4300 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 4300 4300 4400
$Comp
L power:+5V #PWR0113
U 1 1 5D2A0AD0
P 4300 1200
F 0 "#PWR0113" H 4300 1050 50  0001 C CNN
F 1 "+5V" H 4315 1373 50  0000 C CNN
F 2 "" H 4300 1200 50  0001 C CNN
F 3 "" H 4300 1200 50  0001 C CNN
	1    4300 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1200 4300 1300
Text GLabel 5100 1600 2    50   Output ~ 0
A0
Text GLabel 5100 1700 2    50   Output ~ 0
A1
Text GLabel 5100 1800 2    50   Output ~ 0
A2
Text GLabel 5100 1900 2    50   Output ~ 0
A3
Text GLabel 5100 2000 2    50   Output ~ 0
A4
Text GLabel 5100 2100 2    50   Output ~ 0
A5
Text GLabel 5100 2200 2    50   Output ~ 0
A6
Text GLabel 5100 2300 2    50   Output ~ 0
A7
Text GLabel 5100 2400 2    50   Output ~ 0
A8
Text GLabel 5100 2500 2    50   Output ~ 0
A9
Text GLabel 5100 2600 2    50   Output ~ 0
A10
Text GLabel 5100 2700 2    50   Output ~ 0
A11
Text GLabel 5100 2800 2    50   Output ~ 0
A12
Text GLabel 5100 2900 2    50   Output ~ 0
A13
Text GLabel 5100 3000 2    50   Output ~ 0
A14
Wire Wire Line
	5000 1600 5100 1600
Wire Wire Line
	5100 1700 5000 1700
Wire Wire Line
	5000 1800 5100 1800
Wire Wire Line
	5100 1900 5000 1900
Wire Wire Line
	5000 2000 5100 2000
Wire Wire Line
	5100 2100 5000 2100
Wire Wire Line
	5000 2200 5100 2200
Wire Wire Line
	5100 2300 5000 2300
Wire Wire Line
	5000 2400 5100 2400
Wire Wire Line
	5100 2500 5000 2500
Wire Wire Line
	5000 2600 5100 2600
Wire Wire Line
	5100 2700 5000 2700
Wire Wire Line
	5000 2800 5100 2800
Wire Wire Line
	5100 2900 5000 2900
Wire Wire Line
	5000 3000 5100 3000
Text GLabel 5100 3300 2    50   BiDi ~ 0
D0
Text GLabel 5100 3400 2    50   BiDi ~ 0
D1
Text GLabel 5100 3500 2    50   BiDi ~ 0
D2
Text GLabel 5100 3600 2    50   BiDi ~ 0
D3
Text GLabel 5100 3700 2    50   BiDi ~ 0
D4
Text GLabel 5100 3800 2    50   BiDi ~ 0
D5
Text GLabel 5100 3900 2    50   BiDi ~ 0
D6
Text GLabel 5100 4000 2    50   BiDi ~ 0
D7
Wire Wire Line
	5100 3300 5000 3300
Wire Wire Line
	5000 3400 5100 3400
Wire Wire Line
	5100 3500 5000 3500
Wire Wire Line
	5000 3600 5100 3600
Wire Wire Line
	5100 3700 5000 3700
Wire Wire Line
	5000 3800 5100 3800
Wire Wire Line
	5100 3900 5000 3900
Wire Wire Line
	5000 4000 5100 4000
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5D2ADAF0
P 1700 2000
F 0 "A1" H 1350 2950 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 2300 2950 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 1850 1050 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 1700 1000 50  0001 C CNN
	1    1700 2000
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0114
U 1 1 5D2ADB58
P 1700 3100
F 0 "#PWR0114" H 1700 2850 50  0001 C CNN
F 1 "GND" H 1705 2927 50  0000 C CNN
F 2 "" H 1700 3100 50  0001 C CNN
F 3 "" H 1700 3100 50  0001 C CNN
	1    1700 3100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1700 3000 1700 3050
Wire Wire Line
	1700 3050 1600 3050
Wire Wire Line
	1600 3050 1600 3000
Connection ~ 1700 3050
Wire Wire Line
	1700 3050 1700 3100
$Comp
L Diode:1N4148 D1
U 1 1 5D2AF86D
P 1350 900
F 0 "D1" H 1350 1116 50  0000 C CNN
F 1 "1N4148" H 1350 1025 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 1350 725 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/1N4148_1N4448.pdf" H 1350 900 50  0001 C CNN
	1    1350 900 
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1500 900  1500 1000
NoConn ~ 1600 1000
NoConn ~ 1800 1000
$Comp
L power:+5V #PWR0115
U 1 1 5D2B25DD
P 1100 800
F 0 "#PWR0115" H 1100 650 50  0001 C CNN
F 1 "+5V" H 1115 973 50  0000 C CNN
F 2 "" H 1100 800 50  0001 C CNN
F 3 "" H 1100 800 50  0001 C CNN
	1    1100 800 
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1100 800  1100 900 
NoConn ~ 1200 1400
NoConn ~ 1200 1500
NoConn ~ 1200 1800
Text GLabel 1100 2000 0    50   Output ~ 0
CLK_SEL
Text GLabel 1100 2100 0    50   Output ~ 0
CLK_OVR
Wire Wire Line
	1200 2000 1100 2000
Wire Wire Line
	1100 2100 1200 2100
Text GLabel 3500 3300 0    50   Output ~ 0
~RD
Wire Wire Line
	3500 3300 3600 3300
Text GLabel 1100 2400 0    50   Input ~ 0
~RD
Text GLabel 900  2500 0    50   Output ~ 0
~RST
Wire Wire Line
	900  2500 1200 2500
Wire Wire Line
	1100 2400 1200 2400
NoConn ~ 1200 2600
NoConn ~ 1200 2700
$Comp
L 74xx:74LS138 U2
U 1 1 5D2CA2C9
P 1700 6700
F 0 "U2" H 1450 7150 50  0000 C CNN
F 1 "74LS138" H 1900 7150 50  0000 C CNN
F 2 "" H 1700 6700 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS138" H 1700 6700 50  0001 C CNN
	1    1700 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5D2CA412
P 1700 7500
F 0 "#PWR0116" H 1700 7250 50  0001 C CNN
F 1 "GND" H 1705 7327 50  0000 C CNN
F 2 "" H 1700 7500 50  0001 C CNN
F 3 "" H 1700 7500 50  0001 C CNN
	1    1700 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 7400 1700 7500
$Comp
L power:+5V #PWR0117
U 1 1 5D2CB8F0
P 1700 6000
F 0 "#PWR0117" H 1700 5850 50  0001 C CNN
F 1 "+5V" H 1715 6173 50  0000 C CNN
F 2 "" H 1700 6000 50  0001 C CNN
F 3 "" H 1700 6000 50  0001 C CNN
	1    1700 6000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1700 6000 1700 6100
Text GLabel 1100 7050 0    50   Input ~ 0
~MEM_REQ
Wire Wire Line
	1200 7100 1150 7100
$Comp
L power:+5V #PWR0118
U 1 1 5D2CFC4B
P 1100 6900
F 0 "#PWR0118" H 1100 6750 50  0001 C CNN
F 1 "+5V" V 1115 7028 50  0000 L CNN
F 2 "" H 1100 6900 50  0001 C CNN
F 3 "" H 1100 6900 50  0001 C CNN
	1    1100 6900
	0    -1   1    0   
$EndComp
Wire Wire Line
	1200 6900 1100 6900
Wire Wire Line
	1100 7050 1150 7050
Wire Wire Line
	1150 7050 1150 7000
Wire Wire Line
	1150 7000 1200 7000
Text GLabel 1100 7200 0    50   Input ~ 0
~RD
Wire Wire Line
	1150 6500 1200 6500
Wire Wire Line
	1100 6400 1200 6400
Wire Wire Line
	1100 7200 1150 7200
Wire Wire Line
	1150 7100 1150 7200
$Comp
L power:GND #PWR0119
U 1 1 5D2DD05E
P 1100 6600
F 0 "#PWR0119" H 1100 6350 50  0001 C CNN
F 1 "GND" V 1105 6472 50  0000 R CNN
F 2 "" H 1100 6600 50  0001 C CNN
F 3 "" H 1100 6600 50  0001 C CNN
	1    1100 6600
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 6600 1150 6600
Wire Wire Line
	1150 6500 1150 6600
Connection ~ 1150 6600
Wire Wire Line
	1150 6600 1100 6600
NoConn ~ 2200 6600
NoConn ~ 2200 6700
NoConn ~ 2200 6800
NoConn ~ 2200 6900
NoConn ~ 2200 7000
NoConn ~ 2200 7100
Text GLabel 2300 6350 2    50   Output ~ 0
~SR_OE
Text GLabel 2300 6500 2    50   Output ~ 0
~RAM_OE
Wire Wire Line
	2300 6500 2200 6500
Wire Wire Line
	2200 6400 2250 6400
Wire Wire Line
	2250 6400 2250 6350
Wire Wire Line
	2250 6350 2300 6350
Text GLabel 2300 1900 2    50   Output ~ 0
SR_SER
Text GLabel 2300 2100 2    50   Output ~ 0
SR_MODE
Text GLabel 2300 2200 2    50   Output ~ 0
SR_CLK
Wire Wire Line
	2300 2100 2200 2100
Wire Wire Line
	2200 2200 2300 2200
Wire Wire Line
	2200 1900 2300 1900
NoConn ~ 2200 2400
NoConn ~ 2200 2500
NoConn ~ 2200 2600
NoConn ~ 2200 2700
NoConn ~ 2200 1400
NoConn ~ 2200 1500
Wire Wire Line
	1100 900  1200 900 
$Comp
L Memory_RAM:CY7C199 U5
U 1 1 5D37B8A5
P 8500 2900
F 0 "U5" H 8100 4000 50  0000 C CNN
F 1 "CY7C199" H 8800 4000 50  0000 C CNN
F 2 "" H 8500 2900 50  0001 C CNN
F 3 "" H 8500 2900 50  0001 C CNN
	1    8500 2900
	1    0    0    -1  
$EndComp
Text GLabel 9200 2000 2    50   BiDi ~ 0
D7
Wire Wire Line
	9100 2000 9200 2000
Text GLabel 9200 2100 2    50   BiDi ~ 0
D1
Wire Wire Line
	9100 2100 9200 2100
Text GLabel 9200 2200 2    50   BiDi ~ 0
D0
Wire Wire Line
	9200 2200 9100 2200
Text GLabel 9200 2300 2    50   BiDi ~ 0
D2
Wire Wire Line
	9200 2300 9100 2300
Text GLabel 9200 2400 2    50   BiDi ~ 0
D6
Wire Wire Line
	9200 2400 9100 2400
Text GLabel 9200 2500 2    50   BiDi ~ 0
D5
Wire Wire Line
	9100 2500 9200 2500
Text GLabel 9200 2600 2    50   BiDi ~ 0
D3
Wire Wire Line
	9100 2600 9200 2600
Text GLabel 9200 2700 2    50   BiDi ~ 0
D4
Wire Wire Line
	9200 2700 9100 2700
Text GLabel 7800 2400 0    50   Output ~ 0
A11
Wire Wire Line
	7800 2400 7900 2400
Text GLabel 7800 2300 0    50   Output ~ 0
A12
Text GLabel 7800 2200 0    50   Output ~ 0
A13
Text GLabel 7800 2100 0    50   Output ~ 0
A14
Wire Wire Line
	7900 2300 7800 2300
Wire Wire Line
	7800 2200 7900 2200
Wire Wire Line
	7900 2100 7800 2100
Text GLabel 7800 2000 0    50   Output ~ 0
A0
Wire Wire Line
	7900 2000 7800 2000
Text GLabel 7800 3400 0    50   Output ~ 0
A1
Text GLabel 7800 3300 0    50   Output ~ 0
A2
Text GLabel 7800 3200 0    50   Output ~ 0
A3
Text GLabel 7800 3100 0    50   Output ~ 0
A4
Text GLabel 7800 3000 0    50   Output ~ 0
A5
Text GLabel 7800 2900 0    50   Output ~ 0
A6
Text GLabel 7800 2800 0    50   Output ~ 0
A7
Text GLabel 7800 2700 0    50   Output ~ 0
A8
Text GLabel 7800 2600 0    50   Output ~ 0
A9
Text GLabel 7800 2500 0    50   Output ~ 0
A10
Wire Wire Line
	7800 3400 7900 3400
Wire Wire Line
	7900 3300 7800 3300
Wire Wire Line
	7800 3200 7900 3200
Wire Wire Line
	7900 3100 7800 3100
Wire Wire Line
	7800 3000 7900 3000
Wire Wire Line
	7900 2900 7800 2900
Wire Wire Line
	7800 2800 7900 2800
Wire Wire Line
	7900 2700 7800 2700
Wire Wire Line
	7800 2600 7900 2600
Wire Wire Line
	7900 2500 7800 2500
Text GLabel 7800 3550 0    50   Input ~ 0
~MEM_REQ
Text GLabel 7800 3700 0    50   Input ~ 0
~RAM_OE
Text GLabel 3250 3300 0    50   Output ~ 0
~WR
Wire Wire Line
	3300 3400 3600 3400
Text GLabel 7800 3850 0    50   Input ~ 0
~WR
Wire Wire Line
	7800 3700 7900 3700
Wire Wire Line
	7900 3600 7850 3600
Wire Wire Line
	7850 3600 7850 3550
Wire Wire Line
	7850 3550 7800 3550
Wire Wire Line
	7800 3850 7850 3850
Wire Wire Line
	7850 3850 7850 3800
Wire Wire Line
	7850 3800 7900 3800
$Comp
L power:GND #PWR0123
U 1 1 5D412AA9
P 8500 4200
F 0 "#PWR0123" H 8500 3950 50  0001 C CNN
F 1 "GND" H 8505 4027 50  0000 C CNN
F 2 "" H 8500 4200 50  0001 C CNN
F 3 "" H 8500 4200 50  0001 C CNN
	1    8500 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 4100 8500 4200
$Comp
L power:+5V #PWR0124
U 1 1 5D4197CC
P 8500 1600
F 0 "#PWR0124" H 8500 1450 50  0001 C CNN
F 1 "+5V" H 8515 1773 50  0000 C CNN
F 2 "" H 8500 1600 50  0001 C CNN
F 3 "" H 8500 1600 50  0001 C CNN
	1    8500 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 1600 8500 1700
NoConn ~ 3600 4000
NoConn ~ 3600 2900
NoConn ~ 3600 2700
NoConn ~ 3600 2600
Text GLabel 3500 3500 0    50   Output ~ 0
~MEM_REQ
Wire Wire Line
	3250 3300 3300 3300
Wire Wire Line
	3300 3300 3300 3400
Wire Wire Line
	3600 3500 3500 3500
$Comp
L power:+5V #PWR0125
U 1 1 5D457D7E
P 10600 800
F 0 "#PWR0125" H 10600 650 50  0001 C CNN
F 1 "+5V" H 10615 973 50  0000 C CNN
F 2 "" H 10600 800 50  0001 C CNN
F 3 "" H 10600 800 50  0001 C CNN
	1    10600 800 
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5D457DDB
P 10600 900
F 0 "#FLG0101" H 10600 975 50  0001 C CNN
F 1 "PWR_FLAG" H 10600 1073 50  0000 C CNN
F 2 "" H 10600 900 50  0001 C CNN
F 3 "~" H 10600 900 50  0001 C CNN
	1    10600 900 
	-1   0    0    1   
$EndComp
Wire Wire Line
	10600 900  10600 800 
NoConn ~ 5000 3100
Wire Wire Line
	3950 6800 4550 6800
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5D4751A2
P 10550 2300
F 0 "#FLG01" H 10550 2375 50  0001 C CNN
F 1 "PWR_FLAG" H 10550 2473 50  0000 C CNN
F 2 "" H 10550 2300 50  0001 C CNN
F 3 "~" H 10550 2300 50  0001 C CNN
	1    10550 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 2300 10550 2400
$Comp
L power:GND #PWR012
U 1 1 5D489F33
P 10550 2400
F 0 "#PWR012" H 10550 2150 50  0001 C CNN
F 1 "GND" H 10555 2227 50  0000 C CNN
F 2 "" H 10550 2400 50  0001 C CNN
F 3 "" H 10550 2400 50  0001 C CNN
	1    10550 2400
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS299 U1
U 1 1 5D2A7D49
P 1700 4500
F 0 "U1" H 1450 5150 50  0000 C CNN
F 1 "74LS299" H 1900 5150 50  0000 C CNN
F 2 "" H 1700 4500 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS299" H 1700 4500 50  0001 C CNN
	1    1700 4500
	1    0    0    -1  
$EndComp
Text GLabel 2300 4300 2    50   BiDi ~ 0
D0
Text GLabel 2300 4400 2    50   BiDi ~ 0
D1
Text GLabel 2300 4500 2    50   BiDi ~ 0
D2
Text GLabel 2300 4600 2    50   BiDi ~ 0
D3
Text GLabel 2300 4700 2    50   BiDi ~ 0
D4
Text GLabel 2300 4800 2    50   BiDi ~ 0
D5
Text GLabel 2300 4900 2    50   BiDi ~ 0
D6
Text GLabel 2300 5000 2    50   BiDi ~ 0
D7
Wire Wire Line
	2300 4300 2200 4300
Wire Wire Line
	2200 4400 2300 4400
Wire Wire Line
	2300 4500 2200 4500
Wire Wire Line
	2200 4600 2300 4600
Wire Wire Line
	2300 4700 2200 4700
Wire Wire Line
	2200 4800 2300 4800
Wire Wire Line
	2300 4900 2200 4900
Wire Wire Line
	2200 5000 2300 5000
Text GLabel 1100 4000 0    50   Input ~ 0
SR_SER
Text GLabel 2300 4100 2    50   Output ~ 0
SR_OUT
Wire Wire Line
	2200 4100 2300 4100
NoConn ~ 2200 4000
Wire Wire Line
	1100 4000 1200 4000
$Comp
L power:+5V #PWR04
U 1 1 5D2C9E11
P 1700 3600
F 0 "#PWR04" H 1700 3450 50  0001 C CNN
F 1 "+5V" H 1715 3773 50  0000 C CNN
F 2 "" H 1700 3600 50  0001 C CNN
F 3 "" H 1700 3600 50  0001 C CNN
	1    1700 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 3600 1700 3700
$Comp
L power:GND #PWR05
U 1 1 5D2D28C6
P 1700 5500
F 0 "#PWR05" H 1700 5250 50  0001 C CNN
F 1 "GND" H 1705 5327 50  0000 C CNN
F 2 "" H 1700 5500 50  0001 C CNN
F 3 "" H 1700 5500 50  0001 C CNN
	1    1700 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5400 1700 5500
$Comp
L power:+5V #PWR01
U 1 1 5D2DB444
P 1000 4300
F 0 "#PWR01" H 1000 4150 50  0001 C CNN
F 1 "+5V" H 1015 4473 50  0000 C CNN
F 2 "" H 1000 4300 50  0001 C CNN
F 3 "" H 1000 4300 50  0001 C CNN
	1    1000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 4300 1150 4300
Text GLabel 1100 4400 0    50   Input ~ 0
SR_MODE
Wire Wire Line
	1150 4300 1150 4100
Wire Wire Line
	1150 4100 1200 4100
Connection ~ 1150 4300
Wire Wire Line
	1150 4300 1200 4300
Wire Wire Line
	1100 4400 1200 4400
Text GLabel 1100 4600 0    50   Input ~ 0
~SR_OE
Wire Wire Line
	1100 4600 1200 4600
$Comp
L power:GND #PWR02
U 1 1 5D3095DE
P 1100 4750
F 0 "#PWR02" H 1100 4500 50  0001 C CNN
F 1 "GND" H 1105 4577 50  0000 C CNN
F 2 "" H 1100 4750 50  0001 C CNN
F 3 "" H 1100 4750 50  0001 C CNN
	1    1100 4750
	0    1    1    0   
$EndComp
Wire Wire Line
	1100 4750 1150 4750
Wire Wire Line
	1150 4750 1150 4700
Wire Wire Line
	1150 4700 1200 4700
Text GLabel 1100 4900 0    50   Input ~ 0
SR_CLK
Wire Wire Line
	1100 4900 1200 4900
$Comp
L power:+5V #PWR03
U 1 1 5D31DBB1
P 1100 5100
F 0 "#PWR03" H 1100 4950 50  0001 C CNN
F 1 "+5V" H 1115 5273 50  0000 C CNN
F 2 "" H 1100 5100 50  0001 C CNN
F 3 "" H 1100 5100 50  0001 C CNN
	1    1100 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1100 5100 1200 5100
Text GLabel 2300 2000 2    50   Input ~ 0
SR_OUT
Wire Wire Line
	2200 2000 2300 2000
$Comp
L power:+5V #PWR06
U 1 1 5D349977
P 8200 5100
F 0 "#PWR06" H 8200 4950 50  0001 C CNN
F 1 "+5V" H 8215 5273 50  0000 C CNN
F 2 "" H 8200 5100 50  0001 C CNN
F 3 "" H 8200 5100 50  0001 C CNN
	1    8200 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5D349996
P 8200 6100
F 0 "#PWR07" H 8200 5850 50  0001 C CNN
F 1 "GND" H 8205 5927 50  0000 C CNN
F 2 "" H 8200 6100 50  0001 C CNN
F 3 "" H 8200 6100 50  0001 C CNN
	1    8200 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 6000 8200 6100
Wire Wire Line
	8200 5100 8200 5200
Text GLabel 3100 3650 0    50   Output ~ 0
~IO_REQ
Wire Wire Line
	3600 3600 3550 3600
Wire Wire Line
	3550 3600 3550 3650
Wire Wire Line
	3550 3650 3500 3650
Text GLabel 9200 5800 0    50   Input ~ 0
~IO_REQ
Text GLabel 3500 2800 0    50   Input ~ 0
~WAIT
Text GLabel 10100 5900 2    50   Output ~ 0
~WAIT
Text GLabel 9200 5300 0    50   Input ~ 0
~IO_ACK
Text GLabel 2300 2300 2    50   Output ~ 0
~IO_ACK
Wire Wire Line
	2300 2300 2200 2300
NoConn ~ 2200 1700
Text GLabel 2300 1600 2    50   BiDi ~ 0
~IO_REQ
Wire Wire Line
	2300 1600 2200 1600
$Comp
L power:+5V #PWR08
U 1 1 5D3F14DC
P 9300 5700
F 0 "#PWR08" H 9300 5550 50  0001 C CNN
F 1 "+5V" H 9315 5873 50  0000 C CNN
F 2 "" H 9300 5700 50  0001 C CNN
F 3 "" H 9300 5700 50  0001 C CNN
	1    9300 5700
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS107 U6
U 1 1 5D403DD8
P 9700 5800
F 0 "U6" H 9700 6167 50  0000 C CNN
F 1 "74LS107" H 9700 6076 50  0000 C CNN
F 2 "" H 9700 5800 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS107" H 9700 5800 50  0001 C CNN
	1    9700 5800
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS107 U6
U 2 1 5D403E46
P 10600 5300
F 0 "U6" H 10600 5667 50  0000 C CNN
F 1 "74LS107" H 10600 5576 50  0000 C CNN
F 2 "" H 10600 5300 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS107" H 10600 5300 50  0001 C CNN
	2    10600 5300
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS107 U6
U 3 1 5D403EC1
P 8200 5600
F 0 "U6" H 8430 5646 50  0000 L CNN
F 1 "74LS107" H 8430 5555 50  0000 L CNN
F 2 "" H 8200 5600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS107" H 8200 5600 50  0001 C CNN
	3    8200 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5D403FBF
P 9300 5900
F 0 "#PWR09" H 9300 5650 50  0001 C CNN
F 1 "GND" H 9305 5727 50  0000 C CNN
F 2 "" H 9300 5900 50  0001 C CNN
F 3 "" H 9300 5900 50  0001 C CNN
	1    9300 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 5900 9400 5900
Wire Wire Line
	9400 5700 9300 5700
Wire Wire Line
	10100 5900 10000 5900
$Comp
L power:+5V #PWR010
U 1 1 5D41FA3B
P 10200 5200
F 0 "#PWR010" H 10200 5050 50  0001 C CNN
F 1 "+5V" H 10215 5373 50  0000 C CNN
F 2 "" H 10200 5200 50  0001 C CNN
F 3 "" H 10200 5200 50  0001 C CNN
	1    10200 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5D41FA41
P 10200 5400
F 0 "#PWR011" H 10200 5150 50  0001 C CNN
F 1 "GND" H 10205 5227 50  0000 C CNN
F 2 "" H 10200 5400 50  0001 C CNN
F 3 "" H 10200 5400 50  0001 C CNN
	1    10200 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5400 10300 5400
Wire Wire Line
	10300 5200 10200 5200
Wire Wire Line
	10600 5600 10600 5700
Wire Wire Line
	10600 5700 10000 5700
Wire Wire Line
	10900 5400 11000 5400
Wire Wire Line
	11000 5400 11000 6200
Wire Wire Line
	11000 6200 9700 6200
Wire Wire Line
	9700 6200 9700 6100
Wire Wire Line
	9200 5800 9400 5800
Wire Wire Line
	9200 5300 10300 5300
NoConn ~ 10900 5200
Text GLabel 6000 1400 0    50   Output ~ 0
~RST
$Comp
L Device:R R2
U 1 1 5D4A0E06
P 6250 1400
F 0 "R2" V 6043 1400 50  0000 C CNN
F 1 "47K" V 6134 1400 50  0000 C CNN
F 2 "" V 6180 1400 50  0001 C CNN
F 3 "~" H 6250 1400 50  0001 C CNN
	1    6250 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	6500 1400 6400 1400
Wire Wire Line
	6100 1400 6000 1400
$Comp
L power:GND #PWR013
U 1 1 5D4B4F94
P 6500 1400
F 0 "#PWR013" H 6500 1150 50  0001 C CNN
F 1 "GND" H 6505 1227 50  0000 C CNN
F 2 "" H 6500 1400 50  0001 C CNN
F 3 "" H 6500 1400 50  0001 C CNN
	1    6500 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5D2BCE88
P 6250 1900
F 0 "R1" V 6050 1900 50  0000 C CNN
F 1 "47K" V 6150 1900 50  0000 C CNN
F 2 "" V 6180 1900 50  0001 C CNN
F 3 "~" H 6250 1900 50  0001 C CNN
	1    6250 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 3650 3200 3650
Text GLabel 1100 6400 0    50   Input ~ 0
~IO_REQ
Text GLabel 6000 1900 0    50   Output ~ 0
~IO_REQ
Wire Wire Line
	6000 1900 6100 1900
Wire Wire Line
	6500 1900 6400 1900
$Comp
L power:+5V #PWR?
U 1 1 5D2FB87F
P 6500 1900
F 0 "#PWR?" H 6500 1750 50  0001 C CNN
F 1 "+5V" H 6515 2073 50  0000 C CNN
F 2 "" H 6500 1900 50  0001 C CNN
F 3 "" H 6500 1900 50  0001 C CNN
	1    6500 1900
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4148 D?
U 1 1 5D2FBB80
P 3350 3650
F 0 "D?" H 3200 3750 50  0000 C CNN
F 1 "1N4148" H 3450 3750 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 3350 3475 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/1N4148_1N4448.pdf" H 3350 3650 50  0001 C CNN
	1    3350 3650
	-1   0    0    1   
$EndComp
NoConn ~ 2200 1800
Text GLabel 900  2300 0    50   Input ~ 0
~WR
Text GLabel 700  2200 0    50   Input ~ 0
A0
Wire Wire Line
	900  2300 1200 2300
Wire Wire Line
	1200 2200 700  2200
$EndSCHEMATC
