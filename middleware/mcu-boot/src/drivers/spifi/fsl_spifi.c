/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spifi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the SPIFI instance from peripheral base address.
 *
 * @param base SPIFI peripheral base address.
 * @return SPIFI instance.
 */
uint32_t SPIFI_GetInstance(SPIFI_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Array of SPIFI peripheral base address. */
static SPIFI_Type *const s_spifiBases[] = SPIFI_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Array of SPIFI clock name. */
static const clock_ip_name_t s_spifiClock[] = SPIFI_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t SPIFI_GetInstance(SPIFI_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_spifiBases); instance++)
    {
        if (s_spifiBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_spifiBases));

    return instance;
}

void SPIFI_GetDefaultConfig(spifi_config_t *config)
{
    config->timeout = 0xFFFFU;
    config->csHighTime = 0xFU;
    config->disablePrefetch = false;
    config->disableCachePrefech = false;
    config->isFeedbackClock = true;
    config->spiMode = kSPIFI_SPISckLow;
    config->isReadFullClockCycle = true;
    config->dualMode = kSPIFI_QuadMode;
}

void SPIFI_Init(SPIFI_Type *base, const spifi_config_t *config)
{
    assert(config);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the SPIFI clock */
    CLOCK_EnableClock(s_spifiClock[SPIFI_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Reset the Command register */
    SPIFI_ResetCommand(base);

    /* Set time delay parameter */
    base->CTRL = SPIFI_CTRL_TIMEOUT(config->timeout) | SPIFI_CTRL_CSHIGH(config->csHighTime) |
                 SPIFI_CTRL_D_PRFTCH_DIS(config->disablePrefetch) | SPIFI_CTRL_MODE3(config->spiMode) |
                 SPIFI_CTRL_PRFTCH_DIS(config->disableCachePrefech) | SPIFI_CTRL_DUAL(config->dualMode) |
                 SPIFI_CTRL_RFCLK(config->isReadFullClockCycle) | SPIFI_CTRL_FBCLK(config->isFeedbackClock);
}

void SPIFI_Deinit(SPIFI_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable the SPIFI clock */
    CLOCK_DisableClock(s_spifiClock[SPIFI_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void SPIFI_SetCommand(SPIFI_Type *base, spifi_command_t *cmd)
{
    /* Wait for the CMD and MCINT flag all be 0 */
    while (SPIFI_GetStatusFlag(base) & (SPIFI_STAT_MCINIT_MASK | SPIFI_STAT_CMD_MASK))
    {
    }
    base->CMD = SPIFI_CMD_DATALEN(cmd->dataLen) | SPIFI_CMD_POLL(cmd->isPollMode) | SPIFI_CMD_DOUT(cmd->direction) |
                SPIFI_CMD_INTLEN(cmd->intermediateBytes) | SPIFI_CMD_FIELDFORM(cmd->format) |
                SPIFI_CMD_FRAMEFORM(cmd->type) | SPIFI_CMD_OPCODE(cmd->opcode);

    /* Wait for the command written */
    while ((base->STAT & SPIFI_STAT_CMD_MASK) == 0U)
    {
    }
}

void SPIFI_SetMemoryCommand(SPIFI_Type *base, spifi_command_t *cmd)
{
    /* Wait for the CMD and MCINT flag all be 0 */
    while (SPIFI_GetStatusFlag(base) & (SPIFI_STAT_MCINIT_MASK | SPIFI_STAT_CMD_MASK))
    {
    }

    base->MCMD = SPIFI_MCMD_POLL(0U) | SPIFI_MCMD_DOUT(0U) | SPIFI_MCMD_INTLEN(cmd->intermediateBytes) |
                 SPIFI_MCMD_FIELDFORM(cmd->format) | SPIFI_MCMD_FRAMEFORM(cmd->type) | SPIFI_MCMD_OPCODE(cmd->opcode);

    /* Wait for the memory command written */
    while ((base->STAT & SPIFI_STAT_MCINIT_MASK) == 0U)
    {
    }
}
