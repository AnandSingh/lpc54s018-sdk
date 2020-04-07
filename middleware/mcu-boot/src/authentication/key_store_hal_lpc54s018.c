/*
 * Copyright 2017-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "key_store_hal.h"
#include "key_store.h"

skboot_key_store_t s_keyStoreRam = { 0 };
skboot_key_store_t *const  s_keyStore = &s_keyStoreRam;
puf_config_t *pufConf;

#include <string.h>
extern int memset_s(void *, size_t, int, size_t);

/* 400 ms per KBL-2802 */
#define SKBOOT_HAL_KEYSTORE_PUF_DISCHARGE_TIME_MAX_MS 400

/* Worst-case time in ms to fully discharge PUF SRAM */
#define PUF_DISCHARGE_TIME 250

/*! Compile time sizeof() check */
#define BUILD_ASSURE(condition, msg) extern int msg[1 - 2 * (!(condition))] __attribute__((unused))

/*  for lpc54s018 we do not imlement NVM storage for key store */
skboot_status_t skboot_hal_key_secretbox_write_to_nonvolatile(uint32_t memoryId)
{
    skboot_status_t ret = kStatus_SKBOOT_Fail;
    return ret;
}

/*  for lpc54s018 we do not imlement NVM storage for key store */
skboot_status_t skboot_hal_key_secretbox_read_from_nonvolatile(const void *param, void *tempBuffer)
{
    skboot_status_t ret = kStatus_SKBOOT_Fail;
    return ret;
}

skboot_key_store_t *skboot_hal_key_secretbox_get_address(void)
{
    return (skboot_key_store_t *)(uintptr_t)&s_keyStoreRam;
}


skboot_status_t skboot_hal_key_secretbox_init(void)
{
    status_t status = kStatus_SKBOOT_Fail;

    /* make sure PUF is initialized.  */
    debug_printf("skboot_hal_key_secretbox_init");
    
    pufConf->dischargeTimeMsec = PUF_DISCHARGE_TIME;
    pufConf->coreClockFrequencyHz = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    
    status = PUF_Init(PUF, pufConf);
    if (kStatus_Success != status)
    {
        return status;
    }

    return status;
}

static skboot_status_t skboot_hal_key_secretbox_power_cycle(uint32_t pufDischargeTime_ms)
{
    skboot_status_t status = kStatus_SKBOOT_Fail;
    
    pufConf->dischargeTimeMsec = pufDischargeTime_ms;
    pufConf->coreClockFrequencyHz = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    
    PUF_Deinit(PUF, pufConf); /* power off */
    
    status = PUF_Init(PUF, pufConf);

    return status;
} 

skboot_status_t skboot_hal_key_secretbox_enroll(void)
{ 
    status_t status = kStatus_SKBOOT_Fail;

    status = skboot_hal_key_secretbox_init();
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Enroll PUF */
    status = PUF_Enroll(PUF, s_keyStore->activationCode.b, sizeof(s_keyStore->activationCode.b));

    /* If fails, try to power off, wait discharge time (maximum per SoC), and try again */
    if (status != kStatus_Success)
    {
        debug_printf("FAIL\nPUF Power Cycle\n");
        status = skboot_hal_key_secretbox_power_cycle(SKBOOT_HAL_KEYSTORE_PUF_DISCHARGE_TIME_MAX_MS);
        if (status != kStatus_Success)
        {
            return status;
        }

        debug_printf("PUF Start\n");
        status = PUF_Enroll(PUF, s_keyStore->activationCode.b, sizeof(s_keyStore->activationCode.b));
    }

    return status;
}

skboot_status_t skboot_hal_key_secretbox_start(void)
{
    status_t status = kStatus_SKBOOT_Fail;

    /* make sure PUF is initialized.  */
    status = skboot_hal_key_secretbox_init();
    if (kStatus_Success != status)
    {
        return status;
    }

    /* if already started, return success */
    if (true == PUF_IsGetKeyAllowed(PUF))
    {
        return kStatus_Success;
    }

    /* reconstruct digital fingerprint */
    debug_printf("PUF Start");
    status = PUF_Start(PUF, s_keyStore->activationCode.b, sizeof(s_keyStore->activationCode.b));

    /* If fails, try to power off, wait discharge time (maximum per SoC), and try again */
    if (status != kStatus_Success)
    {
        debug_printf("FAIL\nPUF Power Cycle\n");

        status = skboot_hal_key_secretbox_power_cycle(SKBOOT_HAL_KEYSTORE_PUF_DISCHARGE_TIME_MAX_MS);
        if (status != kStatus_Success)
        {
            return status;
        }

        debug_printf("PUF Start\n");
        status = PUF_Start(PUF, s_keyStore->activationCode.b, sizeof(s_keyStore->activationCode.b));
    }

    return status;
}

skboot_status_t skboot_hal_key_secretbox_open_secret(skboot_key_metadata_t *keyMetadata, uint32_t keyMask)
{
    status_t status = kStatus_SKBOOT_Fail;

    debug_printf("PUF Get Hw Key\n");

    switch (keyMetadata->keyType)
    {
        case kPUF_KeyType_FirmwareUpdateKey0:
            status = PUF_GetHwKey(PUF, s_keyStore->Image_Key_Code.keyCode.b, sizeof(s_keyStore->Image_Key_Code.keyCode.b), kPUF_KeySlot0,
                                  keyMask);
            break;

        case kPUF_KeyType_UserKey:
            status = PUF_GetHwKey(PUF, s_keyStore->FW_Update_Key_Code.keyCode.b, sizeof(s_keyStore->FW_Update_Key_Code.keyCode.b),
                                  kPUF_KeySlot0, keyMask);
            break;

        default:
            status = kStatus_SKBOOT_Fail;
            break;
    }

    return status;
}
