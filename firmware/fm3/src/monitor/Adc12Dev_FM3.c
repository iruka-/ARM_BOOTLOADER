/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  ADC(12bit) Driver
 */

#include "../mcu.h"
#include "Adc12Dev.h"
#include "Adc12Dev_FM3.h"

/*
 *  Driver Information Control Struct
 */
typedef struct {
    int32_t       State;      /* Status           */
    uint8_t       ScanTrgTim; /* SCAN TRG Timer   */
    uint8_t       PriTrgTim;  /* PRI TRG Timer    */
    uint8_t       Esce;       /* PCCR ESCE bit    */
    FM3_ADC_TypeDef   *pReg;      /* Register Struct  */
    Adc12Dev_IOB  *pIOB;      /* IOB Struct       */
    uint16_t      *pData;     /* Data             */
    uint16_t      *pInfo;     /* Info             */
    uint8_t       *pFifoNum;  /* Fifo Number      */
    IRQn_Type     Adc12_IRQn; /* IRQ Number       */
} Adc12_INFO;

/******************************************************************************
 *  Function Prototype declaration
 *****************************************************************************/
static int32_t adc12_Init(Adc12_INFO *pInfo);
static void adc12_UnInit(Adc12_INFO *pInfo);
static void adc12_SetSWTrigger(Adc12_INFO *pInfo, uint8_t Flag);
static void adc12_Stop(Adc12_INFO *pInfo);
static void adc12_Abort(Adc12_INFO *pInfo);
static void adc12_ClearFifo(Adc12_INFO *pInfo, uint8_t Fifo);
static void adc12_Read(Adc12_INFO *pInfo, uint8_t Fifo);
static void adc12_IrqHandler(uint8_t Unit, uint8_t Irq);

static int32_t Adc12Dev_Init(uint8_t Unit);
static int32_t Adc12Dev_UnInit(uint8_t Unit);
static int32_t Adc12Dev_SetSWTrigger(uint8_t Unit, uint8_t Flag);
static int32_t Adc12Dev_Stop(uint8_t Unit, uint8_t Flag);
static int32_t Adc12Dev_ClearFifo(uint8_t Unit, uint8_t Fifo);
static int32_t Adc12Dev_Read(uint8_t Unit, uint16_t *pData, uint16_t *pInfo, uint8_t *pFifoNum,
                           uint8_t Fifo);

/* Unit0 */
static int32_t Adc12Dev_Unit0_Init(void);
static int32_t Adc12Dev_Unit0_UnInit(void);
static int32_t Adc12Dev_Unit0_SetSWTrigger(uint8_t Flag);
static int32_t Adc12Dev_Unit0_Stop(uint8_t Flag);
static int32_t Adc12Dev_Unit0_ClearFifo(uint8_t Fifo);
static int32_t Adc12Dev_Unit0_Read(uint16_t *pData, uint16_t *pInfo, uint8_t *pFifoNum,
                                 uint8_t Fifo);

/* Unit1 */
static int32_t Adc12Dev_Unit1_Init(void);
static int32_t Adc12Dev_Unit1_UnInit(void);
static int32_t Adc12Dev_Unit1_SetSWTrigger(uint8_t Flag);
static int32_t Adc12Dev_Unit1_Stop(uint8_t Flag);
static int32_t Adc12Dev_Unit1_ClearFifo(uint8_t Fifo);
static int32_t Adc12Dev_Unit1_Read(uint16_t *pData, uint16_t *pInfo, uint8_t *pFifoNum,
                                 uint8_t Fifo);

/* Unit2 */
static int32_t Adc12Dev_Unit2_Init(void);
static int32_t Adc12Dev_Unit2_UnInit(void);
static int32_t Adc12Dev_Unit2_SetSWTrigger(uint8_t Flag);
static int32_t Adc12Dev_Unit2_Stop(uint8_t Flag);
static int32_t Adc12Dev_Unit2_ClearFifo(uint8_t Fifo);
static int32_t Adc12Dev_Unit2_Read(uint16_t *pData, uint16_t *pInfo, uint8_t *pFifoNum,
                                 uint8_t Fifo);


/*
 *  IOB Unit Information
 */
/* deviation from MISRA-C:2004 Rule 8.8 */
Adc12Dev_IOB Adc12Dev[] = {
  /* {{     ScanChSel,      ScanRepeat,    ScanFifoAlloc,
            ScanFifoNum,      Pri1ChSel,      Pri2ChSel,      PriExAct,
            PriFifoNum,     CompCh,     CompValue,     CompTime,
           CompEnable,     CompMode0,     CompMode1,     IntEnable,
            SampTimeN0,      SampTime0,      SampTimeN1,      SampTime1,
            SampTimeSel},
                    Init,              UnInit,              SetSWTrigger,
                    Stop,              ClearFifo,              Read,
       CallBackIrq},*/
/*0*/{{ADC12_SCAN_CHSEL, ADC12_SCAN_REPEAT, ADC12_SCAN_FIFOALC,
       ADC12_SCAN_FIFONUM, ADC12_PRI1_CHSEL, ADC12_PRI2_CHSEL, ADC12_PRI_EXACT,
       ADC12_PRI_FIFONUM, ADC12_CMP_CH, ADC12_CMP_VALUE, ADC12_CMP_TIME,
       ADC12_CMP_ENABLE, ADC12_CMP_MODE0, ADC12_CMP_MODE1, ADC12_INTENABLE,
       ADC12_SAMP_TIMEN0, ADC12_SAMP_TIME0, ADC12_SAMP_TIMEN1, ADC12_SAMP_TIME1,
       ADC12_SAMP_TIMESEL},
       Adc12Dev_Unit0_Init, Adc12Dev_Unit0_UnInit, Adc12Dev_Unit0_SetSWTrigger,
       Adc12Dev_Unit0_Stop, Adc12Dev_Unit0_ClearFifo, Adc12Dev_Unit0_Read,
              NULL},
/*1*/{{ADC12_SCAN_CHSEL, ADC12_SCAN_REPEAT, ADC12_SCAN_FIFOALC,
       ADC12_SCAN_FIFONUM, ADC12_PRI1_CHSEL, ADC12_PRI2_CHSEL, ADC12_PRI_EXACT,
       ADC12_PRI_FIFONUM, ADC12_CMP_CH, ADC12_CMP_VALUE, ADC12_CMP_TIME,
       ADC12_CMP_ENABLE, ADC12_CMP_MODE0, ADC12_CMP_MODE1, ADC12_INTENABLE,
       ADC12_SAMP_TIMEN0, ADC12_SAMP_TIME0, ADC12_SAMP_TIMEN1, ADC12_SAMP_TIME1,
       ADC12_SAMP_TIMESEL},
       Adc12Dev_Unit1_Init, Adc12Dev_Unit1_UnInit, Adc12Dev_Unit1_SetSWTrigger,
       Adc12Dev_Unit1_Stop, Adc12Dev_Unit1_ClearFifo, Adc12Dev_Unit1_Read,
              NULL},
/*2*/{{ADC12_SCAN_CHSEL, ADC12_SCAN_REPEAT, ADC12_SCAN_FIFOALC,
       ADC12_SCAN_FIFONUM, ADC12_PRI1_CHSEL, ADC12_PRI2_CHSEL, ADC12_PRI_EXACT,
       ADC12_PRI_FIFONUM, ADC12_CMP_CH, ADC12_CMP_VALUE, ADC12_CMP_TIME,
       ADC12_CMP_ENABLE, ADC12_CMP_MODE0, ADC12_CMP_MODE1, ADC12_INTENABLE,
       ADC12_SAMP_TIMEN0, ADC12_SAMP_TIME0, ADC12_SAMP_TIMEN1, ADC12_SAMP_TIME1,
       ADC12_SAMP_TIMESEL},
       Adc12Dev_Unit2_Init, Adc12Dev_Unit2_UnInit, Adc12Dev_Unit2_SetSWTrigger,
       Adc12Dev_Unit2_Stop, Adc12Dev_Unit2_ClearFifo, Adc12Dev_Unit2_Read,
              NULL},
};

/*
 *  Driver Unit Information
 */
/* deviation from MISRA-C:2004 Rule 11.3 */
static Adc12_INFO Adc12Info[] = {
   /*{            State,                  ScanTrgTim,
                       PriTrgTim,                           Esce,
      pReg,       pIOB, pData, pInfo, pFlag,  Adc12_IRQn}
*/
/*0*/{ADC12_UNINITIALIZED, Adc12Dev_UNIT0_SCAN_TRG_TIMER,
      Adc12Dev_UNIT0_PRI_TRG_TIMER, Adc12Dev_UNIT0_PRI1_INPUT_SELECT,
      FM3_ADC0, &Adc12Dev[0],  NULL,  NULL,  NULL, ADC0_IRQn},
/*1*/{ADC12_UNINITIALIZED, Adc12Dev_UNIT1_SCAN_TRG_TIMER,
      Adc12Dev_UNIT1_PRI_TRG_TIMER, Adc12Dev_UNIT1_PRI1_INPUT_SELECT,
      FM3_ADC1, &Adc12Dev[1],  NULL,  NULL,  NULL, ADC1_IRQn},
/*2*/{ADC12_UNINITIALIZED, Adc12Dev_UNIT2_SCAN_TRG_TIMER,
      Adc12Dev_UNIT2_PRI_TRG_TIMER, Adc12Dev_UNIT2_PRI1_INPUT_SELECT,
      FM3_ADC2, &Adc12Dev[2],  NULL,  NULL,  NULL, ADC2_IRQn},
};


/******************************************************************************
 *  Internal Function
 *****************************************************************************/
/*
 *  Initialize adc
 */
static int32_t adc12_Init(Adc12_INFO *pInfo)
{
    int32_t ret = 0;
    uint8_t sccr_shen;
    uint8_t pccr_phen;

#if	0	// ch16`ch31‚ðŽg—p‹ÖŽ~???
    /* Check Cfg parameter */
    if (pInfo->pIOB->Cfg.ScanChSel > 0x0000FFFFU) {
        ret = -1;
    }
#endif

    if (pInfo->pIOB->Cfg.CompCh > 0x0FU) {
        ret = -1;
    }

#if	0	// Type0‚Ìê‡‚Í 0`7
    if (pInfo->pIOB->Cfg.CompTime > 0x07U) {
        ret = -1;
    }
#else	// Type2‚Ìê‡‚Í 0x00`0x3F ‚à‚µ‚­‚Í 0x80
    if((pInfo->pIOB->Cfg.CompTime != 0x80)&&
       (pInfo->pIOB->Cfg.CompTime  > 0x3f)) {
        ret = -1;
    }
#endif

    if (pInfo->pIOB->Cfg.IntEnable > 0x0FU) {
        ret = -1;
    }
    
    if (pInfo->pIOB->Cfg.Pri2ChSel > 0x0FU) {
        ret = -1;
    }
    
    if (pInfo->pIOB->Cfg.SampTimeSel > 0x0000FFFFU) {
        ret = -1;
    }
    
    /* Check ScanTrgTim */
    if (pInfo->ScanTrgTim > 0x0A) {
        sccr_shen = 0;
        ret = -1;
    } else if (pInfo->ScanTrgTim != 0) {
        sccr_shen = 2;
    } else {
        sccr_shen = 0;
    }
    
    /* Check PriTrgTim */
    if (pInfo->PriTrgTim > 0x0A) {
        pccr_phen = 0;
        ret = -1;
    } else if (pInfo->PriTrgTim != 0) {
        pccr_phen = 2;
    } else {
        pccr_phen = 0;
    }
    
    if (ret == 0) {
        /* Set Input Channel to SCAN */
        pInfo->pReg->SCIS0 = (uint8_t)(pInfo->pIOB->Cfg.ScanChSel &
                                       0x000000FF);
        pInfo->pReg->SCIS1 = (uint8_t)((pInfo->pIOB->Cfg.ScanChSel >> 8) &
                                       0x000000FF);
        pInfo->pReg->SCIS2 = (uint8_t)((pInfo->pIOB->Cfg.ScanChSel >> 16) &
                                       0x000000FF);
        pInfo->pReg->SCIS3 = (uint8_t)((pInfo->pIOB->Cfg.ScanChSel >> 24) &
                                       0x000000FF);
        
        /* Set Input Channel to PRI */
        pInfo->pReg->PCIS = ((pInfo->pIOB->Cfg.Pri2ChSel << 3) |
                             pInfo->pIOB->Cfg.Pri1ChSel);
        
        /* Set Sampling Time */
        pInfo->pReg->ADST0 = ((pInfo->pIOB->Cfg.SampTimeN0 << 5) |
                              pInfo->pIOB->Cfg.SampTime0);
        
        pInfo->pReg->ADST1 = ((pInfo->pIOB->Cfg.SampTimeN1 << 5) |
                              pInfo->pIOB->Cfg.SampTime1);
        
        /* Set Sampling Time Select */
        pInfo->pReg->ADSS0 = (uint8_t)(pInfo->pIOB->Cfg.SampTimeSel &
                                       0x000000FF);
        pInfo->pReg->ADSS1 = (uint8_t)((pInfo->pIOB->Cfg.SampTimeSel >> 8) &
                                       0x000000FF);
        pInfo->pReg->ADSS2 = (uint8_t)((pInfo->pIOB->Cfg.SampTimeSel >> 16) &
                                       0x000000FF);
        pInfo->pReg->ADSS3 = (uint8_t)((pInfo->pIOB->Cfg.SampTimeSel >> 24) &
                                       0x000000FF);
        
        /* Set Compare Value */
        pInfo->pReg->ADCT = pInfo->pIOB->Cfg.CompTime;
        
        /* Set SCAN FIFO number */
        pInfo->pReg->SFNS = pInfo->pIOB->Cfg.ScanFifoNum;
        
        /* Set PRI FIFO number */
        pInfo->pReg->PFNS = pInfo->pIOB->Cfg.PriFifoNum;
        
        /* Set FIFO data allocation */
        pInfo->pReg->ADSR = pInfo->pIOB->Cfg.ScanFifoAlloc;
        
        /* Set Compare Data */
        pInfo->pReg->CMPD = (uint16_t)(pInfo->pIOB->Cfg.CompValue << 6);
        
        /* Set Compare Function */
        pInfo->pReg->CMPCR = ((pInfo->pIOB->Cfg.CompEnable << 7) |
                              (pInfo->pIOB->Cfg.CompMode1 << 6) |
                              (pInfo->pIOB->Cfg.CompMode0 << 5) |
                              pInfo->pIOB->Cfg.CompCh);
        
        /* Set Timer Trigger of SCAN */
        pInfo->pReg->SCTSL = pInfo->ScanTrgTim;
        
        /* Set Timer Trigger of PRI */
        pInfo->pReg->PRTSL = pInfo->PriTrgTim;
        
        /* Set Interrupt Enable */
        pInfo->pReg->ADCR = pInfo->pIOB->Cfg.IntEnable;
        
        /* Set SCAN Function */
        pInfo->pReg->SCCR = ((pInfo->pIOB->Cfg.ScanRepeat << 2) | SCCR_SFCLR |
                             sccr_shen);
        
        /* Set PRI Function */
        pInfo->pReg->PCCR = ((pInfo->pIOB->Cfg.PriExAct << 2) | PCCR_PFCLR |
                             pccr_phen | pInfo->Esce);
        
        /* Set ADC Enable */
        pInfo->pReg->ADCEN = ADCEN_ENBL;
        while((pInfo->pReg->ADCEN & ADCEN_READY) != ADCEN_READY) {
            /* Wait for READY bit = 1 */
            /* Memo : Add the time-out logic if necessary. */
        }
    }
    
    return ret;
}

/*
 *  Uninitialize adc
 */
static void adc12_UnInit(Adc12_INFO *pInfo)
{
    /* Set ADC Disable */
    pInfo->pReg->ADCEN = 0U;
    while((pInfo->pReg->ADCEN & ADCEN_READY) == ADCEN_READY) {
        /* Wait for READY bit = 0 */
        /* Memo : Add the time-out logic if necessary. */
    }
    
    /* Disable Timer Act of SCAN */
    pInfo->pReg->SCCR = 0U;
    
    /* Disable Timer Act of PRI */
    pInfo->pReg->PCCR = 0U;
    
    /* Check ADC stopped */
    if ((pInfo->pReg->ADSR & (ADSR_PCNS | ADSR_PCS | ADSR_SCS)) != 0U) {
        /* Abort A/D Converter */
        adc12_Abort(pInfo);
    }
    
    /* Disable Interrupt */
    pInfo->pReg->ADCR = 0;
    
    /* Initialize member of pInfo */
    pInfo->pData = NULL;
    pInfo->pFifoNum = NULL;
    pInfo->pIOB->Cfg.ScanChSel = ADC12_SCAN_CHSEL;
    pInfo->pIOB->Cfg.ScanRepeat = ADC12_SCAN_REPEAT;
    pInfo->pIOB->Cfg.ScanFifoAlloc = ADC12_SCAN_FIFOALC;
    pInfo->pIOB->Cfg.ScanFifoNum = ADC12_SCAN_FIFONUM;
    pInfo->pIOB->Cfg.Pri1ChSel = ADC12_PRI1_CHSEL;
    pInfo->pIOB->Cfg.Pri2ChSel = ADC12_PRI2_CHSEL;
    pInfo->pIOB->Cfg.PriExAct = ADC12_PRI_EXACT;
    pInfo->pIOB->Cfg.PriFifoNum = ADC12_PRI_FIFONUM;
    pInfo->pIOB->Cfg.CompCh = ADC12_CMP_CH;
    pInfo->pIOB->Cfg.CompValue = ADC12_CMP_VALUE;
    pInfo->pIOB->Cfg.CompTime = ADC12_CMP_TIME;
    pInfo->pIOB->Cfg.CompEnable = ADC12_CMP_ENABLE;
    pInfo->pIOB->Cfg.CompMode0 = ADC12_CMP_MODE0;
    pInfo->pIOB->Cfg.CompMode1 = ADC12_CMP_MODE1;
    pInfo->pIOB->Cfg.IntEnable = ADC12_INTENABLE;
    pInfo->pIOB->Cfg.SampTimeN0 = ADC12_SAMP_TIMEN0;
    pInfo->pIOB->Cfg.SampTime0 = ADC12_SAMP_TIME0;
    pInfo->pIOB->Cfg.SampTimeN1 = ADC12_SAMP_TIMEN1;
    pInfo->pIOB->Cfg.SampTime1 = ADC12_SAMP_TIME1;
    pInfo->pIOB->Cfg.SampTimeSel = ADC12_SAMP_TIMESEL;
    pInfo->pIOB->CallBackIrq = NULL;
}


/*
 *  Set Software Trigger
 */
static void adc12_SetSWTrigger(Adc12_INFO *pInfo, uint8_t Flag)
{
    if (Flag == Adc12Dev_SCAN_ACT) {
        pInfo->pReg->SCCR = (pInfo->pReg->SCCR | SCCR_SOVR | SCCR_SSTR);
    } else {       /* Flag == Adc12Dev_PRI2_ACT */
        pInfo->pReg->PCCR = (pInfo->pReg->PCCR | PCCR_POVR | PCCR_PSTR);
    }
}


/*
 *  Stop adc
 */
static void adc12_Stop(Adc12_INFO *pInfo)
{
    pInfo->pReg->SCCR = (pInfo->pReg->SCCR | SCCR_SOVR) & ~(SCCR_RPT);
}


/*
 *  Abort adc
 */
static void adc12_Abort(Adc12_INFO *pInfo)
{
    pInfo->pReg->ADSR = (pInfo->pReg->ADSR | ADSR_ADSTP);
}


/*
 *  Clear FIFO
 */
static void adc12_ClearFifo(Adc12_INFO *pInfo, uint8_t Fifo)
{
    if (Fifo == Adc12Dev_SCAN_FIFO) {
        pInfo->pReg->SCCR = (pInfo->pReg->SCCR | SCCR_SOVR | SCCR_SFCLR);
    } else {
        pInfo->pReg->PCCR = (pInfo->pReg->PCCR | PCCR_POVR | PCCR_PFCLR);
    }
}


/*
 *  Read FIFO data
 */
static void adc12_Read(Adc12_INFO *pInfo, uint8_t Fifo)
{
    int8_t i;
    int8_t cnt;
    
    if (Fifo == Adc12Dev_SCAN_FIFO) {
        
        cnt = (int8_t)(*(pInfo->pFifoNum));
        
        /* Read data */
        for (i = 0; i < cnt; i++) {
            /* Check InValid bit */
            if ((pInfo->pReg->SCFDL & SCFDL_INVL) == 0) {
                pInfo->pInfo[i] = pInfo->pReg->SCFDL;
                pInfo->pData[i] = pInfo->pReg->SCFDH;
            } else {
                break;
            }
        }
        
        /* Set Read size */
        *(pInfo->pFifoNum) = (uint8_t)i;
        
    } else {
        
        cnt = (int8_t)(*(pInfo->pFifoNum));
        
        /* Read data */
        for (i = 0; i < cnt; i++) {
            /* Check InValid bit */
            if ((pInfo->pReg->PCFDL & PCFDL_INVL) == 0) {
                pInfo->pInfo[i] = pInfo->pReg->PCFDL;
                pInfo->pData[i] = pInfo->pReg->PCFDH;
            } else {
                break;
            }
        }
        
        /* Set Read size */
        *(pInfo->pFifoNum) = (uint8_t)i;
        
    }
}


/*
 *  Interrupt Handler
 */
static void adc12_IrqHandler(uint8_t Unit, uint8_t Irq)
{
    if ((Irq & 0x01U) != 0) {  /* PRI Converter Interrupt */
        /* Clear Interrupt */
        Adc12Info[Unit].pReg->ADCR = (Adc12Info[Unit].pReg->ADCR | ADCR_IFALL) &
                                   ~(ADCR_PCIF);
        
        if (Adc12Info[Unit].pIOB->CallBackIrq != NULL) {
            /* Call CallBackIrq */
            Adc12Info[Unit].pIOB->CallBackIrq(Adc12Dev_IRQSTATUS_PRI);
        }
    }
    
    if ((Irq & 0x02U) != 0) {  /* SCAN Converter Interrupt */
        /* Clear Interrupt */
        Adc12Info[Unit].pReg->ADCR = (Adc12Info[Unit].pReg->ADCR | ADCR_IFALL) &
                                   ~(ADCR_SCIF);
        
        if (Adc12Info[Unit].pIOB->CallBackIrq != NULL) {
            /* Call CallBackIrq */
            Adc12Info[Unit].pIOB->CallBackIrq(Adc12Dev_IRQSTATUS_SCAN);
        }
    }
    
    if ((Irq & 0x04U) != 0) {  /* Over run Interrupt */
        /* Check interrupt */
        if ((Adc12Info[Unit].pReg->SCCR & SCCR_SOVR) != 0) {
            /* Clear Interrupt */
            Adc12Info[Unit].pReg->SCCR = (Adc12Info[Unit].pReg->SCCR &
                                        ~(SCCR_SOVR));
            
            if (Adc12Info[Unit].pIOB->CallBackIrq != NULL) {
                /* Call CallBackIrq */
                Adc12Info[Unit].pIOB->CallBackIrq(Adc12Dev_IRQSTATUS_SOVR);
            }
        }
        
        if ((Adc12Info[Unit].pReg->PCCR & PCCR_POVR) != 0) {
            /* Clear Interrupt */
            Adc12Info[Unit].pReg->PCCR = (Adc12Info[Unit].pReg->PCCR &
                                        ~(PCCR_POVR));
            
            if (Adc12Info[Unit].pIOB->CallBackIrq != NULL) {
                /* Call CallBackIrq */
                Adc12Info[Unit].pIOB->CallBackIrq(Adc12Dev_IRQSTATUS_POVR);
            }
        }
    }
    
    if ((Irq & 0x08U) != 0) {  /* Compare Interrupt */
        /* Clear Interrupt */
        Adc12Info[Unit].pReg->ADCR = (Adc12Info[Unit].pReg->ADCR | ADCR_IFALL) &
                                   ~(ADCR_CMPIF);
        
        if (Adc12Info[Unit].pIOB->CallBackIrq != NULL) {
            /* Call CallBackIrq */
            Adc12Info[Unit].pIOB->CallBackIrq(Adc12Dev_IRQSTATUS_COMP);
        }
    }
}


/******************************************************************************
 *  Unit Common Function
 *****************************************************************************/
/*
 *  Initialize Driver
 */
static int32_t Adc12Dev_Init(uint8_t Unit)
{
    int32_t ret;
    
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_UNINITIALIZED) {
        
        /* Call adc12_Init() */
        ret = adc12_Init(&Adc12Info[Unit]);
        
        if (ret == 0) {
            /* Interrupt Enable */
            ADC12_ENABLE_IRQ(Adc12Info[Unit].Adc12_IRQn);	
#if	1
			//Š„‚èž‚Ý—Dæ“x‚ÌÝ’è 0`15 (”’l‚ª¬‚³‚¢‚Ù‚Ç—Dæ)
            NVIC_SetPriority(Adc12Info[Unit].Adc12_IRQn, 8);
#endif
            /* Change State */
            Adc12Info[Unit].State = ADC12_INITIALIZED;
        }
    } else {
        /* error end */
        ret = -1;
    }
    
    return ret;
}


/*
 *  Uninitialize Driver
 */
static int32_t Adc12Dev_UnInit(uint8_t Unit)
{
    int32_t ret;
    
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_INITIALIZED) {
        
        /* Change State */
        Adc12Info[Unit].State = ADC12_UNINITIALIZED;
        
        /* Interrupt Disable */
        ADC12_DISABLE_IRQ(Adc12Info[Unit].Adc12_IRQn);
        
        /* Call adc12_UnInit() */
        adc12_UnInit(&Adc12Info[Unit]);
        
        /* normal end */
        ret = 0;
    } else {
        /* error end */
        ret = -1;
    }
    
    return ret;
}


/*
 *  Set Software Trigger
 */
static int32_t Adc12Dev_SetSWTrigger(uint8_t Unit, uint8_t Flag)
{
    int32_t ret;
    
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_INITIALIZED) {
        
        /* Check Argument */
        if (Flag > 1) {
            /* error end */
            ret = -1;
        } else {
            adc12_SetSWTrigger(&Adc12Info[Unit], Flag);
            
            /* normal end */
            ret = 0;
        }
    } else {
        
        /* error end */
        ret = -1;
    }
    
    return ret;
}


/*
 *  Stop A/D Comverter
 */
static int32_t Adc12Dev_Stop(uint8_t Unit, uint8_t Flag)
{
    int32_t ret;
    
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_INITIALIZED) {
        /*Check Argument */
        if (Flag == 0) {
            /* Call adc12_Stop() */
            adc12_Stop(&Adc12Info[Unit]);
            
            /* normal end */
            ret = 0;
            
        } else if (Flag == 1) {
            /* Call adc12_Abort() */
            adc12_Abort(&Adc12Info[Unit]);
            
            /* normal end */
            ret = 0;
            
        } else {
            /* error end */
            ret = -1;
        }
    } else {
        /* error end */
        ret = -1;
    }
    
    return ret;
}


/*
 *  Clear FIFO data
 */
static int32_t Adc12Dev_ClearFifo(uint8_t Unit, uint8_t Fifo)
{
    int32_t ret;
    
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_INITIALIZED) {
        /* Check Argument */
        if (Fifo > 1) {
            /* error end */
            ret = -1;
        } else {
            /* Call adc12_ClearFifo() */
            adc12_ClearFifo(&Adc12Info[Unit], Fifo);
            
            /* normal end */
            ret = 0;
        }
    } else {
        /* error end */
        ret = -1;
    }
    
    return ret;
}


/*
 *  Read FIFO data
 */
static int32_t Adc12Dev_Read(uint8_t Unit, uint16_t *pData, uint16_t *pInfo,
                             uint8_t *pFifoNum, uint8_t Fifo)
{
    int32_t ret = 0;
    
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_INITIALIZED) {
        /* Check Argument */
        if (pData == NULL) {
            /* error end */
            ret = -1;
        }
        
        if (pInfo == NULL) {
            /* error end */
            ret = -1;
        }
        
        if (pFifoNum == NULL) {
            /* error end */
            ret = -1;
        } else {
            if (Fifo == Adc12Dev_SCAN_FIFO) {
                if (((*pFifoNum) > ADC12_SCAN_MAX_FIFONUM) ||
                    ((*pFifoNum) == 0)) {
                    /* error end */
                    ret = -1;
                }
            } else if (Fifo == Adc12Dev_PRI_FIFO) {
                if (((*pFifoNum) > ADC12_PRI_MAX_FIFONUM) ||
                    ((*pFifoNum) == 0)) {
                    /* error end */
                    ret = -1;
                }
            } else {
                /* error end */
                ret = -1;
            }
        }
        
        if (ret == 0) {
            /* Set parameter */
            Adc12Info[Unit].pData = pData;
            Adc12Info[Unit].pInfo = pInfo;
            Adc12Info[Unit].pFifoNum = pFifoNum;
            
            /* Call adc12_Read() */
            adc12_Read(&Adc12Info[Unit], Fifo);
            
            /* normal end */
            ret = 0;
        }
        
    } else {
        /* error end */
        ret = -1;
    }
    
    return ret;
}

/*
 *  Read FIFO data
 */
static inline void adc12LiteR(Adc12_INFO *pInfo,uint16_t *pData,uint8_t *pFifoNum)
{
    int i,ch_inf;
    int cnt = *pFifoNum;
    
    /* Read data */
    for (i = 0; i < cnt; i++) {
        /* Check InValid bit */
        ch_inf = pInfo->pReg->SCFDL;
        if ((ch_inf & SCFDL_INVL) == 0) {
            pData[i] = pInfo->pReg->SCFDH | (ch_inf & 0x0f) ;
        } else {
            break;
        }
    }
    
    /* Set Read size */
    *pFifoNum = i;
}



//
//	Œy—Ê”Å:
//		pInfo‚ÉŠi”[‚³‚ê‚éch”Ô†‚ÍApData‚Ì‰ºˆÊ4bit‚É OR ‘‚«‚·‚é.
//
int32_t Adc12Lite_Read(uint8_t Unit, uint16_t *pData,uint8_t *pFifoNum)
{
    int32_t ret = 0;
    /* Check State */
    if (Adc12Info[Unit].State == ADC12_INITIALIZED) {
            /* Set parameter */
            //Adc12Info[Unit].pData = pData;
            //Adc12Info[Unit].pFifoNum = pFifoNum;
            
            /* Call adc12_Read() */
            adc12LiteR(&Adc12Info[Unit],pData,pFifoNum);
            
            /* normal end */
            ret = 0;
    } else {
        /* error end */
        ret = -1;
    }
    return ret;
}


/******************************************************************************
 *  Unit Function
 *****************************************************************************/
/* Unit0 */
static int32_t Adc12Dev_Unit0_Init(void)
{
    return (Adc12Dev_Init(0U));
}

static int32_t Adc12Dev_Unit0_UnInit(void)
{
    return (Adc12Dev_UnInit(0U));
}

static int32_t Adc12Dev_Unit0_SetSWTrigger(uint8_t Flag)
{
    return (Adc12Dev_SetSWTrigger(0U, Flag));
}

static int32_t Adc12Dev_Unit0_Stop(uint8_t Flag)
{
    return (Adc12Dev_Stop(0U, Flag));
}

static int32_t Adc12Dev_Unit0_ClearFifo(uint8_t Fifo)
{
    return (Adc12Dev_ClearFifo(0U, Fifo));
}

static int32_t Adc12Dev_Unit0_Read(uint16_t *pData, uint16_t *pInfo,
                                   uint8_t *pFifoNum, uint8_t Fifo)
{
    return (Adc12Dev_Read(0U, pData, pInfo, pFifoNum, Fifo));
}


/*
 *  Interrupt Handler
 */
void ADC0_IRQHandler(void)
{
    uint8_t irq;
    
    /* deviation from MISRA-C:2004 Rule 11.3 */
    irq = (uint8_t)(FM3_INTREQ->IRQ25MON & 0x0000000FU);
    
    adc12_IrqHandler(0U, irq);
}


/* Unit1 */
static int32_t Adc12Dev_Unit1_Init(void)
{
    return (Adc12Dev_Init(1U));
}

static int32_t Adc12Dev_Unit1_UnInit(void)
{
    return (Adc12Dev_UnInit(1U));
}

static int32_t Adc12Dev_Unit1_SetSWTrigger(uint8_t Flag)
{
    return (Adc12Dev_SetSWTrigger(1U, Flag));
}

static int32_t Adc12Dev_Unit1_Stop(uint8_t Flag)
{
    return (Adc12Dev_Stop(1U, Flag));
}

static int32_t Adc12Dev_Unit1_ClearFifo(uint8_t Fifo)
{
    return (Adc12Dev_ClearFifo(1U, Fifo));
}

static int32_t Adc12Dev_Unit1_Read(uint16_t *pData, uint16_t *pInfo,
                                   uint8_t *pFifoNum, uint8_t Fifo)
{
    return (Adc12Dev_Read(1U, pData, pInfo, pFifoNum, Fifo));
}

/*
 *  Interrupt Handler
 */
void ADC1_IRQHandler(void)
{
    uint8_t irq;
    
    /* deviation from MISRA-C:2004 Rule 11.3 */
    irq = (uint8_t)(FM3_INTREQ->IRQ26MON & 0x0000000FU);
    
    adc12_IrqHandler(1U, irq);
}


/* Unit2 */
static int32_t Adc12Dev_Unit2_Init(void)
{
    return (Adc12Dev_Init(2U));
}

static int32_t Adc12Dev_Unit2_UnInit(void)
{
    return (Adc12Dev_UnInit(2U));
}

static int32_t Adc12Dev_Unit2_SetSWTrigger(uint8_t Flag)
{
    return (Adc12Dev_SetSWTrigger(2U, Flag));
}

static int32_t Adc12Dev_Unit2_Stop(uint8_t Flag)
{
    return (Adc12Dev_Stop(2U, Flag));
}

static int32_t Adc12Dev_Unit2_ClearFifo(uint8_t Fifo)
{
    return (Adc12Dev_ClearFifo(2U, Fifo));
}

static int32_t Adc12Dev_Unit2_Read(uint16_t *pData, uint16_t *pInfo,
                                   uint8_t *pFifoNum, uint8_t Fifo)
{
    return (Adc12Dev_Read(2U, pData, pInfo, pFifoNum, Fifo));
}

/*
 *  Interrupt Handler
 */
void ADC2_IRQHandler(void)
{
    uint8_t irq;
    
    /* deviation from MISRA-C:2004 Rule 11.3 */
    irq = (uint8_t)(FM3_INTREQ->IRQ27MON & 0x0000000FU);
    
    adc12_IrqHandler(2U, irq);
}
