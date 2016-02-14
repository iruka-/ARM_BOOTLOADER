#include "includes.h"

#include "monitor/gpio.h"
#include "monitor/monit.h"
#include "cpu.h"

//PF6  = BLUE  LED
//PC5  = WHITE LED

#define	BLUELED	 PC5
#define	WHITELED PF6
void	CDC_read_kick(void);
void	CDC_start_check(void);
void	cache_enable();


//�^�C�}�[���荞�݂ōX�V����J�E���^.
volatile int g_TimerTick=0;

#define	CMTLVL	8		//�^�C�}�[���荞�݂̗D��x(1..15)

/********************************************************************
 *	TIMER �̐���:�^�C�}�[���荞�݂ōX�V����J�E���^�l���擾.
 ********************************************************************
 */
int	TIMER_getTick(void)
{
	return g_TimerTick;	// �P�b�Ԃ�10000�J�E���g.
}

/********************************************************************
 *	TIMER �̐���:������	10kHz���荞��.
 ********************************************************************
 */
void init_cmt0(int lvl)
{
	/* �X�^���o�C�R���g���[�����W�X�^7 */
	CPG.STBCR7.BIT.MSTP72 = 0;			/* �R���y�A�}�b�`�^�C�}�փN���b�N������ */

	/* �R���y�A�}�b�`�^�C�}�X�^�[�g���W�X�^ */
	CMT.CMSTR.BIT.STR0 = 0;				/* CMCNT_0�̓J�E���g���~ */

	/* �R���y�A�}�b�`�^�C�}�R���g���[��/�X�e�[�^�X���W�X�^ */
	if(1 == CMT.CMCSR0.BIT.CMF)
	{
		CMT.CMCSR0.BIT.CMF = 0;			/* �R���y�A�}�b�`�t���O���N���A */
	}
	CMT.CMCSR0.BIT.CMIE = 1;			/* �R���y�A�}�b�`�����݂����� */
	CMT.CMCSR0.BIT.CKS = 0;				/* �N���b�N�Z���N�g P��/8 */

	/* �R���y�A�}�b�`�R���X�^���g���W�X�^ */
	CMT.CMCOR0.WORD = 24000000/8/1000 - 1;		/* 1KHz */
//	CMT.CMCOR0.WORD = 24000000/8/10000 - 1;		/* 10KHz */
//	CMT.CMCOR0.WORD = 24000000/8/40000 - 1;		/* 10KHz */

	/* �R���y�A�}�b�`�^�C�}�X�^�[�g���W�X�^ */
	CMT.CMSTR.BIT.STR0 = 1;				/* CMCNT_0�̓J�E���g���J�n */

	/* ���荞�ݗD�惌�x���ݒ背�W�X�^10 */
	INTC.IPR10.BIT._CMT0 = lvl;			/* �R���y�A�}�b�`�^�C�} �`���l��1 */
}

/********************************************************************
 *	TIMER �̐���:��~	10kHz���荞��.
 ********************************************************************
 */
void reset_cmt0(void)
{
	/* �X�^���o�C�R���g���[�����W�X�^7 */
	CPG.STBCR7.BIT.MSTP72 = 1;			/* �R���y�A�}�b�`�^�C�}�փN���b�N������ */

	/* �R���y�A�}�b�`�^�C�}�X�^�[�g���W�X�^ */
	CMT.CMSTR.BIT.STR0 = 1;				/* CMCNT_0�̓J�E���g���~ */

	CMT.CMCSR0.BIT.CMIE = 0;			/* �R���y�A�}�b�`�����݂����� */
	CMT.CMCSR0.BIT.CKS = 0;				/* �N���b�N�Z���N�g P��/8 */

	/* �R���y�A�}�b�`�^�C�}�X�^�[�g���W�X�^ */
	CMT.CMSTR.BIT.STR0 = 0;				/* CMCNT_0�̓J�E���g���J�n */

	/* ���荞�ݗD�惌�x���ݒ背�W�X�^10 */
	INTC.IPR10.BIT._CMT0 = 0;			/* �R���y�A�}�b�`�^�C�} �`���l��1 */

	return;
}


/********************************************************************
 *	TIMER �̐���:���荞�ݏ���
 ********************************************************************
 */
void int_cmt_cmi0(void)
{
	if(	CMT.CMCSR0.BIT.CMF == 1) {
		CMT.CMCSR0.BIT.CMF = 0;			/* �R���y�A�}�b�`�t���O���N���A */
	}
	g_TimerTick++;
#if	0
	if(	g_TimerTick & 0x400) {
		digitalWrite(BLUELED,1);
	}else{
		digitalWrite(BLUELED,0);
	}
#endif

#if	1
	CDC_read_kick();	// USB-CDC�ǂݍ��݊J�n�L�b�N.
#endif
}

#if	1
static int wled_data=0;
static int bled_data=0;

void BLUELED_flip()
{
	bled_data ++;
	digitalWrite(BLUELED	,bled_data&1);
}

static int wcnt=0;
void WHITELED_flip(int sft)
{
	wcnt++;
	if(	wcnt >= (1<<sft) ) {
		wcnt = 0;
		wled_data++;
		digitalWrite(WHITELED	,wled_data&1);
	}
}

void BLUELED_on()
{
	bled_data=0;
	digitalWrite(BLUELED	,bled_data&1);
}

void WHITELED_on()
{
	wcnt = 0;
	wled_data=0;
	digitalWrite(WHITELED	,wled_data&1);
}

#endif

/********************************************************************
 *	LED �̐���:������
 ********************************************************************
 */
void LED_init(void)
{
	pinMode(PC8,OUTPUT);
	pinModeEx(PC8,0);

	pinMode(WHITELED,OUTPUT);
	pinMode(BLUELED	,OUTPUT);

	init_cmt0(CMTLVL);
	
	digitalWrite(WHITELED	,1);	//off
	digitalWrite(BLUELED	,1);	//off
}

static int  led_data =0;

/********************************************************************
 *	LED �̐���:���]
 ********************************************************************
 */
void LED_flip()
{
	led_data++;
	digitalWrite(PC8		,led_data&1);
}

/********************************************************************
 *	LED �̐���:ON
 ********************************************************************
 */
void led_on()
{
	led_data=0;
	digitalWrite(PC8		,led_data&1);
}

/********************************************************************
 *	LED �̐���:OFF
 ********************************************************************
 */
void led_off()
{
	led_data=1;
	digitalWrite(PC8		,led_data&1);
}

/********************************************************************
 *	LED �̐���:�����ԂŔ��] (sft�� 2�ׂ̂��ɒ������)
 ********************************************************************
 */
void led_blink(int sft)
{
	static int cnt=0;
	cnt++;
	if(	cnt >= (1<<sft) ) {
		cnt = 0;
		LED_flip();
	}
}

/********************************************************************
 *	�e��f�o�b�O�@�\�̏�����.
 ********************************************************************
 */
void DEBUG_init(void)
{
	cache_enable();
	LED_init();
	CDC_start_check();
#if	0
	int i;

	for(i=0;i<32;i++) {
		LED_flip();
		wait_ms(20);
	}
#endif
	init_cmt0(CMTLVL);
}

