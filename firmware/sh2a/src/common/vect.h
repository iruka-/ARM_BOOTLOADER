/******************************************************************************
*   DISCLAIMER
*
*   This software is supplied by Renesas Technology Corp. and is only 
*   intended for use with Renesas products. No other uses are authorized.
*
*   This software is owned by Renesas Technology Corp. and is protected under 
*   all applicable laws, including copyright laws.
*
*   THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
*   REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
*   INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
*   PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
*   DISCLAIMED.
*
*   TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
*   TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
*   FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
*   FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
*   AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
*   Renesas reserves the right, without notice, to make changes to this 
*   software and to discontinue the availability of this software.
*   By using this software, you agree to the additional terms and 
*   conditions found by accessing the following link: 
*   http://www.renesas.com/disclaimer
********************************************************************************
*   Copyright (C) 2009. Renesas Technology Corp., All Rights Reserved.
*""FILE COMMENT""*********** Technical reference data **************************
*   System Name : SH7264 Sample Program
*   File Name   : vect.h
*   Abstract    : interrupt function define
*   Version     : 1.02.00
*   Device      : SH7262/SH7264
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.02 Release00).
*   OS          : None
*   H/W Platform: M3A-HS64G50(CPU board)
*   Description : 
********************************************************************************
*   History     : Mar.11,2009 Ver.1.00.00  
*               : Jun.29,2009 Ver.1.01.00 Changed FILE FORMAT
*               : Nov.05,2009 Ver.1.02.00 Corrected typographical error
*               :                              "User Vecter" -> "User Vector"
*""FILE COMMENT END""**********************************************************/
#ifndef VECT_H
#define VECT_H

// for GCC
#ifdef	USE_RESBANK
#define	_INTERRUPT_ __attribute__ ((interrupt_handler,resbank))
#else
#define	_INTERRUPT_ __attribute__ ((interrupt_handler))
#endif

//;<<VECTOR DATA START (POWER ON RESET)>>
//;0 Power On Reset PC
extern void PowerON_Reset_PC(void);

//;<<VECTOR DATA END (POWER ON RESET)>>
// 1 Power On Reset SP

//;<<VECTOR DATA START (MANUAL RESET)>>
//;2 Manual Reset PC
extern void Manual_Reset_PC(void);

//;<<VECTOR DATA END (MANUAL RESET)>>
// 3 Manual Reset SP

// 4 Illegal code
#pragma interrupt INT_Illegal_code
extern void INT_Illegal_code(void) _INTERRUPT_;

// 5 Reserved

// 6 Illegal slot
#pragma interrupt INT_Illegal_slot
extern void INT_Illegal_slot(void) _INTERRUPT_;

// 7 Reserved

// 8 Reserved

// 9 CPU Address error
#pragma interrupt INT_CPU_Address
extern void INT_CPU_Address(void) _INTERRUPT_;

// 10 BUS error
#pragma interrupt INT_DMAC_Address
extern void INT_DMAC_Address(void) _INTERRUPT_;

// 11 NMI
#pragma interrupt INT_NMI
extern void INT_NMI(void) _INTERRUPT_;

// 12 Reserved

// 13 INT_FPU
#pragma interrupt INT_FPU
extern void INT_FPU(void) _INTERRUPT_;

// 14 H-UDI
#pragma interrupt INT_HUDI
extern void INT_HUDI(void) _INTERRUPT_;

// 15 Bank Overflow
#pragma interrupt INT_Bank_Overflow
extern void INT_Bank_Overflow(void) _INTERRUPT_;

// 16 Bank Underflow
#pragma interrupt INT_Bank_Underflow
extern void INT_Bank_Underflow(void) _INTERRUPT_;

// 17 Divide by zero
#pragma interrupt INT_Divide_by_Zero
extern void INT_Divide_by_Zero(void) _INTERRUPT_;

// 18 Divide Overflow
#pragma interrupt INT_Divide_Overflow
extern void INT_Divide_Overflow(void) _INTERRUPT_;

// 19 Reserved

// 20 Reserved

// 21 Reserved

// 22 Reserved

// 23 Reserved

// 24 Reserved

// 25 Reserved

// 26 Reserved

// 27 Reserved

// 28 Reserved

// 29 Reserved

// 30 Reserved

// 31 Reserved

// 32 TRAPA (User Vector)
#pragma interrupt INT_TRAPA32
extern void INT_TRAPA32(void) _INTERRUPT_;

// 33 TRAPA (User Vector)
#pragma interrupt INT_TRAPA33
extern void INT_TRAPA33(void) _INTERRUPT_;

// 34 TRAPA (User Vector)
#pragma interrupt INT_TRAPA34
extern void INT_TRAPA34(void) _INTERRUPT_;

// 35 TRAPA (User Vector)
#pragma interrupt INT_TRAPA35
extern void INT_TRAPA35(void) _INTERRUPT_;

// 36 TRAPA (User Vector)
#pragma interrupt INT_TRAPA36
extern void INT_TRAPA36(void) _INTERRUPT_;

// 37 TRAPA (User Vector)
#pragma interrupt INT_TRAPA37
extern void INT_TRAPA37(void) _INTERRUPT_;

// 38 TRAPA (User Vector)
#pragma interrupt INT_TRAPA38
extern void INT_TRAPA38(void) _INTERRUPT_;

// 39 TRAPA (User Vector)
#pragma interrupt INT_TRAPA39
extern void INT_TRAPA39(void) _INTERRUPT_;

// 40 TRAPA (User Vector)
#pragma interrupt INT_TRAPA40
extern void INT_TRAPA40(void) _INTERRUPT_;

// 41 TRAPA (User Vector)
#pragma interrupt INT_TRAPA41
extern void INT_TRAPA41(void) _INTERRUPT_;

// 42 TRAPA (User Vector)
#pragma interrupt INT_TRAPA42
extern void INT_TRAPA42(void) _INTERRUPT_;

// 43 TRAPA (User Vector)
#pragma interrupt INT_TRAPA43
extern void INT_TRAPA43(void) _INTERRUPT_;

// 44 TRAPA (User Vector)
#pragma interrupt INT_TRAPA44
extern void INT_TRAPA44(void) _INTERRUPT_;

// 45 TRAPA (User Vector)
#pragma interrupt INT_TRAPA45
extern void INT_TRAPA45(void) _INTERRUPT_;

// 46 TRAPA (User Vector)
#pragma interrupt INT_TRAPA46
extern void INT_TRAPA46(void) _INTERRUPT_;

// 47 TRAPA (User Vector)
#pragma interrupt INT_TRAPA47
extern void INT_TRAPA47(void) _INTERRUPT_;

// 48 TRAPA (User Vector)
#pragma interrupt INT_TRAPA48
extern void INT_TRAPA48(void) _INTERRUPT_;

// 49 TRAPA (User Vector)
#pragma interrupt INT_TRAPA49
extern void INT_TRAPA49(void) _INTERRUPT_;

// 50 TRAPA (User Vector)
#pragma interrupt INT_TRAPA50
extern void INT_TRAPA50(void) _INTERRUPT_;

// 51 TRAPA (User Vector)
#pragma interrupt INT_TRAPA51
extern void INT_TRAPA51(void) _INTERRUPT_;

// 52 TRAPA (User Vector)
#pragma interrupt INT_TRAPA52
extern void INT_TRAPA52(void) _INTERRUPT_;

// 53 TRAPA (User Vector)
#pragma interrupt INT_TRAPA53
extern void INT_TRAPA53(void) _INTERRUPT_;

// 54 TRAPA (User Vector)
#pragma interrupt INT_TRAPA54
extern void INT_TRAPA54(void) _INTERRUPT_;

// 55 TRAPA (User Vector)
#pragma interrupt INT_TRAPA55
extern void INT_TRAPA55(void) _INTERRUPT_;

// 56 TRAPA (User Vector)
#pragma interrupt INT_TRAPA56
extern void INT_TRAPA56(void) _INTERRUPT_;

// 57 TRAPA (User Vector)
#pragma interrupt INT_TRAPA57
extern void INT_TRAPA57(void) _INTERRUPT_;

// 58 TRAPA (User Vector)
#pragma interrupt INT_TRAPA58
extern void INT_TRAPA58(void) _INTERRUPT_;

// 59 TRAPA (User Vector)
#pragma interrupt INT_TRAPA59
extern void INT_TRAPA59(void) _INTERRUPT_;

// 60 TRAPA (User Vector)
#pragma interrupt INT_TRAPA60
extern void INT_TRAPA60(void) _INTERRUPT_;

// 61 TRAPA (User Vector)
#pragma interrupt INT_TRAPA61
extern void INT_TRAPA61(void) _INTERRUPT_;

// 62 TRAPA (User Vector)
#pragma interrupt INT_TRAPA62
extern void INT_TRAPA62(void) _INTERRUPT_;

// 63 TRAPA (User Vector)
#pragma interrupt INT_TRAPA63
extern void INT_TRAPA63(void) _INTERRUPT_;

// 64 Interrupt IRQ0
#pragma interrupt INT_IRQ_IRQ0(resbank)
extern void INT_IRQ_IRQ0(void) _INTERRUPT_;

// 65 Interrupt IRQ1
#pragma interrupt INT_IRQ_IRQ1(resbank)
extern void INT_IRQ_IRQ1(void) _INTERRUPT_;

// 66 Interrupt IRQ2
#pragma interrupt INT_IRQ_IRQ2(resbank)
extern void INT_IRQ_IRQ2(void) _INTERRUPT_;

// 67 Interrupt IRQ3
#pragma interrupt INT_IRQ_IRQ3(resbank)
extern void INT_IRQ_IRQ3(void) _INTERRUPT_;

// 68 Interrupt IRQ4
#pragma interrupt INT_IRQ_IRQ4(resbank)
extern void INT_IRQ_IRQ4(void) _INTERRUPT_;

// 69 Interrupt IRQ5
#pragma interrupt INT_IRQ_IRQ5(resbank)
extern void INT_IRQ_IRQ5(void) _INTERRUPT_;

// 70 Interrupt IRQ6
#pragma interrupt INT_IRQ_IRQ6(resbank)
extern void INT_IRQ_IRQ6(void) _INTERRUPT_;

// 71 Interrupt IRQ7
#pragma interrupt INT_IRQ_IRQ7(resbank)
extern void INT_IRQ_IRQ7(void) _INTERRUPT_;

// 72 Reserved

// 73 Reserved

// 74 Reserved

// 75 Reserved

// 76 Reserved

// 77 Reserved

// 78 Reserved

// 79 Reserved

// 80 Interrupt PINT0
#pragma interrupt INT_PINT_PINT0(resbank)
extern void INT_PINT_PINT0(void) _INTERRUPT_;

// 81 Interrupt PINT1
#pragma interrupt INT_PINT_PINT1(resbank)
extern void INT_PINT_PINT1(void) _INTERRUPT_;

// 82 Interrupt PINT2
#pragma interrupt INT_PINT_PINT2(resbank)
extern void INT_PINT_PINT2(void) _INTERRUPT_;

// 83 Interrupt PINT3
#pragma interrupt INT_PINT_PINT3(resbank)
extern void INT_PINT_PINT3(void) _INTERRUPT_;

// 84 Interrupt PINT4
#pragma interrupt INT_PINT_PINT4(resbank)
extern void INT_PINT_PINT4(void) _INTERRUPT_;

// 85 Interrupt PINT5
#pragma interrupt INT_PINT_PINT5(resbank)
extern void INT_PINT_PINT5(void) _INTERRUPT_;

// 86 Interrupt PINT6
#pragma interrupt INT_PINT_PINT6(resbank)
extern void INT_PINT_PINT6(void) _INTERRUPT_;

// 87 Interrupt PINT7
#pragma interrupt INT_PINT_PINT7(resbank)
extern void INT_PINT_PINT7(void) _INTERRUPT_;

// 88 Reserved

// 89 Reserved

// 90 Reserved

// 91 Reserved

// 92 Reserved

// 93 Reserved

// 94 Reserved

// 95 Reserved

// 96 Reserved

// 97 Reserved

// 98 Reserved

// 99 Reserved

// 100 Reserved

// 101 Reserved

// 102 Reserved

// 103 Reserved

// 104 Reserved

// 105 Reserved

// 106 Reserved

// 107 Reserved

// 108 DMAC0 DEI0
#pragma interrupt INT_DMAC0_DEI0(resbank)
extern void INT_DMAC0_DEI0(void) _INTERRUPT_;

// 109 DMAC0 HEI0
#pragma interrupt INT_DMAC0_HEI0(resbank)
extern void INT_DMAC0_HEI0(void) _INTERRUPT_;

// 110 Reserved

// 111 Reserved

// 112 DMAC1 DEI1
#pragma interrupt INT_DMAC1_DEI1(resbank)
extern void INT_DMAC1_DEI1(void) _INTERRUPT_;

// 113 DMAC1 HEI1
#pragma interrupt INT_DMAC1_HEI1(resbank)
extern void INT_DMAC1_HEI1(void) _INTERRUPT_;

// 114 Reserved

// 115 Reserved

// 116 DMAC2 DEI2
#pragma interrupt INT_DMAC2_DEI2(resbank)
extern void INT_DMAC2_DEI2(void) _INTERRUPT_;

// 117 DMAC2 HEI2
#pragma interrupt INT_DMAC2_HEI2(resbank)
extern void INT_DMAC2_HEI2(void) _INTERRUPT_;

// 118 Reserved

// 119 Reserved

// 120 DMAC3 DEI3
#pragma interrupt INT_DMAC3_DEI3(resbank)
extern void INT_DMAC3_DEI3(void) _INTERRUPT_;

// 121 DMAC3 HEI3
#pragma interrupt INT_DMAC3_HEI3(resbank)
extern void INT_DMAC3_HEI3(void) _INTERRUPT_;

// 122 Reserved

// 123 Reserved

// 124 DMAC4 DEI4
#pragma interrupt INT_DMAC4_DEI4(resbank)
extern void INT_DMAC4_DEI4(void) _INTERRUPT_;

// 125 DMAC4 HEI4
#pragma interrupt INT_DMAC4_HEI4(resbank)
extern void INT_DMAC4_HEI4(void) _INTERRUPT_;

// 126 Reserved

// 127 Reserved

// 128 DMAC5 DEI5
#pragma interrupt INT_DMAC5_DEI5(resbank)
extern void INT_DMAC5_DEI5(void) _INTERRUPT_;

// 129 DMAC5 HEI5
#pragma interrupt INT_DMAC5_HEI5(resbank)
extern void INT_DMAC5_HEI5(void) _INTERRUPT_;

// 130 Reserved

// 131 Reserved

// 132 DMAC6 DEI6
#pragma interrupt INT_DMAC6_DEI6(resbank)
extern void INT_DMAC6_DEI6(void) _INTERRUPT_;

// 133 DMAC6 HEI6
#pragma interrupt INT_DMAC6_HEI6(resbank)
extern void INT_DMAC6_HEI6(void) _INTERRUPT_;

// 134 Reserved

// 135 Reserved

// 136 DMAC7 DEI7
#pragma interrupt INT_DMAC7_DEI7(resbank)
extern void INT_DMAC7_DEI7(void) _INTERRUPT_;

// 137 DMAC7 HEI7
#pragma interrupt INT_DMAC7_HEI7(resbank)
extern void INT_DMAC7_HEI7(void) _INTERRUPT_;

// 138 Reserved

// 139 Reserved

// 140 DMAC8 DEI8
#pragma interrupt INT_DMAC8_DEI8(resbank)
extern void INT_DMAC8_DEI8(void) _INTERRUPT_;

// 141 DMAC8 HEI8
#pragma interrupt INT_DMAC8_HEI8(resbank)
extern void INT_DMAC8_HEI8(void) _INTERRUPT_;

// 142 Reserved

// 143 Reserved

// 144 DMAC9 DEI9
#pragma interrupt INT_DMAC9_DEI9(resbank)
extern void INT_DMAC9_DEI9(void) _INTERRUPT_;

// 145 DMAC9 HEI9
#pragma interrupt INT_DMAC9_HEI9(resbank)
extern void INT_DMAC9_HEI9(void) _INTERRUPT_;

// 146 Reserved

// 147 Reserved

// 148 DMAC10 DEI10
#pragma interrupt INT_DMAC10_DEI10(resbank)
extern void INT_DMAC10_DEI10(void) _INTERRUPT_;

// 149 DMAC10 HEI10
#pragma interrupt INT_DMAC10_HEI10(resbank)
extern void INT_DMAC10_HEI10(void) _INTERRUPT_;

// 150 Reserved

// 151 Reserved

// 152 DMAC11 DEI11
#pragma interrupt INT_DMAC11_DEI11(resbank)
extern void INT_DMAC11_DEI11(void) _INTERRUPT_;

// 153 DMAC11 HEI11
#pragma interrupt INT_DMAC11_HEI11(resbank)
extern void INT_DMAC11_HEI11(void) _INTERRUPT_;

// 154 Reserved

// 155 Reserved

// 156 DMAC12 DEI12
#pragma interrupt INT_DMAC12_DEI12(resbank)
extern void INT_DMAC12_DEI12(void) _INTERRUPT_;

// 157 DMAC12 HEI12
#pragma interrupt INT_DMAC12_HEI12(resbank)
extern void INT_DMAC12_HEI12(void) _INTERRUPT_;

// 158 Reserved

// 159 Reserved

// 160 DMAC13 DEI13
#pragma interrupt INT_DMAC13_DEI13(resbank)
extern void INT_DMAC13_DEI13(void) _INTERRUPT_;

// 161 DMAC13 HEI13
#pragma interrupt INT_DMAC13_HEI13(resbank)
extern void INT_DMAC13_HEI13(void) _INTERRUPT_;

// 162 Reserved

// 163 Reserved

// 164 DMAC14 DEI14
#pragma interrupt INT_DMAC14_DEI14(resbank)
extern void INT_DMAC14_DEI14(void) _INTERRUPT_;

// 165 DMAC14 HEI14
#pragma interrupt INT_DMAC14_HEI14(resbank)
extern void INT_DMAC14_HEI14(void) _INTERRUPT_;

// 166 Reserved

// 167 Reserved

// 168 DMAC15 DEI15
#pragma interrupt INT_DMAC15_DEI15(resbank)
extern void INT_DMAC15_DEI15(void) _INTERRUPT_;

// 169 DMAC15 HEI15
#pragma interrupt INT_DMAC15_HEI15(resbank)
extern void INT_DMAC15_HEI15(void) _INTERRUPT_;

// 170 USB USBI
#pragma interrupt INT_USB_USBI(resbank)
extern void INT_USB_USBI(void) _INTERRUPT_;

// 171 VDC3 VIVSYNCJ
#pragma interrupt INT_VDC3_VIVSYNCJ(resbank)
extern void INT_VDC3_VIVSYNCJ(void) _INTERRUPT_;

// 172 VDC3 VBUFERRJ
#pragma interrupt INT_VDC3_VBUFERR(resbank)
extern void INT_VDC3_VBUFERR(void) _INTERRUPT_;

// 173 VDC3 VIFIELDE
#pragma interrupt INT_VDC3_VIFIELDE(resbank)
extern void INT_VDC3_VIFIELDE(void) _INTERRUPT_;

// 174 VDC3 VOLINE
#pragma interrupt INT_VDC3_VOLINE(resbank)
extern void INT_VDC3_VOLINE(void) _INTERRUPT_;

// 175 CMT CMT0 CMI0
#pragma interrupt INT_CMT0_CMI0(resbank)
extern void INT_CMT0_CMI0(void) _INTERRUPT_;

// 176 CMT CMT1 CMI1
#pragma interrupt INT_CMT1_CMI1(resbank)
extern void INT_CMT1_CMI1(void) _INTERRUPT_;

// 177 BSC CMI
#pragma interrupt INT_BSC_CMI(resbank)
extern void INT_BSC_CMI(void) _INTERRUPT_;

// 178 WDT
#pragma interrupt INT_WDT_ITI(resbank)
extern void INT_WDT_ITI(void) _INTERRUPT_;

// 179 MTU2 MTU0 TGI0A
#pragma interrupt INT_MTU2_TGI0A(resbank)
extern void INT_MTU2_TGI0A(void) _INTERRUPT_;

// 180 MTU2 MTU0 TGI0B
#pragma interrupt INT_MTU2_TGI0B(resbank)
extern void INT_MTU2_TGI0B(void) _INTERRUPT_;

// 181 MTU2 MTU0 TGI0C
#pragma interrupt INT_MTU2_TGI0C(resbank)
extern void INT_MTU2_TGI0C(void) _INTERRUPT_;

// 182 MTU2 MTU0 TGI0D
#pragma interrupt INT_MTU2_TGI0D(resbank)
extern void INT_MTU2_TGI0D(void) _INTERRUPT_;

// 183 MTU2 MTU0 TCI0V
#pragma interrupt INT_MTU2_TCI0V(resbank)
extern void INT_MTU2_TCI0V(void) _INTERRUPT_;

// 184 MTU2 MTU0 TGI0E
#pragma interrupt INT_MTU2_TGI0E(resbank)
extern void INT_MTU2_TGI0E(void) _INTERRUPT_;

// 185 MTU2 MTU0 TGI0F
#pragma interrupt INT_MTU2_TGI0F(resbank)
extern void INT_MTU2_TGI0F(void) _INTERRUPT_;

// 186 MTU2 MTU1 TGI1A
#pragma interrupt INT_MTU2_TGI1A(resbank)
extern void INT_MTU2_TGI1A(void) _INTERRUPT_;

// 187 MTU2 MTU1 TGI1B
#pragma interrupt INT_MTU2_TGI1B(resbank)
extern void INT_MTU2_TGI1B(void) _INTERRUPT_;

// 188 MTU2 MTU1 TCI1V
#pragma interrupt INT_MTU2_TCI1V(resbank)
extern void INT_MTU2_TCI1V(void) _INTERRUPT_;

// 189 MTU2 MTU1 TCI1U
#pragma interrupt INT_MTU2_TCI1U(resbank)
extern void INT_MTU2_TCI1U(void) _INTERRUPT_;

// 190 MTU2 MTU2 TGI2A
#pragma interrupt INT_MTU2_TGI2A(resbank)
extern void INT_MTU2_TGI2A(void) _INTERRUPT_;

// 191 MTU2 MTU2 TGI2B
#pragma interrupt INT_MTU2_TGI2B(resbank)
extern void INT_MTU2_TGI2B(void) _INTERRUPT_;

// 192 MTU2 MTU2 TCI2V
#pragma interrupt INT_MTU2_TCI2V(resbank)
extern void INT_MTU2_TCI2V(void) _INTERRUPT_;

// 193 MTU2 MTU2 TCI2U
#pragma interrupt INT_MTU2_TCI2U(resbank)
extern void INT_MTU2_TCI2U(void) _INTERRUPT_;

// 194 MTU2 MTU3 TGI3A
#pragma interrupt INT_MTU2_TGI3A(resbank)
extern void INT_MTU2_TGI3A(void) _INTERRUPT_;

// 195 MTU2 MTU3 TGI3B
#pragma interrupt INT_MTU2_TGI3B(resbank)
extern void INT_MTU2_TGI3B(void) _INTERRUPT_;

// 196 MTU2 MTU3 TGI3C
#pragma interrupt INT_MTU2_TGI3C(resbank)
extern void INT_MTU2_TGI3C(void) _INTERRUPT_;

// 197 MTU2 MTU3 TGI3D
#pragma interrupt INT_MTU2_TGI3D(resbank)
extern void INT_MTU2_TGI3D(void) _INTERRUPT_;

// 198 MTU2 MTU3 TCI3V
#pragma interrupt INT_MTU2_TCI3V(resbank)
extern void INT_MTU2_TCI3V(void) _INTERRUPT_;

// 199 MTU2 MTU4 TGI4A
#pragma interrupt INT_MTU2_TGI4A(resbank)
extern void INT_MTU2_TGI4A(void);

// 200 MTU2 MTU4 TGI4B
#pragma interrupt INT_MTU2_TGI4B(resbank)
extern void INT_MTU2_TGI4B(void) _INTERRUPT_;

// 201 MTU2 MTU4 TGI4C
#pragma interrupt INT_MTU2_TGI4C(resbank)
extern void INT_MTU2_TGI4C(void) _INTERRUPT_;

// 202 MTU2 MTU4 TGI4D
#pragma interrupt INT_MTU2_TGI4D(resbank)
extern void INT_MTU2_TGI4D(void) _INTERRUPT_;

// 203 MTU2 MTU4 TCI4V
#pragma interrupt INT_MTU2_TCI4V(resbank)
extern void INT_MTU2_TCI4V(void) _INTERRUPT_;

// 204 PWM PWM1 
#pragma interrupt INT_PWM1(resbank)
extern void INT_PWM1(void) _INTERRUPT_;

// 205 PWM PWM2
#pragma interrupt INT_PWM2(resbank)
extern void INT_PWM2(void) _INTERRUPT_;

// 206 A/D ADI
#pragma interrupt INT_AD_ADI(resbank)
extern void INT_AD_ADI(void) _INTERRUPT_;

// 207 SSI0 SSIF0
#pragma interrupt INT_SSI0_SSIF0(resbank)
extern void INT_SSI0_SSIF0(void) _INTERRUPT_;

// 208 SSI0 SSIRXI0
#pragma interrupt INT_SSI0_SSIRXI0(resbank)
extern void INT_SSI0_SSIRXI0(void) _INTERRUPT_;

// 209 SSI0 SSITXI0
#pragma interrupt INT_SSI0_SSITXI0(resbank)
extern void INT_SSI0_SSITXI0(void) _INTERRUPT_;

// 210 SSI1 SSII1
#pragma interrupt INT_SSI1_SSII1(resbank)
extern void INT_SSI1_SSII1(void) _INTERRUPT_;

// 211 SSI1 SSIRTI1
#pragma interrupt INT_SSI1_SSIRTI1(resbank)
extern void INT_SSI1_SSIRTI1(void) _INTERRUPT_;

// 212 SSI2 SSII2
#pragma interrupt INT_SSI2_SSII2(resbank)
extern void INT_SSI2_SSII2(void) _INTERRUPT_;

// 213 SSI2 SSIRTI2
#pragma interrupt INT_SSI2_SSIRTI2(resbank)
extern void INT_SSI2_SSIRTI2(void) _INTERRUPT_;

// 214 SSI3 SSII3
#pragma interrupt INT_SSI3_SSII3(resbank)
extern void INT_SSI3_SSII3(void) _INTERRUPT_;

// 215 SSI3 SSIRTI3
#pragma interrupt INT_SSI3_SSIRTI3(resbank)
extern void INT_SSI3_SSIRTI3(void) _INTERRUPT_;

// 216 SPDIF SPDIFI
#pragma interrupt INT_SPDIF_SPDIFI(resbank)
extern void INT_SPDIF_SPDIFI(void) _INTERRUPT_;

// 217 IIC3 IIC0 STPI0
#pragma interrupt INT_IIC3_STPI0(resbank)
extern void INT_IIC3_STPI0(void) _INTERRUPT_;

// 218 IIC3 IIC0 NAKI0
#pragma interrupt INT_IIC3_NAKI0(resbank)
extern void INT_IIC3_NAKI0(void) _INTERRUPT_;

// 219 IIC3 IIC0 RXI0
#pragma interrupt INT_IIC3_RXI0(resbank)
extern void INT_IIC3_RXI0(void) _INTERRUPT_;

// 220 IIC3 IIC0 TXI0
#pragma interrupt INT_IIC3_TXI0(resbank)
extern void INT_IIC3_TXI0(void) _INTERRUPT_;

// 221 IIC3 IIC0 TEI0
#pragma interrupt INT_IIC3_TEI0(resbank)
extern void INT_IIC3_TEI0(void) _INTERRUPT_;

// 222 IIC3 IIC1 STPI1
#pragma interrupt INT_IIC3_STPI1(resbank)
extern void INT_IIC3_STPI1(void) _INTERRUPT_;

// 223 IIC3 IIC1 NAKI1
#pragma interrupt INT_IIC3_NAKI1(resbank)
extern void INT_IIC3_NAKI1(void) _INTERRUPT_;

// 224 IIC3 IIC1 RXI1
#pragma interrupt INT_IIC3_RXI1(resbank)
extern void INT_IIC3_RXI1(void) _INTERRUPT_;

// 225 IIC3 IIC1 TXI1
#pragma interrupt INT_IIC3_TXI1(resbank)
extern void INT_IIC3_TXI1(void) _INTERRUPT_;

// 226 IIC3 IIC1 TEI1
#pragma interrupt INT_IIC3_TEI1(resbank)
extern void INT_IIC3_TEI1(void) _INTERRUPT_;

// 227 IIC3 IIC2 STPI2
#pragma interrupt INT_IIC3_STPI2(resbank)
extern void INT_IIC3_STPI2(void) _INTERRUPT_;

// 228 IIC3 IIC2 NAKI2
#pragma interrupt INT_IIC3_NAKI2(resbank)
extern void INT_IIC3_NAKI2(void) _INTERRUPT_;

// 229 IIC3 IIC2 RXI2
#pragma interrupt INT_IIC3_RXI2(resbank)
extern void INT_IIC3_RXI2(void) _INTERRUPT_;

// 230 IIC3 IIC2 TXI2
#pragma interrupt INT_IIC3_TXI2(resbank)
extern void INT_IIC3_TXI2(void) _INTERRUPT_;

// 231 IIC3 IIC2 TEI2
#pragma interrupt INT_IIC3_TEI2(resbank)
extern void INT_IIC3_TEI2(void) _INTERRUPT_;

// 232 SCIF SCIF0 BRI0
#pragma interrupt INT_SCIF0_BRI0(resbank)
extern void INT_SCIF0_BRI0(void) _INTERRUPT_;

// 233 SCIF SCIF0 ERI0
#pragma interrupt INT_SCIF0_ERI0(resbank)
extern void INT_SCIF0_ERI0(void) _INTERRUPT_;

// 234 SCIF SCIF0 RXI0
#pragma interrupt INT_SCIF0_RXI0(resbank)
extern void INT_SCIF0_RXI0(void) _INTERRUPT_;

// 235 SCIF SCIF0 TXI0
#pragma interrupt INT_SCIF0_TXI0(resbank)
extern void INT_SCIF0_TXI0(void) _INTERRUPT_;

// 236 SCIF SCIF1 BRI1
#pragma interrupt INT_SCIF1_BRI1(resbank)
extern void INT_SCIF1_BRI1(void) _INTERRUPT_;

// 237 SCIF SCIF1 ERI1
#pragma interrupt INT_SCIF1_ERI1(resbank)
extern void INT_SCIF1_ERI1(void) _INTERRUPT_;

// 238 SCIF SCIF1 RXI1
#pragma interrupt INT_SCIF1_RXI1(resbank)
extern void INT_SCIF1_RXI1(void) _INTERRUPT_;

// 239 SCIF SCIF1 TXI1
#pragma interrupt INT_SCIF1_TXI1(resbank)
extern void INT_SCIF1_TXI1(void) _INTERRUPT_;

// 240 SCIF SCIF2 BRI2
#pragma interrupt INT_SCIF2_BRI2(resbank)
extern void INT_SCIF2_BRI2(void) _INTERRUPT_;

// 241 SCIF SCIF2 ERI2
#pragma interrupt INT_SCIF2_ERI2(resbank)
extern void INT_SCIF2_ERI2(void) _INTERRUPT_;

// 242 SCIF SCIF2 RXI2
#pragma interrupt INT_SCIF2_RXI2(resbank)
extern void INT_SCIF2_RXI2(void) _INTERRUPT_;

// 243 SCIF SCIF2 TXI2
#pragma interrupt INT_SCIF2_TXI2(resbank)
extern void INT_SCIF2_TXI2(void) _INTERRUPT_;

// 244 SCIF SCIF3 BRI3
#pragma interrupt INT_SCIF3_BRI3(resbank)
extern void INT_SCIF3_BRI3(void) _INTERRUPT_;

// 245 SCIF SCIF3 ERI3
#pragma interrupt INT_SCIF3_ERI3(resbank)
extern void INT_SCIF3_ERI3(void) _INTERRUPT_;

// 246 SCIF SCIF3 RXI3
#pragma interrupt INT_SCIF3_RXI3(resbank)
extern void INT_SCIF3_RXI3(void) _INTERRUPT_;

// 247 SCIF SCIF3 TXI3
#pragma interrupt INT_SCIF3_TXI3(resbank)
extern void INT_SCIF3_TXI3(void) _INTERRUPT_;

// 248 SCIF SCIF4 BRI4
#pragma interrupt INT_SCIF4_BRI4(resbank)
extern void INT_SCIF4_BRI4(void) _INTERRUPT_;

// 249 SCIF SCIF4 ERI4
#pragma interrupt INT_SCIF4_ERI4(resbank)
extern void INT_SCIF4_ERI4(void) _INTERRUPT_;

// 250 SCIF SCIF4 RXI4
#pragma interrupt INT_SCIF4_RXI4(resbank)
extern void INT_SCIF4_RXI4(void) _INTERRUPT_;

// 251 SCIF SCIF4 TXI4
#pragma interrupt INT_SCIF4_TXI4(resbank)
extern void INT_SCIF4_TXI4(void) _INTERRUPT_;

// 252 SCIF SCIF5 BRI5
#pragma interrupt INT_SCIF5_BRI5(resbank)
extern void INT_SCIF5_BRI5(void) _INTERRUPT_;

// 253 SCIF SCIF5 ERI5
#pragma interrupt INT_SCIF5_ERI5(resbank)
extern void INT_SCIF5_ERI5(void) _INTERRUPT_;

// 254 SCIF SCIF5 RXI5
#pragma interrupt INT_SCIF5_RXI5(resbank)
extern void INT_SCIF5_RXI5(void) _INTERRUPT_;

// 255 SCIF SCIF5 TXI5
#pragma interrupt INT_SCIF5_TXI5(resbank)
extern void INT_SCIF5_TXI5(void) _INTERRUPT_;

// 256 SCIF SCIF6 BRI6
#pragma interrupt INT_SCIF6_BRI6(resbank)
extern void INT_SCIF6_BRI6(void) _INTERRUPT_;

// 257 SCIF SCIF6 ERI6
#pragma interrupt INT_SCIF6_ERI6(resbank)
extern void INT_SCIF6_ERI6(void) _INTERRUPT_;

// 258 SCIF SCIF6 RXI6
#pragma interrupt INT_SCIF6_RXI6(resbank)
extern void INT_SCIF6_RXI6(void) _INTERRUPT_;

// 259 SCIF SCIF6 TXI6
#pragma interrupt INT_SCIF6_TXI6(resbank)
extern void INT_SCIF6_TXI6(void) _INTERRUPT_;

// 260 SCIF SCIF7 BRI7
#pragma interrupt INT_SCIF7_BRI7(resbank)
extern void INT_SCIF7_BRI7(void) _INTERRUPT_;

// 261 SCIF SCIF7 ERI7
#pragma interrupt INT_SCIF7_ERI7(resbank)
extern void INT_SCIF7_ERI7(void) _INTERRUPT_;

// 262 SCIF SCIF7 RXI7
#pragma interrupt INT_SCIF7_RXI7(resbank)
extern void INT_SCIF7_RXI7(void) _INTERRUPT_;

// 263 SCIF SCIF7 TXI7
#pragma interrupt INT_SCIF7_TXI7(resbank)
extern void INT_SCIF7_TXI7(void) _INTERRUPT_;

// 264 SIOF SIOFI
#pragma interrupt INT_SIOF_SIOFI(resbank)
extern void INT_SIOF_SIOFI(void) _INTERRUPT_;

// 265 RSPI RSPI0 SPEI0
#pragma interrupt INT_RSPI0_SPEI0(resbank)
extern void INT_RSPI0_SPEI0(void) _INTERRUPT_;

// 266 RSPI RSPI0 SPRI0
#pragma interrupt INT_RSPI0_SPRI0(resbank)
extern void INT_RSPI0_SPRI0(void) _INTERRUPT_;

// 267 RSPI RSPI0 SPTI0
#pragma interrupt INT_RSPI0_SPTI0(resbank)
extern void INT_RSPI0_SPTI0(void) _INTERRUPT_;

// 268 RSPI RSPI1 SPEI1
#pragma interrupt INT_RSPI1_SPEI1(resbank)
extern void INT_RSPI1_SPEI1(void) _INTERRUPT_;

// 269 RSPI RSPI1 SPRI1
#pragma interrupt INT_RSPI1_SPRI1(resbank)
extern void INT_RSPI1_SPRI1(void) _INTERRUPT_;

// 270 RSPI RSPI1 SPTI1
#pragma interrupt INT_RSPI1_SPTI1(resbank)
extern void INT_RSPI1_SPTI1(void) _INTERRUPT_;

// 271 RCAN RCAN0 ERS0 
#pragma interrupt INT_RCAN0_ERS0(resbank)
extern void INT_RCAN0_ERS0(void) _INTERRUPT_;

// 272 RCAN RCAN0 OVR0 
#pragma interrupt INT_RCAN0_OVR0(resbank)
extern void INT_RCAN0_OVR0(void) _INTERRUPT_;

// 273 RCAN RCAN0 RM00 
#pragma interrupt INT_RCAN0_RM00(resbank)
extern void INT_RCAN0_RM00(void) _INTERRUPT_;

// 274 RCAN RCAN0 RM10 
#pragma interrupt INT_RCAN0_RM10(resbank)
extern void INT_RCAN0_RM10(void) _INTERRUPT_;

// 275 RCAN RCAN0 SLE0 
#pragma interrupt INT_RCAN0_SLE0(resbank)
extern void INT_RCAN0_SLE0(void) _INTERRUPT_;

// 276 RCAN RCAN1 ERS1 
#pragma interrupt INT_RCAN1_ERS1(resbank)
extern void INT_RCAN1_ERS1(void) _INTERRUPT_;

// 277 RCAN RCAN1 OVR1 
#pragma interrupt INT_RCAN1_OVR1(resbank)
extern void INT_RCAN1_OVR1(void) _INTERRUPT_;

// 278 RCAN RCAN1 RM01 
#pragma interrupt INT_RCAN1_RM01(resbank)
extern void INT_RCAN1_RM01(void) _INTERRUPT_;

// 279 RCAN RCAN1 RM11 
#pragma interrupt INT_RCAN1_RM11(resbank)
extern void INT_RCAN1_RM11(void) _INTERRUPT_;

// 280 RCAN RCAN1 SLE1 
#pragma interrupt INT_RCAN1_SLE1(resbank)
extern void INT_RCAN1_SLE1(void) _INTERRUPT_;

// 281 IEB IEB 
#pragma interrupt INT_IEB_IEB(resbank)
extern void INT_IEB_IEB(void) _INTERRUPT_;

// 282 ROMDEC ISY 
#pragma interrupt INT_ROMDEC_ISY(resbank)
extern void INT_ROMDEC_ISY(void) _INTERRUPT_;

// 283 ROMDEC IERR 
#pragma interrupt INT_ROMDEC_IERR(resbank)
extern void INT_ROMDEC_IERR(void) _INTERRUPT_;

// 284 ROMDEC ITARG 
#pragma interrupt INT_ROMDEC_ITARG(resbank)
extern void INT_ROMDEC_ITARG(void) _INTERRUPT_;

// 285 ROMDEC ISEC 
#pragma interrupt INT_ROMDEC_ISEC(resbank)
extern void INT_ROMDEC_ISEC(void) _INTERRUPT_;

// 286 ROMDEC IBUF 
#pragma interrupt INT_ROMDEC_IBUF(resbank)
extern void INT_ROMDEC_IBUF(void) _INTERRUPT_;

// 287 ROMDEC IREADY 
#pragma interrupt INT_ROMDEC_IREADY(resbank)
extern void INT_ROMDEC_IREADY(void) _INTERRUPT_;

// 288 FLCTL FLSTEI 
#pragma interrupt INT_FLCTL_FLSTEI(resbank)
extern void INT_FLCTL_FLSTEI(void) _INTERRUPT_;

// 289 FLCTL FLTENDI 
#pragma interrupt INT_FLCTL_FLTENDI(resbank)
extern void INT_FLCTL_FLTENDI(void) _INTERRUPT_;

// 290 FLCTL FLTREQ0I 
#pragma interrupt INT_FLCTL_FLTREQ0I(resbank)
extern void INT_FLCTL_FLTREQ0I(void) _INTERRUPT_;

// 291 FLCTL FLTREQ1I 
#pragma interrupt INT_FLCTL_FLTREQ1I(resbank)
extern void INT_FLCTL_FLTREQ1I(void) _INTERRUPT_;

// 292 SDHI SDHI3 
#pragma interrupt INT_SDHI_SDHI3(resbank)
extern void INT_SDHI_SDHI3(void) _INTERRUPT_;

// 293 SDHI SDHI0 
#pragma interrupt INT_SDHI_SDHI0(resbank)
extern void INT_SDHI_SDHI0(void) _INTERRUPT_;

// 294 SDHI SDHI1 
#pragma interrupt INT_SDHI_SDHI1(resbank)
extern void INT_SDHI_SDHI1(void) _INTERRUPT_;

// 295 Reserved

// 296 RTC ARM 
#pragma interrupt INT_RTC_ARM(resbank)
extern void INT_RTC_ARM(void) _INTERRUPT_;

// 297 RTC PRD 
#pragma interrupt INT_RTC_PRD(resbank)
extern void INT_RTC_PRD(void) _INTERRUPT_;

// 298 RTC CUP 
#pragma interrupt INT_RTC_CUP(resbank)
extern void INT_RTC_CUP(void) _INTERRUPT_;

// 299 SRC SRC0 OVF0 
#pragma interrupt INT_SRC0_OVF0(resbank)
extern void INT_SRC0_OVF0(void) _INTERRUPT_;

// 300 SRC SRC0 UDF0 
#pragma interrupt INT_SRC0_UDF0(resbank)
extern void INT_SRC0_UDF0(void) _INTERRUPT_;

// 301 SRC SRC0 CEF0 
#pragma interrupt INT_SRC0_CEF0(resbank)
extern void INT_SRC0_CEF0(void) _INTERRUPT_;

// 302 SRC SRC0 ODFI0 
#pragma interrupt INT_SRC0_ODFI0(resbank)
extern void INT_SRC0_ODFI0(void) _INTERRUPT_;

// 303 SRC SRC0 IDEI0 
#pragma interrupt INT_SRC0_IDEI0(resbank)
extern void INT_SRC0_IDEI0(void) _INTERRUPT_;

// 304 SRC SRC1 OVF1 
#pragma interrupt INT_SRC1_OVF1(resbank)
extern void INT_SRC1_OVF1(void) _INTERRUPT_;

// 305 SRC SRC1 UDF1 
#pragma interrupt INT_SRC1_UDF1(resbank)
extern void INT_SRC1_UDF1(void) _INTERRUPT_;

// 306 SRC SRC1 CEF1 
#pragma interrupt INT_SRC1_CEF1(resbank)
extern void INT_SRC1_CEF1(void) _INTERRUPT_;

// 307 SRC SRC1 ODFI1 
#pragma interrupt INT_SRC1_ODFI1(resbank)
extern void INT_SRC1_ODFI1(void) _INTERRUPT_;

// 308 SRC SRC1 IDEI1 
#pragma interrupt INT_SRC1_IDEI1(resbank)
extern void INT_SRC1_IDEI1(void) _INTERRUPT_;

// 309 Reserved

// 310 DCU DCUEI 
#pragma interrupt INT_DCU_DCUEI(resbank)
extern void INT_DCU_DCUEI(void) _INTERRUPT_;

// 311 DCU OFFI 
#pragma interrupt INT_DCU_OFFI(resbank)
extern void INT_DCU_OFFI(void) _INTERRUPT_;

// 312 DCU IFEI 
#pragma interrupt INT_DCU_IFEI(resbank)
extern void INT_DCU_IFEI(void) _INTERRUPT_;


#pragma interrupt Dummy(resbank)
extern void Dummy(void) _INTERRUPT_;

#endif /* VECT_H */
/* End of File */
