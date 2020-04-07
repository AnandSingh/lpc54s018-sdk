/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "emwin_support.h"

#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "BUTTON.h"

#include "pin_mux.h"
#include "fsl_sctimer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitPWM(void)
{
    sctimer_config_t config;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;

    CLOCK_AttachClk(kMAIN_CLK_to_SCT_CLK);

    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 2, true);

    SCTIMER_GetDefaultConfig(&config);

    SCTIMER_Init(SCT0, &config);

    pwmParam.output           = kSCTIMER_Out_5;
    pwmParam.level            = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = 5;

    SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, CLOCK_GetSctClkFreq(), &event);
}


int main(void)
{
    int termWidth;
    int termHeight;

    int charWidth;
    int lineHeight;

    int c      = 0;
    int c_prev = 0;

    /* Board pin, clock, debug console init */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Route Main clock to LCD. */
    CLOCK_AttachClk(kMAIN_CLK_to_LCD_CLK);

    /* attach 12 MHz clock to FLEXCOMM2 (I2C master for touch controller) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    CLOCK_EnableClock(kCLOCK_Gpio2);

    CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);

    BOARD_InitPins();
    BOARD_BootClockPLL180M();
    BOARD_InitDebugConsole();
    BOARD_InitSDRAM();

    /* Set the back light PWM. */
    BOARD_InitPWM();

    /* emWin start */
    GUI_Init();

    WM_SetSize(WM_HBKWIN, LCD_WIDTH, LCD_HEIGHT);

    /* Solid color display */
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    GUI_MULTIBUF_Begin();
    GUI_Exec();
    GUI_MULTIBUF_End();

    GUI_SetColor(GUI_BLACK);

    termWidth  = WM_GetWindowSizeX(0);
    termHeight = WM_GetWindowSizeY(0);

    GUI_SetFont(GUI_FONT_8X16);
    lineHeight = GUI_GetFontDistY();
    charWidth  = GUI_GetCharDistX('_');

    GUI_DispString("Waiting for data - connect using serial terminal\n>");

    for (;;)
    {
        /* show cursor */
        GUI_MULTIBUF_Begin();
        GUI_DispChar('_');
        GUI_GotoX(GUI_GetDispPosX() - charWidth);
        GUI_MULTIBUF_End();

        c = GETCHAR();
        if (c == '\r')
        {
            /* translate CR to LF */
            c_prev = c;
            c      = '\n';
        }
        else if (c == '\n' && c_prev == '\r')
        {
            /* ignore LF following CR */
            c_prev = c;
            continue;
        }
        c_prev = c;

        /* hide cursor */
        GUI_MULTIBUF_Begin();
        GUI_DispChar(' ');
        GUI_GotoX(GUI_GetDispPosX() - charWidth);

        GUI_DispChar(c);

        if (GUI_GetDispPosX() >= termWidth)
        {
            GUI_DispNextLine();
        }

        if (GUI_GetDispPosY() + lineHeight > termHeight)
        {
            GUI_Clear();
            GUI_GotoY(0);
        }
        GUI_MULTIBUF_End();
    }
}
