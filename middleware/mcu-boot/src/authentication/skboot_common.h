/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef _SKBOOT_COMMON_H_
#define _SKBOOT_COMMON_H_

#define SECURE_FALSE (0x5aa55aa5U)
#define SECURE_TRACKER_VERIFIED (0x55aacc33U)

#if defined __ICCARM__ || defined __GNUC__ || __CC_ARM
#include <stddef.h>
#include "fsl_device_registers.h"

typedef enum _skboot_status {
    kStatus_SKBOOT_Success = 0x5ac3c35au,
    kStatus_SKBOOT_Fail = 0xc35ac35au,
    kStatus_SKBOOT_InvalidArgument = 0xc35a5ac3u,
    kStatus_SKBOOT_KeyStoreMarkerInvalid = 0xc3c35a5au,
} skboot_status_t;

typedef enum _secure_termination {
    kTERM_PART_LOCK = 0x5ac3c35au,
    kTERM_PART_OPEN = 0xc35ac35au,
    kTERM_SECURE_PART_OPEN = 0xc35a5ac3u,
    kTERM_FA_PART_OPEN = 0xc3c35a5au,
} secure_termination_t;

typedef enum _secure_bool {
    kSECURE_TRUE = 0xc33cc33cU,
    kSECURE_FALSE = SECURE_FALSE,
    kSECURE_CALLPROTECT_SECURITY_FLAGS = 0xc33c5aa5U,
    kSECURE_CALLPROTECT_IS_APP_READY = 0x5aa5c33cU,
    kSECURE_TRACKER_VERIFIED = SECURE_TRACKER_VERIFIED,
} secure_bool_t;


static inline uint32_t skboot_addr_to_nonsecure_tzm_addr(uint32_t addr)
{
    /* Remove bit A28 to force all data accesses through non-secure alias */
    return (addr & 0xEFFFFFFFu);
}

#endif /* __ICCARM__ || defined __GNUC__ || __CC_ARM */
#endif /* _SKBOOT_COMMON_H_ */
