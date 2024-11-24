add_library(LIS3DH-Pico INTERFACE)

target_sources(LIS3DH-Pico INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/lis3dh.cpp
)

target_link_libraries(LIS3DH-Pico INTERFACE
    pico_stdlib
    hardware_i2c
)
