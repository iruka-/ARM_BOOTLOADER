/**
* @file          usbf_msc_scsi.c
* @brief         SCSI command process module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "usbf_msc.h"
#include "usbf_msc_api.h"
#include "usbf_msc_scsi.h"
#include "usbf_msc_bot.h"

/*----------------------------------------------------------------------------*/
/* Local Constant Definition                                                  */
/*----------------------------------------------------------------------------*/

/* the length of REQUEST SENSE command from Windows */
#define SCSI_CMD_REQUEST_SENSE_LENGTH_WINDOWS                             (0x0C)

/*----------------------------------------------------------------------------*/
/* Local Variable Definition                                                  */
/*----------------------------------------------------------------------------*/

static uint8_t  s_current_lun;         /* current LUN number           */
static uint32_t s_current_lba;         /* current logic block address  */
static uint32_t s_remain_size;         /* remain data size             */
static uint32_t s_transfered_size;     /* trasferred data size         */

static USBF_MSC_ST_LUN_DATA          s_lun_info[USBF_MSC_LUN_NUM];/* storage of information about LUN                 */
static SCSI_ST_REQUEST_SENSE_DATA    s_request_sense_data;        /* response data for REQUEST SENSE command          */
static SCSI_ST_MODE_SENSE_6_DATA     s_mode_sense_data;           /* response data for MODE SENSE(6)command           */
static SCSI_ST_READ_CAPACITY_10_DATA s_read_capacity_10_data;     /* response data for READ CAPACITY(10)command       */

/* response data for INQUIRY command(support only 36bytes) */
static SCSI_ST_INQUIRY_DATA      s_inquiry_data =
{
#if (CPU_BITORDER == CPU_LSBFIRST)
    SCSI_DEVICE_TYPE_SBC2,               /* Peripheral device type                        */
    SCSI_QUALIFIER,                      /* Peripheral qualifier                          */
    0x00,                                /* Reserved bits                                 */
    SCSI_REMOVABLE_MEDIA,                /* Is media removable ?                          */
    SCSI_VERSION_SPC3,                   /* SPC version used                              */
    0x02,                                /* Must be 0x2                                   */
    0x00,                                /* Hierarchical addressing used ?                */
    0x00,                                /* ACA attribute supported ?                     */
    0x00,                                /* Obsolete bits                                 */
    0x1F,                                /* Length of remaining INQUIRY data              */
    0x00,                                /* Protection info supported ?                   */
    0x00,                                /* Reserved bits                                 */
    0x00,                                /* Third-party copy supported ?                  */
    0x00,                                /* Target port support group                     */
    0x00,                                /* Access control coordinator ?                  */
    0x00,                                /* Embedded SCC ?                                */
    0x00,                                /* 16 bit address                                */
    0x00,                                /* Obsolete bits                                 */
    0x00,                                /* medium changer bit                            */
    0x00,                                /* Multi-port device ?                           */
    0x00,                                /* vendor specific                               */
    0x00,                                /* Embedded enclosure service comp?              */
    0x00,                                /* Task management model supported ?             */
    0x00,                                /* vendor specific                               */
    0x00,                                /* Task management model supported ?             */
    0x00,                                /* Obsolete bit                                  */
    0x00,                                /* Linked commands  supported ?                  */
    0x00,                                /* synchronous transfer bit                      */
    0x00,                                /* wide bus 16 bit                               */
    0x00,                                /* Obsolete bits                                 */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    SCSI_QUALIFIER,                      /* Peripheral qualifier                          */
    SCSI_DEVICE_TYPE_SBC2,               /* Peripheral device type                        */
    SCSI_REMOVABLE_MEDIA,                /* Is media removable ?                          */
    0x00,                                /* Reserved bits                                 */
    SCSI_VERSION_SPC3,                   /* SPC version used                              */
    0x00,                                /* Obsolete bits                                 */
    0x00,                                /* ACA attribute supported ?                     */
    0x00,                                /* Hierarchical addressing used ?                */
    0x02,                                /* Must be 0x2                                   */
    0x1F,                                /* Length of remaining INQUIRY data              */
    0x00,                                /* Embedded SCC ?                                */
    0x00,                                /* Access control coordinator ?                  */
    0x00,                                /* Target port support group                     */
    0x00,                                /* Third-party copy supported ?                  */
    0x00,                                /* Reserved bits                                 */
    0x00,                                /* Protection info supported ?                   */
    0x00,                                /* Task management model supported ?             */
    0x00,                                /* Embedded enclosure service comp ?             */
    0x00,                                /* vendor specific                               */
    0x00,                                /* Multi-port device ?                           */
    0x00,                                /* medium changer bit                            */
    0x00,                                /* Obsolete bits                                 */
    0x00,                                /* 16 bit address                                */
    0x00,                                /* Obsolete bits                                 */
    0x00,                                /* wide bus 16 bit                               */
    0x00,                                /* synchronous transfer bit                      */
    0x00,                                /* Linked commands supported ?                   */
    0x00,                                /* Obsolete bit                                  */
    0x00,                                /* Task management model supported ?             */
    0x00,                                /* vendor specific                               */
#endif

    {USBF_MSC_VENDOR_ID},              /* T10 vendor identification            */
    {USBF_MSC_PRODUCT_ID},             /* Vendor-defined product ID            */
    {USBF_MSC_PRODUCT_REVISION_LEVEL}, /* Vendor-defined product revision      */
};

/*----------------------------------------------------------------------------*/
/* Local Function Prototypes                                                  */
/*----------------------------------------------------------------------------*/
/* sense data initialize */
static void initialize_sense_data(void);

/* sense data setting */
static void set_sense_data(uint8_t sense_key, uint8_t asc, uint8_t ascq);

/* check INQUIRY command */
static uint8_t inquiry_check(const SCSI_ST_CDB *scsi_cdb,
                             uint8_t           *data_type_ptr,
                             uint32_t          *data_size_ptr);

/* INQUIRY command process */
static void inquiry_process(uint8_t *data_buffer, uint32_t buffer_size);

/* check READ CAPACITY(10)command */
static uint8_t read_capacity10_check(const SCSI_ST_CDB *scsi_cdb,
                                     uint8_t           *data_type_ptr,
                                     uint32_t          *data_size_ptr);

/* READ CAPACITY(10) command process */
static void read_capacity10_process(uint8_t *data_buffer, uint32_t buffer_size);


/* check REQUEST SENSE command */
static uint8_t request_sense_check(const SCSI_ST_CDB *scsi_cdb,
                                   uint8_t           *data_type_ptr,
                                   uint32_t          *data_size_ptr);

/* REQUEST SENSE command process */
static void request_sense_process(uint8_t *data_buffer, uint32_t buffer_size);

/* check READ(10) command */
static uint8_t read10_check(const SCSI_ST_CDB *scsi_cdb,
                            uint8_t           *data_type_ptr,
                            uint32_t          *data_size_ptr);

/* READ(10) command process */
static uint8_t read10_process(const SCSI_ST_READ_10_CMD *cmd,
                              const uint8_t                      *data_buffer,
                              uint32_t                            buffer_size);

/* check TEST UNIT READY command */
static uint8_t test_unit_ready_check(const SCSI_ST_CDB *scsi_cdb,
                                     uint8_t           *data_type_ptr,
                                     uint32_t          *data_size_ptr);

/* TEST UNIT READY command process */
static uint8_t test_unit_ready_process(void);

/* check WRITE(10)command */
static uint8_t write10_check(const SCSI_ST_CDB *scsi_cdb,
                             uint8_t           *data_type_ptr,
                             uint32_t          *data_size_ptr);

/* TEST UNIT READY command process */
static uint8_t write10_process(const SCSI_ST_WRITE_10_CMD *cmd,
                               const uint8_t                       *data_buffer,
                               uint32_t                            buffer_size);

/* check PREVENT/ALLOW MEDIUM REMOVAL command */
static uint8_t medium_removal_check(const SCSI_ST_CDB *scsi_cdb,
                                    uint8_t           *data_type_ptr,
                                    uint32_t          *data_size_ptr);

/* PREVENT/ALLOW MEDIUM REMOVAL command process */
static uint8_t medium_removal_process(const SCSI_ST_MEDIUM_REMOVAL_CMD *cmd);

/* check START STOP UNIT command */
static uint8_t start_stop_unit_check(const SCSI_ST_CDB *scsi_cdb,
                                     uint8_t           *data_type_ptr,
                                     uint32_t          *data_size_ptr);

/* START STOP UNIT command process */
static uint8_t start_stop_unit_process(const SCSI_ST_START_STOP_UNIT_CMD *cmd);

/* chenk VERIFY(10) command */
static uint8_t verify10_check(const SCSI_ST_CDB *scsi_cdb,
                              uint8_t           *data_type_ptr,
                              uint32_t          *data_size_ptr);

/* VERIFY(10) command process */
static uint8_t verify10_process(const SCSI_ST_VERIFY_10_CMD *cmd);

/* check MODE SENSE(6)command */
static uint8_t mode_sense6_check(const SCSI_ST_CDB *scsi_cdb,
                                 uint8_t           *data_type_ptr,
                                 uint32_t          *data_size_ptr);

/* MODE SENSE(6) command process */
static void mode_sense6_process(uint8_t *data_buffer, uint32_t buffer_size);

/* converter function from big endian byte buffer to 32 bit integer */
static uint32_t LOAD_UI32(uint8_t* bytes);

/* converter function from big endian byte buffer to 16 bit integer */
static uint16_t LOAD_UI16(uint8_t* bytes);

/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/
/* modify macro to local-function for less-ROM  */
/* converter function from big endian byte buffer to 32 bit integer */
static uint32_t LOAD_UI32(uint8_t* bytes)
{
    return ((uint32_t) (((uint32_t)bytes[0] << 24) | \
                        ((uint32_t)bytes[1] << 16) | \
                        ((uint32_t)bytes[2] <<  8) | \
                         (uint32_t)bytes[3]));
}
/* converter function from big endian byte buffer to 16 bit integer */
static uint16_t LOAD_UI16(uint8_t* bytes)
{
    return  ((uint16_t) (((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1]));
}
/**
* @brief            SCSI initialize process
*
* @param            max_block_size_ptr   max size of block storage
* @return           TRUE        initialize normally end
* @return           FALSE       initialize innormally end
*
**/
uint8_t msc_scsi_init(uint32_t *max_block_size_ptr)
{
    uint8_t  i;                         /* loop count */
    uint8_t  media_ret = TRUE;          /* return value */
    uint32_t l_block_size = 0;          /* larger block size */

    /* block information of LUN initialize */
    (void)memset(s_lun_info, 0, sizeof(USBF_MSC_ST_LUN_DATA) * USBF_MSC_LUN_NUM);

    /* block information of LUN in support initialize */
    for (i = 0; i < USBF_MSC_LUN_NUM; i++)
    {
        /* get information about LUN */
        media_ret = usbf_msc_api_media_getluninfo(i, &s_lun_info[i]);

        if ((media_ret == TRUE) &&                                            /* when getted information about LUN is normall */
            (s_lun_info[i].BlockSize != 0) &&                                 /* when block size is not 0 */
            ((s_lun_info[i].BlockSize % USBF_MSC_BULKOUT_EP_PKT_SIZE) == 0))  /* when it is mulpile of bulk-out transfer endpoint packet size  */
        {
            /* set larger block size */
            if (l_block_size < s_lun_info[i].BlockSize)
            {
                l_block_size = s_lun_info[i].BlockSize;
            }
        }
        else
        {
            /* the condition of above is abnormally endity,skip from loop */
            break;
        }
    }

    if (media_ret == TRUE)
    {
        /* set max block size */
        *max_block_size_ptr = l_block_size;

        /* initialize response data for MODE SENSE(6) command */
        (void)memset(&s_mode_sense_data, 0, sizeof(SCSI_ST_MODE_SENSE_6_DATA));
        s_mode_sense_data.ModeDataLength = sizeof(SCSI_ST_MODE_SENSE_6_DATA) - 1;

        /* initialize response data for READ CAPACITY(10) command */
        (void)memset(&s_read_capacity_10_data, 0, sizeof(SCSI_ST_READ_CAPACITY_10_DATA));

        /* sense data initialize */
        initialize_sense_data();
    }

    return(media_ret);
}

/**
* @brief            check SCSI command
*
* @param            lun                  specified LUN
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        SCSI command check process normally end
* @return           FALSE       SCSI command check process abnormally end
*
**/
uint8_t msc_scsi_cmd_check(uint8_t               lun,
                           const SCSI_ST_CDB     *scsi_cdb,
                           uint8_t               *data_type_ptr,
                           uint32_t              *data_size_ptr)
{
    uint8_t  ret;                                           /* return value */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* set current LUN number */
    s_current_lun = lun;

        /* check INQUIRY command */
        if (cmd->OperationCode == SCSI_CMD_INQUIRY)
        {
            ret = inquiry_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }
        /* check READ CAPACITY(10) command */
        else if (cmd->OperationCode == SCSI_CMD_READ_CAPACITY_10)
        {
            ret = read_capacity10_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check REQUEST SENSE command */
        else if (cmd->OperationCode == SCSI_CMD_REQUEST_SENSE)
        {
            ret = request_sense_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check TEST UNIT READY command */
        else if (cmd->OperationCode == SCSI_CMD_TEST_UNIT_READY)
        {
            ret = test_unit_ready_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check READ(10) command */
        else if (cmd->OperationCode == SCSI_CMD_READ_10)
        {
            ret = read10_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check WRITE(10) command */
        else if (cmd->OperationCode == SCSI_CMD_WRITE_10)
        {
            ret = write10_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check PREVENT/ALLOW MEDIUM REMOVAL command */
        else if (cmd->OperationCode == SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL)
        {
            ret = medium_removal_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check START STOP UNIT command */
        else if (cmd->OperationCode == SCSI_CMD_START_STOP_UNIT)
        {
            ret = start_stop_unit_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* check MODE SENSE(6) command */
        else if (cmd->OperationCode == SCSI_CMD_MODE_SENSE_6)
        {

            ret = mode_sense6_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }

        /* chenk VERIFY(10) command */
        else if (cmd->OperationCode == SCSI_CMD_VERIFY_10)
        {
            ret = verify10_check(scsi_cdb, data_type_ptr, data_size_ptr);
        }
        else
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_COMMAND_OP_CODE,
                           SCSI_ASCQ_INVALID_COMMAND_OP_CODE);

            /* rerurn command error */
            ret = FALSE;
        }


    /* retreat transfer remain size */
    s_remain_size = *data_size_ptr;

    /* retreat tranferred size */
    s_transfered_size = 0;

    return(ret);
}

/**
* @brief            SCSI command process
*
* @param            scsi_cdb             SCSI command
* @param            data_buffer          transfer data buffer storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        command check process normally end
* @return           FALSE       command check process abnormally end
*
**/
uint8_t msc_scsi_cmd_process(const SCSI_ST_CDB *scsi_cdb,
                             uint8_t           *data_buffer,
                             uint32_t          buffer_size)
{
    uint8_t ret = TRUE;                                     /* return value */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

        /* INQUIRY command process */
        if (cmd->OperationCode == SCSI_CMD_INQUIRY)
        {
            inquiry_process(data_buffer, buffer_size);
        }

        /* READ CAPACITY(10) command process */
        else if (cmd->OperationCode == SCSI_CMD_READ_CAPACITY_10)
        {
            read_capacity10_process(data_buffer, buffer_size);
        }

        /* REQUEST SENSE command process */
        else if (cmd->OperationCode == SCSI_CMD_REQUEST_SENSE)
        {
            request_sense_process(data_buffer, buffer_size);
        }

        /* TEST UNIT READY command process */
        else if (cmd->OperationCode == SCSI_CMD_TEST_UNIT_READY)
        {
            ret = test_unit_ready_process();
        }

        /* READ(10) command process */
        else if (cmd->OperationCode == SCSI_CMD_READ_10)
        {
            ret = read10_process(&(cmd->Read10), data_buffer, buffer_size);
        }

        /* WRITE(10) command process */
        else if (cmd->OperationCode == SCSI_CMD_WRITE_10)
        {
            ret = write10_process(&(cmd->Write10), data_buffer, buffer_size);
        }

        /* PREVENT/ALLOW MEDIUM REMOVAL command process */
        else if (cmd->OperationCode == SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL)
        {
            ret = medium_removal_process(&(cmd->MediumRemoval));
        }

        /* START STOP UNIT command process */
        else if (cmd->OperationCode == SCSI_CMD_START_STOP_UNIT)
        {
            ret = start_stop_unit_process(&(cmd->StartStopUnit));
        }

        /* MODE SENSE(6) command process */
        else if (cmd->OperationCode == SCSI_CMD_MODE_SENSE_6)
        {
            mode_sense6_process(data_buffer, buffer_size);
        }

        /* VERIFY(10) command process */
        else if (cmd->OperationCode == SCSI_CMD_VERIFY_10)
        {
            ret = verify10_process(&(cmd->Verify10));
        }

        /* excluding above */
        else
        {

            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_COMMAND_OP_CODE,
                           SCSI_ASCQ_INVALID_COMMAND_OP_CODE);

            /* return command process abnormally end */
            ret = FALSE;
        }

    return(ret);
}

/**
* @brief            sense data initialize
*
* @param            none
* @return           none
*
**/
static void initialize_sense_data(void)
{
    /* initialize sense data by 0 */
    (void)memset(&s_request_sense_data, 0, sizeof(SCSI_ST_REQUEST_SENSE_DATA));

    /* fix RESPONSE CODE by 0x70 */
    s_request_sense_data.ResponseCode = SCSI_REQUEST_SENSE_RESPONSE_CODE;

    /* set ADDITIONAL SENSE LENGTH */
    s_request_sense_data.AdditionalSenseLength = sizeof(SCSI_ST_REQUEST_SENSE_DATA) - 7;

    return;
}

/**
* @brief            set sense data
*
* @param            sense_key         SENSE KEY
* @param            asc               ADDITIONAL SENSE CODE
* @param            ascq              ADDITIONAL SENSE CODE QUALIFIER
* @return           none
*
**/
static void set_sense_data(uint8_t sense_key, uint8_t asc, uint8_t ascq)
{
    /* set SENSE KEY */
    s_request_sense_data.SenseKey = sense_key;

    /* set ADDITIONAL SENSE CODE */
    s_request_sense_data.AdditionalSenseCode = asc;

    /* set ADDITIONAL SENSE CODE QUALIFIER */
    s_request_sense_data.AdditionalSenseCodeQualifier = ascq;

    return;
}

/**
* @brief            check INQUIRY command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        INQUIRY command check process normally end
* @return           FALSE       INQUIRY command check process abnormally end
*
**/
static uint8_t inquiry_check(const SCSI_ST_CDB *scsi_cdb,
                             uint8_t           *data_type_ptr,
                             uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;                                    /* initialize return value by normally end */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* when CDB length is not correspond with INQUIRY command */
    if (scsi_cdb->CDBLength != SCSI_CMD_INQUIRY_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /* when vital product data(VPD)is disable(EVPD = 0) */
        if (cmd->Inquiry.EVPD == 0)
        {
            /* when PageCode is not 0,and AllocationLength is smaller then 5 */
            if ((LOAD_UI16(cmd->Inquiry.AllocationLength) < SCSI_INQUIRY_MIN_ALLOCATION_LENGTH) ||
                (cmd->Inquiry.PageCode != 0))
            {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                               SCSI_ASC_INVALID_FIELD_IN_CDB,
                               SCSI_ASCQ_INVALID_FIELD_IN_CDB);

                /* return abnormally end */
                ret = FALSE;
            }
            else
            {
                /* set [transfer from device from host] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_DEVICE_TO_HOST;

                /*set the smaller one that between  AllocationLength fiels and standard INQUIRY data size to response data size */
                 *data_size_ptr = ( sizeof(SCSI_ST_INQUIRY_DATA) < LOAD_UI16(cmd->Inquiry.AllocationLength))
                                  ? sizeof(SCSI_ST_INQUIRY_DATA) : LOAD_UI16(cmd->Inquiry.AllocationLength);
            }
        }
        /* when vital product data(VPD) is enable(EVPD = 1) */
        else /* when EVPD = 1,because it is whitout support ,processed as invalid request */
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
    }
    return(ret);
}

/**
* @brief            INQUIRY command process
*
* @param            data_buffer          transfer data buffer storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        INQUIRY command process normally end
* @return           FALSE       INQUIRY command process abnormally end
*
**/
static void inquiry_process(uint8_t *data_buffer, uint32_t buffer_size)
{
    uint32_t size;    /* transfer size */

    /* use the smaller one that between transfer data buffer size and transfer remain data size as transfer size */
    size = (buffer_size > s_remain_size) ? s_remain_size : buffer_size;

    /* set response data */
    (void)memcpy(data_buffer, &(((uint8_t*)&s_inquiry_data)[s_transfered_size]), size);

    /* renew transfer remain data size */
    s_remain_size -= size;

    /* renew transferred data size */
    s_transfered_size += size;

    return;
}

/**
* @brief            check READ CAPACITY(10) command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t read_capacity10_check(const SCSI_ST_CDB *scsi_cdb,
                                     uint8_t           *data_type_ptr,
                                     uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;                                    /* initialize return value by that normally end */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* when CDB length is not correspond with READ CAPACITY(10) command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_READ_CAPACITY_10_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /* when PMI is not 0,do the following process */
        if (cmd->ReadCapacity10.PMI != 0)
        {
            /* when logic block address is smaller then max logic block address */
            if (LOAD_UI32(cmd->ReadCapacity10.LogicalBlockAddress) <= (s_lun_info[s_current_lun].BlockNum - 1))
            {
                /* set [transfer fron devise to host] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_DEVICE_TO_HOST;

                /* set CAPACITY data size */
                *data_size_ptr = sizeof(SCSI_ST_READ_CAPACITY_10_DATA);
            }
            else
            {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                               SCSI_ASC_INVALID_FIELD_IN_CDB,
                               SCSI_ASCQ_INVALID_FIELD_IN_CDB);

                /* return abnormally end */
                ret = FALSE;
            }
        }
        /* when PMI is 0,do the following process */
        else
        {
            /* when logic block address is 0 */
            if (LOAD_UI32(cmd->ReadCapacity10.LogicalBlockAddress) == 0)
            {
                /* set [transfer fron devise to host] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_DEVICE_TO_HOST;

                /* set CAPACITY data size */
                *data_size_ptr = sizeof(SCSI_ST_READ_CAPACITY_10_DATA);
            }
            else
            {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                               SCSI_ASC_INVALID_FIELD_IN_CDB,
                               SCSI_ASCQ_INVALID_FIELD_IN_CDB);

                /* return abnormally end */
                ret = FALSE;
            }

        }
    }
    return(ret);
}

/**
* @brief            READ CAPACITY(10) command process
*
* @param            data_buffer          transfer data buffer storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static void read_capacity10_process(uint8_t *data_buffer, uint32_t buffer_size)
{
    uint32_t                         size;     /* transfer size */

    /* set max logic block address */
    s_read_capacity_10_data.LogicalBlockAddress = WORD_CPU_TO_BE(s_lun_info[s_current_lun].BlockNum - 1);

    /* set block length */
    s_read_capacity_10_data.LogicalBlockLength = WORD_CPU_TO_BE(s_lun_info[s_current_lun].BlockSize);

    /* use the smaller one that between transfer data buffer size and transfer remain data size as transfer size */
    size = (buffer_size > s_remain_size) ? s_remain_size : buffer_size;

    /* set response data */
    (void)memcpy(data_buffer, &(((uint8_t*)&s_read_capacity_10_data)[s_transfered_size]), size);

    /* renew transfer remain data size */
    s_remain_size -= size;

    /* renew transferred data size */
    s_transfered_size += size;

    return;
}

/**
* @brief            check TEST UNIT READY command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t test_unit_ready_check(const SCSI_ST_CDB *scsi_cdb,
                                     uint8_t           *data_type_ptr,
                                     uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;    /* initialize return value by that normally end */

    /* when CDB length is not correspond with TEST UNIT READY command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_TEST_UNIT_READY_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    else
    {
        /* set [no transfer] to data transfer type */
        *data_type_ptr = TRANSFER_TYPE_NO_TRANSFER;

        /* set 0 to response data size */
        *data_size_ptr = 0;
    }

    return(ret);
}

/**
* @brief            TEST UNIT READY command process
*
* @param            none
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t test_unit_ready_process(void)
{
    USBF_MSC_ST_LUN_DATA lun_data; /* LUN information */
    uint8_t  ret = FALSE;          /* return value */
    uint8_t  func_ret;             /* call function return value */

    /* get LUN information from high-rank layer application */
    func_ret = usbf_msc_api_media_getluninfo(s_current_lun, &lun_data);

    /* when getting is normally end,do the following process */
    if (func_ret == TRUE)
    {
        /* judge media status of LUN information */
        /* good status */
        if (lun_data.MediaStatus == SCSI_UNIT_STATUS_GOOD)
        {
                /* return normally end */
                ret = TRUE;
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NO_SENSE,
                               SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION,
                               SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION);
        }

        /* logical unit not supported */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_SUPPORTED)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                               SCSI_ASC_LOGICAL_UNIT_NOT_SUPPORTED,
                               SCSI_ASCQ_LOGICAL_UNIT_NOT_SUPPORTED);
        }

        /* logical unit does not respond to selection */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION,
                               SCSI_ASCQ_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION);
        }

        /* medium not present */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_MEDIUM_NOT_PRESENT)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_MEDIUM_NOT_PRESENT,
                               SCSI_ASCQ_MEDIUM_NOT_PRESENT);
        }

        /* logical unit not ready,cause not reportable */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_CAUSE_NOT_REPORTABLE)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_LOGICAL_UNIT_NOT_READY_CAUSE_NOT_REPORTABLE,
                               SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_CAUSE_NOT_REPORTABLE);
        }

        /* logical unit is in process of becoming ready */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_IS_IN_PROCESS_OF_BECOMING_READY)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_LOGICAL_UNIT_IS_IN_PROCESS_OF_BECOMING_READY,
                               SCSI_ASCQ_LOGICAL_UNIT_IS_IN_PROCESS_OF_BECOMING_READY);
        }

        /* logical unit not ready,initializing command required */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_INITIALIZING_COMMAND_REQUIRED)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_LOGICAL_UNIT_NOT_READY_INITIALIZING_COMMAND_REQUIRED,
                               SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_INITIALIZING_COMMAND_REQUIRED);
        }

        /* logical unit not ready,manual intervention required */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED,
                               SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED);
        }

        /* logical unit not ready,format in progress */
        else if (lun_data.MediaStatus == SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_FORMAT_IN_PROGRESS)
        {
                /* set sense data */
                set_sense_data(SCSI_SENSE_KEY_NOT_READY,
                               SCSI_ASC_LOGICAL_UNIT_NOT_READY_FORMAT_IN_PROGRESS,
                               SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_FORMAT_IN_PROGRESS);
        }

        else
        {
        }
    }

    return(ret);
}

/**
* @brief            check PREVENT/ALLOW MEDIUM REMOVAL command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t medium_removal_check(const SCSI_ST_CDB *scsi_cdb,
                                    uint8_t           *data_type_ptr,
                                    uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;                                    /* initialize return value by that normally end */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* when CDB length is not correspond with PREVENT/ALLOW MEDIUM REMOVAL command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /* when PREVENT is abnormally end */
        if ((cmd->MediumRemoval.Prevent != 0x00) && (cmd->MediumRemoval.Prevent != 0x01))
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
        else
        {
            /* set [no transfer] to data transfer type */
            *data_type_ptr = TRANSFER_TYPE_NO_TRANSFER;

            /* set 0 to response data size */
            *data_size_ptr = 0;
        }
    }
    return(ret);
}

/**
* @brief            PREVENT/ALLOW MEDIUM REMOVAL command process
*
* @param            cmd                  SCSI command
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t medium_removal_process(const SCSI_ST_MEDIUM_REMOVAL_CMD *cmd)
{
    uint8_t  ret = TRUE;                       /* return value */
    uint8_t  media_ret;                        /* call function return value */
    USBF_MSC_ST_SET_MEDIUM_REMOVAL   removal;  /* call function parameter */

    /* set call function parameter */
    removal.RemovalType = cmd->Prevent;

    /* media process event notify */
    media_ret = usbf_msc_api_media_event(s_current_lun, USBF_MSC_MEDIUM_EVENT_SET_REMOVAL, &removal);

    /* when process failed */
    if (media_ret != TRUE)
    {
        /* set sense data */
        set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                       SCSI_ASC_INVALID_FIELD_IN_CDB,
                       SCSI_ASCQ_INVALID_FIELD_IN_CDB);

        /* return abnormally end */
        ret = FALSE;
    }

    return(ret);
}

/**
* @brief            check START STOP UNIT command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t start_stop_unit_check(const SCSI_ST_CDB *scsi_cdb,
                                     uint8_t           *data_type_ptr,
                                     uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;                                    /* initialize return value by that normally end */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* when CDB length is not correspond with START STOP UNIT command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_START_STOP_UNIT_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /* when PowerCondition field is not corrected
           or IMMED field is not corrected */
        if (((cmd->StartStopUnit.PowerCondition != 0x00) &&
             (cmd->StartStopUnit.PowerCondition != 0x01) &&
             (cmd->StartStopUnit.PowerCondition != 0x02) &&
             (cmd->StartStopUnit.PowerCondition != 0x03) &&
             (cmd->StartStopUnit.PowerCondition != 0x07) &&
             (cmd->StartStopUnit.PowerCondition != 0x0A) &&
             (cmd->StartStopUnit.PowerCondition != 0x0B)) ||
             (cmd->StartStopUnit.IMMED == 0x00))
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
        else
        {
            /* set [no transfer] to data transfer type */
            *data_type_ptr = TRANSFER_TYPE_NO_TRANSFER;

            /* set 0 to response data size */
            *data_size_ptr = 0;
        }
    }

    return(ret);
}

/**
* @brief            START STOP UNIT command process
*
* @param            cmd                  SCSI command
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t start_stop_unit_process(const SCSI_ST_START_STOP_UNIT_CMD *cmd)
{
    uint8_t  ret = TRUE;                           /* initialize return value by that normally end */
    uint8_t  media_ret;                            /* call function return value */
    USBF_MSC_ST_POWER_CONDITION power_cdtion;      /* call function parameter */

    /* set call function parameter */
    /* when PowerCondition is 0,do the process accroding load_eject and start */
    if (cmd->PowerCondition == 0)
    {
        power_cdtion.PowerCondition = cmd->PowerCondition;
        power_cdtion.LoadEject = cmd->LOEJ;
        power_cdtion.Start = cmd->Start;
    }
    /* others */
    /* when PowerConditionis not 0,clear load_eject and start field */
    else
    {
        power_cdtion.PowerCondition = cmd->PowerCondition;
        power_cdtion.LoadEject = 0;
        power_cdtion.Start = 0;
    }

    /* media process event notify */
    media_ret = usbf_msc_api_media_event(s_current_lun, USBF_MSC_MEDIUM_EVENT_SET_POWER_CONDITION, &power_cdtion);

    /* when process failed */
    if (media_ret != TRUE)
    {
        /* return abnormally end */
        ret = FALSE;

        /* when PowerCondition is FORCE_IDLE_0 or FORCE_STANDBY_0,set sense data */
        if ((cmd->PowerCondition == 0x0A) ||
            (cmd->PowerCondition == 0x0B))
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);
        }
    }

    return(ret);
}

/**
* @brief            check READ(10) command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t read10_check(const SCSI_ST_CDB *scsi_cdb,
                            uint8_t           *data_type_ptr,
                            uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;                                       /* initialize return value by that normally end */
    uint16_t length;                                           /* analysised transfer length in command */
    uint32_t lba;                                              /* analysised logic block address in command */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;
                                                               /* retreat SCSI command */

    /* when CDB length is not correspond with READ(10) command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_READ_10_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /*set logic block address */
        lba = LOAD_UI32(cmd->Read10.LogicalBlockAddress);

        /* set transfer length */
        length = LOAD_UI16(cmd->Read10.TransferLength);

        if (((cmd->Read10.FUA == 0) && (cmd->Read10.FUA_NV == 1)) ||  /* when FUA is 0 and FUA_NV is not 0 */
            (cmd->Read10.RdProtect != 0))                             /* RDPROTECT is not 0 */
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
        /* when the sum of specified logic block address and transfer length is lager then max logic block address */
        else if ((lba + length) > s_lun_info[s_current_lun].BlockNum)
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_LBA_OUT_OF_RANGE,
                           SCSI_ASCQ_LBA_OUT_OF_RANGE);

            /* return abnormally end */
            ret = FALSE;
        }
        /* when it is normally end */
        else
        {
            /* set READ(10) data size */
            *data_size_ptr = length * s_lun_info[s_current_lun].BlockSize;

            /* when it has transfer data */
            if (length != 0)
            {
                /* set [transfer fron devise to host] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_DEVICE_TO_HOST;
            }
            /* when it is no transfer data */
            else
            {
                /* set [no transfer] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_NO_TRANSFER;
            }

            /*set logic block address */
            s_current_lba = lba;
        }
    }

    return(ret);
}

/**
* @brief            READ(10) command process
*
* @param            cmd                  SCSI command
* @param            data_buffer          transfer data buffer storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t read10_process(const SCSI_ST_READ_10_CMD *cmd,
                           const uint8_t                *data_buffer,
                                 uint32_t               buffer_size)
{
    uint8_t  ret = TRUE;               /* initialize return value by that normally end */
    uint8_t  media_ret;                /* call function return value */
    uint32_t lba_offset;               /* logic block address offset */
    uint32_t buffer_block_num;         /* transfer times which used as transfer data buffer size */
    uint32_t remain_block_num;         /* transfer times which used as transfer remain data  size */
    uint32_t min_block_num;            /* actual transfer times */
    uint32_t transfered_size;          /* current transferred size */
    USBF_MSC_ST_CACHE_INFO cache_info; /* cache information */

    /* when it has transfer data */
    if (buffer_size != 0)
    {
        /* when transfer data buffer size is not mulpile of block size  */
        if ((buffer_size % s_lun_info[s_current_lun].BlockSize) != 0)
        {
            /* return abnormally end */
            ret = FALSE;
        }
        else
        {
            /* calculate transfer times which used as transfer data buffer size */
            buffer_block_num = buffer_size / s_lun_info[s_current_lun].BlockSize;
            /* calculate transfer times which used as transfer remain data  size */
            remain_block_num = s_remain_size / s_lun_info[s_current_lun].BlockSize;
            /* among the above transfer times,set the small one to actual transfer times */
            min_block_num = (buffer_block_num > remain_block_num) ? remain_block_num : buffer_block_num;

            /* clear current transferred size */
            transfered_size = 0;

            /* set cache information */
            cache_info.OperationCode    = SCSI_CMD_READ_10;
            cache_info.DisablePageOut  = cmd->DPO;
            cache_info.ForceUnitAccess = cmd->FUA;

            /* notify media process event which setted by cache information  */
            media_ret = usbf_msc_api_media_event(s_current_lun, USBF_MSC_MEDIUM_EVENT_SET_CACHE_INFO, &cache_info);

            /* when normally end,do the following process */
            if (media_ret == TRUE)
            {
                /* actual transfer times circulate */
                for (lba_offset = 0; lba_offset < min_block_num; lba_offset++)
                {
                    /* read 1 block size data from media that begins specified block address */
                    media_ret = usbf_msc_api_media_read(s_current_lun, (s_current_lba + lba_offset), &(data_buffer[transfered_size]));

                    /* when normally end,do the following process */
                    if (media_ret == TRUE)
                    {
                        /* renew current transferred size  */
                        transfered_size += s_lun_info[s_current_lun].BlockSize;

                        /* renew transfer remain data size */
                        s_remain_size -= s_lun_info[s_current_lun].BlockSize;

                        /* renew transferred data size */
                        s_transfered_size += s_lun_info[s_current_lun].BlockSize;
                    }
                    else
                    {
                        /* return abnormally end */
                        ret = FALSE;
                        break;
                    }
                }

                /* renew current logic block address */
                s_current_lba += lba_offset;

            }
            /* when process failed */
            else
            {
                /* return abnormally end */
                ret = FALSE;
            }
        }
    }

    return(ret);
}

/**
* @brief            check WRITE(10) command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t write10_check(const SCSI_ST_CDB *scsi_cdb,
                          uint8_t              *data_type_ptr,
                          uint32_t             *data_size_ptr)
{
    uint8_t  ret = TRUE;                                     /* initialize return value by that normally end */
    uint16_t length;                                         /* analysised transfer length in command  */
    uint32_t lba;                                            /* analysised logic block address in command */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;
                                                             /* retreat SCSI command */

    /* when CDB length is not correspond with WRITE(10) command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_WRITE_10_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /*set logic block address */
        lba = LOAD_UI32(cmd->Write10.LogicalBlockAddress);

        /* set transfer length */
        length = LOAD_UI16(cmd->Write10.TransferLength);

        if (((cmd->Write10.FUA == 0) && (cmd->Write10.FUA_NV == 1)) ||  /* FUA is 0 and FUA_NV is not 0 */
            (cmd->Write10.WrProtect != 0))                              /* WRPROTECT is not 0 */
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
        /* when the sum of specified logic block address and transfer length is larger then max logic block address */
        else if ((lba + length) > s_lun_info[s_current_lun].BlockNum)
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_LBA_OUT_OF_RANGE,
                           SCSI_ASCQ_LBA_OUT_OF_RANGE);

            /* return abnormally end */
            ret = FALSE;
        }
        /* when it is normally end */
        else
        {
            /* set WRITE(10) data size */
            *data_size_ptr = length * s_lun_info[s_current_lun].BlockSize;

            /* when it has transfer data */
            if (length != 0)
            {
                /* set [transfer from host to device] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_HOST_TO_DEVICE;
            }
            /* when it has no transfer data */
            else
            {
                /* set [no transfer] to data transfer type */
                *data_type_ptr = TRANSFER_TYPE_NO_TRANSFER;
            }

            /*set logic block address */
            s_current_lba = lba;

        }
    }
    return(ret);
}

/**
* @brief           WRITE(10) command process
*
* @param            cmd                  SCSI command
* @param            data_buffer          transfer data buffer storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t write10_process(const SCSI_ST_WRITE_10_CMD *cmd,
                               const uint8_t                       *data_buffer,
                               uint32_t                            buffer_size)
{
    uint8_t  ret = TRUE;               /* initialize retuen value by that normally end */
    uint8_t  media_ret;                /* call function return value */
    uint32_t lba_offset;               /* logic block address offset */
    uint32_t buffer_block_num;         /* transfer times which used as transfer data buffer size */
    uint32_t remain_block_num;         /* transfer times which used as transfer remain data  size */
    uint32_t min_block_num;            /* actual transfer times */
    uint32_t transfered_size;          /* current transferred size */
    USBF_MSC_ST_CACHE_INFO  cache_info;/* cache information */

    /* when it has transfer data */
    if (buffer_size != 0)
    {
        /* when transfer data buffer size is not mulpile of block size */
        if ((buffer_size % s_lun_info[s_current_lun].BlockSize) != 0)
        {
            /* return abnormally end */
            ret = FALSE;
        }
        else
        {
            /* calculate transfer times which used as transfer data buffer size */
            buffer_block_num = buffer_size / s_lun_info[s_current_lun].BlockSize;
            /* calculate transfer times which used as transfer remain data  size */
            remain_block_num = s_remain_size / s_lun_info[s_current_lun].BlockSize;
            /* among the above transfer times,set the small one to actual transfer times */
            min_block_num = (buffer_block_num > remain_block_num) ? remain_block_num : buffer_block_num;

            /* clear current transferred size */
            transfered_size = 0;

            /* set cache information */
            cache_info.OperationCode    = SCSI_CMD_WRITE_10;
            cache_info.DisablePageOut  = cmd->DPO;
            cache_info.ForceUnitAccess = cmd->FUA;

            /* notify media process event which setted by cache information  */
            media_ret = usbf_msc_api_media_event(s_current_lun, USBF_MSC_MEDIUM_EVENT_SET_CACHE_INFO, &cache_info);

            /* when normally end,do the following process */
            if (media_ret == TRUE)
            {
                /* actual transfer times circulate */
                for (lba_offset = 0; lba_offset < min_block_num; lba_offset++)
                {
                    /* write 1 block size data from media that begins specified block address */
                    media_ret = usbf_msc_api_media_write(s_current_lun, (s_current_lba + lba_offset), &(data_buffer[transfered_size]));

                    /* when normally end,do the following process */
                    if (media_ret == TRUE)
                    {
                         /* renew current transferred size  */
                        transfered_size += s_lun_info[s_current_lun].BlockSize;

                        /* renew transfer remain data size */
                        s_remain_size -= s_lun_info[s_current_lun].BlockSize;

                        /* renew transferred data size */
                        s_transfered_size += s_lun_info[s_current_lun].BlockSize;
                    }
                    else
                    {
                        /* return abnormally end */
                        ret = FALSE;
                        break;
                    }
                }

                /* renew current logic block address */
                s_current_lba += lba_offset;
            }
            /* when process failed */
            else
            {
                /* return abnormally end */
                ret = FALSE;
            }
        }
    }

    return(ret);
}

/**
* @brief            check MODE SENSE(6) command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t mode_sense6_check(const SCSI_ST_CDB *scsi_cdb,
                                 uint8_t           *data_type_ptr,
                                 uint32_t          *data_size_ptr)
{
    uint8_t  ret = TRUE;                                    /* initialize return value by that normally end */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* when CDB length is not correspond with MODE SENSE(6) command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_MODE_SENSE_6_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    else
    {
         /* when PageCode field is normally end */
        if (cmd->ModeSense6.PageCode == 0x00)
        {
            /* set [transfer fron devise to host] to data transfer type */
            *data_type_ptr = TRANSFER_TYPE_DEVICE_TO_HOST;

            /* set MODE SENSE(6) data size */
            *data_size_ptr = sizeof(SCSI_ST_MODE_SENSE_6_DATA);
        }
        else
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
    }

    return(ret);
}

/**
* @brief            check REQUEST SENSE command
*
* @param            data_buffer          transfer data storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static void mode_sense6_process(uint8_t *data_buffer, uint32_t buffer_size)
{
    uint32_t   size;                       /* transfer size */

    /* use the smaller one that between transfer data buffer size and transfer remain data size as transfer size */
    size = (buffer_size > s_remain_size) ? s_remain_size : buffer_size;

    /* set response data */
    (void)memcpy(data_buffer, &(((uint8_t*)&s_mode_sense_data)[s_transfered_size]), size);

    /* renew transfer remain data size */
    s_remain_size -= size;

    /* renew transferred data size */
    s_transfered_size += size;

    return;
}

/**
* @brief            check REQUEST SENSE command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t request_sense_check(const SCSI_ST_CDB *scsi_cdb,
                                uint8_t              *data_type_ptr,
                                uint32_t             *data_size_ptr)
{
    uint8_t  ret = TRUE;                                    /* initialize return value by that normally end */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;/* retreat SCSI command */

    /* when CDB length is not correspond with REQUEST SENSE command size */
    if ((scsi_cdb->CDBLength != SCSI_CMD_REQUEST_SENSE_LENGTH) &&
        (scsi_cdb->CDBLength != SCSI_CMD_REQUEST_SENSE_LENGTH_WINDOWS))
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /* when DESC is not 0,process as invalid request */
        if (cmd->RequestSense.Desc != 0)
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
        else
        {
            /* set [transfer fron devise to host] to data transfer type */
            *data_type_ptr = TRANSFER_TYPE_DEVICE_TO_HOST;

            /* set the one between AllocationLength field and Request Sense data size to response data size */
            *data_size_ptr = ( sizeof(SCSI_ST_REQUEST_SENSE_DATA) < cmd->RequestSense.AllocationLength)
                             ? sizeof(SCSI_ST_REQUEST_SENSE_DATA) : cmd->RequestSense.AllocationLength;

        }
    }
    return(ret);
}

/**
* @brief            REQUEST SENSE command process
*
* @param            data_buffer          transfer data buffer storage
* @param            buffer_size          transfer data buffer size
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static void request_sense_process(uint8_t *data_buffer, uint32_t buffer_size)
{
    uint32_t size;    /* transfer size */

    /* use the smaller one that between transfer data buffer size and transfer remain data size as transfer size */
    size = (buffer_size > s_remain_size) ? s_remain_size : buffer_size;

    /* set response data */
    (void)memcpy(data_buffer, &(((uint8_t*)&s_request_sense_data)[s_transfered_size]), size);

    /* renew transfer remain data size */
    s_remain_size -= size;

    /* renew transferred data size */
    s_transfered_size += size;

    return;
}

/**
* @brief            chenk VERIFY(10) command
*
* @param            scsi_cdb             SCSI command
* @param            data_type_ptr        data transfer type storage
* @param            data_size_ptr        data size storage
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t verify10_check(const SCSI_ST_CDB *scsi_cdb,
                              uint8_t              *data_type_ptr,
                              uint32_t             *data_size_ptr)
{
    uint8_t  ret = TRUE;                                       /* initialize return value by that normally end */
    uint16_t length;                                           /* analysised transfer length in command  */
    uint32_t lba;                                              /* analysised logic block address in command */
    SCSI_UN_COMMAND *cmd = (SCSI_UN_COMMAND *)scsi_cdb->CDB;
                                                               /* retreat SCSI command */

    /* when CDB length is not correspond with VERIFY(10) command size */
    if (scsi_cdb->CDBLength != SCSI_CMD_VERIFY_10_LENGTH)
    {
        /* return abnormally end */
        ret = FALSE;
    }
    /* when it is the same */
    else
    {
        /*set logic block address */
        lba = LOAD_UI32(cmd->Verify10.LogicalBlockAddress);

        /* set transfer length */
        length = LOAD_UI16(cmd->Verify10.VerificationLength);

        /* when BYTCHK field is abnormally end or Vrprotect field is abnormally end */
        if ((cmd->Verify10.BYTCHK != 0x00) || (cmd->Verify10.Vrprotect != 0x00))
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);

            /* return abnormally end */
            ret = FALSE;
        }
        /* when the sum of specified logic block address and transfer length is larger then max logic block address */
        else if ((lba + length) > s_lun_info[s_current_lun].BlockNum)
        {
            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_LBA_OUT_OF_RANGE,
                           SCSI_ASCQ_LBA_OUT_OF_RANGE);

            /* return abnormally end */
            ret = FALSE;
        }
        else
        {
            /* set [no transfer] to data transfer type */
            *data_type_ptr = TRANSFER_TYPE_NO_TRANSFER;

            /* set 0 to response data size */
            *data_size_ptr = 0;
        }
    }

    return(ret);
}

/**
* @brief            VERIFY(10) command process
*
* @param            cmd                  SCSI command
* @return           TRUE        command process normally end
* @return           FALSE       command process abnormally end
*
**/
static uint8_t verify10_process(const SCSI_ST_VERIFY_10_CMD *cmd)
{
    uint8_t  ret = TRUE;                  /* initialize return value by that normally end */
    uint8_t  media_ret;                   /* call function return value */
    USBF_MSC_ST_VERIFY     verify;        /* call function parameter */
    USBF_MSC_ST_CACHE_INFO cache_info;    /* cache information */

    /* set cache information */
    cache_info.OperationCode    = SCSI_CMD_VERIFY_10;
    cache_info.DisablePageOut  = cmd->DPO;
    cache_info.ForceUnitAccess = 0;

    /* notify media process event which setted by cache information  */
    media_ret = usbf_msc_api_media_event(s_current_lun, USBF_MSC_MEDIUM_EVENT_SET_CACHE_INFO, &cache_info);

    /* when normally end,do the following process */
    if (media_ret == TRUE)
    {
        /* set call function parameter */
        verify.StartLba = LOAD_UI32((uint8_t *)cmd->LogicalBlockAddress);
        verify.VerifyLength = LOAD_UI16((uint8_t *)cmd->VerificationLength);

        /* VERIFY media process event notify */
        media_ret = usbf_msc_api_media_event(s_current_lun, USBF_MSC_MEDIUM_EVENT_VERIFY, &verify);

        /* if process failed,do the following process */
        if (media_ret != TRUE)
        {
            /* return abnormally end */
            ret = FALSE;

            /* set sense data */
            set_sense_data(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
                           SCSI_ASC_INVALID_FIELD_IN_CDB,
                           SCSI_ASCQ_INVALID_FIELD_IN_CDB);
        }
    }
    /* when process failed */
    else
    {
        /* return abnormally end */
        ret = FALSE;
    }

    return(ret);
}
