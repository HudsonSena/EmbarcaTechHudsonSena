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

project(mentoria04_02_2025 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(mentoria04_02_2025 mentoria04_02_2025.c inc/ssd1306_i2c.c Joystick_led/joystick.c buzzer/buzzer.c led_pwm/led_pwm.c)

pico_set_program_name(mentoria04_02_2025 "mentoria04_02_2025")
pico_set_program_version(mentoria04_02_2025 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(mentoria04_02_2025 0)
pico_enable_stdio_usb(mentoria04_02_2025 0)

# Add the standard library to the build
target_link_libraries(mentoria04_02_2025
        pico_stdlib
        hardware_i2c
        hardware_adc
        hardware_clocks
        hardware_pwm)

# Add the standard include files to the build
target_include_directories(mentoria04_02_2025 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(mentoria04_02_2025 
        hardware_i2c
        hardware_adc
        hardware_clocks
        hardware_pwm        
        )

pico_add_extra_outputs(mentoria04_02_2025)

