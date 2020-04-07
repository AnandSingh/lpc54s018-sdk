/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "GUIDRV_Lin.h"
#include "emwin_support.h"

#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_lcdc.h"
#include "fsl_i2c.h"
#include "fsl_ft5406.h"

/* Frame end flag. */
static int32_t s_LCDpendingBuffer;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_LCD_IRQHandler(void)
{
    uint32_t addr;
    uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(APP_LCD);

    LCDC_ClearInterruptsStatus(APP_LCD, intStatus);

    if (intStatus & kLCDC_VerticalCompareInterrupt)
    {
        if (s_LCDpendingBuffer >= 0)
        {
            /* Calculate address of the given buffer */
            addr = VRAM_ADDR + VRAM_SIZE * s_LCDpendingBuffer;
            /* Make the given buffer visible */
            LCDC_SetPanelAddr(APP_LCD, kLCDC_UpperPanel, addr);
            /* Send a confirmation that the given buffer is visible */
            GUI_MULTIBUF_Confirm(s_LCDpendingBuffer);
            s_LCDpendingBuffer = -1;
        }
    }
    __DSB();
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

status_t APP_LCDC_Init(void)
{
    /* Initialize the display. */
    lcdc_config_t lcdConfig;

    LCDC_GetDefaultConfig(&lcdConfig);

    lcdConfig.panelClock_Hz  = LCD_PANEL_CLK;
    lcdConfig.ppl            = LCD_PPL;
    lcdConfig.hsw            = LCD_HSW;
    lcdConfig.hfp            = LCD_HFP;
    lcdConfig.hbp            = LCD_HBP;
    lcdConfig.lpp            = LCD_LPP;
    lcdConfig.vsw            = LCD_VSW;
    lcdConfig.vfp            = LCD_VFP;
    lcdConfig.vbp            = LCD_VBP;
    lcdConfig.polarityFlags  = LCD_POL_FLAGS;
    lcdConfig.upperPanelAddr = VRAM_ADDR;
    lcdConfig.bpp            = kLCDC_16BPP565;
    lcdConfig.display        = kLCDC_DisplayTFT;
    lcdConfig.swapRedBlue    = false;

    LCDC_Init(APP_LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

    /* Trigger interrupt at start of every vertical back porch. */
    LCDC_SetVerticalInterruptMode(APP_LCD, kLCDC_StartOfBackPorch);
    LCDC_EnableInterrupts(APP_LCD, kLCDC_VerticalCompareInterrupt);
    NVIC_EnableIRQ(APP_LCD_IRQn);

    LCDC_Start(APP_LCD);
    LCDC_PowerUp(APP_LCD);

    return kStatus_Success;
}

/*******************************************************************************
 * Application implemented functions required by emWin library
 ******************************************************************************/
void LCD_X_Config(void)
{
    status_t status;

    GUI_MULTIBUF_Config(GUI_BUFFERS);
    GUI_DEVICE_CreateAndLink(GUIDRV_LIN_16, GUICC_565, 0, 0);

    LCD_SetSizeEx(0, LCD_WIDTH, LCD_HEIGHT);
    LCD_SetVSizeEx(0, LCD_WIDTH, LCD_HEIGHT);

    LCD_SetVRAMAddrEx(0, (void *)VRAM_ADDR);

    /* Initialize LCD controller */
    status = APP_LCDC_Init();
    if (status != kStatus_Success)
    {
        PRINTF("LCD init failed\n");
    }
    assert(status == kStatus_Success);
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *p)
{
    int r = 0;
    LCD_X_SHOWBUFFER_INFO *pData;
    switch (Cmd)
    {
        case LCD_X_SHOWBUFFER:
        {
            pData = (LCD_X_SHOWBUFFER_INFO *)p;
            //
            // Remember buffer index to be used by ISR
            //
            s_LCDpendingBuffer = pData->Index;
            return 0;
        }
        default:
            r = -1;
    }
    return r;
}

void GUI_X_Config(void)
{
    /* Assign work memory area to emWin */
    GUI_ALLOC_AssignMemory((void *)GUI_MEMORY_ADDR, GUI_NUMBYTES);

    /* Select default font */
    GUI_SetDefaultFont(GUI_FONT_6X8);
}

void GUI_X_Init(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_InitOS(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_Lock(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_Unlock(void)
{
}

/* Dummy RTOS stub required by emWin */
U32 GUI_X_GetTaskId(void)
{
    return 0;
}

void GUI_X_ExecIdle(void)
{
}

GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return 0;
}

void GUI_X_Delay(int Period)
{
}

void *emWin_memcpy(void *pDst, const void *pSrc, long size)
{
    return memcpy(pDst, pSrc, size);
}
