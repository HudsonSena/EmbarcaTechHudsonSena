#ifndef LED_PWM_H
#define LED_PWM_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED 12
#define PERIOD 2000
#define DIVIDER_PWM 16.0
#define LED_STEP 100

extern uint16_t led_level;

void setup_pwm();
void adjust_led_brightness(uint *up_down);

#endif