#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "hardware/clocks.h"

// Definições da matriz de LED
#define LED_PIN 7      // GPIO onde o WS2812 está conectado
#define LED_COUNT 25   // Número de LEDs na matriz (5x5)
#define MATRIX_WIDTH 5 // Largura da matriz
#define MATRIX_HEIGHT 5 // Altura da matriz

#define PIO_INSTANCE pio0 // Instância do PIO
#define SM 0              // Máquina de estado (State Machine) usada

// Função para inicializar o WS2812 no PIO
void ws2812_program_init(PIO pio, uint sm, uint offset, uint pin, uint freq, bool rgbw) {
    pio_sm_config c = ws2812_program_get_default_config(offset);

    // Configurar o pino de saída
    sm_config_set_sideset_pins(&c, pin);
    pio_gpio_init(pio, pin);
    gpio_set_dir(pin, GPIO_OUT);

    // Configurar o estado da máquina
    sm_config_set_out_shift(&c, true, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    // Configurar a frequência
    float clk_div = (float)clock_get_hz(clk_sys) / freq;
    sm_config_set_clkdiv(&c, clk_div);

    // Inicializar a máquina de estado
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

// Função para enviar cor a um LED específico na matriz
void set_led_color(uint32_t *matrix, uint x, uint y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT) {
        uint index = y * MATRIX_WIDTH + x;
        matrix[index] = (r << 16) | (g << 8) | b; // Formato RGB
    }
}

// Função para atualizar os LEDs enviando os dados via PIO
void update_leds(PIO pio, uint sm, uint32_t *matrix, size_t led_count) {
    for (size_t i = 0; i < led_count; i++) {
        pio_sm_put_blocking(pio, sm, matrix[i]);
    }
}

int main() {
    stdio_init_all();

    // Inicializar o PIO e o programa WS2812
    uint offset = pio_add_program(PIO_INSTANCE, &ws2812_program);
    ws2812_program_init(PIO_INSTANCE, SM, offset, LED_PIN, 800000, false);

    // Criar a matriz de LEDs (inicialmente desligados)
    uint32_t led_matrix[LED_COUNT] = {0};

    while (true) {
        // Exemplo: Acender LEDs específicos
        set_led_color(led_matrix, 0, 0, 255, 0, 0); // LED (0,0) em vermelho
        set_led_color(led_matrix, 1, 1, 0, 255, 0); // LED (1,1) em verde
        set_led_color(led_matrix, 2, 2, 0, 0, 255); // LED (2,2) em azul
        set_led_color(led_matrix, 3, 3, 255, 255, 0); // LED (3,3) em amarelo
        set_led_color(led_matrix, 4, 4, 255, 255, 255); // LED (4,4) em branco

        // Enviar os dados para os LEDs
        update_leds(PIO_INSTANCE, SM, led_matrix, LED_COUNT);

        // Pausar por 1 segundo
        sleep_ms(1000);

        // Desligar todos os LEDs
        for (int i = 0; i < LED_COUNT; i++) {
            led_matrix[i] = 0;
        }

        // Atualizar os LEDs (desligar todos)
        update_leds(PIO_INSTANCE, SM, led_matrix, LED_COUNT);

        // Pausar por 1 segundo
        sleep_ms(1000);
    }

    return 0;
}
