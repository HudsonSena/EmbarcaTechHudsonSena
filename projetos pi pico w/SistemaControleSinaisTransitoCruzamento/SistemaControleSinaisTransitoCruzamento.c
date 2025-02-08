// Bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

// Configuração dos pinos
#define LEDG1 11 // Led verde carros" ver
#define LEDY 12 // Led amarelo
#define LEDR 13 // Led vermelho
#define LEDG2 11 // Led verde pedestre
#define BUZZER_PIN 21 // Buzzer/Sinalização sonora
#define BUTTON_PIN 5 // Botão/botoeira

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 1000

// Função para ajustar o pino e frequência do buzzer
void pwm_init_buzzer(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
  pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(pin, 0);
}

// Função para aplicar o pino, tempo de operção e tempo de pausa do Buzzer
void beep(uint pin, uint duration_ms, uint pause_ms) {
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_set_gpio_level(pin, 1024);
  sleep_ms(duration_ms);
  pwm_set_gpio_level(pin, 0);
  sleep_ms(pause_ms);
}

// Função para variar as cores do semáforo conforme acionados
void led_loop_put(bool r, bool g, bool y) {
  gpio_put(LEDR, r);
  gpio_put(LEDG1, g);
  gpio_put(LEDY, y);
}

// Função para manter o botão como prioridade
void modo_alerta() {
  led_loop_put(true, true, false); // Luz amarela acesa
  sleep_ms(5000);
  led_loop_put(true, false, false); //Luz vermelha
  led_loop_put(false, false, false);
  gpio_put(LEDY, true); // Luz verde 2 acesa
  for (int i = 0; i < 15; i++) {
    beep(BUZZER_PIN, 900, 100); // Beep a cada 900ms com pausa de 100ms
  }
  gpio_put(LEDG2, false); // Apaga a luz verde após o beep terminar
  led_loop_put(false, false, false);
}

// Função do funcionamento do semáforo, com funções especificas para o monitoramento do botão
void modo_normal() {
  // Dividido em etapas para permitir interrupção no meio
  led_loop_put(false, true, false); // Verde
  for (int i = 0; i < 80; i++) { // 8000ms dividido em 100ms intervalos
    sleep_ms(100);
    if (gpio_get(BUTTON_PIN) == 0) return; // Sai se o botão for pressionado
  }
  
  led_loop_put(true, true, false); // Amarelo
  for (int i = 0; i < 20; i++) { // 2000ms dividido em 100ms intervalos
    sleep_ms(100);
    if (gpio_get(BUTTON_PIN) == 0) return; // Sai se o botão for pressionado
  }
  
  led_loop_put(true, false, false); // Vermelho
  for (int i = 0; i < 100; i++) { // 10000ms dividido em 100ms intervalos
    sleep_ms(100);
    if (gpio_get(BUTTON_PIN) == 0) return; // Sai se o botão for pressionado
  }
}

// Função principal, para inicializar a aplicação
int main() {
  stdio_init_all();
  pwm_init_buzzer(BUZZER_PIN);

  gpio_init(BUTTON_PIN); // Iniciando o pino do botão
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN); // Ativando o resistor interno

// Iniciando os leds
  gpio_init(LEDG2);
  gpio_init(LEDY);
  gpio_init(LEDG1);
  gpio_init(LEDR);
  gpio_set_dir(LEDG2, true);
  gpio_set_dir(LEDY, true);
  gpio_set_dir(LEDG1, true);
  gpio_set_dir(LEDR, true);

// Função de loop para variar cores e ativar o botão
  while (true) {
    if (gpio_get(BUTTON_PIN) == 0) {
      modo_alerta();
    } else {
      modo_normal();
    }
  }
  return 0;
}