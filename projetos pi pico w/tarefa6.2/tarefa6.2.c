#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

struct render_area frame_area = {
    start_column: 0,
    end_column: ssd1306_width - 1,
    start_page: 0,
    end_page: ssd1306_n_pages -1
};

uint8_t ssd[ssd1306_buffer_length];

void MenuPrint1(){
    char *text[] = {
        "",
        "  x opcao 1  ",
        "",
        "    opcao 2  ",
        "",
        "    opcao 3  ",
    };
    int y = 0;
    for(uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void MenuPrint2(){
    char *text[] = {
        "",
        "    opcao 1  ",
        "",
        "  x opcao 2  ",
        "",
        "    opcao 3  ",
    };
    int y = 0;
    for(uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void MenuPrint3(){
    char *text[] = {
        "",
        "    opcao 1  ",
        "",
        "    opcao 2  ",
        "",
        "  x opcao 3  ",
    };
    int y = 0;
    for(uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

char positionJoy(){
    adc_select_input(0);
    uint adc_y_raw = adc_read();

    if (adc_y_raw > 4000){
        sleep_ms(200);
        return 'C'; 
    }else if(adc_y_raw < 100){
        sleep_ms(200);
        return 'B';
    } else{
        return 'N';
    }
}

int main(){
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    calculate_render_area_buffer_length(&frame_area);

    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    MenuPrint1();
    
    int opcao = 1;

    while (1){
        char joySelect = positionJoy();

        if(joySelect == 'C'){
            if(opcao > 0){
                opcao = opcao - 1;
            }
        } else if(joySelect == 'B'){
            if(opcao < 3){
                opcao = opcao + 1;
            }
        } else{
            set_leds(0, 0, 0);
        }

        //
        switch (opcao){
            case 1:
                MenuPrint1();
                break;

            case 2:
                MenuPrint2();
                break;
            
            case 3:
                MenuPrint3();
                break;
            
            default:
                break;
        }
    }
    
}