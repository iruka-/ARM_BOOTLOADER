/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
********************************************************************************
* Copyright (C) 2009. Renesas Technology Corp., All Rights Reserved.           *
********************************************************************************/

/*""FILE COMMENT""*********** Technical reference data *************************
 *
 *       System Name : SH7264 USB Sample Program
 *       File Name   : c_def7264.h
 *       Abstract    : Define SH7264 USB module Register
 *       Version     : 1.00.00
 *       Device      : SH7264/SH7262
 *       Tool-Chain  : High-performance Embedded Workshop (Ver.4.04.01).
 *                   : C/C++ compiler package for the SuperH RISC engine family
 *                   :                             (Ver.9.02 Release00).
 *       OS          : None
 *       H/W Platform: M3A-HS64G50(CPU board)
 *
 *       History   : May.13.2009 ver.1.00.00
 *""FILE COMMENT END""**********************************************************/
#ifndef		__DEF7264_H__
#define		__DEF7264_H__

/* System Configuration Status Register */
/* SYSCFG */
#define	BITSCKE				( 0x0400u )	/* b10: USB clock enable 			*/
#define	BITHSE				( 0x0080u )	/* b7: Hi-speed enable 				*/
#define	BITDCFM				( 0x0040u )	/* b6: Controller function select	*/
#define	BITDRPD				( 0x0020u )	/* b5: D- pull down control 		*/
#define	BITDPRPU			( 0x0010u )	/* b4: D+ pull up control 			*/
#define	BITUSBE				( 0x0001u )	/* b0: USB module operation enable	*/
#define BITBWAIT			( 0x000fu )	/* b3-0: CPU BUS access wait 		*/

#define	HOST_CONTROLER		( 0x0040u )
#define	PERI_CONTROLER		( 0x0000u )

/* System Configuration Status Register */
/*LNST  b1-0: D+, D- line status */
#define	BITLNST				0x0003u		/* b1-b0: LNST 			*/
#define	  SE1				 0x0003u	/* SE1 					*/
#define	  FS_KSTS			 0x0002u	/* Full-Speed K State	*/
#define	  FS_JSTS			 0x0001u	/* Full-Speed J State 	*/
#define	  LS_JSTS			 0x0002u	/* Low-Speed J State 	*/
#define	  LS_KSTS			 0x0001u	/* Low-Speed K State 	*/
#define	  SE0				 0x0000u	/* SE0 					*/
#define	  CHIRPJ			 0x0001u	/* Chirp J 				*/
#define	  CHIRPK			 0x0002u	/* Chirp J 				*/

/* Device State Control Register */
/* DVSTCTR */
#define	BITWKUP				( 0x0100u )	/* b8: Remote wakeup 			*/
#define	BITRWUPE			( 0x0080u )	/* b7: Remote wakeup sense 		*/
#define	BITUSBRST			( 0x0040u )	/* b6: USB reset enable 		*/
#define	BITRESUME			( 0x0020u )	/* b5: Resume enable 			*/
#define	BITUACT				( 0x0010u )	/* b4: USB bus enable 			*/
#define	BITRHST				( 0x0007u )	/* b1-0: Reset handshake status */
#define	  HSPROC			 0x0004u	/* HS handshake is processing 	*/
#define	  HSMODE			 0x0003u	/* Hi-Speed mode 				*/
#define	  FSMODE			 0x0002u	/* Full-Speed mode 				*/
#define	  LSMODE			 0x0001u	/* Low-Speed mode 				*/
#define	  UNDECID			 0x0000u	/* Undecided 					*/

/* Test Mode Register */
/* TESTMODE */
/* UTST: Test select */
#define	  H_TST_FORCE		 0x000Du	/* HOST TEST Force 		*/
#define	  H_TST_PACKET		 0x000Cu	/* HOST TEST Packet 	*/
#define	  H_TST_SE0_NAK		 0x000Bu	/* HOST TEST SE0 NAK	*/
#define	  H_TST_K			 0x000Au	/* HOST TEST K 			*/
#define	  H_TST_J			 0x0009u	/* HOST TEST J 			*/
#define	  H_TST_NORMAL		 0x0000u	/* HOST Normal Mode 	*/
#define	  P_TST_PACKET		 0x0004u	/* PERI TEST Packet 	*/
#define	  P_TST_SE0_NAK		 0x0003u	/* PERI TEST SE0 NAK 	*/
#define	  P_TST_K			 0x0002u	/* PERI TEST K 			*/
#define	  P_TST_J			 0x0001u	/* PERI TEST J 			*/
#define	  P_TST_NORMAL		 0x0000u	/* PERI Normal Mode 	*/

/* DMA-FIFO Bus Configuration Register */
//D0FBCFG
//D1FBCFG
#define	DFACC_CYC			( 0x0000u )	/* cycle still 				*/
#define DFACC_16			( 0x0001u )	/* 16byte 					*/
#define DFACC_32			( 0x0002u )	/* 32byte 					*/
#define	BITDFACC			( 0x3000u )	/* b13-b12: Bus Access mode */
#define		BITDFACC_CYC	( 0x0000u )	/* cycle still 				*/
#define 	BITDFACC_16		( 0x1000u )	/* 16byte 					*/
#define 	BITDFACC_32		( 0x2000u )	/* 32byte 					*/

/* CFIFO/DxFIFO Port Select Register */
/* CFIFOSEL */
/* D0FIFOSEL */
/* D1FIFOSEL */
/* Big endian mode */
#define	  BYTE_LITTLE		0x0000u		/* little dendian 	*/
#define	  BYTE_BIG			0x0001u		/* big endifan 		*/
/* MBW: Maximum bit width for FIFO access */
#define	  MBW_8				0x0000u		/*  8bit */
#define	  MBW_16			0x0001u		/* 16bit */
#define	  MBW_32			0x0002u		/* 32bit */

#define	BITRCNT			( 0x8000u )		/*  b15:Read count mode */
#define	BITREW			( 0x4000u )		/*  b14:Buffer rewind */
#define	BITDCLRM		( 0x2000u )		/*  b13:DMA buffer clear mode */
#define	BITDREQE		( 0x1000u )		/*  b12:DREQ output enable */
#define	BITMBW			( 0x0c00u )		/*  b11-b10:Maximum bit width for FIFO access */
#define	  BITMBW_8		 0x0000u		/*  8bit */
#define	  BITMBW_16		 0x0400u		/* 16bit */
#define	  BITMBW_32		 0x0800u		/* 32bit */
#define	BITBIGEND		( 0x0100u )		/*  b8:FIFO Big endian mode */
#define	  BITBYTE_LITTLE  0x0000u		/* little dendian */
#define	  BITBYTE_BIG	  0x0100u		/* big endifan */
#define	BITISEL			( 0x0020u )		/*  b5:DCP FIFO port direction select */
#define	BITCURPIPE		( 0x000fu )		/*  b3-b0:PIPE select */

#define	CFIFO_DIR_OUT		( 0x0020u )
#define	CFIFO_DIR_IN		( 0x0000u )

/* CFIFO/DxFIFO Port Control Register */
/* CFIFOCTR */
/* D0FIFOCTR */
/* D1FIFOCTR */
#define	BITBVAL				( 0x8000u )	/* b15: Buffer valid flag */
#define	BITBCLR				( 0x4000u )	/* b14: Buffer clear */
#define	BITFRDY				( 0x2000u )	/* b13: FIFO ready */
#define	BITDTLN				( 0x0FFFu )	/* b11-0: FIFO received data length */

/* Interrupt Enable Register 0 */
/* INTENB0 */
#define	BITVBSE				( 0x8000u )	/* b15: VBUS interrupt */
#define	BITRSME				( 0x4000u )	/* b14: Resume interrupt */
#define	BITSOFE				( 0x2000u )	/* b13: Frame update interrupt */
#define	BITDVSE				( 0x1000u )	/* b12: Device state transition interrupt */
#define	BITCTRE				( 0x0800u )	/* b11: Control transfer stage */
										/*      transition interrupt */
#define	BITBEMPE			( 0x0400u )	/* b10: Buffer empty interrupt */
#define	BITNRDYE			( 0x0200u )	/* b9: Buffer not ready interrupt */
#define	BITBRDYE			( 0x0100u )	/* b8: Buffer ready interrupt */

/* Interrupt Enable Register 0 */
/* INTENB1 */
#define	BITBCHGE		( 0x4000u )	/*  b14:USB bus chenge interrupt( port0 ) */
#define	BITDTCHE		( 0x1000u )	/*  b12:Detach sense interrupt( port0 ) */
#define	BITATTCHE		( 0x0800u )	/*  b11:Attach sense interrupt( port0 ) */
#define	BITEOFERRE		( 0x0040u )	/*  b6:EOF error detect interrupt( port0 ) */
#define	BITSIGNE		( 0x0020u )	/*  b5:SETUP IGNORE interrupt( port0 ) */
#define	BITSACKE		( 0x0010u )	/*  b4:SETUP ACK interrupt( port0 ) */

/* SOF pin configration */
/* SOFCFG */
#define	BITTRNENSEL		( 0x0100u )	/*  b8:Transaction term-of-validity change */
#define	BITBRDYM		( 0x0040u )	/*  b6:BRDY interrupt status clear timeming change */

/* Interrupt Status Register 0 */
/* INTSTS0 */
/* DVSQ: Device state */
#define	BITVBINT			( 0x8000u )	/* b15: VBUS interrupt */
#define	BITRESM				( 0x4000u )	/* b14: Resume interrupt */
#define	BITSOFR				( 0x2000u )	/* b13: SOF frame update interrupt */
#define	BITDVST				( 0x1000u )	/* b12: Device state transition interrupt */
#define	BITCTRT				( 0x0800u )	/* b11: Control transfer stage */
										/*      transition interrupt */
#define	BITBEMP				( 0x0400u )	/* b10: Buffer empty interrupt */
#define	BITNRDY				( 0x0200u )	/* b9: Buffer not ready interrupt */
#define	BITBRDY				( 0x0100u )	/* b8: Buffer ready interrupt */
#define	BITVBSTS			( 0x0080u )	/* b7: VBUS input port */
#define	BITDVSQ				( 0x0070u )	/* b6-4: Device state */
#define	BITDVSQS			( 0x0030u )	/* b5-4: Device state */
#define	  DS_SPD_CNFG		 0x0070u	  /* Suspend Configured */
#define	  DS_SPD_ADDR		 0x0060u	  /* Suspend Address */
#define	  DS_SPD_DFLT		 0x0050u	  /* Suspend Default */
#define	  DS_SPD_POWR		 0x0040u	  /* Suspend Powered */
#define	  DS_SUSP			 0x0040u	  /* Suspend */
#define	  DS_CNFG			 0x0030u	  /* Configured */
#define	  DS_ADDS			 0x0020u	  /* Address */
#define	  DS_DFLT			 0x0010u	  /* Default */
#define	  DS_POWR			 0x0000u	  /* Powered */
#define	BITVALID			( 0x0008u )	/* b3: Setup packet detected flag */
#define	BITCTSQ				( 0x0007u )	/* b2-0: Control transfer stage */
#define	  CS_SQER			 0x0006u	  /* Sequence error */
#define	  CS_WRND			 0x0005u	  /* Control write nodata status stage */
#define	  CS_WRSS			 0x0004u	  /* Control write status stage */
#define	  CS_WRDS			 0x0003u	  /* Control write data stage */
#define	  CS_RDSS			 0x0002u	  /* Control read status stage */
#define	  CS_RDDS			 0x0001u	  /* Control read data stage */
#define	  CS_IDST			 0x0000u	  /* Idle or setup stage */

/* Interrupt Status Register 1 */
/* INTSTS1 */
#define	BITBCHG			( 0x4000u )	/*  b14:USB bus chenge interrupt( port0 ) */
#define	BITDTCH			( 0x1000u )	/*  b12:Detach sense interrupt( port0 ) */
#define	BITATTCH		( 0x0800u )	/*  b11:Attach sense interrupt( port0 ) */
#define	BITEOFERR		( 0x0040u )	/*  b6:EOF error detect interrupt( port0 ) */
#define	BITSIGN			( 0x0020u )	/*  b5:SETUP IGNORE interrupt( port0 ) */
#define	BITSACK			( 0x0010u )	/*  b4:SETUP ACK interrupt( port0 ) */

/* Frame number */
/* FRMNUM */
#define	BITOVRN			( 0x8000u )	/*  b15:Overrun error */
#define	BITCRCE			( 0x4000u )	/*  b14:Received data error */
#define	BITFRNM			( 0x03ffu )	/*  b10-b0:Frame number */

/* Buffer ready interrupt */
/* BRDYENB,BRDYSTS */
#define	BITBRDY9		( 0x0200u )		/* b9:PIPE9 */
#define	BITBRDY8		( 0x0100u )		/* b8:PIPE8 */
#define	BITBRDY7		( 0x0080u )		/* b7:PIPE7 */
#define	BITBRDY6		( 0x0040u )		/* b6:PIPE6 */
#define	BITBRDY5		( 0x0020u )		/* b5:PIPE5 */
#define	BITBRDY4		( 0x0010u )		/* b4:PIPE4 */
#define	BITBRDY3		( 0x0008u )		/* b3:PIPE3 */
#define	BITBRDY2		( 0x0004u )		/* b2:PIPE2 */
#define	BITBRDY1		( 0x0002u )		/* b1:PIPE1 */
#define	BITBRDY0		( 0x0001u )		/* b0:PIPE0 */

/* Buffer not ready interrupt */
/* NRDYENB,NRDYSTS */
#define	BITNRDY9		( 0x0200u )		/* b9:PIPE9 */
#define	BITNRDY8		( 0x0100u )		/* b8:PIPE8 */
#define	BITNRDY7		( 0x0080u )		/* b7:PIPE7 */
#define	BITNRDY6		( 0x0040u )		/* b6:PIPE6 */
#define	BITNRDY5		( 0x0020u )		/* b5:PIPE5 */
#define	BITNRDY4		( 0x0010u )		/* b4:PIPE4 */
#define	BITNRDY3		( 0x0008u )		/* b3:PIPE3 */
#define	BITNRDY2		( 0x0004u )		/* b2:PIPE2 */
#define	BITNRDY1		( 0x0002u )		/* b1:PIPE1 */
#define	BITNRDY0		( 0x0001u )		/* b0:PIPE0 */

/* Buffer empty interrupt */
/* BEMPENB,BEMPSTS */
#define	BITBEMP9		( 0x0200u )		/* b9:PIPE9 */
#define	BITBEMP8		( 0x0100u )		/* b8:PIPE8 */
#define	BITBEMP7		( 0x0080u )		/* b7:PIPE7 */
#define	BITBEMP6		( 0x0040u )		/* b6:PIPE6 */
#define	BITBEMP5		( 0x0020u )		/* b5:PIPE5 */
#define	BITBEMP4		( 0x0010u )		/* b4:PIPE4 */
#define	BITBEMP3		( 0x0008u )		/* b3:PIPE3 */
#define	BITBEMP2		( 0x0004u )		/* b2:PIPE2 */
#define	BITBEMP1		( 0x0002u )		/* b1:PIPE1 */
#define	BITBEMP0		( 0x0001u )		/* b0:PIPE0 */

/* u Frame number */
/* UFRMNUM */
#define	BITUFRNM		( 0x0007u )	/*  b2-b0:Micro frame number */

/* USB address */
/* USBADDR */
#define	BITUSBADDR		( 0x003fu )	/*  b6-b0:USB address */

/* USB Request Type Register */
/* USBREQ */
/* BMREQSTTYPE: RequestType */
#define	bRequest			( 0xFF00u )	/* b15-8: bRequest */
#define	bmRequestType		( 0x00FFu )	/* b7-0: bmRequestType */
#define	bmRequestTypeDir	( 0x0080u )	/* b7  : Data transfer direction */
#define	bmRequestTypeType	( 0x0060u )	/* b6-5: Type */
#define	bmRequestTypeRecip	( 0x001Fu )	/* b4-0: Recipient */

/* USB Request Value Register */
/* USBVAL */
#define	DT_TYPE				( 0xFF00u )
#define	DT_INDEX			( 0x00FFu )
#define	CONF_NUM			( 0x00FFu )
#define	ALT_SET				( 0x00FFu )
#define	GET_DT_TYPE( v )		( ( ( v ) & DT_TYPE ) >> 8 )

#define	wIndex				( 0xFFFFu )	/* b15-0: wIndex */
#define	TEST_SELECT			( 0xFF00u )	/* b15-b8: Test Mode Selectors */
#define	  TEST_J			 0x0100u	  /* Test_J */
#define	  TEST_K			 0x0200u	  /* Test_K */
#define	  TEST_SE0_NAK		 0x0300u	  /* Test_SE0_NAK */
#define	  TEST_PACKET		 0x0400u	  /* Test_Packet */
#define	  TEST_FORCE_ENABLE	 0x0500u	  /* Test_Force_Enable */
#define	  TEST_STSelectors	 0x0600u	  /* Standard test selectors */
#define	  TEST_Reserved		 0x4000u	  /* Reserved */
#define	  TEST_VSTModes		 0xC000u	  /* Vendor-specific test modes */
#define	EP_DIR				( 0x0080u )	/* b7: Endpoint Direction */
#define	  EP_DIR_IN			 0x0080u
#define	  EP_DIR_OUT		 0x0000u

/* default control pipe configration */
/* DCPCFG */
#define	BITDIR				( 0x0010u )	/*  b4:Control transfer DIR select */
#define	DCP_DIRBIT_OUT		1			/* DCP Out Direction					*/
#define	DCP_DIRBIT_IN		0			/* DCP In Direction						*/

/* default control pipe max packet size */
/* DCPMAXP */
#define	BITDCPDEVSEL			( 0xf000u )	/*  b15-b12:Device address select */
#define	BITDCPMXPS				( 0x007fu )	/*  b6-b0:Maxpacket size */
											/*        of default control pipe */
/* DEVSEL: Device address select */
#define	  DEVICE_0			 0x0000u	  /* Device address 0 */
#define	  DEVICE_1			 0x0001u	  /* Device address 1 */
#define	  DEVICE_2			 0x0002u	  /* Device address 2 */
#define	  DEVICE_3			 0x0003u	  /* Device address 3 */
#define	  DEVICE_4			 0x0004u	  /* Device address 3 */
#define	  DEVICE_5			 0x0005u	  /* Device address 3 */
#define	  DEVICE_6			 0x0006u	  /* Device address 3 */
#define	  DEVICE_7			 0x0007u	  /* Device address 3 */
#define	  DEVICE_8			 0x0008u	  /* Device address 3 */
#define	  DEVICE_9			 0x0009u	  /* Device address 3 */
#define	  DEVICE_10			 0x000Au	  /* Device address 3 */

/* default control pipe control */
/* DCPCTR */
/* PIPE1CTR-PIPE5CTR */
/* PIPE6CTR-PIPE9CTR */
#define	BITBSTS				( 0x8000u )	/*  b15:Buffer status */
#define	BITSUREQ			( 0x4000u )	/*  b14:Send setup taken */
#define	BITCSCLR			( 0x2000u )	/*  b13:Split Transaction status clear */
#define	BITCSSTS			( 0x1000u )	/*  b12:Split Transaction status */
#define	BITSUREQCLR			( 0x0800u )	/*  b11:Send USB request clear */
#define	BITSQCLR			( 0x0100u )	/*  b8:Sequence toggle bit clear */
#define	BITSQSET			( 0x0080u )	/*  b7:Sequence toggle bit set */
#define	BITSQMON			( 0x0040u )	/*  b6:Sequence toggle bit monitor */
#define	BITPBUSY			( 0x0020u )	/*  b5:pipe busy */
#define	BITPINGE			( 0x0010u )	/*  b4:Ping taken enable*/
#define	BITCCPL				( 0x0004u )	/*  b2:Enable control transfer complete */
#define	BITPID				( 0x0003u )	/*  b1-b0:Response PID */
#define	  PID_STALL2		 0x0003u	  /* STALL */
#define	  PID_STALL			 0x0002u	  /* STALL */
#define	  PID_BUF			 0x0001u	  /* BUF */
#define	  PID_NAK			 0x0000u	  /* NAK */
/* PIPE1CTR-PIPE5CTR */
#define	BITINBUFM			( 0x4000u )	/*  b14:IN buffer monitor */
#define	BITATREPM			( 0x0400u )	/*  b10:auto responce mode */
#define	BITACLRM			( 0x0200u )	/*  b9:Out buffer auto clear mode */

#define	DCP_DATA_TOGGLE0		( 0x0100u )
#define	DCP_DATA_TOGGLE1		( 0x0080u )
#define	DCP_DATA_TOGGLE1_PING	( 0x0090u )

#define	DCP_DIRBIT_OUT		1			/* DCP Out Direction					*/
#define	DCP_DIRBIT_IN		0			/* DCP In Direction						*/

/* pipe windows select */
/* PIPESEL */
#define	BITPIPESEL			( 0x000fu )	/*  b3-b0:Pipe select */

/* PIPEn configration */
/* PIPECFG */
#define	TYPFIELD			( 0xC000u )	/* b15-14: Transfer type */
#define	  ISO				 0xC000u	  /* Isochronous */
#define	  INT				 0x8000u	  /* Interrupt */
#define	  BULK				 0x4000u	  /* Bulk */
#define	BFREFIELD			( 0x0400u )	/* b10: Buffer ready interrupt mode select */
#define	  BFREON			 0x0400u
#define	  BFREOFF			 0x0000u
#define	DBLBFIELD			( 0x0200u )	/* b9: Double buffer mode select */
#define	  DBLBON			 0x0200u
#define	  DBLBOFF			 0x0000u
#define	CNTMDFIELD			( 0x0100u )	/* b8: Continuous transfer mode select */
#define	  CNTMDON			 0x0100u
#define	  CNTMDOFF			 0x0000u
#define	SHTNAKFIELD			( 0x0080u )	/* b7: Transfer end NAK */
#define	  SHTNAKON			 0x0080u
#define	  SHTNAKOFF			 0x0000u
#define	DIRFIELD			( 0x0010u )	/* b4: Transfer direction select */
#define	  DIR_H_OUT			 0x0010u	  /* HOST OUT */
#define	  DIR_P_IN			 0x0010u	  /* PERI IN */
#define	  DIR_H_IN			 0x0000u	  /* HOST IN */
#define	  DIR_P_OUT			 0x0000u	  /* PERI OUT */
#define	EPNUMFIELD			( 0x000Fu )	/* b3-0: Eendpoint number select */
#define	MAX_EP_NO			15u			/* EP0 EP1 ... EP15 */
#define	  EP0				 0x0000u
#define	  EP1				 0x0001u
#define	  EP2				 0x0002u
#define	  EP3				 0x0003u
#define	  EP4				 0x0004u
#define	  EP5				 0x0005u
#define	  EP6				 0x0006u
#define	  EP7				 0x0007u
#define	  EP8				 0x0008u
#define	  EP9				 0x0009u
#define	  EP10				 0x000Au
#define	  EP11				 0x000Bu
#define	  EP12				 0x000Cu
#define	  EP13				 0x000Du
#define	  EP14				 0x000Eu
#define	  EP15				 0x000Fu
#define	  BYTE_LITTLE		  0x0000u	/* little dendian */

/* PIPEn buffer select */
/* PIPEBUF */
#define	BITBUFSIZE			( 0x7c00u )	/*  b14-b10:Pipe buffer size */
#define	BITBUFNMB			( 0x00ffu )	/*  b7-b0:Pipe buffer number */
#define	PIPE0BUF			( 256u )
#define	PIPExBUF			( 64u )

/* PIPEn max packet size */
/* PIPEMAXP */
#define	BITDEVSEL			( 0xf000u )	/*  b15-b14:Pipe Device select */
#define	BITMXPS				( 0x07ffu )	/*  b10-b0:Pipe Maxpacket size */

/* PIPEn interval control */
/* PIPEPERI */
#define	BITIFIS				( 0x1000u )	/*  b12:Isochronous in-buffer */
										/*      flush mode select */
#define	BITIITV				( 0x0007u )	/*  b3-b0:Isochronous interval */

/* PIPEnCTR ( Only for PIPE1 to 5 ) */

/* PIPEnCTR ( Only for PIPE6 to 9 ) */

/* PIPEnTRE ( Only for PIPE1 to 5 ) */
/* PIPE1TRE-PIPE5TRE */
#define	BITTRENB			( 0x0020u )	/*  b9:Transaction counter enable */
#define	BITTRCLR			( 0x0010u )	/*  b8:Transaction counter clear */

/* DEVADDn ( n=0-A ) */
/* DEVADD0-DEVADDA */
typedef union {						/* DEVADDn ( n=0-A ) */
	USB_U16 WORD;					/*  Word Access */
	struct {
		USB_U16 :1;
		USB_U16 UPPHUB:4;			/*  b14-11:target HUB address */
		USB_U16 HUBPORT:3;			/*  b10-8:target HUB port */
		USB_U16 USBSPD:2;			/*  b7-6:target Device speed */
		USB_U16 :6;
	} BIT;
}DEVADD;

#define	BITUPPHUB			( 0x7800u )	/*  b14-11:target HUB address */
#define	BITHUBPORT			( 0x0700u )	/*  b10-8:target HUB port */
#define	BITUSBSPD			( 0x00c0u )	/*  b7-6:target Device speed */
#define USBSPD_NON			( 0x0000u )
#define USBSPD_LOW			( 0x0001u )
#define USBSPD_FUL			( 0x0002u )
#define USBSPD_HI			( 0x0003u )

#endif		/*  __DEF7264_H__ */

/*===========================================================================*/
/* End of File */
