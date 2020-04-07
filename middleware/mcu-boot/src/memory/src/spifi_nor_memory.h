/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __SPIFI_NOR_MEMORY_H__
#define __SPIFI_NOR_MEMORY_H__

#include "fsl_common.h"

enum
{
    //! @brief Max page size used to create a page buffer
    kSpifiNorMemory_MaxPageSize = 1024U,
    //! @brief Max total size of NOR FLASH supported by SPIFI
    kSpifiNorMemory_MaxSize = 256U * 1024 * 1024,
};

typedef enum _spifi_nor_property
{
    kSpifiNorProperty_InitStatus = 0,
    kSpifiNorProperty_StartAddress = 1,           //!< Tag used to retrieve start address
    kSpifiNorProperty_TotalFlashSizeInKBytes = 2, //!< Tag used to retrieve total flash size in terms of KByte
    kSpifiNorProperty_PageSize = 3,               //!< Tag used to retrieve page size in terms of byte
    kSpifiNorProperty_SectorSize = 4,             //!< Tag used to retrieve sector size in term of byte
    kSpifiNorProperty_BlockSize = 5,              //!< Tag used to retrieve block size in terms of byte

    kSpifiNorProperty_TotalFlashSize = 0x10, //!< Tag used to retrieve total flash size in terms of byte
} spifi_nor_property_t;

//! @brief Initialize SPI NOR memory
extern status_t spifi_nor_mem_init(void);

//! @brief Read SPI NOR memory.
extern status_t spifi_nor_mem_read(uint32_t address, uint32_t length, uint8_t *buffer);

//! @brief Write SPI NOR memory.
extern status_t spifi_nor_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer);

//! @brief Fill SPI NOR memory with a word pattern.
extern status_t spifi_nor_mem_fill(uint32_t address, uint32_t length, uint32_t pattern);

//! @brief Erase SPI NOR memory
extern status_t spifi_nor_mem_erase(uint32_t address, uint32_t length);

//! @brief flush cached data to SPI NOR memory
extern status_t spifi_nor_mem_flush(void);

#endif /* __SPIFI_NOR_MEMORY_H__ */
