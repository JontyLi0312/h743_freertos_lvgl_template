cmake_minimum_required(VERSION 3.22)

project(user_drivers LANGUAGES C ASM)

set(Defines_Syms
    USE_HAL_DRIVER
    STM32H743xx
    USE_PWR_LDO_SUPPLY
    $<$<CONFIG:Debug>:DEBUG>
)

set(User_Drivers_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
)

set(User_Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_fonts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_pwm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/touch_800x480.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/touch_iic.c
)

add_library(user_drivers INTERFACE)

target_compile_definitions(user_drivers INTERFACE ${Defines_Syms})

target_include_directories(user_drivers INTERFACE ${User_Drivers_Inc})

target_sources(user_drivers INTERFACE ${User_Drivers_Src})