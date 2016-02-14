/********************************************************************
 *	Port PF3(LED) に PWM波形(矩形波)を出力する.
 ********************************************************************
 */
#include "mcu.h"

//#define PWM_CYCLE_CNT (6000)   // PWM 周期16bit
#define PWM_CYCLE_CNT (60000)   // PWM 周期16bit

/*
 *  InitPort
 */
static void InitPort(void)
{
    /* LED */
    FM3_GPIO->PFRF_f.P3 = 1;          //PF3 set to Base Timer Function(TIOA06_0)
    FM3_GPIO->PZRF_f.P3 = 1;          //PF3 set to Open-drain mode

    FM3_GPIO->EPFR05_f.TIOA6E1 = 0;   //0b01:TIOA6 is TIOA06_0
    FM3_GPIO->EPFR05_f.TIOA6E0 = 1;
}


int PWM_init()
{
    InitPort();
    /* PWM init */
    FM3_BT6_PWM->TMCR2_f.CKS3 = 0;  // 0b0000: Φ/1
    FM3_BT6_PWM->TMCR_f.CKS2 = 0; 
    FM3_BT6_PWM->TMCR_f.CKS1 = 0;
    FM3_BT6_PWM->TMCR_f.CKS0 = 0;
    FM3_BT6_PWM->TMCR_f.RTGEN = 0;  // 再起動禁止
    FM3_BT6_PWM->TMCR_f.PMSK = 0;   // 通常出力(maskしない)
    FM3_BT6_PWM->TMCR_f.EGS1 = 0;   // 0b00:トリガ入力無効
    FM3_BT6_PWM->TMCR_f.EGS0 = 0;
    FM3_BT6_PWM->TMCR_f.FMD = 1;   //0b001:PWM 機能選択
    FM3_BT6_PWM->TMCR_f.OSEL = 1;   //リセット後 "H"出力
    FM3_BT6_PWM->TMCR_f.MDSE = 0;	//1;   //ワンショット動作
    FM3_BT6_PWM->TMCR_f.CTEN = 0;   //カウント動作停止
    FM3_BT6_PWM->TMCR_f.STRG = 0;   //ソフトウェアトリガ無効

    FM3_BT6_PWM->STC_f.TGIE = 0;    //トリガ割込み禁止
    FM3_BT6_PWM->STC_f.DTIE = 0;    //デューティ一致割込み禁止
    FM3_BT6_PWM->STC_f.UDIE = 0;    //アンダフロー割込み禁止
    FM3_BT6_PWM->STC_f.TGIR = 0;    //トリガ割込みクリア
    FM3_BT6_PWM->STC_f.DTIR = 0;    //デューティ一致割込みクリア
    FM3_BT6_PWM->STC_f.UDIR = 0;    //アンダフロー割込みクリア
    
    FM3_BT6_PWM->PCSR = PWM_CYCLE_CNT;  // PWM 周期

    FM3_BT6_PWM->PDUT = PWM_CYCLE_CNT/2;	//0;          // PWM デューティ

    /* 動作開始 */
//	FM3_BT6_PWM->STC_f.UDIR = 0;    //アンダフロー割込みクリア
//	FM3_BT6_PWM->STC_f.UDIE = 1;    //アンダフロー割込み許可
//	NVIC_EnableIRQ(BTIM0_7_IRQn);

    FM3_BT6_PWM->TMCR_f.CTEN = 1;   //カウント動作許可
    FM3_BT6_PWM->TMCR_f.STRG = 1;   //ソフトウェアによる起動開始
}


#if	0
//
//	割り込みでPWMのデューティを変更する場合は、以下を有効にする.
//

/*
 *  BT0_7_IRQHandler
 */
void BT0_7_IRQHandler(void)
{
    static uint8_t dec_fg = 0;
    static uint16_t cnt = 0;
    uint16_t read_val;
      
    FM3_BT6_PWM->STC_f.UDIR = 0;    //アンダフロー割込みクリア

    read_val = FM3_BT6_PWM->PDUT;   //PWM デューティ

    if (dec_fg == 0) {
        read_val++;
        if (read_val >= PWM_CYCLE_CNT) {
            dec_fg = 1;
        }
    } else if (dec_fg == 1) {
        read_val--;
        if (read_val == 0) {
		    FM3_BT6_PWM->TMCR_f.PMSK = 1;   // maskする
            dec_fg = 2;
            cnt = 0;
        }
    } else {
        cnt++;
        if (cnt >= PWM_CYCLE_CNT) {
		    FM3_BT6_PWM->TMCR_f.PMSK = 0;   // 通常出力(maskしない)
            dec_fg = 0;
        }
    }
    
    FM3_BT6_PWM->PDUT = read_val;   //PWM デューティ
    
    FM3_BT6_PWM->TMCR_f.STRG = 1;   //ソフトウェアによる起動開始
    
}
#endif
