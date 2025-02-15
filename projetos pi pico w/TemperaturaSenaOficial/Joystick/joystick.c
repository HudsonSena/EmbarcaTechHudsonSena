#include "joystick.h"

// Inicialização das variáveis globais
uint16_t led_b_level = 100, led_r_level = 100;
uint slice_led_b, slice_led_r;

// Configuração do joystick
void setup_joystick()
{
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

// Configuração do PWM para um LED
void setup_pwm_led(uint led, uint *slice, uint16_t level)
{
    gpio_set_function(led, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_wrap(*slice, PERIOD);
    pwm_set_gpio_level(led, level);
    pwm_set_enabled(*slice, true);
}

// Configuração geral
void setup()
{
    setup_pwm_led(LED_B, &slice_led_b, led_b_level);
    setup_pwm_led(LED_R, &slice_led_r, led_r_level);
}

// Leitura do joystick
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
    adc_select_input(ADC_CHANNEL_0);
    sleep_us(2);
    *vrx_value = adc_read();

    adc_select_input(ADC_CHANNEL_1);
    sleep_us(2);
    *vry_value = adc_read();
}
