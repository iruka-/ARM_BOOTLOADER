/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "monitor/utype.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"

#include "stm32_eval.h"

#include "monitor/usbio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define	LED_DEBUG	0

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL         5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
char    flag_EP3_OUT_PENDING = 0;
//extern  uint8_t USART_Rx_Buffer[];
extern uint32_t USART_Rx_ptr_out;
extern uint32_t USART_Rx_length;
extern uint8_t  USB_Tx_State;

extern	volatile int countTx;	// hostPCへ送信中Packet の byte数.

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//static uchar Recv_Buffer[64];
//int USART_RecvData(uchar *result);

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

#define EP_TX_STALL    (0x0010) /* EndPoint TX STALLed */
#define EP_TX_NAK      (0x0020) /* EndPoint TX NAKed */
#define EP_TX_VALID    (0x0030) /* EndPoint TX VALID */
//
//	仮想COMポートからのデータをPC側に返送する.
//
void EP1_IN_Callback (void)
{
#if	0
	if(	GetEPTxStatus(ENDP1) == EP_TX_VALID) return;

	int Recv_Size = USART_RecvData(Recv_Buffer);
	if(	Recv_Size ) {
		UserToPMABufferCopy(Recv_Buffer, ENDP1_TXADDR, Recv_Size);
		SetEPTxCount(ENDP1, Recv_Size);
		SetEPTxValid(ENDP1);
	}
#if	LED_DEBUG
	led2_blink(0);
#endif
#endif
	countTx = 0;
}

int usbGetPending() {
  return countTx;
}

#if	0
void Handle_USBAsynchXfer (void)
{
	if(	GetEPTxStatus(ENDP1) == EP_TX_VALID) return;

	int Recv_Size = USART_RecvData(Recv_Buffer);
	if(	Recv_Size ) {
		UserToPMABufferCopy(Recv_Buffer, ENDP1_TXADDR, Recv_Size);
		SetEPTxCount(ENDP1, Recv_Size);
		SetEPTxValid(ENDP1);
	}
}
#endif

#define	MAX_SEND_BYTES_CDC		(64)

int	usbSendBytes(uchar *data,int size)
{
	if(size==0) wait_us(140);			// 0byte packet対策.

  /* Last transmission hasn't finished, abort */
  	if (countTx) {
    	return 0;
  	}
	if(	GetEPTxStatus(ENDP1) == EP_TX_VALID) return 0;

	if(	size >= MAX_SEND_BYTES_CDC) {
		size  = MAX_SEND_BYTES_CDC;
	}

	UserToPMABufferCopy(data, ENDP1_TXADDR, size);
	SetEPTxCount(ENDP1, size);

    countTx += size;
	if(size==0) countTx++;				// 0byte packet対策.

	SetEPTxValid(ENDP1);

	if(size==0) wait_us(140);			// 0byte packet対策.

	return size;
}


#if	0
void EP1_IN_Callback (void)
{
	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;

#if	LED_DEBUG
	led2_blink(0);
#endif

	if (USB_Tx_State == 1) {
		if (USART_Rx_length == 0) {
			USB_Tx_State = 0;
		} else {
			if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE) {
				USB_Tx_ptr = USART_Rx_ptr_out;
				USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

				USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
				USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;
			} else {
				USB_Tx_ptr = USART_Rx_ptr_out;
				USB_Tx_length = USART_Rx_length;

				USART_Rx_ptr_out += USART_Rx_length;
				USART_Rx_length = 0;
			}

#ifdef USE_STM3210C_EVAL
			USB_SIL_Write(EP1_IN, &USART_Rx_Buffer[USB_Tx_ptr], USB_Tx_length);
#else
			UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
			SetEPTxCount(ENDP1, USB_Tx_length);
			SetEPTxValid(ENDP1);
#endif
		}
	}
}
#endif


/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
//
//	PCから仮想COMポート側へデータを送信する.
//
void EP3_OUT_Callback(void)
{
	uint16_t USB_Rx_Cnt;

#if	LED_DEBUG
	led2_blink(0);
#endif


	/* Get the received data buffer and update the counter */
	USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Buffer);

	/* USB data will be immediately processed, this allow next USB traffic beeing
	NAKed till the end of the USART Xfet */
	USBenqueue(USB_Rx_Buffer, USB_Rx_Cnt);
//	if(USB_Rx_Cnt) led2_blink(0);

#ifndef STM32F10X_CL
	if(	USBgetfreesize() >= 64 ) {
		/* Enable the receive of data on EP3 */
		SetEPRxValid(ENDP3);
		flag_EP3_OUT_PENDING=0;
	}else{
		// 受信バッファの空きがないときは一旦ペンディングする.
		flag_EP3_OUT_PENDING=1;
	}
#endif /* STM32F10X_CL */
}


/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef STM32F10X_CL
void INTR_SOFINTR_Callback(void)
#else
void SOF_Callback(void)
#endif /* STM32F10X_CL */
{
//	static uint32_t FrameCount = 0;

	if (bDeviceState == CONFIGURED) {
		// ダウンストリーム の ペンディング中?.
		if(	flag_EP3_OUT_PENDING ) {
			// 受信バッファの空きが出来たときはペンディング解除する.
			if(	USBgetfreesize() >= 64 ) {
				/* Enable the receive of data on EP3 */
				SetEPRxValid(ENDP3);
				flag_EP3_OUT_PENDING=0;
			}
		}
		/* Check the data to be sent through IN pipe */
		//Handle_USBAsynchXfer();
	}
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

