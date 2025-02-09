#ifndef LED_PWM_H
#define LED_PWM_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições de pino e parâmetros do PWM
#define LED 12            // Pino do LED conectado
#define PERIOD 2000      // Período do PWM (valor máximo do contador)
#define DIVIDER_PWM 16.0 // Divisor fracional do clock para o PWM
#define LED_STEP 100     // Passo de incremento/decremento para o duty cycle do LED

extern uint16_t led_level; // Nível inicial do PWM (duty cycle)

void setup_pwm();
void adjust_led_brightness(uint up_down);

#endif // LED_PWM_H