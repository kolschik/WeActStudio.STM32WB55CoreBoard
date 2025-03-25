cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME 01-BLE_Test)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Src/bsp.c
    Core/Src/spi.c
    stm_lib/tmr/tmr.c   
    stm_lib/adc/adc.c       
    u8g2/csrc/u8x8_setup.c
    u8g2/csrc/u8x8_display.c
    u8g2/csrc/u8g2_setup.c
    u8g2/csrc/u8g2_d_setup.c
    u8g2/csrc/u8g2_ll_hvline.c
    u8g2/csrc/u8x8_cad.c
    u8g2/csrc/u8g2_font.c
    u8g2/csrc/u8g2_arc.c
    u8g2/csrc/u8g2_hvline.c
    u8g2/csrc/u8g2_d_memory.c
    u8g2/csrc/u8x8_byte.c
    u8g2/csrc/u8g2_intersection.c
    u8g2/csrc/u8x8_d_ssd1305.c
    u8g2/csrc/u8x8_gpio.c
    u8g2/csrc/u8g2_circle.c
    u8g2/csrc/u8g2_buffer.c
    u8g2/csrc/u8g2_fonts.c
    u8g2/csrc/u8x8_8x8.c

    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_ll_tim.c
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    u8g2/csrc  
    stm_lib/tmr    
    stm_lib/adc     
    Drivers/STM32WBxx_HAL_Driver/Inc
    .
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    -DSTM32WB
    -DUSE_FULL_LL_DRIVER
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
