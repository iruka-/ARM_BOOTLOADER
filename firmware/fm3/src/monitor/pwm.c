/********************************************************************
 *	Port PF3(LED) �� PWM�g�`(��`�g)���o�͂���.
 ********************************************************************
 */
#include "mcu.h"

//#define PWM_CYCLE_CNT (6000)   // PWM ����16bit
#define PWM_CYCLE_CNT (60000)   // PWM ����16bit

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
    FM3_BT6_PWM->TMCR2_f.CKS3 = 0;  // 0b0000: ��/1
    FM3_BT6_PWM->TMCR_f.CKS2 = 0; 
    FM3_BT6_PWM->TMCR_f.CKS1 = 0;
    FM3_BT6_PWM->TMCR_f.CKS0 = 0;
    FM3_BT6_PWM->TMCR_f.RTGEN = 0;  // �ċN���֎~
    FM3_BT6_PWM->TMCR_f.PMSK = 0;   // �ʏ�o��(mask���Ȃ�)
    FM3_BT6_PWM->TMCR_f.EGS1 = 0;   // 0b00:�g���K���͖���
    FM3_BT6_PWM->TMCR_f.EGS0 = 0;
    FM3_BT6_PWM->TMCR_f.FMD = 1;   //0b001:PWM �@�\�I��
    FM3_BT6_PWM->TMCR_f.OSEL = 1;   //���Z�b�g�� "H"�o��
    FM3_BT6_PWM->TMCR_f.MDSE = 0;	//1;   //�����V���b�g����
    FM3_BT6_PWM->TMCR_f.CTEN = 0;   //�J�E���g�����~
    FM3_BT6_PWM->TMCR_f.STRG = 0;   //�\�t�g�E�F�A�g���K����

    FM3_BT6_PWM->STC_f.TGIE = 0;    //�g���K�����݋֎~
    FM3_BT6_PWM->STC_f.DTIE = 0;    //�f���[�e�B��v�����݋֎~
    FM3_BT6_PWM->STC_f.UDIE = 0;    //�A���_�t���[�����݋֎~
    FM3_BT6_PWM->STC_f.TGIR = 0;    //�g���K�����݃N���A
    FM3_BT6_PWM->STC_f.DTIR = 0;    //�f���[�e�B��v�����݃N���A
    FM3_BT6_PWM->STC_f.UDIR = 0;    //�A���_�t���[�����݃N���A
    
    FM3_BT6_PWM->PCSR = PWM_CYCLE_CNT;  // PWM ����

    FM3_BT6_PWM->PDUT = PWM_CYCLE_CNT/2;	//0;          // PWM �f���[�e�B

    /* ����J�n */
//	FM3_BT6_PWM->STC_f.UDIR = 0;    //�A���_�t���[�����݃N���A
//	FM3_BT6_PWM->STC_f.UDIE = 1;    //�A���_�t���[�����݋���
//	NVIC_EnableIRQ(BTIM0_7_IRQn);

    FM3_BT6_PWM->TMCR_f.CTEN = 1;   //�J�E���g���싖��
    FM3_BT6_PWM->TMCR_f.STRG = 1;   //�\�t�g�E�F�A�ɂ��N���J�n
}


#if	0
//
//	���荞�݂�PWM�̃f���[�e�B��ύX����ꍇ�́A�ȉ���L���ɂ���.
//

/*
 *  BT0_7_IRQHandler
 */
void BT0_7_IRQHandler(void)
{
    static uint8_t dec_fg = 0;
    static uint16_t cnt = 0;
    uint16_t read_val;
      
    FM3_BT6_PWM->STC_f.UDIR = 0;    //�A���_�t���[�����݃N���A

    read_val = FM3_BT6_PWM->PDUT;   //PWM �f���[�e�B

    if (dec_fg == 0) {
        read_val++;
        if (read_val >= PWM_CYCLE_CNT) {
            dec_fg = 1;
        }
    } else if (dec_fg == 1) {
        read_val--;
        if (read_val == 0) {
		    FM3_BT6_PWM->TMCR_f.PMSK = 1;   // mask����
            dec_fg = 2;
            cnt = 0;
        }
    } else {
        cnt++;
        if (cnt >= PWM_CYCLE_CNT) {
		    FM3_BT6_PWM->TMCR_f.PMSK = 0;   // �ʏ�o��(mask���Ȃ�)
            dec_fg = 0;
        }
    }
    
    FM3_BT6_PWM->PDUT = read_val;   //PWM �f���[�e�B
    
    FM3_BT6_PWM->TMCR_f.STRG = 1;   //�\�t�g�E�F�A�ɂ��N���J�n
    
}
#endif
