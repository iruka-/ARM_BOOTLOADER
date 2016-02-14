

#ifdef	LPC214x
#include	"usbhw_lpc214x.h"
#else
#include	"usbhw_lpc2388.h"
#endif



/* USBIntSt bits */
#define USB_INT_REQ_LP				(1<<0)
#define USB_INT_REQ_HP				(1<<1)
#define USB_INT_REQ_DMA				(1<<2)
#define USB_need_clock				(1<<8)
#define EN_USB_BITS					(1<<31)

/* USBDevInt... bits */
#define FRAME						(1<<0)
#define EP_FAST						(1<<1)
#define EP_SLOW						(1<<2)
#define DEV_STAT					(1<<3)
#define CCEMTY						(1<<4)
#define CDFULL						(1<<5)
#define RxENDPKT					(1<<6)
#define TxENDPKT					(1<<7)
#define EP_RLZED					(1<<8)
#define ERR_INT						(1<<9)

/* USBRxPLen bits */
#define PKT_LNGTH					(1<<0)
#define PKT_LNGTH_MASK				0x3FF
#define DV							(1<<10)
#define PKT_RDY						(1<<11)

/* USBCtrl bits */
#define RD_EN						(1<<0)
#define WR_EN						(1<<1)
#define LOG_ENDPOINT				(1<<2)

/* protocol engine command codes */
	/* device commands */
#define CMD_DEV_SET_ADDRESS			0xD0
#define CMD_DEV_CONFIG				0xD8
#define CMD_DEV_SET_MODE			0xF3
#define CMD_DEV_READ_CUR_FRAME_NR	0xF5
#define CMD_DEV_READ_TEST_REG		0xFD
#define CMD_DEV_STATUS				0xFE		/* read/write */
#define CMD_DEV_GET_ERROR_CODE		0xFF
#define CMD_DEV_READ_ERROR_STATUS	0xFB
	/* endpoint commands */
#define CMD_EP_SELECT				0x00
#define CMD_EP_SELECT_CLEAR			0x40
#define CMD_EP_SET_STATUS			0x40
#define CMD_EP_CLEAR_BUFFER			0xF2
#define CMD_EP_VALIDATE_BUFFER		0xFA

/* set address command */
#define DEV_ADDR					(1<<0)
#define DEV_EN						(1<<7)

/* configure device command */
#define CONF_DEVICE					(1<<0)

/* set mode command */
#define AP_CLK						(1<<0)
#define INAK_CI						(1<<1)
#define INAK_CO						(1<<2)
#define INAK_II						(1<<3)
#define INAK_IO						(1<<4)
#define INAK_BI						(1<<5)
#define INAK_BO						(1<<6)

/* set get device status command */
#define CON							(1<<0)
#define CON_CH						(1<<1)
#define SUS							(1<<2)
#define SUS_CH						(1<<3)
#define RST							(1<<4)

/* get error code command */
// ...

/* Select Endpoint command read bits */
#define EPSTAT_FE					(1<<0)
#define EPSTAT_ST					(1<<1)
#define EPSTAT_STP					(1<<2)
#define EPSTAT_PO					(1<<3)
#define EPSTAT_EPN					(1<<4)
#define EPSTAT_B1FULL				(1<<5)
#define EPSTAT_B2FULL				(1<<6)

/* CMD_EP_SET_STATUS command */
#define EP_ST						(1<<0)
#define EP_DA						(1<<5)
#define EP_RF_MO					(1<<6)
#define EP_CND_ST					(1<<7)

/* read error status command */
#define PID_ERR						(1<<0)
#define UEPKT						(1<<1)
#define DCRC						(1<<2)
#define TIMEOUT						(1<<3)
#define EOP							(1<<4)
#define B_OVRN						(1<<5)
#define BTSTF						(1<<6)
#define TGL_ERR						(1<<7)








