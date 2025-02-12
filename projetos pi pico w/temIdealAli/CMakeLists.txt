# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(temIdealAli C CXX ASM)

set(LWIP_DIR "${PICO_SDK_PATH}/lib/lwip")
set(LWIP_DIR "${PICO_SDK_PATH}/lib/lwip_tcp")

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(temIdealAli temIdealAli.c )

pico_set_program_name(temIdealAli "temIdealAli")
pico_set_program_version(temIdealAli "0.1")

# Generate PIO header
pico_generate_pio_header(temIdealAli ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(temIdealAli 0)
pico_enable_stdio_usb(temIdealAli 1)


# Add the standard library to the build
target_link_libraries(temIdealAli
        pico_stdlib
        pico_lwip)

# Add the standard include files to the build
target_include_directories(temIdealAli PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(temIdealAli 
        hardware_spi
        hardware_i2c
        hardware_pio
        hardware_clocks
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        )

pico_add_extra_outputs(temIdealAli)

# Configure as opções de compilação do LWIP, caso necessário
target_compile_definitions(temIdealAli PRIVATE
    PICO_USE_LWIP
)

