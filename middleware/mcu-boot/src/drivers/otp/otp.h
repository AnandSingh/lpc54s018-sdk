/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef __OTP_H__
#define __OTP_H__

/**********************************************************************************************************************
 * Definitions
 *********************************************************************************************************************/
#define LPC_OTP_BASE         0x40015000

typedef struct {
    uint32_t   OTP[4][4];/* 0x0 - OTP[0][0] .. OTP[3][3] */
} LPC_OTP_T;

#define LPC_OTP	 ((LPC_OTP_T*) LPC_OTP_BASE)
/**********************************************************************************************************************
 * API
 *********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

//!@brief Enable clock
status_t ocotp_init();
  
//!@brief Program OCOTP
status_t ocotp_program_once(uint32_t index, uint32_t *src, uint32_t lengthInBytes);

//!@brief Read OCOTP
status_t ocotp_read_once(uint32_t index, uint32_t *dst, uint32_t lengthInBytes);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __OTP_H__
