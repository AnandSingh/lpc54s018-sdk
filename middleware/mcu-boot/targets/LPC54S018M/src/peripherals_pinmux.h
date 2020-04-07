/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! peripheral enable configurations
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_FLEXCOMM_USART_0)
#define BL_ENABLE_PINMUX_I2C2 (BL_CONFIG_FLEXCOMM_I2C_2)
#define BL_ENABLE_PINMUX_SPI9 (BL_CONFIG_FLEXCOMM_SPI_9)

//! UART pinmux configurations
#define UART0_ENABLE_GINT               (0)
#define UART0_ENABLE_PINT               (1)
#if UART0_ENABLE_GINT
#define UART0_RX_GINT_BASE              GINT0
#define UART0_RX_GINT_GROUP             kGINT_Port0
#define UART0_RX_GPIO_IRQn              GINT0_IRQn
#define UART0_RX_GPIO_IRQHandler        GINT0_IRQHandler
#elif UART0_ENABLE_PINT
#define UART0_RX_PINT_BASE              PINT
#define UART0_RX_PINT_INT_SRC           kINPUTMUX_GpioPort0Pin29ToPintsel
#define UART0_RX_PINT_INT_TYPE          kPINT_PinInt0
#define UART0_RX_GPIO_IRQn              PIN_INT0_IRQn
#define UART0_RX_GPIO_IRQHandler        PIN_INT0_IRQHandler
#endif
#define UART0_RX_IOCON_BASE             IOCON
#define UART0_RX_GPIO_BASE              GPIO
#define UART0_RX_GPIO_PIN_GROUP         0
#define UART0_RX_GPIO_PIN_NUM           29              // PIO0_29
#define UART0_RX_FUNC_ALT_MODE          IOCON_FUNC1     // (FC0)FUNC mode for UART0 RX
#define UART0_RX_GPIO_ALT_MODE          IOCON_FUNC0     // FUNC mode for GPIO
#define UART0_TX_IOCON_BASE             IOCON
#define UART0_TX_GPIO_PIN_GROUP         0
#define UART0_TX_GPIO_PIN_NUM           30              // PIO0_30
#define UART0_TX_FUNC_ALT_MODE          IOCON_FUNC1     // (FC0)FUNC mode for UART0 TX
#define UART0_IRQHandler                FLEXCOMM0_IRQHandler

//! I2C pinmux configurations
#define I2C2_SCL_IOCON_BASE             IOCON
#define I2C2_SCL_GPIO_PIN_GROUP         3
#define I2C2_SCL_GPIO_PIN_NUM           24              // PIO3_24
#define I2C2_SCL_FUNC_ALT_MODE          IOCON_FUNC1     // (FC2)ALT mode for I2C2 SCL
#define I2C2_SDA_IOCON_BASE             IOCON
#define I2C2_SDA_GPIO_PIN_GROUP         3
#define I2C2_SDA_GPIO_PIN_NUM           23              // PIO3_23
#define I2C2_SDA_FUNC_ALT_MODE          IOCON_FUNC1     // (FC2)ALT mode for I2C2 SDA
#define I2C2_IRQHandler                 FLEXCOMM2_IRQHandler

//! SPI pinmux configurations
#define SPI9_SELECTED_SSEL              0
#define SPI9_SSEL_IOCON_BASE            IOCON
#define SPI9_SSEL_GPIO_PIN_GROUP        3
#define SPI9_SSEL_GPIO_PIN_NUM          30              // PIO3_30
#define SPI9_SSEL_FUNC_ALT_MODE         IOCON_FUNC1     // (FC9)ALT mode for SPI9 SSEL
#define SPI9_SCK_IOCON_BASE             IOCON
#define SPI9_SCK_GPIO_PIN_GROUP         3
#define SPI9_SCK_GPIO_PIN_NUM           20              // PIO3_20
#define SPI9_SCK_FUNC_ALT_MODE          IOCON_FUNC1     // (FC9)ALT mode for SPI9 SCK
#define SPI9_MISO_IOCON_BASE            IOCON
#define SPI9_MISO_GPIO_PIN_GROUP        3
#define SPI9_MISO_GPIO_PIN_NUM          22              // PIO3_22
#define SPI9_MISO_FUNC_ALT_MODE         IOCON_FUNC1     // (FC9)ALT mode for SPI9 MISO
#define SPI9_MOSI_IOCON_BASE            IOCON
#define SPI9_MOSI_GPIO_PIN_GROUP        3
#define SPI9_MOSI_GPIO_PIN_NUM          21              // PIO3_21
#define SPI9_MOSI_FUNC_ALT_MODE         IOCON_FUNC1     // (FC9)ALT mode for SPI9 MOSI
#define SPI9_IRQHandler                 FLEXCOMM9_IRQHandler

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
