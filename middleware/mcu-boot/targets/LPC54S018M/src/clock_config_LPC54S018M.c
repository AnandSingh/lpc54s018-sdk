/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "property/property.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_power.h"
#include "target_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if defined BL_TARGET_FPGA
uint32_t busClock = 6000000u; //! 6MHz default bus clock
#endif

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//Set up SYS PLL to 480MHz
static void CLOCK_SetupSysPll480M();
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    // General procedure to be implemented:
    // 1. Read clock flags and divider from bootloader config in property store
    bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;
    uint8_t options = config->clockFlags;

    // Check if the USB HID peripheral is enabled. If it is enabled, we always
    // use the 48MHz IRC.
    bool isUsbEnabled = config->enabledPeripherals & kPeripheralType_USB_HID;

    POWER_DisablePD(kPDRUNCFG_PD_FRO_EN); /*!< Ensure FRO is on so that we can switch to its 12MHz mode temporarily*/
    CLOCK_AttachClk(
        kFRO12M_to_MAIN_CLK);          /*!< Switch to 12MHz first to ensure we can change voltage without accidentally
                                       being below the voltage for current speed */

    // 2. If NOT High Speed and USB isn't enabled, do nothing (use reset clock config)
    if ((options & kClockFlag_HighSpeed) && (!isUsbEnabled))
    {
        // High speed flag is set (meaning disabled), so just use default clocks.
        return;
    }

    POWER_SetVoltageForFreq(96000000U); /*!< Set voltage for the one of the fastest clock outputs: System clock output */
    CLOCK_SetupFROClocking(96000000U); /*!< Set up high frequency FRO output to selected frequency */
    /*!< Set up dividers */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U, false); /*!< Reset divider counter and set divider to value 1 */
    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK); /*!< Switch MAIN_CLK to FRO_HF */
    /*!< Set up SYS PLL clock and set to 480MHz */
    CLOCK_SetupSysPll480M();
    // Update global SystemCoreClock variable
    SystemCoreClockUpdate();

}
// See bootloader_common.h for documentation on this function.
// Note: this function doesn't apply to FPGA build
uint32_t get_system_core_clock(void)
{
    uint32_t systemCoreClock = SystemCoreClock;

    return systemCoreClock;
}

static void CLOCK_SetupSysPll480M()
{
    uint32_t i, x, encode20;
    x = 0x04000U;
    for (i = 20U; i <= 0x8000U; i++)
    {
        x = (((x ^ (x >> 1U)) & 1U) << 14U) | ((x >> 1U) & 0x3FFFU);
    }
    encode20 = x & (0x1FFFFUL);
    
    /*!< Set up SYS PLL. PLL_OUTPUT = 2 * 20 * 12MHz = 480MHz*/
    pll_setup_t pllSetup = {
        .pllctrl = SYSCON_SYSPLLCTRL_SELI(24U) | SYSCON_SYSPLLCTRL_SELP(11U) | SYSCON_SYSPLLCTRL_SELR(0)
                  | SYSCON_SYSPLLCTRL_DIRECTI(1) | SYSCON_SYSPLLCTRL_DIRECTO(1),
        .pllmdec = (SYSCON_SYSPLLMDEC_MDEC(encode20)),
        .pllRate = 480000000U,
        .flags = PLL_SETUPFLAG_WAITLOCK | PLL_SETUPFLAG_POWERUP};
    CLOCK_AttachClk(kFRO12M_to_SYS_PLL); /*!< Set sys pll clock source from fro_12M */
    CLOCK_SetPLLFreq(&pllSetup);          /*!< Configure PLL to the desired value */
}

// See bootloader_common.h for documentation on this function.
uint32_t get_bus_clock(void)
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
