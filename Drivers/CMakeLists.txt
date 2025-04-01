cmake_minimum_required(VERSION 3.22)

project(drivers LANGUAGES C ASM)

set(Defines_Syms
    USE_HAL_DRIVER
    STM32H743xx
)

set(Include_Dirs
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/User_Drivers/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include
)

set(Drivers_Src
    ${CMAKE_SOURCE_DIR}/Core/Src/system_stm32h7xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User_Drivers/Src/lcd_fonts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User_Drivers/Src/lcd_image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User_Drivers/Src/lcd_pwm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User_Drivers/Src/touch_800x480.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User_Drivers/Src/touch_iic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nand.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
)

add_library(drivers OBJECT)

target_compile_definitions(drivers PRIVATE ${Defines_Syms})

target_include_directories(drivers PRIVATE ${Include_Dirs})

target_sources(drivers PRIVATE ${Drivers_Src})
