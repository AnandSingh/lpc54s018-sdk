/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LITTLEVGL_SUPPORT_H
#define LITTLEVGL_SUPPORT_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define LCD_FB_BYTE_PER_PIXEL 2

#define DEMO_BUFFER0_ADDR 0xa0000000
#define DEMO_BUFFER1_ADDR (DEMO_BUFFER0_ADDR + LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lv_port_pre_init(void);
void lv_port_disp_init(void);
void lv_port_indev_init(void);

#if defined(__cplusplus)
}
#endif

#endif /*LITTLEVGL_SUPPORT_H */
