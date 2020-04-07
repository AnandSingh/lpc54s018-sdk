INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -g")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC54S018M")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_IS_ARM_CORTEX_M4=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_USE_DEDICATED_RAM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBL_TARGET_RAM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_BM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_NONE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC54S018J4MET180")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -g")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/include)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/crc)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/smc)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src)

include_directories(${ProjDirPath}/../../../../../devices/LPC54S018M)

include_directories(${ProjDirPath}/../../../../../devices/LPC54S018M/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/autobaud)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpc_gpio)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/usb/device)

include_directories(${ProjDirPath}/../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../../../components/osa)

include_directories(${ProjDirPath}/../../../../../components/lists)

add_executable(flashloader.elf 
"${ProjDirPath}/../../../../../devices/LPC54S018M/LPC54S018M.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/LPC54S018M_features.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_crc.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_crc.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_otp.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_puf.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_puf.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/system_LPC54S018M.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/crt0_gcc.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/startup/gcc/startup_LPC54S018M.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/startup/system_LPC54S018M.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/bootloader_hid_report_ids.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/composite.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/composite.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/fat_directory_entry.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/hid_bootloader.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/hid_bootloader.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/msc_disk.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/msc_disk.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/usb_descriptor.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/usb_descriptor.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/autobaud/autobaud.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/autobaud/src/autobaud_irq.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_app_crc_check.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_command.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_context.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_irq_common.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_peripheral.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_peripheral_interface.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_shutdown_cleanup.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_user_entry.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_version.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bootloader.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_app_crc_check.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_command.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_context.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_exception_handler.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_main.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_misc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_shutdown_cleanup.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_tree_root.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_user_entry.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/crc16.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/crc32.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/src/crc16.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/src/crc32.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/include/bootloader_common.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/device_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/device_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/normal_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/normal_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/pattern_fill.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/pattern_fill_gcc.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/sram_init.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/packet/command_packet.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/packet/serial_packet.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/packet/src/serial_packet.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/property/property.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/sbloader/sb_file_format.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/sbloader/sbloader.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/sbloader/src/sbloader.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/ar_asm_macros.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/fsl_assert.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/fsl_rtos_abstraction.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/debug_util.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/fsl_assert.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/fsl_rtos_abstraction.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/vector_table_info.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/gint/fsl_gint.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/gint/fsl_gint.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpc_gpio/fsl_gpio.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpc_gpio/fsl_gpio.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/pint/fsl_pint.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/pint/fsl_pint.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/smc/smc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/property/src/property_lpc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/authentication/key_store.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/authentication/key_store.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/authentication/key_store_hal.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/authentication/key_store_hal_lpc54s018.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/authentication/skboot_common.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/flexcomm_i2c_peripheral_interface.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/flexcomm_spi_peripheral_interface.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/flexcomm_usart_peripheral_interface.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/usb_hid_msc_peripheral_interface.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/spifi_nor_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/spifi_nor_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/sram_init_lpc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/bootloader_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/clock_config_LPC54S018M.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/external_memory_property_map_LPC54S018M.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/hardware_init_LPC54S018M.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/memory_map_LPC54S018M.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/peripherals_LPC54S018M.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/peripherals_pinmux.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/target_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/LPC54S018M/src/usb_device_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/common/src/bl_flexcomm_i2c_irq_config_common.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/common/src/bl_flexcomm_spi_irq_config_common.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/common/src/bl_flexcomm_usart_irq_config_common.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/common/src/pinmux_utility_lpc.c"
"${ProjDirPath}/../usb_device_hid.c"
"${ProjDirPath}/../usb_device_hid.h"
"${ProjDirPath}/../usb_device_msc.c"
"${ProjDirPath}/../usb_device_msc.h"
"${ProjDirPath}/../usb_device_msc_ufi.c"
"${ProjDirPath}/../usb_device_msc_ufi.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_lpcip3511.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_lpcip3511.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/include/bootloader_core.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/startup.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds/microseconds.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds/src/microseconds_sysclk.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/spifi/fsl_spifi.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/spifi/fsl_spifi.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/spifi_nor/spifi_nor_flash.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/spifi_nor/spifi_nor_flash.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/otp/otp.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/otp/otp.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/aes_otp/aes_otp.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/aes_otp/aes_otp.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv81mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/LPC54S018M/drivers/fsl_inputmux_connections.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_bm.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_bm.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
)


set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/LPC54S018M_ram_flashloader.ld -static")

TARGET_LINK_LIBRARIES(flashloader.elf -Wl,--start-group)
target_link_libraries(flashloader.elf optimized m)

target_link_libraries(flashloader.elf optimized c)

target_link_libraries(flashloader.elf optimized gcc)

target_link_libraries(flashloader.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../devices/LPC54S018M/gcc)

target_link_libraries(flashloader.elf optimized ${ProjDirPath}/../../../../../devices/LPC54S018M/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(flashloader.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET flashloader.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/flashloader.elf ${EXECUTABLE_OUTPUT_PATH}/flashloader.bin)

