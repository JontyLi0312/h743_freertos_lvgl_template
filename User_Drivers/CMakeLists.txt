cmake_minimum_required(VERSION 3.12)

project(user_drivers LANGUAGES C ASM)

set(Defines_Syms
    $<$<CONFIG:Debug>:DEBUG>
)

set(User_Drivers_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
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

target_inlcude_directories(user_drivers INTERFACE ${User_Drivers_Inc})

target_sources(user_drivers INTERFACE ${User_Drivers_Src})