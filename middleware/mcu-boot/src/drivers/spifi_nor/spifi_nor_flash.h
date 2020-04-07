/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __SPIFI_NOR_FLASH_H__
#define __SPIFI_NOR_FLASH_H__

#include "fsl_common.h"
#include "spifi/fsl_spifi.h"
#include "bootloader_common.h"

//! @brief Tag value used to validate the Spifi config block.
#define SPIFI_CFG_BLK_TAG FOUR_CHAR_CODE('k', 's', 't', 'r')
//! @brief Version value of Spifi config block.
#define SPIFI_CFG_BLK_VERSION 0x56010000UL  // V1.0.0


//! @brief Serial NOR command codes.
enum _serial_nor_command
{
    // Below commands are common for NOR
    kSerialNorCmd_Invalid = 0x00U,
    kSerialNorCmd_WriteStatus = 0x01U,             /*!< WRSR: Write Status Register*/
    kSerialNorCmd_WriteSecStatus_31 = 0x31U,       /*!< WRSR: Write second Status Register*/
    kSerialNorCmd_WriteSecStatus_3E = 0x3EU,       /*!< WRSR: Write second Status Register*/
    kSerialNorCmd_WriteMemory = 0x02U,             /*!< WRITE: Write Byte/Page Data to Memory Array with less than 4-byte address*/
    kSerialNorCmd_WriteMemoryA32 = 0x12U,          /*!< 4PP: Write Byte/Page Data to Memory Array with 4-byte address*/
    kSerialNorCmd_WriteEnable = 0x06U,             /*!< WREN: Set Write Enable Latch*/
    kSerialNorCmd_WriteDisable = 0x04U,            /*!< WRDI: Reset Write Enable Latch*/
    kSerialNorCmd_ReadStatus = 0x05U,              /*!< RDSR: Read Status Register*/
    kSerialNorCmd_ReadSecStatus_35 = 0x35U,        /*!< RDSR: Read second Status Register*/
    kSerialNorCmd_ReadSecStatus_3F = 0x3FU,        /*!< RDSR: Read second Status Register*/
    kSerialNorCmd_ReadMemory = 0x03U,              /*!< READ: Read Data from Memory Array with less than 4-byte address*/
    kSerialNorCmd_ReadMemoryA32 = 0x13U,           /*!< 4READ: Read Data from Memory Array with 4-byte address*/
    kSerialNorCmd_ReadMemorySDR_1_1_2 = 0x3BU,     /*!< READ: Read Data from Memory with 1(opcode)_1(address)_2(data) mode*/
    kSerialNorCmd_ReadMemorySDR_1_2_2 = 0xBBU,     /*!< READ: Read Data from Memory with 1(opcode)_2(address)_2(data) mode*/
    kSerialNorCmd_ReadMemorySDR_1_1_4 = 0x6BU,     /*!< READ: Read Data from Memory with 1(opcode)_1(address)_4(data) mode*/
    kSerialNorCmd_ReadMemorySDR_1_4_4 = 0xEBU,     /*!< READ: Read Data from Memory with 1(opcode)_4(address)_4(data) mode*/
    kSerialNorCmd_ReadMemorySDR_1_4_4_A32 = 0xECU, /*!< READ: Read Data from 4-bytes address Memory with 1(opcode)_4(address)_4(data) mode*/
    kSerialNorCmd_ReadMemorySDR_1_1_4_A32 = 0x6CU, /*!< READ: Read Data from 4-bytes address Memory with 1(opcode)_1(address)_4(data) mode*/
    kSerialNorCmd_EraseChipNor = 0x60U,            /*!< CE */
    kSerialNorCmd_EraseChip = 0xc7U,               /*!< CE */
    kSerialNorCmd_ErasePage = 0x42U,               /*!< PE */
    kSerialNorCmd_EraseSector4KB = 0x20U,          /*!< SE4KB */
    kSerialNorCmd_EraseSector32KB = 0x52U,         /*!< SE32KB */
    kSerialNorCmd_EraseSector = 0xd8U,             /*!< SE */
    kSerialNorCmd_EraseSector4KBA32 = 0x21U,       /*!< 4SE4KB */
    kSerialNorCmd_EraseSectorA32 = 0xdcU,          /*!< 4SE */
};

enum
{
    kStatusGroup_SPIFINOR = 220,
};

enum
{
    kSpifiNorCfgOption_Tag = 0x0c,
};

/* SPIFI NOR status */
enum _spifi_nor_status
{
    kStatus_SPIFINOR_ProgramFail = MAKE_STATUS(kStatusGroup_SPIFINOR, 0),
    kStatus_SPIFINOR_EraseSectorFail = MAKE_STATUS(kStatusGroup_SPIFINOR, 1),
    kStatus_SPIFINOR_EraseAllFail = MAKE_STATUS(kStatusGroup_SPIFINOR, 2),
    kStatus_SPIFINOR_WaitTimeout = MAKE_STATUS(kStatusGroup_SPIFINOR, 3),

    kStatus_SPIFINOR_NotSupported = MAKE_STATUS(kStatusGroup_SPIFINOR, 4),
    kStatus_SPIFINOR_WriteAlignmentError = MAKE_STATUS(kStatusGroup_SPIFINOR, 5),
    kStatus_SPIFINOR_CommandFailure = MAKE_STATUS(kStatusGroup_SPIFINOR, 6),
    kStatus_SPIFINOR_SFDP_NotFound = MAKE_STATUS(kStatusGroup_SPIFINOR, 7)
};

typedef enum _spifi_nor_erase_type
{
    kSpifiNorEraseType_Sector = 0x0U,
    kSpifiNorEraseType_Bulk
}spifi_nor_erase_type_t;

/*
 *  Spifi module configuration block
 */
typedef struct _spifi_module_config
{
    spifi_command_type_t commandType;            //!< Opcode and address type
    spifi_command_format_t readmemCommandFormt;  //!< Command formt for read memory opration
    uint8_t intLen;                              //!< Intermediate bytes precede the data
    bool isQuadDualNeedEnable;                   //!< Wether quad/dual enable is needed: 1 - Enable, 0 - Disable
    bool writeTwoStausBytes;                     //!< Wether need to write two status bytes to enable quad/dual mode
    uint8_t quadDualEnableCommand;               //!< Write status command which can enable quad/dual mode
    uint8_t quadDualEnableBitShift;              //!< The shift of quad/dual enable bit in the status register
    uint8_t quadDualreadCommand;                 //!< Read status command which can read quad/dual mode enable bit
    spifi_config_t memConfig;                    //!< Common memory configuration info via SPIFI
} spifi_module_config_t;

/*
 *  Serial NOR basic command set structure
 */
typedef struct __serial_nor_command_set
{
    uint8_t writeStatusCommand;
    uint8_t pageWriteMemoryCommand;
    uint8_t readMemoryCommand;
    uint8_t writeDisableCommand;
    uint8_t readStatusCommand;
    uint8_t writeEnableCommand;
    uint8_t eraseSectorCommand;
    uint8_t eraseChipCommand;
} serial_nor_command_set_t;

/*
 *  Serial NOR configuration block
 */
typedef struct _spifi_nor_config
{
    uint32_t tag;                          //!< Set to magic number of 'kstr'
    uint32_t version;                      //!< Version,[31:24] -'V', [23:16] - Major, [15:8] - Minor, [7:0] - bugfix
    uint32_t pageSize;                     //!< Page size in byte of Serial NOR
    uint32_t sectorSize;                   //!< Sector size in byte of Serial NOR
    uint32_t memorySize;                   //!< Memory size in byte of Serial NOR
    uint32_t SerialClkFreq;                //!< Clock frequency for IP command
    serial_nor_command_set_t commandSet;   //!< Serial NOR basic command set
    spifi_module_config_t spifiConfig;     //!< Configuration for spifi module
} spifi_nor_config_t;

/*
 *  Serial NOR quad mode enable setting requirements option
 */
enum
{
    kSerialNorQuadMode_NotConfig = 0,
    kSerialNorQuadMode_StatusReg1_Bit6 = 1,
    kSerialNorQuadMode_StatusReg2_Bit1 = 2,
    kSerialNorQuadMode_StatusReg2_Bit7 = 3,
    kSerialNorQuadMode_StatusReg2_Bit1_0x31 = 4,
};

/*
 * Serial NOR Configuration Option
 */
typedef struct _spifi_nor_config_option
{
    union
    {
        struct
        {
            uint32_t max_freq : 4;          //!< Maximum supported Frequency 
            uint32_t cmd_format : 4;        //!< Command formt for read memory opration
            uint32_t quad_mode_setting : 4; //!< Quad mode enable setting requirements,
                                            //!< which is necessary for flash device which support JESD216 initial version only
            uint32_t reserved : 12;         //!< Reserved for future use
            uint32_t option_size : 4;       //!< Option size, in terms of uint32_t, size = (option_size + 1) * 4
            uint32_t tag : 4;               //!< Tag, must be 0x0c
        } B;
        uint32_t U;
    } option0;

    union
    {
        struct
        {
            uint32_t reserved1;       //!< Reserved for future use
        } B;
        uint32_t U;
    } option1;

} spifi_nor_config_option_t;

#ifdef __cplusplus
extern "C" {
#endif

//!@brief Initialize Norflash device via SPIFI.
extern status_t spifi_norflash_init(SPIFI_Type *base, spifi_nor_config_t *config);

//!@brief Get configure data via configure option.
extern status_t spifi_nor_get_config(SPIFI_Type *base, spifi_nor_config_t *config, spifi_nor_config_option_t *option);

//!@brief Program a page data to Norflash via SPIFI.
extern status_t spifi_norflash_program_page(SPIFI_Type *base, spifi_nor_config_t *config, uint32_t address, const uint8_t *buf);

//!@brief Erase norflash via SPIFI.
extern status_t spifi_norflash_erase(SPIFI_Type *base, spifi_nor_config_t *config, uint32_t address, spifi_nor_erase_type_t type);

//!@brief Read norflash content via SPIFI.
extern status_t spifi_norflash_read(SPIFI_Type *base, spifi_nor_config_t *config,
                                    uint32_t address, uint8_t *buf, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* __SPIFI_NOR_FLASH_H__ */
