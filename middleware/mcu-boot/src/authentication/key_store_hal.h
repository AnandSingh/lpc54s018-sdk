/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _KEY_STORE_HAL_H_
#define _KEY_STORE_HAL_H_

#include "key_store.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*******************************************************************************
 * API
 *******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Write volatile Key Store to non-volatile storage.
 *
 * @return Operation status.
 */
skboot_status_t skboot_hal_key_secretbox_write_to_nonvolatile(uint32_t memoryId);

/*!
 * @brief Read from non-volatile storage to volatile Key Store.
 *
 * @return Operation status.
 */
skboot_status_t skboot_hal_key_secretbox_read_from_nonvolatile(const void *param, void *tempBuffer);
skboot_key_store_t *skboot_hal_key_secretbox_get_address(void);

skboot_status_t skboot_hal_key_secretbox_open_secret(skboot_key_metadata_t *keyMetadata, uint32_t keyMask);
skboot_status_t skboot_hal_key_secretbox_start(void);
skboot_status_t skboot_hal_key_secretbox_enroll(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _KEY_STORE_HAL_H_ */
