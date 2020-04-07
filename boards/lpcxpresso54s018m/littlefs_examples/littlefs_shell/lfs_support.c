/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_spifi.h"
#include "lfs_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COMMAND_NUM (6)
#define READ (0)
#define PROGRAM_PAGE (1)
#define GET_STATUS (2)
#define ERASE_SECTOR (3)
#define WRITE_ENABLE (4)
#define WRITE_REGISTER (5)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if defined FLASH_W25Q
spifi_command_t command[COMMAND_NUM] = {
    {FLASH_PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
    {FLASH_PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x32},
    {1, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x20},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
    {1, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x31}};
#define QUAD_MODE_VAL 0x02
#elif defined FLASH_MX25R
spifi_command_t command[COMMAND_NUM] = {
    {FLASH_PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
    {FLASH_PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandOpcodeSerial, kSPIFI_CommandOpcodeAddrThreeBytes,
     0x38},
    {1, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x20},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
    {1, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x01}};
#define QUAD_MODE_VAL 0x40
#else /* Use MT25Q */
spifi_command_t command[COMMAND_NUM] = {
    {FLASH_PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
    {FLASH_PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandOpcodeSerial, kSPIFI_CommandOpcodeAddrThreeBytes,
     0x38},
    {1, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x20},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
    {1, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x61}};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
void check_if_finish()
{
    uint8_t val = 0;
    /* Check WIP bit */
    do
    {
        SPIFI_SetCommand(EXAMPLE_SPIFI, &command[GET_STATUS]);
        while ((EXAMPLE_SPIFI->STAT & SPIFI_STAT_INTRQ_MASK) == 0U)
        {
        }
        val = SPIFI_ReadDataByte(EXAMPLE_SPIFI);
    } while (val & 0x1);
}

#if defined QUAD_MODE_VAL
void enable_quad_mode()
{
    /* Write enable */
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_ENABLE]);

    /* Set write register command */
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_REGISTER]);

    SPIFI_WriteDataByte(EXAMPLE_SPIFI, QUAD_MODE_VAL);

    check_if_finish();
}
#endif

static int lfs_spifi_read(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    block += LFS_FIRST_SECTOR;

    /* Switch to read mode for sure */
    SPIFI_ResetCommand(EXAMPLE_SPIFI);
    SPIFI_SetMemoryCommand(EXAMPLE_SPIFI, &command[READ]);

    /* The flash is mapped in the address space, just copy the data */
    memcpy(buffer, (void *)(FSL_FEATURE_SPIFI_START_ADDR + block * lfsc->block_size + off), size);

    return LFS_ERR_OK;
}

static int lfs_spifi_prog(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    const uint32_t *bufptr;
    block += LFS_FIRST_SECTOR;

    /* Reset the SPIFI to switch to command mode */
    SPIFI_ResetCommand(EXAMPLE_SPIFI);

    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_ENABLE]);
    SPIFI_SetCommandAddress(EXAMPLE_SPIFI, block * lfsc->block_size + off);
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[PROGRAM_PAGE]);

    bufptr = buffer;
    for (uint32_t i = 0; i < size / 4; i++)
    {
        SPIFI_WriteData(EXAMPLE_SPIFI, *(bufptr++));
    }

    check_if_finish();

    /* Switch to read mode */
    SPIFI_ResetCommand(EXAMPLE_SPIFI);
    SPIFI_SetMemoryCommand(EXAMPLE_SPIFI, &command[READ]);

    return LFS_ERR_OK;
}

static int lfs_spifi_erase(const struct lfs_config *lfsc, lfs_block_t block)
{
    block += LFS_FIRST_SECTOR;

    /* Reset the SPIFI to switch to command mode */
    SPIFI_ResetCommand(EXAMPLE_SPIFI);

    /* Write enable */
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_ENABLE]);
    /* Set address */
    SPIFI_SetCommandAddress(EXAMPLE_SPIFI, block * lfsc->block_size);
    /* Erase sector */
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[ERASE_SECTOR]);

    /* Check if finished */
    check_if_finish();

    /* Switch to read mode */
    SPIFI_ResetCommand(EXAMPLE_SPIFI);
    SPIFI_SetMemoryCommand(EXAMPLE_SPIFI, &command[READ]);

    return LFS_ERR_OK;
}

static int lfs_spifi_sync(const struct lfs_config *lfsc)
{
    return LFS_ERR_OK;
}

static const struct lfs_config lfsc_default = {
    // block device operations
    .read  = lfs_spifi_read,
    .prog  = lfs_spifi_prog,
    .erase = lfs_spifi_erase,
    .sync  = lfs_spifi_sync,

    // block device configuration
    .read_size   = 16,
    .prog_size   = FLASH_PAGE_SIZE,
    .block_size  = FLASH_SECTOR_SIZE,
    .block_count = LFS_SECTORS,
    .lookahead   = 128,
};

int lfs_get_default_config(struct lfs_config *lfsc)
{
    *lfsc = lfsc_default; /* copy pre-initialized lfs config structure */
    return 0;
}

int lfs_storage_init(const struct lfs_config *lfsc)
{
    spifi_config_t config = {0};
    uint32_t sourceClockFreq;

    /* Set SPIFI clock source */
    CLOCK_AttachClk(kFRO_HF_to_SPIFI_CLK);
    sourceClockFreq = CLOCK_GetFroHfFreq();

    /* Set the clock divider */
    CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, sourceClockFreq / EXAMPLE_SPI_BAUDRATE, false);

    /* Initialize SPIFI */
    SPIFI_GetDefaultConfig(&config);
    SPIFI_Init(EXAMPLE_SPIFI, &config);

#if defined QUAD_MODE_VAL
    /* Enable Quad mode */
    enable_quad_mode();
#endif

    /* Switch to read mode */
    SPIFI_ResetCommand(EXAMPLE_SPIFI);
    SPIFI_SetMemoryCommand(EXAMPLE_SPIFI, &command[READ]);

    return 0;
}
