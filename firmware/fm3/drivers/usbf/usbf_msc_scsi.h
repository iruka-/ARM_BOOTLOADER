/**
* @file          usbf_msc_scsi.h
* @brief         SCSI command process module
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/

#ifndef __USBF_MSC_SCSI_INC__
#define __USBF_MSC_SCSI_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Constant Definition                                                 */
/*----------------------------------------------------------------------------*/
/* SCSI subclass command list */
#define SCSI_CMD_INQUIRY                                                       (0x12U) /* INQUIRY command (SPC-3)                      */
#define SCSI_CMD_MODE_SENSE_6                                                  (0x1AU) /* MODE SENSE (6) command (SPC-3)               */
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL                                  (0x1EU) /* PREVENT/ALLOW MEDIUM REMOVAL command (SPC-3) */
#define SCSI_CMD_REQUEST_SENSE                                                 (0x03U) /* REQUEST SENSE command (SPC-3)                */
#define SCSI_CMD_TEST_UNIT_READY                                               (0x00U) /* TEST UNIT READY command (SPC-3)              */
#define SCSI_CMD_START_STOP_UNIT                                               (0x1BU) /* START STOP UNIT command (SBC-2)              */
#define SCSI_CMD_READ_CAPACITY_10                                              (0x25U) /* READ CAPACITY (10) command (SBC-2)           */
#define SCSI_CMD_READ_10                                                       (0x28U) /* READ (10) command (SBC-2)                    */
#define SCSI_CMD_WRITE_10                                                      (0x2AU) /* WRITE (10) command (SBC-2)                   */
#define SCSI_CMD_VERIFY_10                                                     (0x2FU) /* VERIFY (10) command (SBC-2)                  */

/* SCSI command size */
#define SCSI_CMD_INQUIRY_LENGTH                                                ( 6U)  /* INQUIRY command size (SPC-3)                      */
#define SCSI_CMD_MODE_SENSE_6_LENGTH                                           ( 6U)  /* MODE SENSE (6) command size (SPC-3)               */
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL_LENGTH                           ( 6U)  /* PREVENT/ALLOW MEDIUM REMOVAL command size (SPC-3) */
#define SCSI_CMD_REQUEST_SENSE_LENGTH                                          ( 6U)  /* REQUEST SENSE command size (SPC-3)                */
#define SCSI_CMD_TEST_UNIT_READY_LENGTH                                        ( 6U)  /* TEST UNIT READY command size (SPC-3)              */
#define SCSI_CMD_START_STOP_UNIT_LENGTH                                        ( 6U)  /* START STOP UNIT command size (SBC-2)              */
#define SCSI_CMD_READ_CAPACITY_10_LENGTH                                       (10U)  /* READ CAPACITY (10) command size (SBC-2)           */
#define SCSI_CMD_READ_10_LENGTH                                                (10U)  /* READ (10) command size (SBC-2)                    */
#define SCSI_CMD_VERIFY_10_LENGTH                                              (10U)  /* WRITE (10) command size (SBC-2)                   */
#define SCSI_CMD_WRITE_10_LENGTH                                               (10U)  /* VERIFY (10) command size (SBC-2)                  */

/* defination for report data */
#define SCSI_REQUEST_SENSE_RESPONSE_CODE                                       (0x70) /* REQUEST SENSE COMMAND RESPONSE CODE               */
#define SCSI_INQUIRY_MIN_ALLOCATION_LENGTH                                     (5U)   /* min of all location length to INQUIRY             */

/* Sense Key definication for sense data */
#define SCSI_SENSE_KEY_NO_SENSE                                                (0x00) /* NO SENSE        */
#define SCSI_SENSE_KEY_NOT_READY                                               (0x02) /* NOT READY       */
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                                         (0x05) /* ILLEGAL REQUEST */

/* Sense Data Additional Sense Code definication */
#define SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION                               (0x00) /* NO_ADDITIONAL SENSE INFORMATION                      */
#define SCSI_ASC_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION                    (0x05) /* LOGICAL UNIT DOES NOT RESPOND TO SELECTION           */
#define SCSI_ASC_INVALID_COMMAND_OP_CODE                                       (0x20) /* INVALID COMMAND OP CODE                              */
#define SCSI_ASC_LBA_OUT_OF_RANGE                                              (0x21) /* LBA OUT OF RANGE                                     */
#define SCSI_ASC_INVALID_FIELD_IN_CDB                                          (0x24) /* INVALID FIELD IN CDB                                 */
#define SCSI_ASC_LOGICAL_UNIT_NOT_SUPPORTED                                    (0x25) /* LOGICAL UNIT NOT SUPPORTED                           */
#define SCSI_ASC_MEDIUM_NOT_PRESENT                                            (0x39) /* MEDIUM NOT PRESENT                                   */
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY_CAUSE_NOT_REPORTABLE                   (0x40) /* LOGICAL UNIT NOT READY CAUSE NOT REPORTABLE          */
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY_INITIALIZING_COMMAND_REQUIRED          (0x40) /* LOGICAL UNIT NOT READY INITIALIZING COMMAND REQUIRED */
#define SCSI_ASC_LOGICAL_UNIT_IS_IN_PROCESS_OF_BECOMING_READY                  (0x40) /* LOGICAL UNIT IS IN PROCESS_OF BECOMING READY         */
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED           (0x40) /* LOGICAL UNIT NOT READY MANUAL INTERVENTION REQUIRED  */
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY_FORMAT_IN_PROGRESS                     (0x40) /* LOGICAL UNIT NOT READY FORMAT IN PROGRESS            */

/* Sense Data Additional Sense Code Qualifier definication */
#define SCSI_ASCQ_NO_ADDITIONAL_SENSE_INFORMATION                              (0x00) /* NO ADDITIONAL SENSE INFORMATION                      */
#define SCSI_ASCQ_INVALID_COMMAND_OP_CODE                                      (0x00) /* INVALID COMMAND OP CODE                              */
#define SCSI_ASCQ_LBA_OUT_OF_RANGE                                             (0x00) /* LBA OUT OF RANGE                                     */
#define SCSI_ASCQ_INVALID_FIELD_IN_CDB                                         (0x00) /* INVALID FIELD IN CDB                                 */
#define SCSI_ASCQ_LOGICAL_UNIT_NOT_SUPPORTED                                   (0x00) /* LOGICAL UNIT NOT SUPPORTED                           */
#define SCSI_ASCQ_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION                   (0x00) /* LOGICAL UNIT DOES NOT RESPOND TO SELECTION           */
#define SCSI_ASCQ_MEDIUM_NOT_PRESENT                                           (0x00) /* MEDIUM NOT PRESENT                                   */
#define SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_CAUSE_NOT_REPORTABLE                  (0x00) /* LOGICAL UNIT NOT READY CAUSE NOT REPORTABLE          */
#define SCSI_ASCQ_LOGICAL_UNIT_IS_IN_PROCESS_OF_BECOMING_READY                 (0x01) /* LOGICAL UNIT IS IN PROCESS OF BECOMING READY         */
#define SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_INITIALIZING_COMMAND_REQUIRED         (0x02) /* LOGICAL UNIT NOT READY INITIALIZING COMMAND REQUIRED */
#define SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED          (0x03) /* LOGICAL UNIT NOT READY MANUAL INTERVENTION REQUIRED  */
#define SCSI_ASCQ_LOGICAL_UNIT_NOT_READY_FORMAT_IN_PROGRESS                    (0x04) /* LOGICAL UNIT NOT READY FORMAT IN PROGRESS            */

/* response data constant definication for INQUIRY command */
#define SCSI_QUALIFIER                                                         (0x00) /* QUALIFIER                             */
#define SCSI_DEVICE_TYPE_SBC2                                                  (0x00) /* DEVICE TYPE SBC2                      */
#define SCSI_REMOVABLE_MEDIA                                                   (0x01) /* REMOVABLE MEDIA                       */
#define SCSI_VERSION_SPC3                                                      (0x05) /* VERSION SPC3                          */
#define SCSI_INQUIRY_DATA_ADDITIONALLENGTH_OFFSET                              (0x04) /* INQUIRY DATA ADDITIONAL LENGTH OFFSET */

/* SCSI command process status */
#define SCSI_UNIT_STATUS_GOOD                                                  (0x00) /* GOOD                                                 */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_SUPPORTED                            (0x01) /* LOGICAL UNIT NOT SUPPORTED                           */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION            (0x02) /* LOGICAL UNIT DOES NOT RESPOND TO SELECTION           */
#define SCSI_UNIT_STATUS_MEDIUM_NOT_PRESENT                                    (0x03) /* MEDIUM NOT PRESENT                                   */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_CAUSE_NOT_REPORTABLE           (0x04) /* LOGICAL UNIT NOT READY CAUSE NOT REPORTABLE          */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_IS_IN_PROCESS_OF_BECOMING_READY          (0x05) /* LOGICAL UNIT IS IN PROCESS OF BECOMING READY         */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_INITIALIZING_COMMAND_REQUIRED  (0x06) /* LOGICAL UNIT NOT READY INITIALIZING COMMAND REQUIRED */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED   (0x07) /* LOGICAL UNIT NOT READY MANUAL INTERVENTION REQUIRED  */
#define SCSI_UNIT_STATUS_LOGICAL_UNIT_NOT_READY_FORMAT_IN_PROGRESS             (0x08) /* LOGICAL UNIT NOT READY FORMAT IN PROGRESS            */

#define SCSI_CDB_MAX_LENGTH (16)

/*----------------------------------------------------------------------------*/
/* Global Variable Definition                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Type Definition                                                            */
/*----------------------------------------------------------------------------*/

/* INQUIRY command */
/* Reference: SPC3.pdf-Chapter6.4.1-Table80 */
typedef struct _SCSI_ST_INQUIRY_CMD{

    uint8_t  OperationCode;       /* 0x12 : INQUIRY              */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  EVPD     :1,         /* Type of requested data      */
             Reserved1:7;         /* Reserved bits               */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Reserved1:7,         /* Reserved bits               */
             EVPD     :1;         /* Type of requested data      */
#endif
    uint8_t  PageCode;            /* Specifies the VPD to return */
    uint8_t  AllocationLength[2]; /* Size of host buffer         */
    uint8_t  Control;             /* 0x00                        */

}SCSI_ST_INQUIRY_CMD;

/* response data for INQUIRY command */
/* Reference: SPC3.pdf-Chapter6.4.2-Table81 */
typedef struct _SCSI_ST_INQUIRY_DATA{

#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  PeripheralDeviceType:5,  /* Peripheral device type                        */
             PeripheralQualifier :3;  /* Peripheral qualifier                          */
    uint8_t  Reserved1:7,             /* Reserved bits                                 */
             RMB      :1;             /* Is media removable ?                          */
    uint8_t  Version;                 /* SPC version used                              */
    uint8_t  ResponseDataFormat:4,    /* Must be 0x2                                   */
             HIGHSUP           :1,    /* Hierarchical addressing used ?                */
             NORMACA           :1,    /* ACA attribute supported ?                     */
             Obsolete1         :2;    /* Obsolete bits                                 */
    uint8_t  AdditionalLength;        /* Length of remaining INQUIRY data              */
    uint8_t  Protect  :1,             /* Protection info supported ?                   */
             Reserved2:2,             /* Reserved bits                                 */
             _3PC     :1,             /* Third-party copy supported ?                  */
             TPGS     :2,             /* Target port support group                     */
             ACC      :1,             /* Access control coordinator ?                  */
             SCCS     :1;             /* Embedded SCC ?                                */
    uint8_t  Addr16   :1,             /* 16 bit address                                */
             Obsolete2:2,             /* Obsolete bits                                 */
             Mchngr   :1,             /* medium changer bit                            */
             MultiP   :1,             /* Multi-port device ?                           */
             VS1      :1,             /* vendor specific                               */
             EncServ  :1,             /* Embedded enclosure service comp?              */
             Bque     :1;             /* Task management model supported ?             */
    uint8_t  VS2      :1,             /* vendor specific                               */
             CmdQue   :1,             /* Task management model supported ?             */
             Obsolete3:1,             /* Obsolete bit                                  */
             Linked   :1,             /* Linked commands  supported ?                  */
             Sync     :1,             /* synchronous transfer bit                      */
             Wbus16   :1,             /* wide bus 16 bit                               */
             Obsolete4:2;             /* Obsolete bits                                 */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  PeripheralQualifier :3,  /* Peripheral qualifier                          */
             PeripheralDeviceType:5;  /* Peripheral device type                        */
    uint8_t  RMB      :1,             /* Is media removable ?                          */
             Reserved1:7;             /* Reserved bits                                 */
    uint8_t  Version;                 /* SPC version used                              */
    uint8_t  Obsolete1         :2,    /* Obsolete bits                                 */
             NORMACA           :1,    /* ACA attribute supported ?                     */
             HIGHSUP           :1,    /* Hierarchical addressing used ?                */
             ResponseDataFormat:4;    /* Must be 0x2                                   */
    uint8_t  AdditionalLength;        /* Length of remaining INQUIRY data              */
    uint8_t  SCCS     :1,             /* Embedded SCC ?                                */
             ACC      :1,             /* Access control coordinator ?                  */
             TPGS     :2,             /* Target port support group                     */
             _3PC     :1,             /* Third-party copy supported ?                  */
             Reserved2:2,             /* Reserved bits                                 */
             Protect  :1;             /* Protection info supported ?                   */
    uint8_t  Bque     :1,             /* Task management model supported ?             */
             EncServ  :1,             /* Embedded enclosure service comp ?             */
             VS1      :1,             /* vendor specific                               */
             MultiP   :1,             /* Multi-port device ?                           */
             Mchngr   :1,             /* medium changer bit                            */
             Obsolete2:2,             /* Obsolete bits                                 */
             Addr16   :1;             /* 16 bit address                                */
    uint8_t  Obsolete4:2,             /* Obsolete bits                                 */
             Wbus16   :1,             /* wide bus 16 bit                               */
             Sync     :1,             /* synchronous transfer bit                      */
             Linked   :1,             /* Linked commands supported ?                   */
             Obsolete3:1,             /* Obsolete bit                                  */
             CmdQue   :1,             /* Task management model supported ?             */
             VS2      :1;             /* vendor specific                               */
#endif

    uint8_t  VendorID[8];             /* T10 vendor identification                     */
    uint8_t  ProductID[16];           /* Vendor-defined product ID                     */
    uint8_t  ProductRevisionLevel[4]; /* Vendor-defined product revision               */

}SCSI_ST_INQUIRY_DATA;

/* READ(10)command */
/* Reference: SBC2.pdf-Chapter5.6-Table28 */
typedef struct _SCSI_ST_READ_10_CMD{

    uint8_t  OperationCode;          /* 0x28 : READ_10                 */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  Obsolete1:1,            /* Obsolete bit                   */
             FUA_NV   :1,            /* Cache control bit              */
             Reserved1:1,            /* Reserved bit                   */
             FUA      :1,            /* Cache control bit              */
             DPO      :1,            /* Cache control bit              */
             RdProtect:3;            /* Protection information to send */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  RdProtect:3,            /* Protection information to send */
             DPO      :1,            /* Cache control bit              */
             FUA      :1,            /* Cache control bit              */
             Reserved1:1,            /* Reserved bit                   */
             FUA_NV   :1,            /* Cache control bit              */
             Obsolete1:1;            /* Obsolete bit                   */
#endif
    uint8_t  LogicalBlockAddress[4]; /* Index of first block to read   */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  GroupNumber:5,          /* Information grouping           */
             Reserved2  :3;          /* Reserved bits                  */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Reserved2  :3,          /* Reserved bits                  */
             GroupNumber:5;          /* Information grouping           */
#endif
    uint8_t  TransferLength[2];      /* Number of blocks to transmit   */
    uint8_t  Control;                /* 0x00                           */

}SCSI_ST_READ_10_CMD;

/* READ CAPACITY command */
/* Reference: SBC2.pdf-Chapter5.10.1-Table34 */
typedef struct _SCSI_ST_READ_CAPACITY_10_CMD{

    uint8_t  OperationCode;          /* 0x25 : READ_CAPACITY            */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  Obsolete1:1,            /* Obsolete bit                    */
             Reserved1:7;            /* Reserved bits                   */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Reserved1:7,            /* Reserved bits                   */
             Obsolete1:1;            /* Obsolete bit                    */
#endif
    uint8_t  LogicalBlockAddress[4]; /* Block to evaluate if PMI is set */
    uint8_t  Reserved2[2];           /* Reserved bytes                  */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  PMI      :1,            /* Partial medium indicator bit    */
             Reserved3:7;            /* Reserved bits                   */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Reserved3:7,            /* Reserved bits                   */
             PMI      :1;            /* Partial medium indicator bit    */
#endif
    uint8_t  Control;                /* 0x00                            */

}SCSI_ST_READ_CAPACITY_10_CMD;

typedef struct _SCSI_ST_READ_CAPACITY_10_DATA{

    uint32_t  LogicalBlockAddress; /* Address of last logical block   */
    uint32_t  LogicalBlockLength;  /* Length of last logical block    */

}SCSI_ST_READ_CAPACITY_10_DATA;


/* REQUEST SENSE command */
/* Reference: SPC3.pdf-Chapter6.27-Table170 */
typedef struct _SCSI_ST_REQUEST_SENSE_CMD{

    uint8_t  OperationCode;          /* 0x03 : REQUEST_SENSE         */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  Desc     :1,            /* Type of information expected */
             Reserved1:7;            /* Reserved bits                */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Reserved1:7,            /* Reserved bits                */
             Desc     :1;            /* Type of information expected */
#endif
    uint8_t  Reserved2[2];           /* Reserved bytes               */
    uint8_t  AllocationLength;       /* Size of host buffer          */
    uint8_t  Control;                /* 0x00                         */

}SCSI_ST_REQUEST_SENSE_CMD;

/* response data for REQUEST SENSE command */
/* Reference: SPC3.pdf-chater4.5.3-Table26 */
typedef struct _SCSI_ST_REQUEST_SENSE_DATA{

#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  ResponseCode:7,                /* Sense data format                    */
             Valid       :1;                /* Information field is standard        */
    uint8_t  Obsolete1;                     /* Obsolete byte                        */
    uint8_t  SenseKey :4,                   /* Generic error information            */
             Reserved1:1,                   /* Reserved bit                         */
             ILI      :1,                   /* incorrect length indicator bit       */
             EOM      :1,                   /* end-of-medium bit                    */
             Filemark :1;                   /* FILEMARK bit                         */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Valid       :1,                /* Information field is standard        */
             ResponseCode:7;                /* Sense data format                    */
    uint8_t  Obsolete1;                     /* Obsolete byte                        */
    uint8_t  Filemark :1,                   /* FILEMARK bit                         */
             EOM      :1,                   /* end-of-medium bit                    */
             ILI      :1,                   /* incorrect length indicator bit       */
             Reserved1:1,                   /* Reserved bit                         */
             SenseKey :4;                   /* Generic error information            */
#endif
    uint8_t  Information[4];                /* Command-specific                     */
    uint8_t  AdditionalSenseLength;         /* sizeof(SCSI_ST_REQUEST_SENSE_DATA)-7 */
    uint8_t  CommandSpecificInformation[4]; /* Command-specific                     */
    uint8_t  AdditionalSenseCode;           /* Additional error information         */
    uint8_t  AdditionalSenseCodeQualifier;  /* Further error information            */
    uint8_t  FieldReplaceableUnitCode;      /* Specific component code              */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  SenseKeySpecific1:7,           /* Additional exception info            */
             SKSV             :1;           /* Is sense key specific valid?         */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  SKSV             :1,           /* Is sense key specific valid?         */
             SenseKeySpecific1:7;           /* Additional exception info            */
#endif
    uint8_t  SenseKeySpecific2[2];          /* Additional exception info            */

}SCSI_ST_REQUEST_SENSE_DATA;

/* TEST_UNIT_READY command */
/* Reference: SPC3.pdf-Chapter6.33-Table184 */
typedef struct _SCSI_ST_TEST_UNIT_READY_CMD{

    uint8_t  OperationCode;        /* 0x00 : TEST_UNIT_READY     */
    uint8_t  Reserved1[4];         /* Reserved bits              */
    uint8_t  Control;              /* 0x00                       */

}SCSI_ST_TEST_UNIT_READY_CMD;

/* WRITE(10) command */
/* Reference: SBC2.pdf-Chapter5.25-Table62 */
typedef struct _SCSI_ST_WRITE_10_CMD{

    uint8_t  OperationCode;          /* 0x2A : WRITE_10                */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  Obsolete1:1,            /* Obsolete bit                   */
             FUA_NV   :1,            /* Cache control bit              */
             Reserved1:1,            /* Reserved bit                   */
             FUA      :1,            /* Cache control bit              */
             DPO      :1,            /* Cache control bit              */
             WrProtect:3;            /* Protection information to send */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  WrProtect:3,            /* Protection information to send */
             DPO      :1,            /* Cache control bit              */
             FUA      :1,            /* Cache control bit              */
             Reserved1:1,            /* Reserved bit                   */
             FUA_NV   :1,            /* Cache control bit              */
             Obsolete1:1;            /* Obsolete bit                   */
#endif
    uint8_t  LogicalBlockAddress[4]; /* First block to write           */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t  GroupNumber:5,          /* Information grouping           */
             Reserved2  :3;          /* Reserved bits                  */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t  Reserved2  :3,          /* Reserved bits                  */
             GroupNumber:5;          /* Information grouping           */
#endif
    uint8_t  TransferLength[2];      /* Number of blocks to write      */
    uint8_t  Control;                /* 0x00                           */

}SCSI_ST_WRITE_10_CMD;

/* PREVENT/ALLOW_MEDIUM_REMOVAL command */
/* Reference: SPC3.pdf-Chapter6.13-Table118 */
typedef struct _SCSI_ST_MEDIUM_REMOVAL_CMD{

    uint8_t OperationCode; /* 0x1E : PREVENT_ALLOW_MEDIUM_REMOVAL     */
    uint8_t Reserved1[3];  /* Reserved bytes                          */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t Prevent  :2,   /* Accept/prohibit removal                 */
            Reserved2:6;   /* Reserved bits                           */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t Reserved2:6,   /* Reserved bits                           */
            Prevent  :2;   /* Accept/prohibit removal                 */

#endif
    uint8_t Control;       /* 0x00                                    */

}SCSI_ST_MEDIUM_REMOVAL_CMD;

/* MODE SENSE(6) command */
/* Reference: SPC3.pdf-Chapter6.9.1-Table97 */
typedef struct _SCSI_ST_MODE_SENSE_6_CMD{

    uint8_t OperationCode;    /* 0x1A : MODE_SENSE_6                */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t Reserved1:3,      /* Reserved bits                      */
            DBD      :1,      /* Disable block descriptors bit      */
            Reserved2:4;      /* Reserved bits                      */
    uint8_t PageCode :6,      /* Mode page to return                */
            PC       :2;      /* Type of parameter values to return */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t Reserved2:4,      /* Reserved bits                      */
            DBD      :1,      /* Disable block descriptors bit      */
            Reserved1:3;      /* Reserved bits                      */
    uint8_t PC       :2,      /* Type of parameter values to return */
            PageCode :6;      /* Mode page to return                */
#endif
    uint8_t SubpageCode;      /* Mode subpage to return             */
    uint8_t AllocationLength; /* Host buffer allocated size         */
    uint8_t Control;          /* 0x00                               */

}SCSI_ST_MODE_SENSE_6_CMD;

/* response data for MODE SENSE(6)command */
/* Reference: SPC3.pdf-Chapter7.4.3-Table239 */
/* Reference: SPC3.pdf-Chapter6.9.1-Table97 */
typedef struct _SCSI_ST_MODE_SENSE_6_DATA{

    uint8_t ModeDataLength;          /* Length of mode data to follow   */
    uint8_t MediumType;              /* Type of medium                  */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t Reserved1:4,             /* Reserved bits                   */
            DPOFUA   :1,             /* DPO and FUA                     */
            Reserved2:2,             /* Reserved bits                   */
            WP       :1;             /* Write Protect                   */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t WP       :1,             /* Write Protect                   */
            Reserved2:2,             /* Reserved bits                   */
            DPOFUA   :1,             /* DPO and FUA                     */
            Reserved1:4;             /* Reserved bits                   */
#endif
    uint8_t BlockDescriptorLength;   /* Length of all block descriptors */

}SCSI_ST_MODE_SENSE_6_DATA;

/* VERIFY(10) command */
/* Reference: SBC2.pdf-Chapter5.20-Table53 */
typedef struct _SCSI_ST_VERIFY_10_CMD{

    uint8_t OperationCode;          /* 0x2F : VERIFY_10                   */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t Obsolete :1,            /* Obsolete bit                       */
            BYTCHK   :1,            /* Byte check bit                     */
            Reserved1:2,            /* Reserved bits                      */
            DPO      :1,            /* Disable page out bit               */
            Vrprotect:3;            /* Vrprotect bits                     */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t Vrprotect:3,            /* Vrprotect bits                     */
            DPO      :1,            /* Disable page out bit               */
            Reserved1:2,            /* Reserved bits                      */
            BYTCHK   :1,            /* Byte check bit                     */
            Obsolete :1;            /* Obsolete bit                       */
#endif
    uint8_t LogicalBlockAddress[4]; /* First block to write               */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t GroupNumber:5,          /* GroupNumber bits                   */
            Reserved2  :2,          /* Reserved bits                      */
            Restricted :1;          /* Restricted for MMC-4               */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t Restricted :1,          /* Restricted for MMC-4               */
            Reserved2  :2,          /* Reserved bits                      */
            GroupNumber:5;          /* GroupNumber bits                   */
#endif
    uint8_t VerificationLength[2];  /* Number of blocks to verify         */
    uint8_t Control;                /* 0x00                               */

}SCSI_ST_VERIFY_10_CMD;

/* START STOP UNIT command */
/* Reference: SBC2.pdf-Chapter5.17-Table48 */
typedef struct _SCSI_ST_START_STOP_UNIT_CMD{

    uint8_t OperationCode;          /* 0x1B : START STOP UNIT             */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t IMMED    :1,            /* Immediate bit                      */
            Reserved1:7;            /* Reserved bits                      */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t Reserved1:7,            /* Reserved bits                      */
            IMMED    :1;            /* Immediate bit                      */
#endif
    uint8_t Reserved2[2];           /* Reserved bits                      */
#if (CPU_BITORDER == CPU_LSBFIRST)
    uint8_t Start         :1,       /* Start bit                          */
            LOEJ          :1,       /* Reserved bits                      */
            Reserved3     :2,       /* Reserved bits                      */
            PowerCondition:4;       /* bPowerCondition                    */
#elif (CPU_BITORDER == CPU_MSBFIRST)
    uint8_t PowerCondition:4,       /* bPowerCondition                    */
            Reserved3     :2,       /* Reserved bits                      */
            LOEJ          :1,       /* Reserved bits                      */
            Start         :1;       /* Start bit                          */
#endif
    uint8_t Control;                /* 0x00                               */

}SCSI_ST_START_STOP_UNIT_CMD;

/* SCSI command in support*/
typedef union _SCSI_UN_COMMAND{

    uint8_t                      OperationCode;     /* Operation code of the command        */
    SCSI_ST_INQUIRY_CMD          Inquiry;           /* INQUIRY command                      */
    SCSI_ST_READ_10_CMD          Read10;            /* READ (10) command                    */
    SCSI_ST_READ_CAPACITY_10_CMD ReadCapacity10;    /* READ CAPACITY (10) command           */
    SCSI_ST_REQUEST_SENSE_CMD    RequestSense;      /* REQUEST SENSE command                */
    SCSI_ST_TEST_UNIT_READY_CMD  TestUnitReady;     /* TEST UNIT READY command              */
    SCSI_ST_WRITE_10_CMD         Write10;           /* WRITE (10) command                   */
    SCSI_ST_MEDIUM_REMOVAL_CMD   MediumRemoval;     /* PREVENT/ALLOW MEDIUM REMOVAL command */
    SCSI_ST_MODE_SENSE_6_CMD     ModeSense6;        /* MODE SENSE (6) command               */
    SCSI_ST_VERIFY_10_CMD        Verify10;          /* VERIFY (10) command                  */
    SCSI_ST_START_STOP_UNIT_CMD  StartStopUnit;     /* START STOP UNIT command              */

}SCSI_UN_COMMAND;

/* SCSI command */
typedef struct _SCSI_ST_CDB
{
    uint8_t   CDBLength;                /* CDB length */
    uint8_t   CDB[SCSI_CDB_MAX_LENGTH]; /* CDB */
}SCSI_ST_CDB;

/*----------------------------------------------------------------------------*/
/* Global Function Prototypes                                                 */
/*----------------------------------------------------------------------------*/
/* SCSI initialize process */
extern uint8_t msc_scsi_init(uint32_t *max_block_size_ptr);

/* check SCSI command */
extern uint8_t msc_scsi_cmd_check(uint8_t            lun,
                                  const SCSI_ST_CDB  *scsi_cdb,
                                  uint8_t            *data_type_ptr,
                                  uint32_t           *data_size_ptr);

/* SCSI command process */
extern uint8_t msc_scsi_cmd_process(const SCSI_ST_CDB *scsi_cdb,
                                    uint8_t           *data_buffer,
                                    uint32_t          buffer_size);

#endif   /* __USBF_MSC_SCSI_INC__ */
