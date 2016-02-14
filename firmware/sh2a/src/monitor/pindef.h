#ifndef	_pindef_h_
#define	_pindef_h_

/* STM8S-D
	 +-- 10PIN ST-LINK っぽいもの--------+
   10| (nSRST)未接続   |  -              |9
	8| PB4(JNTRST) 	   | GND 			 |7 
	6| PA15(JTDI) 	   | PA13(JTMS/SWDIO)|5 
	4| PA14(JTCK/SWCLK)| PB3(JTDO) 		 |3 
	2| - 			   | VDD_1 			 |1 
	 +-- 10PIN ST-LINK っぽいもの--------+
*/

#ifdef	STM8S_D
//	ポート出力データ.   		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	TCK	 		PA14			// TCK  PGC =(AVR-SCK)
#define	TDI	 		PA15			// TDI  PGM =(AVR-MOSI)
#define	TDO	 		PB3				// TDO  PGD =(AVR-MISO)
#define	TMS 		PA13			// TMS  MCLR=(AVR-RST)
#define	TRST		PB4				// TRST
#define	SRST		PB5				// SRST
#define	Vcc1		PB6				// Vcc1検出 PB5はSWIM RESET
#define	Vcc2		PB7				// Vcc2検出
//----------------------------------------------------------
#endif	//STM8S_D

/* CQ-STARM

		   +----- J3 ------+
		40 | GND   | 3.3V  | 39
		.. | ..    | ..    | ..
		.. | ..    | ..    | ..
		30 |       |       | 29
		28 |       |       | 27

  Vcc1  26 | PC4   | PC5   | 25   Vcc2
  TRST  24 | PB0   | PB1   | 23   -
  TDI   22 | PE7   | PE8   | 21   -
  TMS   20 | PE9   | PE10  | 19   -
  TCK   18 | PE11  | PE12  | 17   -
  -     16 | PE13  | PE14  | 15   -
  TDO   14 | PE15  | PB10  | 13   -
  SRST	12 | PB11  | PB12  | 11   -
  -		10 | ..    | ..    | 9    -
  -		 8 | ..    | ..    | 7    -

		.. | ..    | ..    | ..
		.. | ..    | ..    | ..
      	 2 | PD13  | GND   | 1
		   +----- J3 ------+
                                [USB mini-B コネ位置]

     上記以外のGPIOはすべて入力方向

*/

#ifdef	CQ_STARM
//	ポート出力データ.    		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	Vcc1		PC4				// Vcc1検出
#define	Vcc2		PC5				// Vcc2検出
#define	TRST		PB0				// TRST
#define	TDI 		PE7				// TDI  PGM =(AVR-MOSI)
#define	TMS			PE9				// TMS  MCLR=(AVR-RST)
#define	TCK 		PE11			// TCK  PGC =(AVR-SCK)
//-
#define	TDO 		PE15			// TDO  PGD =(AVR-MISO)
#define	SRST		PB11			// SRST
//----------------------------------------------------------
#endif	//CQ_STARM


#ifdef	STBEE_MINI
//	ポート出力データ.    		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	TCK 		PB11			// TCK  PGC =(AVR-SCK)
#define	TDI 		PB10			// TDI  PGM =(AVR-MOSI)
#define	TDO 		PB9				// TDO  PGD =(AVR-MISO)
#define	TMS			PB8				// TMS  MCLR=(AVR-RST)
#define	TRST		PB7				// TRST
#define	SRST		PB6				// SRST
#define	Vcc1		PB5				// Vcc1検出
#define	Vcc2		PB4				// Vcc2検出
//----------------------------------------------------------
#endif	//STBEE_MINI

#ifdef	STBEE
//	ポート出力データ.    		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	TCK 		PB11			// TCK  PGC =(AVR-SCK)
#define	TDI 		PB10			// TDI  PGM =(AVR-MOSI)
#define	TDO 		PB9				// TDO  PGD =(AVR-MISO)
#define	TMS			PB8				// TMS  MCLR=(AVR-RST)
#define	TRST		PB7				// TRST
#define	SRST		PB6				// SRST
#define	Vcc1		PB5				// Vcc1検出
#define	Vcc2		PB4				// Vcc2検出
//----------------------------------------------------------
#endif	//STBEE

#ifdef	LPC2388
//	ポート出力データ.    		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	Vcc1		PIO_(4,0)		// Vcc1検出
#define	Vcc2		PIO_(4,1)		// Vcc2検出
#define	TRST		PIO_(4,2)		// TRST
#define	TDI 		PIO_(4,4)		// TDI  PGM =(AVR-MOSI)
#define	TMS			PIO_(4,6)		// TMS  MCLR=(AVR-RST)
#define	TCK 		PIO_(4,8)		// TCK  PGC =(AVR-SCK)
//-
#define	TDO 		PIO_(4,12)		// TDO  PGD =(AVR-MISO)
#define	SRST		PIO_(4,14)		// SRST
//----------------------------------------------------------
/*
         +---------CN1-----------+
Vcc1   1 | P4_0  A0  | P4_1  A1  | 2    Vcc2
TRST   3 | P4_2  A2  | P4_3  A3  | 4    -
TDI    5 | P4_4  A4  | P4_5  A5  | 6    -
TMS    7 | P4_6  A6  | P4_7  A7  | 8    -
TCK    9 | P4_8  A8  | P4_7  A9  | 10   -
-     11 | P4_10 A10 | P4_1  A11 | 12   -
TDO   13 | P4_12 A12 | P4_3  A13 | 14   -
SRST  15 | P4_14 A14 | P4_5  A15 | 16   -
-     17 | ..        | ..        | 18   -
-     19 | ..        | ..        | 20   -

      .. | ..        | ..        | ..
      .. | ..        | ..        | ..
      .. | ..        | ..        | ..
      39 |           |           | 40
         +---------CN1-----------+

                [USB-Bコネ位置]
*/
#endif	//LPC2388


#ifdef	MARY
//	ポート出力データ.   		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	TCK 		P0_8			// TCK  PGC =(AVR-SCK)
#define	TDI 		P1_0			// TDI  PGM =(AVR-MOSI)
#define	TDO 		P0_6			// TDO  PGD =(AVR-MISO)
#define	TMS 		P0_9			// TMS  MCLR=(AVR-RST)
#define	TRST		P1_9			// TRST
#define	SRST		P1_10			// SRST
#define	Vcc1		P0_11			// Vcc1検出
#define	Vcc2		P0_10			// Vcc2検出
//----------------------------------------------------------
/*
 PIC18F pin head
 ----------------
   5    3    1
 +----+----+----+
 |MCLR|PGC |PGD |
 +----+----+----+
 |GND |PGM |Vcc |
 +----+----+----+
   6    4    2


 MARY CN4
 ---------------------
   7    5    3     1
 +----+----+-----+-----+
 |P0_9|P0_8|P0_6 |P1_9 |
 +----+----+-----+-----+
 |GND |P1_0|P0_11|P1_10|
 +----+----+-----+-----+
   8    6    4     2

 JTAG CABLE(AVR compat)
 ----------------
   5    3    1
 +----+----+----+
 |TMS |TCK |TDO |
 +----+----+----+
 |GND |TDI |Vcc |
 +----+----+----+
   6    4    2

*/

#endif	//MARY


#ifdef	RX62N
//	ポート出力データ.   		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	Vcc1		PA_0		// Vcc1検出
#define	Vcc2		PA_1		// Vcc2検出
#define	TRST		PA_2		// TRST
#define	TDI 		PA_4		// TDI  PGM =(AVR-MOSI)
#define	TMS			PA_6		// TMS  MCLR=(AVR-RST)
#define	TCK 		PB_0		// TCK  PGC =(AVR-SCK)
//-                  
#define	TDO 		PB_4		// TDO  PGD =(AVR-MISO)
#define	SRST		PB_6		// SRST
//----------------------------------------------------------
/*
         +---------CN1-----------+
Vcc1   1 | PA_0  A0  | PA_1  A1  | 2    Vcc2
TRST   3 | PA_2  A2  | PA_3  A3  | 4    -
TDI    5 | PA_4  A4  | PA_5  A5  | 6    -
TMS    7 | PA_6  A6  | PA_7  A7  | 8    -
TCK    9 | PB_0  A8  | PB_1  A9  | 10   -
-     11 | PB_2  A10 | PB_3  A11 | 12   -
TDO   13 | PB_4  A12 | PB_5  A13 | 14   -
SRST  15 | PB_6  A14 | PB_7  A15 | 16   -
-     17 | ..    A16 | ..    A17 | 18   -
-     19 | ..    A18 | ..    A19 | 20   -
      21 |     nWAIT |       GND | 22
      23 | PD_0  D0  | PD_1  D1  | 24
      25 | PD_2  D2  | PD_3  D3  | 26

      .. | ..        | ..        | ..
      .. | ..        | ..        | ..
      .. | ..        | ..        | ..
      39 |           |           | 40
         +---------CN1-----------+

				                [USB-Bコネ位置]
*/
#endif	//RX62N


#ifdef	SH2A
//	ポート出力データ.   		  	  JTAG  PIC   AVR
//----------------------------------------------------------
#define	Vcc1		PB0		// Vcc1検出
#define	Vcc2		PB1		// Vcc2検出
#define	TRST		PB2		// TRST
#define	TDI 		PB4		// TDI  PGM =(AVR-MOSI)
#define	TMS			PB6		// TMS  MCLR=(AVR-RST)
#define	TCK 		PB8		// TCK  PGC =(AVR-SCK)
//-                  
#define	TDO 		PB12	// TDO  PGD =(AVR-MISO)
#define	SRST		PB14	// SRST
//----------------------------------------------------------
/*
         +---------CN1---------+
Vcc1   1 | PB0  A0  | PB1  A1  | 2    Vcc2
TRST   3 | PB2  A2  | PB3  A3  | 4    -
TDI    5 | PB4  A4  | PB5  A5  | 6    -
TMS    7 | PB6  A6  | PB7  A7  | 8    -
TCK    9 | PB8  A8  | PB9  A9  | 10   -
-     11 | PB10 A10 | PB11 A11 | 12   -
TDO   13 | PB12 A12 | PB13 A13 | 14   -
SRST  15 | PB14 A14 | PB15 A15 | 16   -
-     17 | ..   ..  | ..   ..  | 18   -
-     19 | ..   ..  | ..   ..  | 20   -

      .. | ..       | ..       | ..
      .. | ..       | ..       | ..
      .. | ..       | ..       | ..
      39 |          |          | 40
         +---------CN1---------+

				                [USB-Bコネ位置]
*/
#endif	//SH2A



//	PIC PGxピンとJTAGピンの対応表.
#define	PGM	 		TDI			// TDI  PGM =(AVR-MOSI)
#define	PGC	    	TCK			// TCK  PGC =(AVR-SCK)
#define	PGD	    	TDO			// TDO  PGD =(AVR-MISO)
#define	MCLR    	TMS			// TMS  MCLR=(AVR-RST)

#endif	// ifndef	_pindef_h_
