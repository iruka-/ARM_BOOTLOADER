/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/

/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */

/*””FILE COMMENT””******************************* Technical reference data ****
* File Name		: rtetypes.h
* Version 		: 1.00
* Device 		: Renesas Generic MCU.
* Tool Chain 	: HEW
* H/W Platform	: RSK Generic
* Description 	: RTE Type definitions
******************************************************************************/

/******************************************************************************
* History 		: 01.07.2009 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

#ifndef _RTETYPES_H_INCLUDED_
#define _RTETYPES_H_INCLUDED_


/***********************************************************************************
 System Includes
***********************************************************************************/

/***********************************************************************************
 User Includes
***********************************************************************************/

/***********************************************************************************
 Defines
***********************************************************************************/
#ifndef NULL                        /* set null ((void *)0) */
#define NULL            (void*)0
#endif


typedef void VOID; /* void return type from functions */

/*
 * Define our basic data types depending upon
 * the toolchain and the processor type.
 */


#define _SBYTE    _SBYTE
typedef signed char _SBYTE;

#define _UBYTE    _UBYTE
typedef unsigned char _UBYTE;

#define _SWORD    _SWORD
typedef signed short _SWORD;

#define _UWORD    _UWORD
typedef unsigned short _UWORD;

#define _SINT    _SINT
typedef signed int _SINT;

#define _UINT    _UINT
typedef unsigned int _UINT;

#define _SDWORD    _SDWORD
typedef signed long _SDWORD;

#define _UDWORD    _UDWORD
typedef unsigned long _UDWORD;

/*
#define _SQWORD    _SQWORD
typedef signed long long _SQWORD;

#define _UQWORD    _UQWORD
typedef unsigned long long _UQWORD;
*/


#define BYTE    BYTE
typedef unsigned char BYTE;

#define WORD    WORD
typedef unsigned short WORD;

#define DWORD   DWORD
typedef unsigned long DWORD;

#define USHORT    USHORT
typedef unsigned short USHORT;

#define ULONG    ULONG
typedef unsigned long ULONG;

#define INT8    INT8
typedef signed char INT8;

#define INT16    INT16
typedef signed short INT16;

#define INT32    INT32
typedef signed long INT32;


#define UINT8    UINT8
typedef unsigned char UINT8;

#define UINT16    UINT16
typedef unsigned short UINT16;

#define UINT32    UINT32
typedef unsigned long UINT32;

/***********************************************************************************
 Constant Macros
***********************************************************************************/


/***********************************************************************************
 Function Macros
***********************************************************************************/


/***********************************************************************************
 Typedefs
***********************************************************************************/


/***********************************************************************************
 Enumerated Types
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/


#endif /* _RTETYPES_H_INCLUDED_ */

