/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "littlevgl_support.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "board.h"
#include "fsl_lcdc.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "fsl_ft5406.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Port Me. Start */
#define TOUCH_I2C I2C2
#define TOUCH_I2C_CLOCK_FREQUENCY CLOCK_GetFlexCommClkFreq(2)
#define TOUCH_I2C_SLAVE_ADDR_7BIT 0x7EU
#define TOUCH_I2C_BAUDRATE 100000U
/* Port Me. End */

#define LCD_PANEL_CLK 9000000
#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#define LCD_POL_FLAGS (kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity)
#define LCD_INPUT_CLK_FREQ CLOCK_GetLcdClkFreq()

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static void DEMO_InitLcdBackLight(void);

static void DEMO_FlushDisplay(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p);

static void DEMO_InitTouch(void);

static bool DEMO_ReadTouch(lv_indev_data_t *data);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_framePending;
static SemaphoreHandle_t s_frameSema;
static ft5406_handle_t touchHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

void lv_port_pre_init(void)
{
    /*
     * Pass in the frame buffer address to LittlevGL manually, the LV_VDB_ADR
     * and LV_VDB2_ADR should be defined to LV_VDB_ADR_INV in lv_conf.h
     */
    memset((void *)DEMO_BUFFER0_ADDR, 0, LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL);
    memset((void *)DEMO_BUFFER1_ADDR, 0, LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL);
    lv_vdb_set_adr((void *)DEMO_BUFFER0_ADDR, (void *)DEMO_BUFFER1_ADDR);
}

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
    disp_drv.disp_flush = DEMO_FlushDisplay;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

void LCD_IRQHandler(void)
{
    BaseType_t taskAwake = pdFALSE;

    uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(LCD);

    LCDC_ClearInterruptsStatus(LCD, intStatus);

    if (s_framePending)
    {
        if (intStatus & kLCDC_BaseAddrUpdateInterrupt)
        {
            s_framePending = false;

            xSemaphoreGiveFromISR(s_frameSema, &taskAwake);

            portYIELD_FROM_ISR(taskAwake);
        }
    }

    __DSB();
}

static void DEMO_InitLcdClock(void)
{
    /* Route Main clock to LCD. */
    CLOCK_AttachClk(kMAIN_CLK_to_LCD_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);
}

static void DEMO_InitLcdBackLight(void)
{
    const gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        1,
    };

    /* LCD back light. */
    GPIO_PinInit(GPIO, BOARD_LCD_BL_GPIO, BOARD_LCD_BL_PIN, &config);
}

static void DEMO_InitLcd(void)
{
    /* Initialize the display. */
    lcdc_config_t lcdConfig;

    s_frameSema = xSemaphoreCreateBinary();
    if (NULL == s_frameSema)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }

    /* No frame pending. */
    s_framePending = false;
    NVIC_SetPriority(LCD_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    DEMO_InitLcdClock();

    LCDC_GetDefaultConfig(&lcdConfig);

    lcdConfig.panelClock_Hz = LCD_PANEL_CLK;
    lcdConfig.ppl           = LCD_WIDTH;
    lcdConfig.hsw           = LCD_HSW;
    lcdConfig.hfp           = LCD_HFP;
    lcdConfig.hbp           = LCD_HBP;
    lcdConfig.lpp           = LCD_HEIGHT;
    lcdConfig.vsw           = LCD_VSW;
    lcdConfig.vfp           = LCD_VFP;
    lcdConfig.vbp           = LCD_VBP;
    lcdConfig.polarityFlags = LCD_POL_FLAGS;
    /* littlevgl starts render in frame buffer 0, so show frame buffer 1 first. */
    lcdConfig.upperPanelAddr = DEMO_BUFFER1_ADDR;
    lcdConfig.bpp            = kLCDC_16BPP565;
    lcdConfig.display        = kLCDC_DisplayTFT;
    lcdConfig.swapRedBlue    = true;

    LCDC_Init(LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

    /* Trigger interrupt at start of every vertical back porch. */
    LCDC_EnableInterrupts(LCD, kLCDC_BaseAddrUpdateInterrupt);
    NVIC_EnableIRQ(LCD_IRQn);

    LCDC_Start(LCD);
    LCDC_PowerUp(LCD);

    /* Back light. */
    DEMO_InitLcdBackLight();
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void DEMO_FlushDisplay(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p)
{
    LCDC_SetPanelAddr(LCD, kLCDC_UpperPanel, (uint32_t)color_p);

    s_framePending = true;

    if (xSemaphoreTake(s_frameSema, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("Wait semaphore error: s_frameSema\r\n");
        assert(0);
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
}

void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad */
    DEMO_InitTouch();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = DEMO_ReadTouch;
    lv_indev_drv_register(&indev_drv);
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;
    i2c_master_config_t masterConfig;
    gpio_pin_config_t pin_config = {
        kGPIO_DigitalOutput,
        0,
    };

    CLOCK_EnableClock(kCLOCK_Gpio2);

    /* attach 12 MHz clock to FLEXCOMM2 (I2C master for touch controller) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = TOUCH_I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(TOUCH_I2C, &masterConfig, TOUCH_I2C_CLOCK_FREQUENCY);

    /* Enable touch panel controller */
    GPIO_PinInit(GPIO, BOARD_TOUCH_PANEL_RST_GPIO, BOARD_TOUCH_PANEL_RST_PIN, &pin_config);
    GPIO_PinWrite(GPIO, BOARD_TOUCH_PANEL_RST_GPIO, BOARD_TOUCH_PANEL_RST_PIN, 1);

    /* Initialize touch panel controller */
    status = FT5406_Init(&touchHandle, TOUCH_I2C);
    if (status != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
        assert(0);
    }
}

/* Will be called by the library to read the touchpad */
static bool DEMO_ReadTouch(lv_indev_data_t *data)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (kStatus_Success == FT5406_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_y;
    data->point.y = touch_x;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}
