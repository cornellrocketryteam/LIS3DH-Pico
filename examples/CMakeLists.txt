cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(accel C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(accel
    poll_data.cpp
    ../lis3dh.cpp
)
# Pull in common dependencies
target_link_libraries(accel pico_stdlib hardware_i2c)

pico_enable_stdio_usb(accel 1)
pico_enable_stdio_uart(accel 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(accel)