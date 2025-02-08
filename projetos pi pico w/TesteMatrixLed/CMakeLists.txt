cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Inicializar o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(TesteMatrixLed C CXX ASM)

pico_sdk_init()

# Adicionar o executável
add_executable(TesteMatrixLed TesteMatrixLed.c)

# Gerar o cabeçalho PIO
pico_generate_pio_header(TesteMatrixLed ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Linkar bibliotecas necessárias
target_link_libraries(TesteMatrixLed
    pico_stdlib
    hardware_pio
    hardware_clocks
)

# Ativar saída extra
pico_add_extra_outputs(TesteMatrixLed)
