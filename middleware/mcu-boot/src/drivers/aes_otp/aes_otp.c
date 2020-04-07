/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_common.h"
#include "otp/otp.h"
#include "aes_otp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AES_KEY_128BITS 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint8_t s_otpInitilized;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief program AES key to OTP.
 *
 * @details Call ROM API to program AES key to OTP Bank 2.
 */

uint32_t scramble_and_program_aes_key(uint8_t const *buffer, uint32_t length)
{
    // Length to of aes key needs to be 128bits(16 bytes)
    if (length != 16)
    {
        return kStatus_InvalidArgument;
    }
    
    uint32_t errcode;
    uint32_t *aeskey = (uint32_t*)&buffer[0];
    status_t status;

    errcode = ROM_API->aesApiBase->aesInit();
    if (errcode != 0) {
       return errcode;
    }
    
    if(!s_otpInitilized)
    {
         status = ocotp_init();
         // Failed to init, return the failure status;
         if (status != kStatus_Success)
         {
             return status;
         }
         s_otpInitilized = 1;
    }
    
    errcode = ROM_API->otpApiBase->otpInit();
    if (errcode != 0) {
       return errcode;
    }

    // Load 128-bit key to regs in AES IP
    errcode = ROM_API->aesApiBase->aesLoadKeyFromSW(AES_KEY_128BITS, aeskey);
    if (errcode != 0) {
       return errcode;
    }

    // Program 128-bit (scrambled) key to OTP
    errcode = ROM_API->aesApiBase->aesProgramKeyToOTP();
    if (errcode != 0) {
       return errcode;
    }

    return errcode;
}
