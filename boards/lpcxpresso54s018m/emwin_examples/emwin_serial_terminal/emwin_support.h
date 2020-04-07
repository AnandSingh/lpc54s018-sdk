/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMWIN_SUPPORT_H_
#define _EMWIN_SUPPORT_H_

#define SDRAM_BASE_ADDR 0xa0000000
#define SDRAM_SIZE_BYTES (8 * 1024 * 1024)

#define APP_LCD LCD
#define APP_LCD_IRQHandler LCD_IRQHandler
#define APP_LCD_IRQn LCD_IRQn

#define LCD_PANEL_CLK 9000000
#define LCD_PPL 480
#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_LPP 272
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#define LCD_POL_FLAGS kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity
#define LCD_INPUT_CLK_FREQ CLOCK_GetLcdClkFreq()
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define LCD_BITS_PER_PIXEL 16

/* Work memory for emWin */
#define GUI_NUMBYTES 0x20000
#define GUI_MEMORY_ADDR (SDRAM_BASE_ADDR)

/* Display framebuffer */
#define GUI_BUFFERS 2
#define VRAM_ADDR (GUI_MEMORY_ADDR + GUI_NUMBYTES)
#define VRAM_SIZE (LCD_HEIGHT * LCD_WIDTH * LCD_BITS_PER_PIXEL / 8)

#define CLEAR_BUTTON_ID (GUI_ID_BUTTON0)

#define COLOR_BUTTONS 8
#define COLOR_BUTTON_FIRST_ID (GUI_ID_USER)
#define COLOR_BUTTON_LAST_ID (COLOR_BUTTON_FIRST_ID + COLOR_BUTTONS - 1)

#endif
