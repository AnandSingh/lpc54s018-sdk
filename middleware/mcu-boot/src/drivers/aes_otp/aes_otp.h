/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef __AES_OTP_H__
#define __AES_OTP_H__

/**********************************************************************************************************************
 * Definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * API
 *********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//!@brief Scramble and program aes_key
uint32_t scramble_and_program_aes_key(uint8_t const *buffer, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __AES_OTP_H__
