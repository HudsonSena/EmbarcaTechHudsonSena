#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

// Definição dos pinos
#define VRX 26
#define VRY 27
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define SW 22
#define LED_B 13
#define LED_R 11
#define DIVIDER_PWM 16.0
#define PERIOD 4096

// Variáveis globais
extern uint16_t led_b_level, led_r_level;
extern uint slice_led_b, slice_led_r;

// Declaração das funções para exportação
void setup_joystick();
void setup_pwm_led(uint led, uint *slice, uint16_t level);
void setup();
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

#endif // JOYSTICK_H
