/**
* @file          common.h
* @brief         commom definition
* @author        jialili
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __COMMON_INC__
#define __COMMON_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "string.h"
#include "cpu_config.h"
#include "typedef.h"

/*----------------------------------------------------------------------------*/
/* Constant Definition                                                        */
/*----------------------------------------------------------------------------*/
#ifndef TRUE
 #define TRUE           (1)
#endif
#ifndef FALSE
 #define FALSE          (0)
#endif
#ifndef NULL
 #define NULL           ((void *)0)
#endif
#ifndef ON
 #define ON             (1)
#endif
#ifndef OFF
 #define OFF            (0)
#endif
#ifndef HIGH
 #define HIGH           (1)
#endif
#ifndef LOW
 #define LOW            (0)
#endif

#ifndef BIT_SET
 #define BIT_SET           (1)
#endif

#ifndef BIT_CLR
 #define BIT_CLR           (0)
#endif

#define BIT0            0x00000001
#define BIT1            0x00000002
#define BIT2            0x00000004
#define BIT3            0x00000008
#define BIT4            0x00000010
#define BIT5            0x00000020
#define BIT6            0x00000040
#define BIT7            0x00000080
#define BIT8            0x00000100
#define BIT9            0x00000200
#define BIT10           0x00000400
#define BIT11           0x00000800
#define BIT12           0x00001000
#define BIT13           0x00002000
#define BIT14           0x00004000
#define BIT15           0x00008000
#define BIT16           0x00010000
#define BIT17           0x00020000
#define BIT18           0x00040000
#define BIT19           0x00080000
#define BIT20           0x00100000
#define BIT21           0x00200000
#define BIT22           0x00400000
#define BIT23           0x00800000
#define BIT24           0x01000000
#define BIT25           0x02000000
#define BIT26           0x04000000
#define BIT27           0x08000000
#define BIT28           0x10000000
#define BIT29           0x20000000
#define BIT30           0x40000000
#define BIT31           0x80000000

/*----------------------------------------------------------------------------*/
/* endian convert macro                                                       */
/*----------------------------------------------------------------------------*/
/* round float to integer */
/* eg: 0.4 -> 0 */
/*     0.6 -> 1 */
#define ROUND(value)                   ((uint32_t)((f32_t)(value) + 0.5))

/* get lower 4 bit of a byte data */
#define LONIBBLE(b)    ((uint8_t) ((uint8_t) (b) & 0x0F))
/* get upper 4 bit of a byte data */
#define HINIBBLE(b)    ((uint8_t) ((uint8_t) (b) >> 4))
/* get lower 1 byte of 16 bit data */
#define LOBYTE(hw)     ((uint8_t) ((uint16_t) (hw) & 0x00FF))
/* get upper 1 byte of 16 bit data */
#define HIBYTE(hw)     ((uint8_t) ((uint16_t) (hw) >> 8))
/* get lower 2 byte of 32 bit data */
#define LOHWORD(w)     ((uint16_t) ((uint32_t) (w) & 0x0000FFFF))
/* get upper 2 byte of 32 bit data */
#define HIHWORD(w)     ((uint16_t) ((uint32_t) (w) >> 16))

/* make 1 byte data from upper 4 bit and lower 4 bit data */
#define MAKEBYTE(nH, nL)    ((uint8_t)  ((((uint8_t) (nH)) << 4U) | ((uint8_t) (nL))))
/* make 2 byte data from upper 1 byte and lower 1 byte data */
#define MAKEHWORD(bH, bL)   ((uint16_t) ((((uint16_t) (bH)) << 8U) | ((uint8_t) (bL))))
/* make 4 byte data from upper 2 byte and lower 2 byte data */
#define MAKEWORD(hwH, hwL)  ((uint32_t) ((((uint32_t) (hwH)) << 16U) | ((uint16_t) (hwL))))

/* swap upper and lower of 16 bit data */
#define SWAP_HWORD(hw) (((hw) >> 8) | ((uint16_t) ((hw) << 8)))
/* swap upper and lower of 32 bit data */
#define SWAP_WORD(w)  (((w) >> 24) | (((w) >> 8) & 0xFF00) | (((w) << 8) & 0xff0000) | ((w) << 24))

/* swap endian of 16 bit data */
#define SWAP_ENDIAN(hw)  \
{\
  ((uint8_t*)&(hw))[0] ^= ((uint8_t*)&(hw))[1];\
  ((uint8_t*)&(hw))[1] ^= ((uint8_t*)&(hw))[0];\
  ((uint8_t*)&(hw))[0] ^= ((uint8_t*)&(hw))[1];\
}

#if (CPU_ENDIAN == CPU_LITTLEENDIAN)
#define HWORD_CPU_TO_LE(hw) (hw)           /* convert 16 bit data from cpu endian to little endian */
#define WORD_CPU_TO_LE(w)   (w)            /* convert 32 bit data from cpu endian to little endian */
#define HWORD_LE_to_CPU(hw) (hw)           /* convert 16 bit data from little endian to cpu endian */
#define WORD_LE_to_CPU(w)   (w)            /* convert 32 bit data from little endian to cpu endian */
#define HWORD_CPU_TO_BE(hw) SWAP_HWORD(hw) /* convert 16 bit data from cpu endian to big endian    */
#define WORD_CPU_TO_BE(w)   SWAP_WORD(w)   /* convert 32 bit data from cpu endian to big endian    */
#define HWORD_BE_to_CPU(hw) SWAP_HWORD(hw) /* convert 16 bit data from big endian to cpu endian    */
#define WORD_BE_to_CPU(w)   SWAP_WORD(w)   /* convert 32 bit data from big endian to cpu endian    */
#else
#define HWORD_CPU_TO_BE(hw) (hw)           /* convert 16 bit data from cpu endian to big endian    */
#define WORD_CPU_TO_BE(w)   (w)            /* convert 32 bit data from cpu endian to big endian    */
#define HWORD_BE_to_CPU(hw) (hw)           /* convert 16 bit data from big endian to cpu endian    */
#define WORD_BE_to_CPU(w)   (w)            /* convert 32 bit data from big endian to cpu endian    */
#define HWORD_CPU_TO_LE(hw) SWAP_HWORD(hw) /* convert 16 bit data from cpu endian to little endian */
#define WORD_CPU_TO_LE(w)   SWAP_WORD(w)   /* convert 32 bit data from cpu endian to little endian */
#define HWORD_LE_to_CPU(hw) SWAP_HWORD(hw) /* convert 16 bit data from little endian to cpu endian */
#define WORD_LE_to_CPU(w)   SWAP_WORD(w)   /* convert 32 bit data from little endian to cpu endian */
#endif

/* converter function from little endian byte buffer to 16 bit integer */
#define LE_BUFFER_TO_HWORD(bytePtr)\
        (uint16_t) (((uint16_t)*(uint8_t*)((bytePtr))) | (((uint16_t)*(uint8_t*)((bytePtr) + 1) << 8) & 0xff00))

/* converter function from 16 bit integer to little endian byte buffer */
#define HWORD_TO_LE_BUFFER(bytePtr, data)\
        {*(bytePtr) = (uint8_t)(data);\
         *((bytePtr) + 1) = (uint8_t)((data) >> 8);\
        }

/* converter function from big endian byte buffer to 16 bit integer */
#define BE_BUFFER_TO_HWORD(bytePtr)\
        (uint16_t) (((uint16_t)*(uint8_t*)((bytePtr) + 1)) | (((uint16_t)*(uint8_t*)(bytePtr) << 8) & 0xff00))

/* converter function from 16 bit integer to big endian byte buffer */
#define HWORD_TO_BE_BUFFER(bytePtr, data)\
        {*((bytePtr) + 1) = (uint8_t)(data);\
         *(bytePtr) = (uint8_t)((data) >> 8);\
        }

/* converter function from little endian byte buffer to 32 bit integer */
#define LE_BUFFER_TO_WORD(bytePtr)\
        (uint32_t)(  ((uint32_t)*  (uint8_t*)(bytePtr)                             ) |\
                   ((((uint32_t)*(((uint8_t*)(bytePtr)) + 1)) << 8 ) & 0xff00UL    ) |\
                   ((((uint32_t)*(((uint8_t*)(bytePtr)) + 2)) << 16) & 0xff0000UL  ) |\
                   ((((uint32_t)*(((uint8_t*)(bytePtr)) + 3)) << 24) & 0xff000000UL)  \
                  )

/* converter function from 32 bit integer to little endian byte buffer */
#define WORD_TO_LE_BUFFER(bytePtr, data)\
        { *((bytePtr))     = (uint8_t)((uint32_t)(data)     ) ;\
          *((bytePtr) + 1) = (uint8_t)((uint32_t)(data) >> 8) ;\
          *((bytePtr) + 2) = (uint8_t)((uint32_t)(data) >> 16);\
          *((bytePtr) + 3) = (uint8_t)((uint32_t)(data) >> 24);\
        }

/* converter function from big endian byte buffer to 32 bit integer */
#define BE_BUFFER_TO_WORD(bytePtr)\
        (uint32_t)(   ((uint32_t)*(uint8_t*)((bytePtr) + 3)) |\
                    ((((uint32_t)*(uint8_t*)((bytePtr) + 2)) << 8  ) & 0xff00UL     ) |\
                    ((((uint32_t)*(uint8_t*)((bytePtr) + 1)) << 16 ) & 0xff0000UL   ) |\
                    ((((uint32_t)*(uint8_t*) (bytePtr)     ) << 24 ) & 0xff000000UL )  \
                  )

/* converter function from 32 bit integer to big endian byte buffer */
#define WORD_TO_BE_BUFFER(bytePtr, data)\
        { *((bytePtr) + 3) = (uint8_t)((uint32_t)(data)      );\
          *((bytePtr) + 2) = (uint8_t)((uint32_t)(data) >> 8 );\
          *((bytePtr) + 1) = (uint8_t)((uint32_t)(data) >> 16);\
          * (bytePtr)      = (uint8_t)((uint32_t)(data) >> 24);\
        }

#endif
