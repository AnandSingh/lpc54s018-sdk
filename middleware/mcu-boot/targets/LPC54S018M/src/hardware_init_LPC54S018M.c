/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"
#include "fsl_clock.h"
#if BL_ENABLE_CRC_CHECK
#include "bootloader/bl_app_crc_check.h"
#endif
#include "fsl_iocon.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_power.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void update_memory_map_lpc_sram(void)
{
}

void init_hardware(void)
{
    /* enable clock for IOCON */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio3);

    /* attach 12 MHz clock to FLEXCOMM0 (USART0) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    /* attach 12 MHz clock to FLEXCOMM2 (I2C2 slave) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
    /* attach 12 MHz clock to FLEXCOMM9 (SPI9 slave) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM9);

    /* reset FLEXCOMM0 for USART0 */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    /* reset FLEXCOMM2 for I2C2 */
    RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);
    /* reset FLEXCOMM9 for SPI9 */
    RESET_PeripheralReset(kFC9_RST_SHIFT_RSTn);
    
    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

#if BL_FEATURE_SPIFI_NOR_MODULE

void spifi_clock_gate(void)
{
    /* If this case runs in RAM, the debuger reset must
       be selected as Core in case not resetting SRAM after downloading,
       so the peripherals used will also not be reset by IDE tool.
       In this case these peripherals used should be reset manually by software
    */
    RESET_PeripheralReset(kSPIFI_RST_SHIFT_RSTn);
    /* Set SPIFI clock source */
    CLOCK_AttachClk(kSYS_PLL_to_SPIFI_CLK);
}

uint32_t spifi_source_clock(void)
{
   return CLOCK_GetFreq(kCLOCK_PllOut);
}


void spifi_iomux_config(void)
{
#define IOCON_IOMUX_SPIFI_CSN_IDX       (23)
#define IOCON_IOMUX_SPIFI_IO0_IDX       (24)
#define IOCON_IOMUX_SPIFI_IO1_IDX       (25)
#define IOCON_IOMUX_SPIFI_CLK_IDX       (26)
#define IOCON_IOMUX_SPIFI_IO3_IDX       (27)
#define IOCON_IOMUX_SPIFI_IO2_IDX       (28)

    uint32_t port_pin_config;

    port_pin_config = /* Pin is configured as SPIFI_CSN */
                        IOCON_FUNC6 |
                        /* Selects pull-up function */
                        IOCON_MODE_PULLUP |
                        /* Enables digital function */
                        IOCON_DIGITAL_EN |
                        /* Input filter disabled */
                        IOCON_INPFILT_OFF;

    IOCON_PinMuxSet(IOCON, 0U, IOCON_IOMUX_SPIFI_CSN_IDX, port_pin_config);

    port_pin_config = /* Pin is configured as SPIFI_IO(0) */
                        IOCON_FUNC6 |
                        /* Selects pull-up function */
                        IOCON_MODE_PULLUP |
                        /* Enables digital function */
                        IOCON_DIGITAL_EN |
                        /* Input filter disabled */
                        IOCON_INPFILT_OFF;

    IOCON_PinMuxSet(IOCON, 0U, IOCON_IOMUX_SPIFI_IO0_IDX, port_pin_config);

    port_pin_config = /* Pin is configured as SPIFI_IO(1) */
                        IOCON_FUNC6 |
                        /* Selects pull-up function */
                        IOCON_MODE_PULLUP |
                        /* Enables digital function */
                        IOCON_DIGITAL_EN |
                        /* Input filter disabled */
                        IOCON_INPFILT_OFF;

    IOCON_PinMuxSet(IOCON, 0U, IOCON_IOMUX_SPIFI_IO1_IDX, port_pin_config);


    port_pin_config = /* Pin is configured as SPIFI_CLK */
                        IOCON_FUNC6 |
                        /* Selects pull-up function */
                        IOCON_MODE_PULLUP |
                        /* Enables digital function */
                        IOCON_DIGITAL_EN |
                        /* Input filter disabled */
                        IOCON_INPFILT_OFF;

    IOCON_PinMuxSet(IOCON, 0U, IOCON_IOMUX_SPIFI_CLK_IDX, port_pin_config);

    port_pin_config = /* Pin is configured as SPIFI_IO(3) */
                        IOCON_FUNC6 |
                        /* Selects pull-up function */
                        IOCON_MODE_PULLUP |
                        /* Enables digital function */
                        IOCON_DIGITAL_EN |
                        /* Input filter disabled */
                        IOCON_INPFILT_OFF;

    IOCON_PinMuxSet(IOCON, 0U, IOCON_IOMUX_SPIFI_IO3_IDX, port_pin_config);

    port_pin_config = /* Pin is configured as SPIFI_IO(2) */
                        IOCON_FUNC6 |
                        /* Selects pull-up function */
                        IOCON_MODE_PULLUP |
                        /* Enables digital function */
                        IOCON_DIGITAL_EN |
                        /* Input filter disabled */
                        IOCON_INPFILT_OFF;

    IOCON_PinMuxSet(IOCON, 0U, IOCON_IOMUX_SPIFI_IO2_IDX, port_pin_config);

}
#endif /* BL_FEATURE_SPIFI_NOR_MODULE */

void deinit_hardware(void)
{
}

bool usb_clock_init(void)
{
    /* reset USB0 and USB1 */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HMR_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HSL_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1RAM_RST_SHIFT_RSTn);

#if BL_CONFIG_USB_HID || BL_CONFIG_USB_MSC
    /* USB vbus pin */
    IOCON_PinMuxSet(IOCON, 0, 22, IOCON_MODE_INACT | IOCON_FUNC7 | IOCON_DIGITAL_EN);

    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*Turn on USB Phy */

    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    *((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);

    /* enable USB0 IP clock */
    CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbSrcFro, CLOCK_GetFreq(kCLOCK_FroHf));
#endif

#if BL_CONFIG_HS_USB_HID || BL_CONFIG_HS_USB_MSC
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY);
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_Usbh1);
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    /* enable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_Usbh1);

    /* enable USB1 IP clock */
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, 0U);
#endif
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif

    return true;
}


//! @brief Return uart clock frequency according to instance
uint32_t get_uart_clock(uint32_t instance)
{
    return 0;
}

//! @brief Returns the current flexcomm clock frequency in Hertz.
uint32_t get_flexcomm_clock(uint32_t instance)
{
    switch(instance)
    {
        case 0:
            return CLOCK_GetFlexCommClkFreq(0U);
        case 2:
            return CLOCK_GetFlexCommClkFreq(2U);
        case 9:
            return CLOCK_GetFlexCommClkFreq(9U);
        default:
            break;
    }

    return 0;
}

bool is_boot_pin_asserted(void)
{
    return false;
}

void dummy_byte_callback(uint8_t byte)
{
    (void)byte;
}

void debug_init(void)
{
}

#if __ICCARM__

size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals()
{
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
