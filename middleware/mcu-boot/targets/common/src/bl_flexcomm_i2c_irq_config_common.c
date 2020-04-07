/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "bootloader_config.h"
#include "bootloader/bl_irq_common.h"
#include "autobaud/autobaud.h"
#include "packet/serial_packet.h"
#include "fsl_device_registers.h"
#include "fsl_flexcomm.h"
#include "fsl_i2c.h"
#include "utilities/fsl_assert.h"

#if BL_CONFIG_FLEXCOMM_I2C
static const IRQn_Type flexcomm_i2c_irq_ids[FSL_FEATURE_SOC_I2C_COUNT] = I2C_IRQS;

void FLEXCOMM_I2C_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    if (set == kPeripheralEnableIRQ)
    {
        NVIC_EnableIRQ(flexcomm_i2c_irq_ids[instance]);
    }
    else
    {
        NVIC_DisableIRQ(flexcomm_i2c_irq_ids[instance]);
    }
}
#endif // BL_CONFIG_FLEXCOMM_I2C

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
