/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader/bl_peripheral_interface.h"
#include "packet/serial_packet.h"

extern void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void i2c_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void spi_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#if !BL_CONFIG_FLEXCOMM_USART && !BL_CONFIG_FLEXCOMM_I2C && !BL_CONFIG_FLEXCOMM_SPI && !BL_CONFIG_USB_HID
#error At least one peripheral must be enabled!
#endif

//! @brief Peripheral array for LPC54608.
const peripheral_descriptor_t g_peripherals[] = {
#if BL_CONFIG_FLEXCOMM_USART_0
    // FLEXCOMM USART0
    {.typeMask = kPeripheralType_UART,
     .instance = 0,
     .pinmuxConfig = uart_pinmux_config,
     .controlInterface = &g_flexcommUsartControlInterface,
     .byteInterface = &g_flexcommUsartByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_LPUART_0

#if BL_CONFIG_FLEXCOMM_I2C_2
    // FLEXCOMM I2C2
    {.typeMask = kPeripheralType_I2CSlave,
     .instance = 2,
     .pinmuxConfig = i2c_pinmux_config,
     .controlInterface = &g_flexcommI2cControlInterface,
     .byteInterface = &g_flexcommI2cByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_FLEXCOMM_I2C_2

#if BL_CONFIG_FLEXCOMM_SPI_9
    // FLEXCOMM SPI9
    {.typeMask = kPeripheralType_SPISlave,
     .instance = 9,
     .pinmuxConfig = spi_pinmux_config,
     .controlInterface = &g_flexcommSpiControlInterface,
     .byteInterface = &g_flexcommSpiByteInterface,
     .packetInterface = &g_framingPacketInterface },
#endif // BL_CONFIG_FLEXCOMM_SPI_9

#if BL_CONFIG_USB_HID
    // USB HID
    {.typeMask = kPeripheralType_USB_HID,
     .instance = 4,
     .pinmuxConfig = NULL,
     .controlInterface = &g_usbHidControlInterface,
     .byteInterface = NULL,
     .packetInterface = &g_usbHidPacketInterface },
#endif    // BL_CONFIG_USB_HID

#if BL_CONFIG_HS_USB_HID
    // USB HS HID
    {.typeMask = kPeripheralType_USB_HID,
     .instance = 6,
     .pinmuxConfig = NULL,
     .controlInterface = &g_usbHidControlInterface,
     .byteInterface = NULL,
     .packetInterface = &g_usbHidPacketInterface },
#endif    // BL_CONFIG_USB_HID

    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
