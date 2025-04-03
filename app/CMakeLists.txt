cmake_minimum_required(VERSION 3.22)

project(app LANGUAGES C ASM)

set(Defines_Syms
    $<$<CONFIG:Debug>:DEBUG>
)

set(App_Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/lvgl-8.3.10
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/music
    ${CMAKE_CURRENT_SOURCE_DIR}/music/assets
)

set(App_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/music/lv_demo_music_list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/music/lv_demo_music_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/music/lv_demo_music.c
)

add_library(app INTERFACE)

target_compile_definitions(app INTERFACE ${Defines_Syms})

target_include_directories(app INTERFACE ${App_Inc})

target_sources(app INTERFACE ${App_Src})