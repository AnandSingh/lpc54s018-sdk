/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "key_store.h"
#include "key_store_hal.h"

/* the RAM address of key store comes from hal (key_store_hal_lpc54s018.c) */
extern skboot_key_store_t *const s_keyStore;

#ifdef SKBOOT_KEY_MANAGEMENT_USE_PUF

/* Key provisioning */
skboot_status_t skboot_key_secretbox_enroll(void)
{
    skboot_status_t status = kStatus_SKBOOT_Fail;

    status = skboot_hal_key_secretbox_enroll();

    return status;
}

skboot_status_t skboot_key_secretbox(const uint8_t *key, size_t keySize, skboot_key_metadata_t *keyMetadata)
{
    skboot_status_t status = kStatus_SKBOOT_Fail;

    switch (keyMetadata->keyType)
    {
        case kPUF_KeyType_FirmwareUpdateKey0:
            /* FirmwareUpdateKey0 store as secret key */
            status = PUF_SetUserKey(PUF, kPUF_KeyIndex_00, key, keySize, s_keyStore->Image_Key_Code.keyCode.b,
                                    sizeof(s_keyStore->Image_Key_Code.keyCode.b));
            break;
#ifdef SKBOOT_KEY_MANAGEMENT_HAS_ENCRYPTED_REGION_KEYS
        case kPUF_KeyType_EncryptedRegionKey0:
            /* PRINCE region 0 key store as secret key */
            status = PUF_SetIntrinsicKey(PUF, kPUF_KeyIndex_00, keySize, s_keyStore->encryptedRegion0.keyCode.b,
                                         sizeof(s_keyStore->encryptedRegion0.keyCode.b));
            if (status == 0)
            {
                s_keyStore->encryptedRegion0.type = kKEY_SECRETBOX_KeyCodeValid;
            }
            break;

        case kPUF_KeyType_EncryptedRegionKey1:
            /* PRINCE region 1 key store as secret key */
            status = PUF_SetIntrinsicKey(PUF, kPUF_KeyIndex_00, keySize, s_keyStore->encryptedRegion1.keyCode.b,
                                         sizeof(s_keyStore->encryptedRegion1.keyCode.b));
            if (status == 0)
            {
                s_keyStore->encryptedRegion1.type = kKEY_SECRETBOX_KeyCodeValid;
            }
            break;

        case kPUF_KeyType_EncryptedRegionKey2:
            /* PRINCE region 2 key store as secret key */
            status = PUF_SetIntrinsicKey(PUF, kPUF_KeyIndex_00, keySize, s_keyStore->encryptedRegion2.keyCode.b,
                                         sizeof(s_keyStore->encryptedRegion2.keyCode.b));
            if (status == 0)
            {
                s_keyStore->encryptedRegion2.type = kKEY_SECRETBOX_KeyCodeValid;
            }
            break;
#endif /* SKBOOT_KEY_MANAGEMENT_HAS_ENCRYPTED_REGION_KEYS */

        case kPUF_KeyType_UDS:
            /* key == NULL means SET INTRINSIC KEY command */
            if (key)
            {
                status = PUF_SetUserKey(PUF, kPUF_KeyIndex_15, key, keySize, s_keyStore->UDS_Key_Code.keyCode.b,
                                        sizeof(s_keyStore->UDS_Key_Code.keyCode.b));
            }
            else
            {
                /* UDS store as key index register 15 */
                status = PUF_SetIntrinsicKey(PUF, kPUF_KeyIndex_15, keySize, s_keyStore->UDS_Key_Code.keyCode.b,
                                             sizeof(s_keyStore->UDS_Key_Code.keyCode.b));
            }
            break;

        case kPUF_KeyType_UserKey:
            /* User Key store as secret key */
            /* key == NULL means SET INTRINSIC KEY command */
            if (key)
            {
                status = PUF_SetUserKey(PUF, kPUF_KeyIndex_00, key, keySize, s_keyStore->FW_Update_Key_Code.keyCode.b,
                                        sizeof(s_keyStore->FW_Update_Key_Code.keyCode.b));
            }
            else
            {
                status = PUF_SetIntrinsicKey(PUF, kPUF_KeyIndex_00, keySize, s_keyStore->FW_Update_Key_Code.keyCode.b,
                                             sizeof(s_keyStore->FW_Update_Key_Code.keyCode.b));
            }
            break;

#ifdef SKBOOT_KEY_MANAGEMENT_HAS_OTFAD_KEK
        case kPUF_KeyType_OtfadKEK:
            if (key)
            {
                status = PUF_SetUserKey(PUF, kPUF_KeyIndex_00, key, keySize, s_keyStore->otfadkek.keyCode.b,
                                        sizeof(s_keyStore->otfadkek.keyCode.b));
            }
            else
            {
                /* Otfad KEK key store as secret key */
                status = PUF_SetIntrinsicKey(PUF, kPUF_KeyIndex_00, keySize, s_keyStore->otfadkek.keyCode.b,
                                             sizeof(s_keyStore->otfadkek.keyCode.b));
            }
            if (status == 0)
            {
                s_keyStore->otfadkek.type = kKEY_SECRETBOX_KeyCodeValid;
            }
            break;
#endif /* SKBOOT_KEY_MANAGEMENT_HAS_OTFAD_KEK */
        default:
            status = kStatus_SKBOOT_Fail;
            break;
    }

    return status;
}

/* Key use */
skboot_status_t skboot_key_secretbox_start(void)
{
    skboot_status_t status = kStatus_SKBOOT_Fail;

    status = skboot_hal_key_secretbox_start();

    return status;
}

skboot_status_t skboot_key_secretbox_open_secret(skboot_key_metadata_t *keyMetadata, uint32_t keyMask)
{
    skboot_status_t status = kStatus_SKBOOT_Fail;

    status = skboot_key_secretbox_start();
    if (status != 0)
    {
        return status;
    }

    switch (keyMetadata->keyType)
    {
        case kPUF_KeyType_FirmwareUpdateKey0:
        case kPUF_KeyType_UserKey:
#ifdef SKBOOT_KEY_MANAGEMENT_HAS_OTFAD_KEK
        case kPUF_KeyType_OtfadKEK:
#endif /* SKBOOT_KEY_MANAGEMENT_HAS_OTFAD_KEK */
#ifdef SKBOOT_KEY_MANAGEMENT_HAS_ENCRYPTED_REGION_KEYS
        case kPUF_KeyType_EncryptedRegionKey0:
        case kPUF_KeyType_EncryptedRegionKey1:
        case kPUF_KeyType_EncryptedRegionKey2:
#endif /* SKBOOT_KEY_MANAGEMENT_HAS_ENCRYPTED_REGION_KEYS */
            /* SBKEK and User key store as secret key */
            status = skboot_hal_key_secretbox_open_secret(keyMetadata, keyMask);
            break;

        default:
            status = kStatus_SKBOOT_Fail;
            break;
    }

    return status;
}

skboot_status_t skboot_key_secretbox_open(uint8_t *key, size_t keySize, skboot_key_metadata_t *keyMetadata)
{
    skboot_status_t status = kStatus_SKBOOT_Fail;

    status = skboot_key_secretbox_start();
    if (status != 0)
    {
        return status;
    }

    switch (keyMetadata->keyType)
    {
        case kPUF_KeyType_UDS:
            /* UDS store as key index register 15 */
            status = PUF_GetKey(PUF, s_keyStore->UDS_Key_Code.keyCode.b, sizeof(s_keyStore->UDS_Key_Code.keyCode.b), key, keySize);
            break;

        default:
            status = kStatus_SKBOOT_Fail;
            break;
    }

    return status;
}
#endif /* SKBOOT_KEY_MANAGEMENT_USE_PUF */
