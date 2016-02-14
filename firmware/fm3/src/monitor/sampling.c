/********************************************************************
 *	A/D変換などの サンプリング処理.
 ********************************************************************
 *	int	sample_init(SAMPLE_PARAM *param);	//サンプリングの初期化.
 *	int	sample_read(char *buf,int size);	//サンプリングデータ取得.
 *
 *	int	sample_start(SAMPLE_PARAM *param);	//サンプリング開始(内部関数).
 *	int	sample_stop();						//サンプリング停止(内部関数).
 ********************************************************************
 *	サンプリングは割り込みにより実行される.
 *	データ取得は格納キューからの取り出しとする.
 *
 */

/*
 ********************************************************************
 *	データ引取りモードの設定
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
 16mS以内に 3200byte を引き取る能力.
 そのうち、1000sample程度を表示する.
 ===================================
 100kHz / 50kHz / 10kHz       : 割り込み.
 500kHz / 1MHz / 6MHz / 12MHz : 全力.
 
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

#define UNIT_NO			0					//使用するユニットNo.(0,1,2)
//#define INPUT_CH_SCAN	Adc12Dev_SCAN_CH0	//スキャン変換のCH

#define	INPUT_USE_CH16_31	1				//AN16～31をアナログ入力にする.

#define INPUT_CH_PRI1	0	//4					//優先変換1のCH
#define INPUT_CH_PRI2	0	//8					//優先変換2のCH
/*
 *  Extern IO Block
 */
extern Adc12Dev_IOB Adc12Dev[];
int32_t Adc12Lite_Read(uint8_t Unit, uint16_t *pData,uint8_t *pFifoNum);

static void sample_callback(int32_t Status);

/********************************************************************
 *	ソフトウェアFIFO
 ********************************************************************
int FIFO_init( FIFO *queue,uchar *buf,int size);	//初期化.
int FIFO_enqueue(FIFO *queue,uchar *buf,int size);	//キューにためる.
int FIFO_dequeue(FIFO *queue,uchar *buf,int size);	//キューから取り出す.
int FIFO_getsize(FIFO *queue);					//キューにたまっているバイト数を得る.
int FIFO_getfreesize(FIFO *queue);				//キューの空き容量バイト数を得る.
 */
#define	FIFOSIZE	32768	// サンプル数.

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
 *	サンプリング開始.
 ********************************************************************
 */
int	sample_start(SAMPLE_PARAM *param)
{
	Adc12Dev_IOB *adc = &Adc12Dev[UNIT_NO];

	fifo_init();

	// GPIOのアナログ入力設定.
	if(param->port_sel == 1) {
		FM3_GPIO->ADE    = 0xFFFF0000;	// AN16～31をアナログ入力にする.
	}else{
		FM3_GPIO->ADE    = 0x0000FFFF;	// AN0 ～15をアナログ入力にする.
	}
	FM3_GPIO->PFR0   = FM3_GPIO->PFR0 | 0x00000080;
	FM3_GPIO->PCR0   = FM3_GPIO->PCR0 & 0xFFFFFF7F;
	FM3_GPIO->EPFR09 = FM3_GPIO->EPFR09 & 0xFF000FFF;  // ADTG_0

	// A/D変換の入力チャネル設定.
//	adc->Cfg.ScanChSel  = INPUT_CH_SCAN;
	adc->Cfg.ScanRepeat = Adc12Dev_REPEAT_MODE;
//	adc->Cfg.ScanFifoAlloc = Adc12Dev_SCAN_FIFONUM10; // HW FiFoに10個溜まったら割り込む.
	adc->Cfg.ScanFifoAlloc = Adc12Dev_SCAN_FIFONUM8; // HW FiFoに8個溜まったら割り込む.
	adc->Cfg.Pri1ChSel  = INPUT_CH_PRI1;
	adc->Cfg.Pri2ChSel  = INPUT_CH_PRI2;
	adc->Cfg.IntEnable  = Adc12Dev_OVR_INT_ENABLE | Adc12Dev_SCAN_INT_ENABLE;

//	adc->Cfg.CompTime   = Adc12Dev_COMP_TIME_DIV2;
//	Type0製品では下位3bit有効で、x2 ～x9 , Type0以外では8bitで、0x40～7fは禁止.
//	adc->Cfg.CompTime   = 7;	// x9
//	Type2製品では0x00(HCLK/2)～0x3F(HCLK/65)と0x80(HCLK/1). 分周比は(CompTime+2),0x80のときは(1)
	adc->Cfg.CompTime   = 7;	// x9

//	adc->Cfg.CompValue  = 0;
	adc->Cfg.CompEnable = 0;
	adc->Cfg.SampTimeN0 = (param->rate     & 0x07);	// (0=1x 1=4x 8x ... 7=256x)
	adc->Cfg.SampTime0  = (param->rate>>8) & 0x1f ;	// 0～31
//	adc->Cfg.SampTimeN0 = Adc12Dev_SAMP_TIME_X256;
	adc->CallBackIrq    = sample_callback;			// 割り込み時のCallback関数.


//	多chからの読み込み.	
	if(param->port_sel == 1) {	// ch16～31を使用.
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
 *	サンプリング停止.
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
 *	サンプリングの初期化.
 ********************************************************************
 */
int	sample_init(SAMPLE_PARAM *param)
{
	if( memcmp(&defparam,param,sizeof(SAMPLE_PARAM)) != 0 ) {	//パラメータ変更された.
		memcpy(&defparam,param,sizeof(SAMPLE_PARAM));
		if(param->subcmd == ADC_STOP) {
			sample_stop();				//停止.
		}else{
			sample_stop();				//停止.
			sample_start(&defparam);	//開始.
		}
	}
	return 0;
}
/********************************************************************
 *	サンプリングデータ取得.
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
 *	サンプリングコールバック.
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
		//	割り込み処理時間が長すぎるので、軽量化したものを呼ぶ.
		//
		ret = Adc12Lite_Read(UNIT_NO, scan_data , &size);
		if (ret == 0) {
//			if(size>=16) size = 16;
			rc = FIFO_enqueue_fast(&adque,scan_data,size);		//キューにためる.
//			rc = FIFO_enqueue(&adque,(uchar *)scan_data,size*2);	//キューにためる.
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
