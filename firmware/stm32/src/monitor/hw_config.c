/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "stm32_eval.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
USART_InitTypeDef USART_InitStructure;

volatile int countTx    = 0;

uint8_t  USART_Rx_Buffer [USART_RX_DATA_SIZE];	// 2048バイトあるらしい.

uint32_t USART_Rx_ptr_in = 0;		//(USARTから)  受信した文字数.
uint32_t USART_Rx_ptr_out = 0;		//(ホストPCに) 送信済み文字数.
uint32_t USART_Rx_length  = 0;

uint8_t  USB_Tx_State = 0;
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Extern variables ----------------------------------------------------------*/
void GPIO_Configuration(void);

extern LINE_CODING linecoding;
int	Board_Init(void);
int	Clk12MHz=0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
	Clk12MHz=Board_Init();
	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS) {
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
//#ifdef	USE_XTAL_12MHz
		if (Clk12MHz) {
			/* PLLCLK = 12MHz * 6 = 72 MHz */
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
//#else
		} else {
			/* PLLCLK =  8MHz * 9 = 72 MHz */
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		}
//#endif

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08) {
		}
	} else {
		/* If HSE fails to start-up, the application will have wrong clock configuration.
		   User can add here some code to deal with this error */

		/* Go to infinite loop */
		while (1) {
		}
	}

	/* Configure the used GPIOs*/
	GPIO_Configuration();

#if	0	//ndef USE_STM3210C_EVAL
	/* Enable USB_DISCONNECT GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

	/* Configure USB pull-up pin */
	GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* USE_STM3210C_EVAL */
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
#ifdef STM32F10X_CL
	/* Select USBCLK source */
	RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

	/* Enable the USB clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
#else
	/* Select USBCLK source */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	/* Enable the USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32F10X_CL */
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
	/* Set the device state to suspend */
	bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo = &Device_Info;

	/* Set the device state to the correct state */
	if (pInfo->Current_Configuration != 0) {
		/* Device configured */
		bDeviceState = CONFIGURED;
	} else {
		bDeviceState = ATTACHED;
	}
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH */
	/* Set the Vector Table base location at 0x0800XXXX */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, _ROMADRS );
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

#ifdef STM32F10X_CL
	/* Enable the USB Interrupts */
	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#else
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif /* STM32F10X_CL */

	/* Enable USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

	countTx    = 0;
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
#if	0
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef USE_STM3210C_EVAL
	if (NewState != DISABLE) {
		USB_DevConnect();
	} else {
		USB_DevDisconnect();
	}
#else /* USE_STM3210B_EVAL or USE_STM3210E_EVAL */
	if (NewState != DISABLE) {
		GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	} else {
		GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	}
#endif /* USE_STM3210C_EVAL */
}
#endif

/*******************************************************************************
* Function Name  :  USART_Config_Default.
* Description    :  configure the EVAL_COM1 with default values.
* Input          :  None.
* Return         :  None.
*******************************************************************************/
void USART_Config_Default(void)
{
	/* EVAL_COM1 default configuration */
	/* EVAL_COM1 configured as follow:
	      - BaudRate = 9600 baud
	      - Word Length = 8 Bits
	      - One Stop Bit
	      - Parity Odd
	      - Hardware flow control desabled
	      - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_Odd;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure and enable the USART */
	STM_EVAL_COMInit(COM1, &USART_InitStructure);

	/* Enable the USART Receive interrupt */
	USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

//	led2_on();
}

/*******************************************************************************
* Function Name  :  USART_Config.
* Description    :  Configure the EVAL_COM1 according to the linecoding structure.
* Input          :  None.
* Return         :  Configuration status
                    TRUE : configuration done with success
                    FALSE : configuration aborted.
*******************************************************************************/
bool USART_Config(void)
{

	/* set the Stop bit*/
	switch (linecoding.format) {
	case 0:
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		break;
	case 1:
		USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
		break;
	case 2:
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
		break;
	default : {
		USART_Config_Default();
		return (FALSE);
	}
	}

	/* set the parity bit*/
	switch (linecoding.paritytype) {
	case 0:
		USART_InitStructure.USART_Parity = USART_Parity_No;
		break;
	case 1:
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		break;
	case 2:
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		break;
	default : {
		USART_Config_Default();
		return (FALSE);
	}
	}

	/*set the data type : only 8bits and 9bits is supported */
	switch (linecoding.datatype) {
	case 0x07:
		/* With this configuration a parity (Even or Odd) should be set */
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	case 0x08:
		if (USART_InitStructure.USART_Parity == USART_Parity_No) {
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		} else {
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		}

		break;
	default : {
		USART_Config_Default();
		return (FALSE);
	}
	}

	USART_InitStructure.USART_BaudRate = linecoding.bitrate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure and enable the USART */
	STM_EVAL_COMInit(COM1, &USART_InitStructure);

	return (TRUE);
}


/*******************************************************************************
* Function Name  : USB_To_USART_Send_Data.
* Description    : send the received data from USB to the UART 0.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
#if	0
void USB_To_USART_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes)
{

	uint32_t i;

	for (i = 0; i < Nb_bytes; i++) {
		USART_SendData(EVAL_COM1, *(data_buffer + i));
		while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET);
	}
}
#endif
/*******************************************************************************
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
#if	0
void Handle_USBAsynchXfer (void)
{

	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;

	if (USB_Tx_State != 1) {
		if (USART_Rx_ptr_out == USART_RX_DATA_SIZE) {
			USART_Rx_ptr_out = 0;
		}

		if (USART_Rx_ptr_out == USART_Rx_ptr_in) {
			USB_Tx_State = 0;
			return;
		}

		if (USART_Rx_ptr_out > USART_Rx_ptr_in) { /* rollback */
			USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out;
		} else {
			USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
		}

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
		USB_Tx_State = 1;

#ifdef USE_STM3210C_EVAL
		USB_SIL_Write(EP1_IN, &USART_Rx_Buffer[USB_Tx_ptr], USB_Tx_length);
#else
		UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
		SetEPTxCount(ENDP1, USB_Tx_length);
		SetEPTxValid(ENDP1);
#endif
	}

}


#endif
/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void USART_To_USB_Send_Data(void)
{
	//
	// USART受信された１文字を 受信バッファに溜める.
	//
	if (linecoding.datatype == 7) {		// 7bitならMASK処理をする.
		USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1) & 0x7F;
	} else if (linecoding.datatype == 8) {
		USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1);
	}

	USART_Rx_ptr_in++;

	//
	//	リングバッファ的な処理.
	//
	/* To avoid buffer overflow */
	if (USART_Rx_ptr_in == USART_RX_DATA_SIZE) {
		USART_Rx_ptr_in = 0;
	}
#if	0
	led2_flip(0);
#endif
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

	Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
	Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
	Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

	Device_Serial0 += Device_Serial2;

	if (Device_Serial0 != 0) {
		IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
		IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
	}
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
	uint8_t idx = 0;

	for ( idx = 0 ; idx < len ; idx ++) {
		if ( ((value >> 28)) < 0xA ) {
			pbuf[ 2* idx] = (value >> 28) + '0';
		} else {
			pbuf[2* idx] = (value >> 28) + 'A' - 10;
		}

		value = value << 4;

		pbuf[ 2* idx + 1] = 0;
	}
}
#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_OTG_BSP_uDelay.
* Description    : provide delay (usec).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
	RCC_ClocksTypeDef  RCC_Clocks;

	/* Configure HCLK clock as SysTick clock source */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	RCC_GetClocksFreq(&RCC_Clocks);

	SysTick_Config(usec * (RCC_Clocks.HCLK_Frequency / 1000000));

	SysTick->CTRL  &= ~SysTick_CTRL_TICKINT_Msk ;

	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}
#endif
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
