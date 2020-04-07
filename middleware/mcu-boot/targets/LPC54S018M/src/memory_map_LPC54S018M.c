/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "memory/memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for LPC54018.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note See memory.h for index requirements.
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000, 0x0002ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },            // SRAMX (192KB)
    { 0x20000000, 0x2000ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },            // SRAM0 (64KB)
    { 0x20010000, 0x20017fff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },            // SRAM1 (32KB)
    { 0x20018000, 0x2001ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },            // SRAM2 (32KB)
    { 0x20020000, 0x20027fff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },            // SRAM3 (32KB)
#if BL_FEATURE_SPIFI_NOR_MODULE
    { 0x10000000, 0x17FFFFFF, kMemoryNotExecutable | kMemoryType_FLASH, &g_spifiMemoryInterface},             // SPIFI NOR flash (128MB)
#endif
    { 0x40000000, 0x4001ffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },           // APB peripherals on APB bridge 0
    { 0x40020000, 0x4003ffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },           // APB peripherals on APB bridge 1
    { 0x40040000, 0x4005ffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },           // Asynchronous APB peripherals
    { 0x40080000, 0x400bffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },           // AHB peripherals
    { 0 }                                                                                 // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
