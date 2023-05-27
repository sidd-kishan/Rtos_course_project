cmake_minimum_required(VERSION 3.19)

# initialize the SDK based on PICO_SDK_PATH
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
project(Rtos_course_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Pico SDK
pico_sdk_init()

add_executable(main src/main.c)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(main pico_stdlib hardware_adc FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)

pico_enable_stdio_usb(main 1)

pico_add_extra_outputs(main)
