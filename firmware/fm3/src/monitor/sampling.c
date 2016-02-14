/********************************************************************
 *	A/D�ϊ��Ȃǂ� �T���v�����O����.
 ********************************************************************
 *	int	sample_init(SAMPLE_PARAM *param);	//�T���v�����O�̏�����.
 *	int	sample_read(char *buf,int size);	//�T���v�����O�f�[�^�擾.
 *
 *	int	sample_start(SAMPLE_PARAM *param);	//�T���v�����O�J�n(�����֐�).
 *	int	sample_stop();						//�T���v�����O��~(�����֐�).
 ********************************************************************
 *	�T���v�����O�͊��荞�݂ɂ����s�����.
 *	�f�[�^�擾�͊i�[�L���[����̎��o���Ƃ���.
 *
 */

/*
 ********************************************************************
 *	�f�[�^����胂�[�h�̐ݒ�
 ********************************************************************
-------------
	a/d	|
	a	| rate | count | ch(s) | trig-mode | trig-ch | port-sel | rep | 
	d	| rate | count | ch(s) | trig-mode | trig-ch | port-sel | rep |

	a = 0 d = 1
	rate  = samples per sec
	count = 
	ch(s) = 1 .. 16
	trig-mode = free-run , hi , lo , up , down , 
				           hi(>lev) , lo(<lev) , 
	trig-ch

	port-sel  a,b,c,d,e,f,g
			  ain(n)

	rep = repeat times

	return short-int * (count)

	//	subcmd	: a/d	a=0 d=1
	//	size	: count
	//	address	: rate , port-sel
	//	data[0]	: trig-mode
	//	data[1]	: trig-ch
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 16bit x 2048 buffer: (4k)

 ===================================
 sampling   : 100kHz
 throughput : 100k x 2byte = 200kB / sec
 transfer rate  :          200byte / 1mSframe

 video rate : 60Hz  (16mS/Vsync) = 200 * 16 = 3200byte 
 
 ===================================
 16mS�ȓ��� 3200byte ���������\��.
 ���̂����A1000sample���x��\������.
 ===================================
 100kHz / 50kHz / 10kHz       : ���荞��.
 500kHz / 1MHz / 6MHz / 12MHz : �S��.
 
 */

#include <string.h>
#include "../mcu.h"
#include "Adc12Dev.h"
#include "fifo.h"

#include "sampling.h"


/********************************************************************
 *	
 ********************************************************************
 */

#define UNIT_NO			0					//�g�p���郆�j�b�gNo.(0,1,2)
//#define INPUT_CH_SCAN	Adc12Dev_SCAN_CH0	//�X�L�����ϊ���CH

#define	INPUT_USE_CH16_31	1				//AN16�`31���A�i���O���͂ɂ���.

#define INPUT_CH_PRI1	0	//4					//�D��ϊ�1��CH
#define INPUT_CH_PRI2	0	//8					//�D��ϊ�2��CH
/*
 *  Extern IO Block
 */
extern Adc12Dev_IOB Adc12Dev[];
int32_t Adc12Lite_Read(uint8_t Unit, uint16_t *pData,uint8_t *pFifoNum);

static void sample_callback(int32_t Status);

/********************************************************************
 *	�\�t�g�E�F�AFIFO
 ********************************************************************
int FIFO_init( FIFO *queue,uchar *buf,int size);	//������.
int FIFO_enqueue(FIFO *queue,uchar *buf,int size);	//�L���[�ɂ��߂�.
int FIFO_dequeue(FIFO *queue,uchar *buf,int size);	//�L���[������o��.
int FIFO_getsize(FIFO *queue);					//�L���[�ɂ��܂��Ă���o�C�g���𓾂�.
int FIFO_getfreesize(FIFO *queue);				//�L���[�̋󂫗e�ʃo�C�g���𓾂�.
 */
#define	FIFOSIZE	32768	// �T���v����.

static	FIFO	adque;
static	short	adbuffer[FIFOSIZE];

static	char	flag_start;
static	char	flag_overrun;
static	SAMPLE_PARAM defparam;

static	int		debug_cnt=0;


void fifo_init()
{
	FIFO_init(&adque,(uchar*)adbuffer,sizeof(adbuffer));
}

/********************************************************************
 *	�T���v�����O�J�n.
 ********************************************************************
 */
int	sample_start(SAMPLE_PARAM *param)
{
	Adc12Dev_IOB *adc = &Adc12Dev[UNIT_NO];

	fifo_init();

	// GPIO�̃A�i���O���͐ݒ�.
	if(param->port_sel == 1) {
		FM3_GPIO->ADE    = 0xFFFF0000;	// AN16�`31���A�i���O���͂ɂ���.
	}else{
		FM3_GPIO->ADE    = 0x0000FFFF;	// AN0 �`15���A�i���O���͂ɂ���.
	}
	FM3_GPIO->PFR0   = FM3_GPIO->PFR0 | 0x00000080;
	FM3_GPIO->PCR0   = FM3_GPIO->PCR0 & 0xFFFFFF7F;
	FM3_GPIO->EPFR09 = FM3_GPIO->EPFR09 & 0xFF000FFF;  // ADTG_0

	// A/D�ϊ��̓��̓`���l���ݒ�.
//	adc->Cfg.ScanChSel  = INPUT_CH_SCAN;
	adc->Cfg.ScanRepeat = Adc12Dev_REPEAT_MODE;
//	adc->Cfg.ScanFifoAlloc = Adc12Dev_SCAN_FIFONUM10; // HW FiFo��10���܂����犄�荞��.
	adc->Cfg.ScanFifoAlloc = Adc12Dev_SCAN_FIFONUM8; // HW FiFo��8���܂����犄�荞��.
	adc->Cfg.Pri1ChSel  = INPUT_CH_PRI1;
	adc->Cfg.Pri2ChSel  = INPUT_CH_PRI2;
	adc->Cfg.IntEnable  = Adc12Dev_OVR_INT_ENABLE | Adc12Dev_SCAN_INT_ENABLE;

//	adc->Cfg.CompTime   = Adc12Dev_COMP_TIME_DIV2;
//	Type0���i�ł͉���3bit�L���ŁAx2 �`x9 , Type0�ȊO�ł�8bit�ŁA0x40�`7f�͋֎~.
//	adc->Cfg.CompTime   = 7;	// x9
//	Type2���i�ł�0x00(HCLK/2)�`0x3F(HCLK/65)��0x80(HCLK/1). �������(CompTime+2),0x80�̂Ƃ���(1)
	adc->Cfg.CompTime   = 7;	// x9

//	adc->Cfg.CompValue  = 0;
	adc->Cfg.CompEnable = 0;
	adc->Cfg.SampTimeN0 = (param->rate     & 0x07);	// (0=1x 1=4x 8x ... 7=256x)
	adc->Cfg.SampTime0  = (param->rate>>8) & 0x1f ;	// 0�`31
//	adc->Cfg.SampTimeN0 = Adc12Dev_SAMP_TIME_X256;
	adc->CallBackIrq    = sample_callback;			// ���荞�ݎ���Callback�֐�.


//	��ch����̓ǂݍ���.	
	if(param->port_sel == 1) {	// ch16�`31���g�p.
		adc->Cfg.ScanChSel  = (param->channels << 16);
	}else{
		adc->Cfg.ScanChSel  = (param->channels);
	}

	adc->Init();
	bFM3_ADC0_PCCR_PEEN = 1; //	ADTG enable.
	adc->SetSWTrigger(Adc12Dev_SCAN_ACT);

	flag_start = 1;
	flag_overrun = 0;
	return 0;
}
/********************************************************************
 *	�T���v�����O��~.
 ********************************************************************
 */
int	sample_stop()
{
	Adc12Dev_IOB *adc = &Adc12Dev[UNIT_NO];

	if( flag_start == 1 ) {
		adc->Stop(1);
		adc->UnInit();
		flag_start = 0;
	}
	return 0;
}
/********************************************************************
 *	�T���v�����O�̏�����.
 ********************************************************************
 */
int	sample_init(SAMPLE_PARAM *param)
{
	if( memcmp(&defparam,param,sizeof(SAMPLE_PARAM)) != 0 ) {	//�p�����[�^�ύX���ꂽ.
		memcpy(&defparam,param,sizeof(SAMPLE_PARAM));
		if(param->subcmd == ADC_STOP) {
			sample_stop();				//��~.
		}else{
			sample_stop();				//��~.
			sample_start(&defparam);	//�J�n.
		}
	}
	return 0;
}
/********************************************************************
 *	�T���v�����O�f�[�^�擾.
 ********************************************************************
 */
int	sample_read(uchar *buf,int size)
{
	SAMPLE_RETURN *hdr = (SAMPLE_RETURN *)buf;
	uchar 		  *dst = buf + sizeof(SAMPLE_RETURN);
	size -= sizeof(SAMPLE_RETURN);

	memcpy(hdr,&defparam,sizeof(SAMPLE_RETURN));
	hdr->cmd = 1;
	hdr->size      = FIFO_dequeue(&adque,dst,size);
	hdr->overrun   = flag_overrun;
	hdr->debug_cnt =  debug_cnt;

	flag_overrun = 0;
	return 0;
}

/********************************************************************
 *	�T���v�����O�R�[���o�b�N.
 ********************************************************************
 */
static void sample_callback(int32_t Status)
{
	static uint16_t scan_data[128];
	static uint16_t pri_data;
	static uint16_t pri_info;
	static uint8_t  size;

	int32_t ret;
	int32_t rc;	//,i;

	switch (Status) {
	case Adc12Dev_IRQSTATUS_SCAN:
		size = 16;
		//
		//	���荞�ݏ������Ԃ���������̂ŁA�y�ʉ��������̂��Ă�.
		//
		ret = Adc12Lite_Read(UNIT_NO, scan_data , &size);
		if (ret == 0) {
//			if(size>=16) size = 16;
			rc = FIFO_enqueue_fast(&adque,scan_data,size);		//�L���[�ɂ��߂�.
//			rc = FIFO_enqueue(&adque,(uchar *)scan_data,size*2);	//�L���[�ɂ��߂�.
			if(rc==0) {
				FIFO_reset(&adque);
				flag_overrun=1;
			}
//			debug_cnt += size;
		}
		break;
	case Adc12Dev_IRQSTATUS_PRI:
		size = 1;
		ret = Adc12Dev[UNIT_NO].Read(&pri_data, &pri_info, &size, Adc12Dev_PRI_FIFO);
		if (ret == 0) {
		}
		break;
	case Adc12Dev_IRQSTATUS_COMP:
		break;
	case Adc12Dev_IRQSTATUS_SOVR:
		ret = Adc12Dev[UNIT_NO].ClearFifo(Adc12Dev_SCAN_FIFO);
		break;
	case Adc12Dev_IRQSTATUS_POVR:
		ret = Adc12Dev[UNIT_NO].ClearFifo(Adc12Dev_PRI_FIFO);
		break;
	default:
		break;
	}
}

/********************************************************************
 *	
 ********************************************************************
 */
