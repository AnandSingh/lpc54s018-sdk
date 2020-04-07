/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "serial_manager.h"

#if !(defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC))

#include "fw_serial.h"
#include "fsl_usart_freertos.h"

static usart_rtos_handle_t g_rtos_handle = {0};
static usart_handle_t g_uart_handle      = {0};
static uint8_t g_background_buffer[8];

int32_t fw_serial_init(void *uart_base, uint32_t baudrate, uint32_t clk_freq)
{
    struct rtos_usart_config uart_config = {
        .baudrate    = baudrate,
        .parity      = kUSART_ParityDisabled,
        .stopbits    = kUSART_OneStopBit,
        .buffer      = g_background_buffer,
        .buffer_size = sizeof(g_background_buffer),
    };
    uart_config.srcclk = clk_freq;
    uart_config.base   = uart_base;
    int32_t result     = USART_RTOS_Init(&g_rtos_handle, &g_uart_handle, &uart_config);
    return result;
}

int32_t fw_serial_receive(uint8_t *buffer, uint32_t length, size_t *received)
{
    int32_t result = USART_RTOS_Receive(&g_rtos_handle, buffer, length, received);
    return result;
}

int32_t fw_serial_send(const uint8_t *buffer, uint32_t length)
{
    int32_t result = USART_RTOS_Send(&g_rtos_handle, (uint8_t *)buffer, length);
    return result;
}

#endif
