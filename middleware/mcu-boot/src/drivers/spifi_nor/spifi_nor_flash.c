/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>
#include "spifi/fsl_spifi.h"
#include "spifi_nor/spifi_nor_flash.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define MAX_24BIT_ADDRESSING_SIZE (16UL * 1024 * 1024)

//!@brief Defintions for SPIFI Serial Clock Frequency
typedef enum _spifi_serial_clk_freq
{
    kSpifiSerialClk_24MHz = 1,
    kSpifiSerialClk_48MHz = 2,
    kSpifiSerialClk_60MHz = 3,
    kSpifiSerialClk_80MHz = 4,
    kSpifiSerialClk_96MHz = 5
} spifi_serial_clk_freq_t;

//! @brief SPIFI Serial Clock Frequency Array
uint32_t spifi_nor_clk_freqOption[kSpifiSerialClk_96MHz + 1] = {
    0, 24000000, 48000000, 60000000, 80000000, 96000000
};

enum
{
    kSerialFlash_ReadSFDP = 0x5A,
    kSerialFlash_ReadManufacturerId = 0x9F,
};

#define SFDP_SIGNATURE 0x50444653 /* ASCII: SFDP */

enum
{
    kSfdp_Version_Major_1_0 = 1,
    kSfdp_Version_Minor_0 = 0, // JESD216
    kSfdp_Version_Minor_A = 5, // JESD216A
    kSfdp_Version_Minor_B = 6, // JESD216B
    kSfdp_Version_Minor_C = 7, // JESD216C

    kSfdp_BasicProtocolTableSize_Rev0 = 36,
    kSfdp_BasicProtocolTableSize_RevA = 64,
    kSfdp_BasicProtocolTableSize_RevB = kSfdp_BasicProtocolTableSize_RevA,
    kSfdp_BasicProtocolTableSize_RevC = 80,
};

typedef struct _sfdp_header
{
    uint32_t signature;
    uint8_t minor_rev;
    uint8_t major_rev;
    uint8_t param_hdr_num;
    uint8_t sfdp_access_protocol; // Defined in JESD216C, reserved for older version
} sfdp_header_t;

enum
{
    kParameterID_BasicSpiProtocol = 0xFF00,
    // New Table added in JESD216B
    kParameterID_SectorMap = 0xFF81,
    kParameterID_4ByteAddressInstructionTable = 0xFF84,
    // New Table added in JESD216C
    kParameterID_xSpiProfile1_0 = 0xFF85,
    kParameterID_xSpiOrofile2_0 = 0xFF86,
    kParameterID_StaCtrlCfgRegMap = 0xFF87,
    kParameterID_OpiEnableSeq = 0xFF09,
};

//!@brief SFDP Parameter Header, see JESD216B doc for more details
typedef struct _sfdp_parameter_header
{
    uint8_t parameter_id_lsb;
    uint8_t minor_rev;
    uint8_t major_rev;
    uint8_t table_length_in_32bit;
    uint8_t parameter_table_pointer[3];
    uint8_t parameter_id_msb;
} sfdp_parameter_header_t;

//!@brief Basic Flash Parameter Table, see JESD216B doc for more details
typedef struct _jedec_flash_param_table
{
    struct
    {
        uint32_t erase_size : 2;
        uint32_t write_granularity : 1;
        uint32_t reserved0 : 2;
        uint32_t unused0 : 3;
        uint32_t erase4k_inst : 8;
        uint32_t support_1_1_2_fast_read : 1;
        uint32_t address_bits : 2;
        uint32_t support_ddr_clocking : 1;
        uint32_t support_1_2_2_fast_read : 1;
        uint32_t supports_1_4_4_fast_read : 1;
        uint32_t support_1_1_4_fast_read : 1;
        uint32_t unused1 : 9;
    } misc;
    uint32_t flash_density;
    struct
    {
        uint32_t dummy_clocks_1_4_4_read : 5;
        uint32_t mode_clocks_1_4_4_read : 3;
        uint32_t inst_1_4_4_read : 8;
        uint32_t dummy_clocks_1_1_4_read : 5;
        uint32_t mode_clocks_1_1_4_read : 3;
        uint32_t inst_1_1_4_read : 8;
    } read_1_4_info;
    struct
    {
        uint32_t dummy_clocks_1_2_2_read : 5;
        uint32_t mode_clocks_1_2_2_read : 3;
        uint32_t inst_1_2_2_read : 8;
        uint32_t dummy_clocks_1_1_2_read : 5;
        uint32_t mode_clocks_1_1_2_read : 3;
        uint32_t inst_1_1_2_read : 8;
    } read_1_2_info;

    struct
    {
        uint32_t support_2_2_2_fast_read : 1;
        uint32_t reserved0 : 3;
        uint32_t support_4_4_4_fast_read : 1;
        uint32_t reserved1 : 27;
    } read_22_44_check;

    struct
    {
        uint32_t reserved0 : 16;
        uint32_t dummy_clocks_2_2_2_read : 5;
        uint32_t mode_clocks_2_2_2_read : 3;
        uint32_t inst_2_2_2_read : 8;
    } read_2_2_info;
    struct
    {
        uint32_t reserved0 : 16;
        uint32_t dummy_clocks_4_4_4_read : 5;
        uint32_t mode_clocks_4_4_4_read : 3;
        uint32_t inst_4_4_4_read : 8;
    } read_4_4_info;

    struct
    {
        uint8_t size;
        uint8_t inst;
    } erase_info[4];

    uint32_t erase_timing;
    struct
    {
        uint32_t reserved0 : 4;
        uint32_t page_size : 4;
        uint32_t reserved1 : 24;
    } chip_erase_progrm_info;

    struct
    {
        uint32_t suspend_resume_spec;
        uint32_t suspend_resume_inst;
    } suspend_resume_info;

    struct
    {
        uint32_t reserved0 : 2;
        uint32_t busy_status_polling : 6;
        uint32_t reserved1 : 24;
    } busy_status_info;

    struct
    {
        uint32_t mode_4_4_4_disable_seq : 4;
        uint32_t mode_4_4_4_enable_seq : 5;
        uint32_t support_mode_0_4_4 : 1;
        uint32_t mode_0_4_4_exit_method : 6;
        uint32_t mode_0_4_4_entry_method : 4;
        uint32_t quad_enable_requirement : 3;
        uint32_t hold_reset_disable : 1;
        uint32_t reserved0 : 8;
    } mode_4_4_info;

    struct
    {
        uint32_t status_reg_write_enable : 7;
        uint32_t reserved0 : 1;
        uint32_t soft_reset_rescue_support : 6;
        uint32_t exit_4_byte_addressing : 10;
        uint32_t enter_4_byte_addrssing : 8;
    } mode_config_info;
} jedec_flash_param_table_t;

//!@brief 4Byte Addressing Instruction Table, see JESD216B doc for more details
typedef struct _jedec_4byte_addressing_inst_table
{
    struct
    {
        uint32_t support_1_1_1_read : 1;
        uint32_t support_1_1_1_fast_read : 1;
        uint32_t support_1_1_2_fast_read : 1;
        uint32_t support_1_2_2_fast_read : 1;
        uint32_t support_1_1_4_fast_read : 1;
        uint32_t support_1_4_4_fast_read : 1;
        uint32_t support_1_1_1_page_program : 1;
        uint32_t support_1_1_4_page_program : 1;
        uint32_t support_1_4_4_page_program : 1;
        uint32_t support_erase_type1_size : 1;
        uint32_t support_erase_type2_size : 1;
        uint32_t support_erase_type3_size : 1;
        uint32_t support_erase_type4_size : 1;
        uint32_t support_1_1_1_dtr_read : 1;
        uint32_t support_1_2_2_dtr_read : 1;
        uint32_t support_1_4_4_dtr_read : 1;
        uint32_t support_volatile_sector_lock_read_cmd : 1;
        uint32_t support_volatile_sector_lock_write_cmd : 1;
        uint32_t support_nonvolatile_sector_lock_read_cmd : 1;
        uint32_t support_nonvolatile_sector_lock_write_cmd : 1;
        uint32_t reserved : 12;
    } cmd_4byte_support_info;

    struct
    {
        uint8_t erase_inst[4];
    } erase_inst_info;
} jedec_4byte_addressing_inst_table_t;

typedef struct _jdec_query_table
{
    uint32_t standard_version; // JESD216 version
    uint32_t flash_param_tbl_size;
    jedec_flash_param_table_t flash_param_tbl;
    bool has_4b_addressing_inst_table;
    jedec_4byte_addressing_inst_table_t flash_4b_inst_tbl;
} jedec_info_table_t;

extern void spifi_clock_gate(void);
extern void spifi_iomux_config(void);
extern uint32_t spifi_source_clock(void);

////////////////////////////////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////////////////////////////////
//!@brief Send Write Enable command to Serial NOR via SPIFI
static status_t spifi_nor_write_enable(SPIFI_Type *base, spifi_nor_config_t *config,
                                       spifi_command_format_t cmdFormat);

//!@brief Send read status command to Serial NOR via SPIFI
static uint8_t spifi_nor_read_status(SPIFI_Type *base, uint8_t readCmd);

//!@brief Enable nor flash quad mode via SPIFI
static status_t spifi_nor_quad_mode_enable(SPIFI_Type *base, spifi_nor_config_t *config);

//!@brief Parse SFDP parameters and then fill into SPIFI Serial NOR Configuration Block
status_t parse_sfdp(SPIFI_Type *base, spifi_nor_config_t *config,
                    jedec_info_table_t *tbl, spifi_nor_config_option_t *option);

//!@brief Read SFDP information via SPIFI
static status_t spifi_nor_read_sfdp_info(SPIFI_Type *base, jedec_info_table_t *tbl);

//!@brief Get default command set for SPIFI NOR
static void spifi_nor_get_default_cmdSet(serial_nor_command_set_t *cmdSet);

//!@brief Read SFDP info specified by arguments
static status_t spifi_nor_read_sfdp(SPIFI_Type *base, uint32_t address, uint32_t *buffer, uint32_t bytes);

//!@brief Get page/sector size from SFDP
status_t get_page_sector_size_from_sfdp(spifi_nor_config_t *config, jedec_info_table_t *tbl,
                                        uint8_t *sector_erase_cmd);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static uint8_t spifi_nor_read_status(SPIFI_Type *base, uint8_t readCmd)
{
    spifi_command_t cmd;
    uint8_t value;

    cmd.dataLen = 0x01U;
    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataInput;
    cmd.intermediateBytes = 0;
    cmd.format = kSPIFI_CommandAllSerial;
    cmd.type = kSPIFI_CommandOpcodeOnly;
    cmd.opcode = readCmd;
    SPIFI_SetCommand(base, &cmd);
    value = *(uint8_t *)&base->DATA;

    return value;
}

static status_t check_norflash_finish(SPIFI_Type *base, spifi_nor_config_t *config)
{
    spifi_command_t cmd;
    uint8_t value;

    /* Using hardware POLL mode to get matched status value.
     * And set DATALEN as 0 means choosing bit0 of Norflash status register to compare with bit3,
     * if both of them are 0, that is, flash is in IDLE state, will trigger INIRQ as 1 in
     * STATUS register.
     */
    cmd.dataLen = 0x00;
    cmd.isPollMode = true;
    cmd.direction = kSPIFI_DataInput;
    cmd.intermediateBytes = 0;
    cmd.format = kSPIFI_CommandAllSerial;
    cmd.type = kSPIFI_CommandOpcodeOnly;
    cmd.opcode = config->commandSet.readStatusCommand;

    /* Clear INTRQ bit first, perhaps set by the previous operation. */
    base->STAT = SPIFI_STAT_INTRQ_MASK;
    SPIFI_SetCommand(base, &cmd);
    while ((base->STAT & SPIFI_STAT_INTRQ_MASK) == 0U)
    {
    }
    value = *(uint8_t *)&base->DATA;

    return (value & 0x01U) ? kStatus_Fail : kStatus_Success;
}

static status_t spifi_nor_write_enable(SPIFI_Type *base, spifi_nor_config_t *config, spifi_command_format_t cmdFormat)
{
    spifi_command_t cmd;
    status_t status;
    uint8_t value;

    /* Enable write operation. */
    cmd.dataLen = 0;
    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataOutput;
    cmd.intermediateBytes = 0;
    cmd.format = cmdFormat;
    cmd.type = kSPIFI_CommandOpcodeOnly;
    cmd.opcode = config->commandSet.writeEnableCommand;
    SPIFI_SetCommand(base, &cmd);

    status = check_norflash_finish(base ,config);
    /* check whether enable write operation success. */
    value = spifi_nor_read_status(base, config->commandSet.readStatusCommand);
    if (0 == (value & 0x2U))
    {
        return kStatus_Fail;
    }
    
    return status;
}

static status_t spifi_nor_quad_mode_enable(SPIFI_Type *base, spifi_nor_config_t *config)
{
    spifi_command_t cmd;
    status_t status;
    uint8_t quadValue;
    uint8_t valueAfter;

    if (kSerialNorCmd_Invalid == config->spifiConfig.quadDualreadCommand)
    {
        return kStatus_InvalidArgument;
    }

    /* Read status register which contain quad/dual enable bit. */
    quadValue = spifi_nor_read_status(base, config->spifiConfig.quadDualreadCommand);
    /* Check whether the mode need to switich. 
       Only consider the bit value in the quad spi mode is 1 here.*/
    if (quadValue & (0x1U << config->spifiConfig.quadDualEnableBitShift))
    {
        return kStatus_Success;
    }

    do
    {
        /* Enable write operation. */
        status = spifi_nor_write_enable(base, config, kSPIFI_CommandAllSerial);
        if (kStatus_Success != status)
        {
            return status;
        }

        if (true == config->spifiConfig.writeTwoStausBytes)
        {
            uint8_t valueStatus;
            uint16_t valueSwitch;
            /* Read status register1 to combine the value of two status registers, then write it back.
               This opration can maintain the state of all other writable status register bits*/
            valueStatus = spifi_nor_read_status(base, config->commandSet.readStatusCommand);
            valueSwitch = valueStatus | ((quadValue | (0x1U << config->spifiConfig.quadDualEnableBitShift)) << 8);

            /* Enable quad/dual spi mode. */
            cmd.dataLen = 0x02U;
            cmd.isPollMode = false;
            cmd.direction = kSPIFI_DataOutput;
            cmd.intermediateBytes = 0;
            cmd.format = kSPIFI_CommandAllSerial;
            cmd.type = kSPIFI_CommandOpcodeOnly;
            cmd.opcode = config->commandSet.writeStatusCommand;
            SPIFI_SetCommand(base, &cmd);
            *(uint16_t *)&base->DATA = valueSwitch;
        }
        else
        {
            /* Enable quad/dual spi mode. */
            cmd.dataLen = 0x01U;
            cmd.isPollMode = false;
            cmd.direction = kSPIFI_DataOutput;
            cmd.intermediateBytes = 0;
            cmd.format = kSPIFI_CommandAllSerial;
            cmd.type = kSPIFI_CommandOpcodeOnly;
            cmd.opcode = config->spifiConfig.quadDualEnableCommand;
            SPIFI_SetCommand(base, &cmd);
            *(uint8_t *)&base->DATA = quadValue | (0x1U << config->spifiConfig.quadDualEnableBitShift);
        }

        check_norflash_finish(base ,config);
        /* Read status register which contain quad/dual enable bit to check whether swtich opration success. */
        valueAfter = spifi_nor_read_status(base, config->spifiConfig.quadDualreadCommand);
    }while ((valueAfter & (0x1U << config->spifiConfig.quadDualEnableBitShift)) == 0);

    return kStatus_Success;
}

status_t spifi_norflash_init(SPIFI_Type *base, spifi_nor_config_t *config)
{
    uint32_t sourceClockFreq;
    spifi_command_t cmd;
    status_t status;

    /* Enable SPIFI clock. */
    spifi_clock_gate();
    sourceClockFreq = spifi_source_clock();
    /* Set the clock divider. */
    if (0 != config->SerialClkFreq)
    {
        CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, sourceClockFreq / config->SerialClkFreq, false);
    }
    else
    {
        return kStatus_InvalidArgument;
    }

    /* Initialize SPIFI. */
    SPIFI_Init(base, &config->spifiConfig.memConfig);
    /* Configure SPIFI IOMUX. */
    spifi_iomux_config();
    /* Enable write operation. */
    status = spifi_nor_write_enable(base, config, kSPIFI_CommandAllSerial);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Disable write protection. */
    cmd.dataLen = 1U;
    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataOutput;
    cmd.intermediateBytes = 0;
    cmd.format = kSPIFI_CommandAllSerial;
    cmd.type = kSPIFI_CommandOpcodeOnly;
    cmd.opcode = config->commandSet.writeStatusCommand;
    SPIFI_SetCommand(base, &cmd);
    *(uint8_t *)&base->DATA = 0x00;

    status = check_norflash_finish(base, config);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Switch to quad mode if it is needed. */
    if (true == config->spifiConfig.isQuadDualNeedEnable)
    {
        status = spifi_nor_quad_mode_enable(base, config);
    }

    return status;
}

static void spifi_nor_get_default_cmdSet(serial_nor_command_set_t *cmdSet)
{
    cmdSet->readStatusCommand = kSerialNorCmd_ReadStatus;
    cmdSet->eraseSectorCommand = kSerialNorCmd_EraseSector;
    cmdSet->pageWriteMemoryCommand = kSerialNorCmd_WriteMemory;
    cmdSet->readMemoryCommand = kSerialNorCmd_ReadMemory;
    cmdSet->writeDisableCommand = kSerialNorCmd_WriteDisable;
    cmdSet->writeEnableCommand = kSerialNorCmd_WriteEnable;
    cmdSet->writeStatusCommand = kSerialNorCmd_WriteStatus;
    cmdSet->eraseChipCommand = kSerialNorCmd_EraseChip;
}

static status_t spifi_nor_read_sfdp(SPIFI_Type *base, uint32_t address,
                                  uint32_t *buffer, uint32_t bytes)
{
    spifi_command_t cmd;

    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataInput;
    cmd.intermediateBytes = 1;
    cmd.format = kSPIFI_CommandAllSerial;
    cmd.type = kSPIFI_CommandOpcodeAddrThreeBytes;
    cmd.opcode = kSerialFlash_ReadSFDP;
    SPIFI_SetMemoryCommand(base, &cmd);

    memcpy(buffer, (void *)(address + FSL_FEATURE_SPIFI_START_ADDR), bytes);
    /* Reset to command mode. */
    SPIFI_ResetCommand(base);

    return kStatus_Success;
}

status_t spifi_nor_read_sfdp_info(SPIFI_Type *base, jedec_info_table_t *tbl)
{
    status_t status = kStatus_SPIFINOR_SFDP_NotFound;
    do
    {
        if (tbl == NULL)
        {
            status = kStatus_InvalidArgument;
            break;
        }

        sfdp_header_t sfdp_header;
        uint32_t address;
        // Read SFDP header
        status = spifi_nor_read_sfdp(base, 0, (uint32_t *)&sfdp_header, sizeof(sfdp_header));
        if (status != kStatus_Success)
        {
            break;
        }

        if (sfdp_header.signature != SFDP_SIGNATURE)
        {
            status = kStatus_SPIFINOR_SFDP_NotFound;
            break;
        }

        // Save the standard version for later use.
        tbl->standard_version = sfdp_header.minor_rev;

        uint32_t parameter_header_number = sfdp_header.param_hdr_num + 1;

        sfdp_parameter_header_t sfdp_param_hdrs[10];
        uint32_t max_hdr_count = parameter_header_number > 10 ? 10 : parameter_header_number;
        address = 0x08;
        // Read parameter headers
        status = spifi_nor_read_sfdp(base, address, (uint32_t *)&sfdp_param_hdrs[0],
                                       max_hdr_count * sizeof(sfdp_parameter_header_t));
        if (status != kStatus_Success)
        {
            break;
        }
        memset(tbl, 0, sizeof(*tbl));

        for (uint32_t i = 0; i < max_hdr_count; i++)
        {
            uint32_t parameter_id =
                sfdp_param_hdrs[i].parameter_id_lsb + ((uint32_t)sfdp_param_hdrs[i].parameter_id_msb << 8);

            if ((parameter_id == kParameterID_BasicSpiProtocol) ||
                (parameter_id == kParameterID_4ByteAddressInstructionTable))
            {
                address = 0;
                for (int32_t index = 2; index >= 0; index--)
                {
                    address <<= 8;
                    address |= sfdp_param_hdrs[i].parameter_table_pointer[index];
                }
                uint32_t table_size = sfdp_param_hdrs[i].table_length_in_32bit * sizeof(uint32_t);

                if (parameter_id == kParameterID_BasicSpiProtocol)
                {
                    // Limit table size to the max supported standard
                    if (table_size > sizeof(jedec_flash_param_table_t))
                    {
                        table_size = sizeof(jedec_flash_param_table_t);
                    }
                    // Read Basic SPI Protocol Table
                    status = spifi_nor_read_sfdp(base, address, (uint32_t *)&tbl->flash_param_tbl, table_size);
                    if (status != kStatus_Success)
                    {
                        break;
                    }
                    tbl->flash_param_tbl_size = table_size;
                }
                else if (parameter_id == kParameterID_4ByteAddressInstructionTable)
                {
                    // Read 4-byte Address Instruction Table
                    status = spifi_nor_read_sfdp(base, address, (uint32_t *)&tbl->flash_4b_inst_tbl, table_size);
                    if (status != kStatus_Success)
                    {
                        break;
                    }
                    tbl->has_4b_addressing_inst_table = true;
                }
            }
            else
            {
                // Unsupported parameter type, ignore
            }
        }

    } while (0);

    return status;
}

status_t get_page_sector_size_from_sfdp(spifi_nor_config_t *config,
                                        jedec_info_table_t *tbl,
                                        uint8_t *sector_erase_cmd)
{
    jedec_flash_param_table_t *param_tbl = &tbl->flash_param_tbl;
    jedec_4byte_addressing_inst_table_t *flash_4b_tbl = &tbl->flash_4b_inst_tbl;

    // Calculate Flash Size
    uint32_t flash_size;
    uint32_t flash_density = tbl->flash_param_tbl.flash_density;

    if (flash_density & (1U << 0x1F))
    {
        // Flash size >= 4G bits
        flash_size = 1U << ((flash_density & ~(1U << 0x1F)) - 3);
    }
    else
    {
        // Flash size < 4G bits
        flash_size = (flash_density + 1) >> 3;
    }
    config->memorySize = flash_size;

    // Calculate Page size
    uint32_t page_size;
    if (tbl->flash_param_tbl_size < 64U)
    {
        config->pageSize = 256U;
    }
    else
    {
        page_size = 1 << (param_tbl->chip_erase_progrm_info.page_size);
        config->pageSize = page_size == (1 << 15) ? 256U : page_size;
    }

    // Calculate Sector Size;
    uint32_t sector_size = 0u;
    uint32_t sector_erase_type = 0;

    for (uint32_t index = 0; index < 4; index++)
    {
        if (param_tbl->erase_info[index].size != 0)
        {
            uint32_t current_erase_size = 1U << param_tbl->erase_info[index].size;
            if (0U == sector_size)
            {
                sector_size = current_erase_size;
                sector_erase_type = index;
            }
            if ((current_erase_size < sector_size) && (current_erase_size > 0U))
            {
                sector_size = current_erase_size;
                sector_erase_type = index;
            }
        }
    }

    config->sectorSize = sector_size;

    if (config->memorySize > MAX_24BIT_ADDRESSING_SIZE)
    {
        if (tbl->has_4b_addressing_inst_table)
        {
            *sector_erase_cmd = flash_4b_tbl->erase_inst_info.erase_inst[sector_erase_type];
        }
        else
        {
            switch (param_tbl->erase_info[sector_erase_type].inst)
            {
                case kSerialNorCmd_EraseSector4KB:
                    *sector_erase_cmd = kSerialNorCmd_EraseSector4KBA32;
                    break;
                case kSerialNorCmd_EraseSector:
                    *sector_erase_cmd = kSerialNorCmd_EraseSectorA32;
                    break;
            }
        }
    }
    else
    {
        *sector_erase_cmd = param_tbl->erase_info[sector_erase_type].inst;
    }

    config->commandSet.eraseSectorCommand = *sector_erase_cmd;

    return kStatus_Success;
}

// Parse SFDP parameters and then fill into SPIFI Serial NOR Configuration Block
status_t parse_sfdp(SPIFI_Type *base, spifi_nor_config_t *config,
                    jedec_info_table_t *tbl, spifi_nor_config_option_t *option)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        jedec_flash_param_table_t *param_tbl = &tbl->flash_param_tbl;
        jedec_4byte_addressing_inst_table_t *flash_4b_tbl = &tbl->flash_4b_inst_tbl;
        uint8_t quadModeSetting = kSerialNorQuadMode_NotConfig;

        config->SerialClkFreq = option->option0.B.max_freq;

        uint32_t dummy_cycles = 0;
        uint8_t mode_cycles = 0;
        uint8_t sector_erase_cmd = 0;
        uint32_t address_bits = 24U;

        get_page_sector_size_from_sfdp(config, tbl, &sector_erase_cmd);

        if (config->memorySize > MAX_24BIT_ADDRESSING_SIZE)
        {
            address_bits = 32U;
            config->spifiConfig.commandType = kSPIFI_CommandOpcodeAddrFourBytes;
            config->commandSet.pageWriteMemoryCommand = kSerialNorCmd_WriteMemoryA32;
            config->commandSet.readMemoryCommand = kSerialNorCmd_ReadMemoryA32;
        }
        else
        {
            config->spifiConfig.commandType = kSPIFI_CommandOpcodeAddrThreeBytes;
        }

        config->spifiConfig.readmemCommandFormt = (spifi_command_format_t)option->option0.B.cmd_format;
        
        if (kSPIFI_CommandAllSerial != config->spifiConfig.readmemCommandFormt)
        {
            if (kSPIFI_CommandOpcodeSerial == config->spifiConfig.readmemCommandFormt
                && param_tbl->misc.supports_1_4_4_fast_read)
            {
                mode_cycles = param_tbl->read_1_4_info.mode_clocks_1_4_4_read;
                dummy_cycles = param_tbl->read_1_4_info.dummy_clocks_1_4_4_read;
                // Calculate intermediate bytes precede the data
                config->spifiConfig.intLen = (mode_cycles + dummy_cycles)/2U;
                config->commandSet.readMemoryCommand = kSerialNorCmd_ReadMemorySDR_1_4_4;
                if ((address_bits == 32U) && flash_4b_tbl->cmd_4byte_support_info.support_1_4_4_fast_read)
                {
                    config->commandSet.readMemoryCommand = kSerialNorCmd_ReadMemorySDR_1_4_4_A32;
                }
            }
            else if (kSPIFI_CommandDataQuad == config->spifiConfig.readmemCommandFormt
                     && param_tbl->misc.support_1_1_4_fast_read)
            {
                mode_cycles = param_tbl->read_1_4_info.mode_clocks_1_1_4_read;
                dummy_cycles = param_tbl->read_1_4_info.dummy_clocks_1_1_4_read;
                // Calculate intermediate bytes precede the data
                config->spifiConfig.intLen = (mode_cycles + dummy_cycles)/8U;
                config->commandSet.readMemoryCommand = kSerialNorCmd_ReadMemorySDR_1_1_4;
                if ((address_bits == 32U) && flash_4b_tbl->cmd_4byte_support_info.support_1_1_4_fast_read)
                {
                    config->commandSet.readMemoryCommand = kSerialNorCmd_ReadMemorySDR_1_1_4_A32;
                }
            }

            // Ideally, we only need one condition here, however, for some Flash devices that actually support JESD216A
            // before the stanadard is publicly released, the JESD minor revsion is still the initial version. That is why
            // we use two conditions to handle below logic.
            if ((tbl->standard_version >= kSfdp_Version_Minor_A) ||
                (tbl->flash_param_tbl_size >= kSfdp_BasicProtocolTableSize_RevA))
            {
                switch (param_tbl->mode_4_4_info.quad_enable_requirement)
                {
                    case 1:
                    case 4:
                    case 5:
                        quadModeSetting = kSerialNorQuadMode_StatusReg2_Bit1;
                        break;
                    case 2:
                        quadModeSetting = kSerialNorQuadMode_StatusReg1_Bit6;
                        break;
                    case 3:
                        quadModeSetting = kSerialNorQuadMode_StatusReg2_Bit7;
                        break;
                    case 6:
                        quadModeSetting = kSerialNorQuadMode_StatusReg2_Bit1_0x31;
                        break;
                    case 0:
                    default:
                        quadModeSetting = kSerialNorQuadMode_NotConfig;
                        break;
                }
            }
            else
            {
                // Use quad mode setting paraments provided in the configure option block.
                // This setting is used for the flash devices which only support JESD216 initial version.
                quadModeSetting = option->option0.B.quad_mode_setting;
            }
            switch (quadModeSetting)
            {
                // Need write status register1 and register2 at the same time with command 0x01. 
                // See JESD216B doc for more details.
                case kSerialNorQuadMode_StatusReg2_Bit1:
                    config->spifiConfig.isQuadDualNeedEnable = true;
                    config->spifiConfig.writeTwoStausBytes = true;
                    config->spifiConfig.quadDualreadCommand = kSerialNorCmd_ReadSecStatus_35;
                    config->spifiConfig.quadDualEnableBitShift = 1;
                    break;
                // Need write status register1 directly with command 0x01.
                case kSerialNorQuadMode_StatusReg1_Bit6:
                    config->spifiConfig.isQuadDualNeedEnable = true;
                    config->spifiConfig.writeTwoStausBytes = false;
                    config->spifiConfig.quadDualreadCommand = kSerialNorCmd_ReadStatus;
                    config->spifiConfig.quadDualEnableCommand = kSerialNorCmd_WriteStatus;
                    config->spifiConfig.quadDualEnableBitShift = 6;
                    break;
                // Need write status register2 directly with special command 0x3E.
                case kSerialNorQuadMode_StatusReg2_Bit7:
                    config->spifiConfig.isQuadDualNeedEnable = true;
                    config->spifiConfig.writeTwoStausBytes = false;
                    config->spifiConfig.quadDualreadCommand = kSerialNorCmd_ReadSecStatus_3F;
                    config->spifiConfig.quadDualEnableCommand = kSerialNorCmd_WriteSecStatus_3E;
                    config->spifiConfig.quadDualEnableBitShift = 7;
                    break;
                // Need write status register2 directly with special command 0x31.
                case kSerialNorQuadMode_StatusReg2_Bit1_0x31:
                    config->spifiConfig.isQuadDualNeedEnable = true;
                    config->spifiConfig.writeTwoStausBytes = false;
                    config->spifiConfig.quadDualreadCommand = kSerialNorCmd_ReadSecStatus_35;
                    config->spifiConfig.quadDualEnableCommand = kSerialNorCmd_WriteSecStatus_31;
                    config->spifiConfig.quadDualEnableBitShift = 1;
                    break;
                case kSerialNorQuadMode_NotConfig:
                default:
                    config->spifiConfig.isQuadDualNeedEnable = false;
                    break;
            }
        }

        status = kStatus_Success;
    } while (0);

    return status;
}

status_t spifi_nor_get_config(SPIFI_Type *base, spifi_nor_config_t *config, spifi_nor_config_option_t *option)
{
    status_t status = kStatus_InvalidArgument;
    jedec_info_table_t jedec_info_tbl;

    do
    {
        if ((config == NULL) || (option == NULL))
        {
            break;
        }

        // Configure the Configuration block to default value
        memset(config, 0, sizeof(spifi_nor_config_t));
        SPIFI_GetDefaultConfig(&config->spifiConfig.memConfig);
        spifi_nor_get_default_cmdSet(&config->commandSet);
        config->SerialClkFreq = spifi_nor_clk_freqOption[kSpifiSerialClk_24MHz];
        config->memorySize = MAX_24BIT_ADDRESSING_SIZE;
        config->tag = SPIFI_CFG_BLK_TAG;
        config->version = SPIFI_CFG_BLK_VERSION;
        // Init SPIFI NOR flash based on default configure value for reading SFDP
        status = spifi_norflash_init(base, config);
        // Read SFDP information via SPIFI
        status = spifi_nor_read_sfdp_info(base, &jedec_info_tbl);
        if (status != kStatus_Success)
        {
            break;
        }
        // Parse SFDP parameters and then fill into SPIFI Serial NOR Configuration Block
        status = parse_sfdp(base, config, &jedec_info_tbl, option);
        if (status == kStatus_Success)
        {
            // Update desired clock frequency to SPIFI Serial NOR Configuration Block
            config->SerialClkFreq = spifi_nor_clk_freqOption[option->option0.B.max_freq];
        }

    } while (0);

    return status;
}

status_t spifi_norflash_program_page(SPIFI_Type *base, spifi_nor_config_t *config, uint32_t address, const uint8_t *buf)
{
    uint32_t data, i;
    uint32_t pageSize = config->pageSize;
    spifi_command_t cmd;
    status_t status;

    /* Enable write operation. */
    status = spifi_nor_write_enable(base, config, kSPIFI_CommandAllSerial);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Keep address as page align. */
    address &= ~(pageSize - 1U );
    SPIFI_SetCommandAddress(base, address);

    cmd.dataLen = pageSize;
    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataOutput;
    cmd.intermediateBytes = 0;
    cmd.format = kSPIFI_CommandAllSerial;
    cmd.type = config->spifiConfig.commandType;
    cmd.opcode = config->commandSet.pageWriteMemoryCommand;
    SPIFI_SetCommand(base, &cmd);
    for (i = 0; i < pageSize; i += 4U)
    {
        data = *(uint32_t *)buf;
        buf += 4U;
        SPIFI_WriteData(base, data);
    }

    status = check_norflash_finish(base ,config);
    if (kStatus_Success != status)
    {
        status = kStatus_SPIFINOR_ProgramFail;
    }

    return status;
}

status_t spifi_norflash_erase(SPIFI_Type *base, spifi_nor_config_t *config, uint32_t address, spifi_nor_erase_type_t type)
{
    uint8_t opcode;
    uint32_t sectorSize = config->sectorSize;
    spifi_command_t cmd;
    spifi_command_type_t cmdType;
    status_t status;

    /* Check erase type. */
    switch (type)
    {
        case kSpifiNorEraseType_Sector:
            opcode = config->commandSet.eraseSectorCommand;
            address &= ~(sectorSize - 1 );
            cmdType = config->spifiConfig.commandType;
            break;

        case kSpifiNorEraseType_Bulk:
            opcode = config->commandSet.eraseChipCommand;
            address = FSL_FEATURE_SPIFI_START_ADDR;
            cmdType = kSPIFI_CommandOpcodeOnly;
            break;

        default:
            return kStatus_InvalidArgument;
    }

    /* Enable write operation. */
    status = spifi_nor_write_enable(base, config, kSPIFI_CommandAllSerial);
    if (kStatus_Success != status)
    {
        return status;
    }
    /* Set address. */
    SPIFI_SetCommandAddress(base, address);

     /* Do erase operation. */
    cmd.dataLen = 0;
    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataOutput;
    cmd.intermediateBytes = 0;
    cmd.format = kSPIFI_CommandAllSerial;
    cmd.type = cmdType;
    cmd.opcode = opcode;
    SPIFI_SetCommand(base, &cmd);

    /* Check if finished. */
    status = check_norflash_finish(base, config);
    if (kStatus_Success != status)
    {
        status = (kSpifiNorEraseType_Sector == type) ?
                 kStatus_SPIFINOR_EraseSectorFail : kStatus_SPIFINOR_EraseAllFail;
    }

    return status;
}

status_t spifi_norflash_read(SPIFI_Type *base, spifi_nor_config_t *config,
                             uint32_t address, uint8_t *buf, uint32_t length)
{
    spifi_command_t cmd;

    cmd.dataLen = 0;
    cmd.isPollMode = false;
    cmd.direction = kSPIFI_DataInput;
    cmd.intermediateBytes = config->spifiConfig.intLen;
    cmd.format = config->spifiConfig.readmemCommandFormt;
    cmd.type = config->spifiConfig.commandType;
    cmd.opcode = config->commandSet.readMemoryCommand;
    SPIFI_SetMemoryCommand(base, &cmd);

    memcpy(buf, (void *)address, length);
    /* Reset to command mode. */
    SPIFI_ResetCommand(base);

    return kStatus_Success;
}
