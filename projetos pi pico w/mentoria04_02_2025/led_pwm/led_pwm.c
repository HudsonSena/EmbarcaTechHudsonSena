#include "led_pwm.h"

uint16_t led_level = 100;

void setup_pwm()
{
    uint slice;
    gpio_set_function(LED, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(LED);
    pwm_set_clkdiv(slice, DIVIDER_PWM);
    pwm_set_wrap(slice, PERIOD);
    pwm_set_gpio_level(LED, led_level);
    pwm_set_enabled(slice, true);
}

void adjust_led_brightness(uint *up_down)
{
    if (*up_down)
    {
        led_level += LED_STEP;
        if (led_level >= PERIOD)
            *up_down = 0;
    }
    else
    {
        led_level -= LED_STEP;
        if (led_level <= LED_STEP)
            *up_down = 1;
    }
}
