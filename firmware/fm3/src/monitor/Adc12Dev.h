/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  Header File for ADC(12bit) Driver
 */

#ifndef _ADC12DEV_H_
#define _ADC12DEV_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  Device Defines
 */
#define Adc12Dev_SCAN_CH0           0x00000001U
#define Adc12Dev_SCAN_CH1           0x00000002U
#define Adc12Dev_SCAN_CH2           0x00000004U
#define Adc12Dev_SCAN_CH3           0x00000008U
#define Adc12Dev_SCAN_CH4           0x00000010U
#define Adc12Dev_SCAN_CH5           0x00000020U
#define Adc12Dev_SCAN_CH6           0x00000040U
#define Adc12Dev_SCAN_CH7           0x00000080U
#define Adc12Dev_SCAN_CH8           0x00000100U
#define Adc12Dev_SCAN_CH9           0x00000200U
#define Adc12Dev_SCAN_CH10          0x00000400U
#define Adc12Dev_SCAN_CH11          0x00000800U
#define Adc12Dev_SCAN_CH12          0x00001000U
#define Adc12Dev_SCAN_CH13          0x00002000U
#define Adc12Dev_SCAN_CH14          0x00004000U
#define Adc12Dev_SCAN_CH15          0x00008000U

#define Adc12Dev_SINGLE_MODE        0U
#define Adc12Dev_REPEAT_MODE        1U

#define Adc12Dev_DATA_MSB           0U
#define Adc12Dev_DATA_LSB           1U

#define Adc12Dev_SCAN_FIFONUM1       0U
#define Adc12Dev_SCAN_FIFONUM2       1U
#define Adc12Dev_SCAN_FIFONUM3       2U
#define Adc12Dev_SCAN_FIFONUM4       3U
#define Adc12Dev_SCAN_FIFONUM5       4U
#define Adc12Dev_SCAN_FIFONUM6       5U
#define Adc12Dev_SCAN_FIFONUM7       6U
#define Adc12Dev_SCAN_FIFONUM8       7U
#define Adc12Dev_SCAN_FIFONUM9       8U
#define Adc12Dev_SCAN_FIFONUM10      9U
#define Adc12Dev_SCAN_FIFONUM11     10U
#define Adc12Dev_SCAN_FIFONUM12     11U
#define Adc12Dev_SCAN_FIFONUM13     12U
#define Adc12Dev_SCAN_FIFONUM14     13U
#define Adc12Dev_SCAN_FIFONUM15     14U
#define Adc12Dev_SCAN_FIFONUM16     15U

#define Adc12Dev_PRI_EX_ACT_ENABLE  1U

#define Adc12Dev_PRI_FIFONUM1       0U
#define Adc12Dev_PRI_FIFONUM2       1U
#define Adc12Dev_PRI_FIFONUM3       2U
#define Adc12Dev_PRI_FIFONUM4       3U

#define Adc12Dev_COMP_TIME_DIV2     0U
#define Adc12Dev_COMP_TIME_DIV3     1U
#define Adc12Dev_COMP_TIME_DIV4     2U
#define Adc12Dev_COMP_TIME_DIV5     3U
#define Adc12Dev_COMP_TIME_DIV6     4U
#define Adc12Dev_COMP_TIME_DIV7     5U
#define Adc12Dev_COMP_TIME_DIV8     6U
#define Adc12Dev_COMP_TIME_DIV9     7U

#define Adc12Dev_COMP_ENABLE        1U

#define Adc12Dev_COMP_SPEC_CH       0U
#define Adc12Dev_COMP_ALL_CH        1U

#define Adc12Dev_COMP_SMALLER_INT   0U
#define Adc12Dev_COMP_GREATER_INT   1U

#define Adc12Dev_OVR_INT_ENABLE     1U
#define Adc12Dev_COMP_INT_ENABLE    2U
#define Adc12Dev_PRI_INT_ENABLE     4U
#define Adc12Dev_SCAN_INT_ENABLE    8U

#define Adc12Dev_SAMP_TIME_X1       0U
#define Adc12Dev_SAMP_TIME_X4       1U
#define Adc12Dev_SAMP_TIME_X8       2U
#define Adc12Dev_SAMP_TIME_X16      3U
#define Adc12Dev_SAMP_TIME_X32      4U
#define Adc12Dev_SAMP_TIME_X64      5U
#define Adc12Dev_SAMP_TIME_X128     6U
#define Adc12Dev_SAMP_TIME_X256     7U

#define Adc12Dev_SAMP_TIME1_CH0     0x00000001U
#define Adc12Dev_SAMP_TIME1_CH1     0x00000002U
#define Adc12Dev_SAMP_TIME1_CH2     0x00000004U
#define Adc12Dev_SAMP_TIME1_CH3     0x00000008U
#define Adc12Dev_SAMP_TIME1_CH4     0x00000010U
#define Adc12Dev_SAMP_TIME1_CH5     0x00000020U
#define Adc12Dev_SAMP_TIME1_CH6     0x00000040U
#define Adc12Dev_SAMP_TIME1_CH7     0x00000080U
#define Adc12Dev_SAMP_TIME1_CH8     0x00000100U
#define Adc12Dev_SAMP_TIME1_CH9     0x00000200U
#define Adc12Dev_SAMP_TIME1_CH10    0x00000400U
#define Adc12Dev_SAMP_TIME1_CH11    0x00000800U
#define Adc12Dev_SAMP_TIME1_CH12    0x00001000U
#define Adc12Dev_SAMP_TIME1_CH13    0x00002000U
#define Adc12Dev_SAMP_TIME1_CH14    0x00004000U
#define Adc12Dev_SAMP_TIME1_CH15    0x00008000U


#define Adc12Dev_SCAN_ACT           0U
#define Adc12Dev_PRI2_ACT           1U

#define Adc12Dev_SCAN_FIFO          0U
#define Adc12Dev_PRI_FIFO           1U

#define Adc12Dev_IRQSTATUS_SCAN     0x01U
#define Adc12Dev_IRQSTATUS_PRI      0x02U
#define Adc12Dev_IRQSTATUS_COMP     0x04U
#define Adc12Dev_IRQSTATUS_SOVR     0x08U
#define Adc12Dev_IRQSTATUS_POVR     0x10U


/*
 *  ADC(12bit) Device Configuration Structure
 */
typedef struct {
    uint32_t  ScanChSel:32;         /* SCAN Ch Select     */
    uint32_t  ScanRepeat:1;         /* SCAN Repeat Set    */
    uint32_t  ScanFifoAlloc:4;      /* SCAN FIFO Allocate */
    uint32_t  ScanFifoNum:4;        /* SCAN FIFO Number   */
    uint32_t  Pri1ChSel:3;          /* PRI1 Ch Select     */
    uint32_t  Pri2ChSel:5;          /* PRI2 Ch Select     */
    uint32_t  PriExAct:1;           /* PRI External Act   */
    uint32_t  PriFifoNum:2;         /* PRI FIFO Number    */
    uint32_t  CompCh:5;             /* Compare Ch         */
    uint32_t  CompValue:10;         /* Compare Value      */
    uint32_t  CompTime:8;           /* Compare Time       */
    uint32_t  CompEnable:1;         /* Compare Enable     */
    uint32_t  CompMode0:1;          /* Compare Mode 0     */
    uint32_t  CompMode1:1;          /* Compare Mode 1     */
    uint32_t  IntEnable:5;          /* Interrupt Enable   */
    uint32_t  SampTimeN0:3;         /* Sampling Time N0   */
    uint32_t  SampTime0:5;          /* Sampling Time 0    */
    uint32_t  SampTimeN1:3;         /* Sampling Time N1   */
    uint32_t  SampTime1:5;          /* Sampling Time 1    */
    uint32_t  SampTimeSel:32;       /* Sampling Time Sel  */
} Adc12Dev_CFG;


/*
 *  ADC(12bit) Device IO Block Structure
 */
typedef struct {
    Adc12Dev_CFG  Cfg;
    int32_t  (*Init)(void);
    int32_t  (*UnInit)(void);
    int32_t  (*SetSWTrigger)(uint8_t Flag);
    int32_t  (*Stop)(uint8_t Flag);
    int32_t  (*ClearFifo)(uint8_t Fifo);
    int32_t  (*Read)(uint16_t *pData, uint16_t *pInfo, uint8_t *pFifoNum, uint8_t Fifo);
    void  (*CallBackIrq)(int32_t Status);
}Adc12Dev_IOB;


#ifdef __cplusplus
}
#endif

#endif /* _ADC12DEV_H_ */
