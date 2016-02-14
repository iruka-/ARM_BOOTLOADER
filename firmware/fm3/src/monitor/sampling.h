#ifndef	_sampling_h_
#define	_sampling_h_

#include "utype.h"

typedef struct {
//	--------------------------------------------------
	uchar  cmd;			// = HIDASP_SAMPLING 
	uchar  subcmd;		// = a/d;		a=1 d=2 stop=0
	ushort size;		// = size;      取得したいバイト数.
	uint   rate;		// = rate;		サンプリングレート.現在はFM3固有.
//	--------------------------------------------------
	ushort channels;	//	ch(s)  = channels(1..16) 1(chs)
	uchar  trig_mode;	//				0(freerun)
	uchar  trig_ch;		//				0(ch)

	uchar  port_sel;	//				0(port)
	uchar  reserve[3];
//	--------------------------------------------------
} SAMPLE_PARAM;

//	サンプリングパラメータ構造体.
typedef struct {
//	--------------------------------------------------
	uchar  cmd;			//+00				SAMPLING
	uchar  subcmd;		//+01 = a/d;		a=1 d=2 stop=0
	ushort size;		//+02 = count;		1040
	uint   overrun;		//+04
//	--------------------------------------------------
	uint   debug_cnt;
	uchar  port_sel;	//+0c				0(port)
	uchar  reserve[3];	//+0d
//	--------------------------------------------------
} SAMPLE_RETURN;			// 16bytes

enum {
	ADC_STOP,
	ADC_ANALOG,
	ADC_DIGITAL,
};

int	sample_init(SAMPLE_PARAM *param);	//サンプリング開始.
int	sample_stop();						//サンプリング停止.
int	sample_read(uchar *buf,int size);	//サンプリングデータ取得.


#endif
