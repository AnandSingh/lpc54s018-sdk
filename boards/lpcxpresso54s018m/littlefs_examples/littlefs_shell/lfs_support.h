/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LFS_SUPPORT_H_
#define _LFS_SUPPORT_H_

#include "lfs.h"

#define EXAMPLE_SPIFI SPIFI0
#define EXAMPLE_SPI_BAUDRATE (96000000)

#define FLASH_W25Q

#define FLASH_SIZE 16384 /* in KB */
#define FLASH_PAGE_SIZE 256
#define FLASH_SECTOR_SIZE 4096

#define LFS_FIRST_SECTOR 2048 /* use second half of the flash */
#define LFS_SECTORS 2048

extern int lfs_get_default_config(struct lfs_config *lfsc);
extern int lfs_storage_init(const struct lfs_config *lfsc);

#endif
