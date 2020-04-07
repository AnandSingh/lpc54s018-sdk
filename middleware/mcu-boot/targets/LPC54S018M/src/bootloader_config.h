/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BOOTLOADER_CONFIG_H__
#define __BOOTLOADER_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//
// Bootloader configuration options
//

//==============================================================================
//! @name Device type configuration macros
//@{
#if !defined(BL_DEVICE_IS_LPC_SERIES)
#define BL_DEVICE_IS_LPC_SERIES (1)
#endif
//@}

//==============================================================================
//! @name Target-resident configuration macros
//@{
#if !defined(BL_TARGET_FLASH)
#define BL_TARGET_FLASH (0)
#endif
//@}

//==============================================================================
//! @name Boot configuration macros
//@{
// Defined application vector table address in FLASH
// The bootloader will check this address for the application vector table upon startup.
#if !defined(BL_APP_VECTOR_TABLE_ADDRESS)
#define BL_APP_VECTOR_TABLE_ADDRESS 0x8000
#endif

// Defines bootloader peripheral detection default timeout in milliseconds
// After coming out of reset the bootloader will spin in a peripheral detection
// loop for this amount of time. A zero value means no time out.
#if DEBUG
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 0
#else
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000
#endif // DEBUG
//@}

//! @name Peripheral configuration macros
//@{
// Defines uart peripheral instance
#if !defined(BL_CONFIG_FLEXCOMM_USART_0)
#define BL_CONFIG_FLEXCOMM_USART_0 (1)
#endif
#define BL_CONFIG_FLEXCOMM_USART BL_CONFIG_FLEXCOMM_USART_0

// Defines i2c peripheral instance
#if !defined(BL_CONFIG_FLEXCOMM_I2C_2)
#define BL_CONFIG_FLEXCOMM_I2C_2 (1)
#endif
#define BL_CONFIG_FLEXCOMM_I2C BL_CONFIG_FLEXCOMM_I2C_2

// Defines spi peripheral instance
#if !defined(BL_CONFIG_FLEXCOMM_SPI_9)
#define BL_CONFIG_FLEXCOMM_SPI_9 (1)
#endif
#define BL_CONFIG_FLEXCOMM_SPI BL_CONFIG_FLEXCOMM_SPI_9

// Defines usb peripheral instance
#if !defined(BL_CONFIG_USB_HID)
#define BL_CONFIG_USB_HID (0)
#endif
#if !defined(BL_CONFIG_HS_USB_HID)
#define BL_CONFIG_HS_USB_HID (1)
#endif

// Determines optimization for peripheral driver
#define BL_I2C_SIZE_OPTIMIZE 1
#define BL_I2C_USED_INSTANCE 0

// Determines the method of UART autobaud (polling/irq)
#define BL_FEATURE_UART_AUTOBAUD_IRQ (1)

#if BL_CONFIG_HS_USB_HID
#define BL_FEATURE_EXPAND_PACKET_SIZE (1)
// Make sure that BL_EXPANDED_USB_HID_PACKET_SIZE < 1018
#define BL_EXPANDED_USB_HID_PACKET_SIZE (1012)
#endif
//@}

//==============================================================================
//! @name Commmand configuration macros
//@{
#if !defined(BL_FEATURE_MIN_PROFILE)
#define BL_FEATURE_MIN_PROFILE (0)
#endif

#define BL_FEATURE_READ_MEMORY (1)

// Determines whether to support flash-read-resource command
#define BL_FEATURE_HAS_NO_READ_SOURCE (1)
// Determines whether to support flash-erase-all-unsecure command
#define BL_FEATURE_ERASEALL_UNSECURE (0)
// Determines whether to support flash-security-disable command
#define BL_FEATURE_FLASH_SECURITY  (0)
// Determines whetehr to support key-provisioning command.
#define BL_FEATURE_KEY_PROVISIONING (1)
// Determines whetehr to support program-aeskey command.
#define BL_FEATURE_AES_OTP (1)
//@}

//==============================================================================
//! @name Internal flash configuration macros
//@{
// Determines whether to disable HW verification after every flash program/erase operation
#define BL_FEATURE_FLASH_VERIFY_DISABLE (0)
// (Kinetis only) Determines the method of flash programming (word/section)
#define BL_FEATURE_ENABLE_FLASH_PROGRAM_SECTION (0)
// Determines whether to support internal flash
#define BL_FEATURE_HAS_NO_INTERNAL_FLASH (1)
// Determines whether to support secondary internal flash
#define BL_HAS_SECONDARY_INTERNAL_FLASH (0)
// Determines whether to support cumulative check on flash
#define BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE (1)
//@}

#define FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES (0)

//==============================================================================
//! @name External flash configuration macros
//@{
// Determines whether can get external memory property
#define BL_FEATURE_EXTERNAL_MEMORY_PROPERTY (1)
//@}
//==============================================================================
//! @name SPIFI flash configuration macros
//@{
// Support SPIFI NOR flash
#define BL_FEATURE_SPIFI_NOR_MODULE            (1)
#if BL_FEATURE_SPIFI_NOR_MODULE
#define BL_CONFIG_SPIFI SPIFI0
#define SPIFI_NOR_ERASE_VERIFY (1)
#endif //BL_FEATURE_SPIFI_NOR_MODULE
//@}

#define FSL_FEATURE_SIM_HAS_NO_UID (1)
#define FSL_FEATURE_SIM_HAS_NO_SDID (1)

//==============================================================================
//! @name Reliability configuration macros
//@{
// Determines whether to support application integrity check
#define BL_FEATURE_CRC_CHECK (1)
//@}

//==============================================================================
//! @name Security configuration macros
//@{
// Determines whether to support AES encryption in receive-sb-file function
#define BL_FEATURE_ENCRYPTION (0)
//@}

//==============================================================================
//! @name Misc configuration macros
//@{
#define BL_FEATURE_MULTICORE                 (1)
#define BL_FEATURE_MULTI_SRAM_SECTIONS       (1)

#if defined(BL_FEATURE_MULTI_SRAM_SECTIONS) && defined(BL_FEATURE_MULTICORE)
#define BL_FEATURE_CORE0_ITCM     (1)
#define BL_FEATURE_CORE0_DTCM     (1)
#define BL_FEATURE_CORE1_FLASH    (1)
#define BL_FEATURE_CORE1_TCM      (1)
#endif
//@}

#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
