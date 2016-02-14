/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
*  Header File for ADC(12bit) Driver
 */

#ifndef _ADC12DEV_FM3_H_
#define _ADC12DEV_FM3_H_

/*
 *  Select Input Signal of Priority Converter
 */
#define Adc12Dev_UNIT0_SCAN_TRG_TIMER    Adc12Dev_SCAN_TRG_DISABLE
#define Adc12Dev_UNIT1_SCAN_TRG_TIMER    Adc12Dev_SCAN_TRG_DISABLE
#define Adc12Dev_UNIT2_SCAN_TRG_TIMER    Adc12Dev_SCAN_TRG_DISABLE

#define Adc12Dev_UNIT0_PRI_TRG_TIMER     Adc12Dev_PRI_TRG_DISABLE
#define Adc12Dev_UNIT1_PRI_TRG_TIMER     Adc12Dev_PRI_TRG_DISABLE
#define Adc12Dev_UNIT2_PRI_TRG_TIMER     Adc12Dev_PRI_TRG_DISABLE


#define Adc12Dev_SCAN_TRG_DISABLE   0U
#define Adc12Dev_SCAN_TRG_MFT       1U
#define Adc12Dev_SCAN_TRG_BT0       2U
#define Adc12Dev_SCAN_TRG_BT1       3U
#define Adc12Dev_SCAN_TRG_BT2       4U
#define Adc12Dev_SCAN_TRG_BT3       5U
#define Adc12Dev_SCAN_TRG_BT4       6U
#define Adc12Dev_SCAN_TRG_BT5       7U
#define Adc12Dev_SCAN_TRG_BT6       8U
#define Adc12Dev_SCAN_TRG_BT7       9U

#define Adc12Dev_PRI_TRG_DISABLE    0U
#define Adc12Dev_PRI_TRG_MFT        1U
#define Adc12Dev_PRI_TRG_BT0        2U
#define Adc12Dev_PRI_TRG_BT1        3U
#define Adc12Dev_PRI_TRG_BT2        4U
#define Adc12Dev_PRI_TRG_BT3        5U
#define Adc12Dev_PRI_TRG_BT4        6U
#define Adc12Dev_PRI_TRG_BT5        7U
#define Adc12Dev_PRI_TRG_BT6        8U
#define Adc12Dev_PRI_TRG_BT7        9U


#define Adc12Dev_UNIT0_PRI1_INPUT_SELECT      Adc12Dev_PRI1_INPUT_SEL_PCIS
#define Adc12Dev_UNIT1_PRI1_INPUT_SELECT      Adc12Dev_PRI1_INPUT_SEL_PCIS
#define Adc12Dev_UNIT2_PRI1_INPUT_SELECT      Adc12Dev_PRI1_INPUT_SEL_PCIS

#define Adc12Dev_PRI1_INPUT_SEL_PCIS    0x00U
#define Adc12Dev_PRI1_INPUT_SEL_ECS     0x08U

/*
 *  Driver Status
 */
#define ADC12_UNINITIALIZED    0
#define ADC12_INITIALIZED      1

/*
 *  Default Value
 */
#define ADC12_SCAN_CHSEL        0U
#define ADC12_SCAN_REPEAT       Adc12Dev_SINGLE_MODE
#define ADC12_SCAN_FIFOALC      Adc12Dev_DATA_LSB
#define ADC12_SCAN_FIFONUM      Adc12Dev_SCAN_FIFONUM1
#define ADC12_PRI1_CHSEL        0U
#define ADC12_PRI2_CHSEL        0U
#define ADC12_PRI_EXACT         0U
#define ADC12_PRI_FIFONUM       Adc12Dev_PRI_FIFONUM1
#define ADC12_CMP_CH            0U
#define ADC12_CMP_VALUE         0U
#define ADC12_CMP_TIME          Adc12Dev_COMP_TIME_DIV2
#define ADC12_CMP_ENABLE        0U
#define ADC12_CMP_MODE0         Adc12Dev_COMP_SPEC_CH
#define ADC12_CMP_MODE1         Adc12Dev_COMP_SMALLER_INT
#define ADC12_INTENABLE         0U
#define ADC12_SAMP_TIMEN0       Adc12Dev_SAMP_TIME_X1
#define ADC12_SAMP_TIME0        0x10U
#define ADC12_SAMP_TIMEN1       Adc12Dev_SAMP_TIME_X1
#define ADC12_SAMP_TIME1        0x10U
#define ADC12_SAMP_TIMESEL      0U


#define ADC12_SCAN_MAX_FIFONUM  16U
#define ADC12_PRI_MAX_FIFONUM    4U

/*
 *  Register Bit Definition
 */
#define ADCR_OVRIE           0x01U
#define ADCR_CMPIE           0x02U
#define ADCR_PCIE            0x04U
#define ADCR_SCIE            0x08U
#define ADCR_CMPIF           0x20U
#define ADCR_PCIF            0x40U
#define ADCR_SCIF            0x80U
#define ADCR_IFALL           (ADCR_CMPIF | ADCR_PCIF | ADCR_SCIF)

#define ADSR_SCS             0x01U
#define ADSR_PCS             0x02U
#define ADSR_PCNS            0x04U
#define ADSR_FDAS            0x40U
#define ADSR_ADSTP           0x80U

#define SCCR_SSTR            0x01U
#define SCCR_SHEN            0x02U
#define SCCR_RPT             0x04U
#define SCCR_SFCLR           0x10U
#define SCCR_SOVR            0x20U
#define SCCR_SFUL            0x40U
#define SCCR_SEMP            0x80U
#define SCFDL_INVL           0x1000U

#define PCCR_PSTR            0x01U
#define PCCR_PHEN            0x02U
#define PCCR_PEEN            0x04U
#define PCCR_ESCE            0x08U
#define PCCR_PFCLR           0x10U
#define PCCR_POVR            0x20U
#define PCCR_PFUL            0x40U
#define PCCR_PEMP            0x80U
#define PCFDL_INVL           0x1000U

#define CMPCR_CMD0           0x20U
#define CMPCR_CMD1           0x40U
#define CMPCR_CMPEN          0x80U

#define ADCEN_ENBL           0x01U
#define ADCEN_READY          0x02U

/*
 *  Enable/Disable Interrupt Controller
 */
/* deviation from MISRA-C:2004 Rule 19.7 */
#define ADC12_ENABLE_IRQ(n)                 NVIC_EnableIRQ((n))
#define ADC12_DISABLE_IRQ(n)                NVIC_DisableIRQ((n))

/******************************************************************************
 *  Function Prototype declaration
 *****************************************************************************/
void ADC0_IRQHandler(void);
void ADC1_IRQHandler(void);
void ADC2_IRQHandler(void);

#endif /* _ADC12DEV_FM3_H_ */
