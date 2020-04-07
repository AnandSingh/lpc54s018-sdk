/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_otp.h"
#include "otp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t s_otpInitilized = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief program OTP.
 *
 * @details Call ROM API to program OTP.
 */

status_t ocotp_init()
{
   /* enable clock to OTP and reset it */
    CLOCK_EnableClock(kCLOCK_Otp);
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);

    return kStatus_Success;
}

status_t ocotp_program_once(uint32_t index, uint32_t *src, uint32_t lengthInBytes)
{
    // If length to read is 0, then return success.
    if (!lengthInBytes)
    {
        return kStatus_Success;
    }

    // Length to read must be 1 word (4 bytes)
    // Index should be in the bit mask range.
    if ((lengthInBytes != 4) || (index < 4) || (index > 15))//need update, some register cannot access
    {
        return kStatus_InvalidArgument;
    }
    
    status_t status;
    uint32_t bankIndex = index / 4;
    uint32_t regIndex = index % 4;
    otp_bank_t bankMask = (otp_bank_t) (0x1U << bankIndex);
    otp_word_t regMask = (otp_word_t) (0x1U << regIndex);
    
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

    /* Unlock OTP Bank bankIndex for write access */
    status = OTP_EnableBankWriteMask(bankMask);
    if (status != kStatus_Success) 
    {
        return status;
    }
    
    /* Unlocks write access to register regIndex of OTP bank bankIndex */
    status = OTP_EnableBankWriteLock(bankIndex, regMask, 0x0, kOTP_LockDontLock);
    if (status != kStatus_Success) 
    {
	return status;
    }
    
    /* Program Register refIndex of OTP Bank bankIndex */
    status = OTP_ProgramRegister(bankIndex, regIndex, *src);
    if (status != kStatus_Success) 
    {
	return status;
    }

    return kStatus_Success;
}

status_t ocotp_read_once(uint32_t index, uint32_t *dst, uint32_t lengthInBytes)
{
    // If length to read is 0, then return success.
    if (!lengthInBytes)
    {
        return kStatus_Success;
    }

    // Length to read must be 1 word (4 bytes)
    // Index should be in the bit mask range.
    if ((lengthInBytes != 4) || (index > 15))
    {
        return kStatus_InvalidArgument;
    }
    
    status_t status;
    uint32_t bankIndex = index / 4;
    uint32_t regIndex = index % 4;
    otp_word_t regMask = (otp_word_t) (0x1U << regIndex);
    
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

    /* Unlocks read access to register regIndex of OTP bank bankIndex */
    status = OTP_EnableBankReadLock(bankIndex, regMask, 0x0, kOTP_LockDontLock);
    if (status != kStatus_Success) 
    {
        return status;
    }
    
    *dst = LPC_OTP->OTP[bankIndex][regIndex];
    
    return kStatus_Success;
}
