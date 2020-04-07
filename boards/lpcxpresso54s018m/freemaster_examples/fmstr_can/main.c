/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "board.h"
#include "pin_mux.h"
#include "fsl_mcan.h"
#include "fsl_common.h"

#include "freemaster.h"
#include "freemaster_mcan.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
static void init_freemaster_can(void);
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Set MCAN clock 180/6=30MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivCan0Clk, 6U, true);

    /* Board initialization */
    BOARD_InitPins();
    BOARD_BootClockPLL180M();

    /* FreeMASTER communication layer initialization */
    init_freemaster_can();
    
    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while(1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief CAN Module initialization
 */
static void init_freemaster_can(void)
{
    mcan_config_t mcanConfig;

    /* Set MCAN clock 220/27= cca 8MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivCan0Clk, 27U, true);
    
    MCAN_GetDefaultConfig(&mcanConfig);

/* If CAN FD is allowed */    
#if (defined(USE_CANFD) && USE_CANFD)
    mcanConfig.enableCanfdNormal = true;
#endif
    
    /* Arbitration phase baud rate for CAN FD; For normal CAN, it is baud rate. */
    mcanConfig.baudRateA = 500000U;
    /* Data phase baud rate for CAN FD; For normal CAN, it is not used. */
    mcanConfig.baudRateD = 1000000U;
    
    /* Initialize CAN */
    MCAN_Init(CAN0, &mcanConfig, CLOCK_GetMCanClkFreq(0));
    
    
    
    /* Register communication module used by FreeMASTER driver. */
    FMSTR_CanSetBaseAddress(CAN0);
    
#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable RX fifo0 new message interrupt using interrupt line 0. */
    EnableIRQ(CAN0_IRQ0_IRQn);
    /* Enable interrupts. */
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
*   Application interrupt handler of communication peripheral used in interrupt modes
*   of FreeMASTER communication.
*
*   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
*   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
*   application specific implementation. FreeMASTER overrides the original weak definition and
*   redirects the call to its own handler.
*
*/

void CAN0_IRQ0_IRQHandler (void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_CanIsr();
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
