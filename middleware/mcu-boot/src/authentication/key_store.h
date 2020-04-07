/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _KEY_STORE_H_
#define _KEY_STORE_H_

#include <stddef.h>
#include <stdint.h>
#include "skboot_common.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/* PUF based Key Store */
#define SKBOOT_KEY_MANAGEMENT_USE_PUF

#ifdef SKBOOT_KEY_MANAGEMENT_USE_PUF
#include "fsl_puf.h"

typedef puf_key_index_register_t skboot_key_index_register_t;

typedef enum _skboot_key_type {
    kPUF_KeyType_Invalid = 0x0u,
    kPUF_KeyType_FirmwareUpdateKey0 = 0x01u,
    kPUF_KeyType_UDS = 0x02u,
    kPUF_KeyType_UserKey = 0x03u,
} skboot_key_type_t;

typedef struct _skboot_key_metadata
{
    skboot_key_type_t keyType;
    skboot_key_index_register_t keyIndexRegister;
} skboot_key_metadata_t;
#endif /* SKBOOT_KEY_MANAGEMENT_USE_PUF */

typedef struct _key_item
{
    union _key_code {
        uint32_t w[52 / sizeof(uint32_t)];
        uint8_t b[52];
    } keyCode;
} skboot_key_code_item;

typedef struct _key_store
{
    union _activation_code {
        uint32_t w[1192 / sizeof(uint32_t)];
        uint8_t b[1192];
    } activationCode;

    skboot_key_code_item Image_Key_Code;
    skboot_key_code_item UDS_Key_Code;
    skboot_key_code_item FW_Update_Key_Code;
} skboot_key_store_t;

typedef enum _key_item_type {
    kKEY_SECRETBOX_KeyCodeValid = 0x59595959u,
    kKEY_SECRETBOX_ActivationCodeValid = 0x95959595u,
} skboot_key_code_item_type_t;

/*******************************************************************************
 * API
 *******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

skboot_status_t skboot_key_secretbox_enroll(void);
skboot_status_t skboot_key_secretbox_start(void);

skboot_status_t skboot_key_secretbox(const uint8_t *key, size_t keySize, skboot_key_metadata_t *keyMetadata);
skboot_status_t skboot_key_secretbox_open_secret(skboot_key_metadata_t *keyMetadata, uint32_t keyMask);
skboot_status_t skboot_key_secretbox_open(uint8_t *key, size_t keySize, skboot_key_metadata_t *keyMetadata);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _KEY_STORE_H_ */
