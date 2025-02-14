#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

int main()
{
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

// Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
 
    char *text[] = {
        "Boa",
        "",
        "",
        "Noite",
        "Hudson",
        "Diego"};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
 

// Parte do código para exibir a linha no display (algoritmo de Bresenham)
/**
    ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
    render_on_display(ssd, &frame_area);
*/




// Parte do código para exibir o bitmap no display
/*

    const uint8_t bitmap_128x64[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xf5, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xf5, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfd, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xdb, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x25, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf2, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xef, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xf5, 0xbf, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfb, 0x82, 0x7f, 0xff, 0xff, 0x5f, 0xff, 0xff, 0xf5, 0x72, 0xef, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xf6, 0xa5, 0xbd, 0xff, 0xbe, 0x51, 0xff, 0xff, 0xfb, 0xe1, 0x6f, 0xdf, 0x7d, 0xdf, 0xff, 
	0xff, 0xfd, 0x5a, 0xbb, 0x7f, 0xea, 0xb5, 0xff, 0xff, 0xf6, 0xb1, 0x6e, 0xed, 0x54, 0xaf, 0xff, 
	0xff, 0xfd, 0x62, 0xdb, 0xd7, 0xed, 0xbd, 0xff, 0xff, 0xff, 0x5a, 0xdd, 0xbe, 0x32, 0x23, 0xff, 
	0xff, 0xfa, 0xfa, 0xd3, 0xf5, 0xc4, 0x9f, 0xff, 0xff, 0xff, 0xd3, 0xbd, 0xaf, 0x8a, 0x5b, 0xff, 
	0xff, 0xff, 0x3c, 0x57, 0xfe, 0x50, 0xb3, 0xff, 0xff, 0xfd, 0xe3, 0xf5, 0xaa, 0xa5, 0xa7, 0xff, 
	0xff, 0xff, 0x5d, 0x57, 0xdf, 0x91, 0x2f, 0xff, 0xff, 0xfe, 0xfb, 0xfb, 0xbd, 0x45, 0x57, 0xff, 
	0xff, 0xff, 0xa3, 0xd7, 0xeb, 0x92, 0x6f, 0xff, 0xff, 0xff, 0x5d, 0x3e, 0xbd, 0x6a, 0x9f, 0xff, 
	0xff, 0xff, 0xd0, 0xf7, 0xff, 0xd5, 0x7f, 0xff, 0xff, 0xff, 0x22, 0xaf, 0xcd, 0x7e, 0x5f, 0xff, 
	0xff, 0xff, 0xc4, 0x3e, 0xab, 0xa5, 0xbf, 0xff, 0xff, 0xff, 0xa8, 0x1f, 0x87, 0x7e, 0x7f, 0xff, 
	0xff, 0xff, 0xb5, 0xfd, 0x02, 0xd5, 0xff, 0xff, 0xff, 0xff, 0x46, 0x3f, 0x05, 0xbf, 0xff, 0xff, 
	0xff, 0xff, 0xeb, 0xee, 0x5b, 0x25, 0xff, 0xff, 0xff, 0xff, 0xdc, 0xff, 0xb2, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xe3, 0xdf, 0xaf, 0xa7, 0xff, 0xff, 0xff, 0xff, 0xdd, 0xff, 0xfd, 0x6f, 0xff, 0xff, 
	0xff, 0xff, 0xe3, 0xfe, 0xeb, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xdf, 0x2f, 0xde, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xb3, 0xdf, 0xfd, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xee, 0x2d, 0xf7, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xd2, 0x07, 0xfd, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xee, 0x0b, 0xee, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xd8, 0x43, 0xfb, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xae, 0x05, 0xc6, 0xf7, 0xff, 0xff, 
	0xff, 0xff, 0xf9, 0x07, 0xfd, 0xef, 0xff, 0xff, 0xff, 0xff, 0xe6, 0x2d, 0xab, 0xbd, 0xff, 0xff, 
	0xff, 0xff, 0xec, 0x8b, 0x55, 0x7b, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x12, 0xff, 0xef, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x04, 0x1b, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xf6, 0xb6, 0xff, 0xff, 
	0xff, 0xff, 0xde, 0x5a, 0xbf, 0xef, 0xff, 0xff, 0xff, 0xff, 0xda, 0xe5, 0x57, 0xdd, 0xff, 0xff, 
	0xff, 0xff, 0xa7, 0xbf, 0xff, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x5a, 0x5d, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xc5, 0xd5, 0xf7, 0xdb, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xab, 0x5d, 0x3f, 0x7f, 0xff, 
	0xff, 0xff, 0xe2, 0x5a, 0xf6, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xdd, 0xa5, 0xbf, 0xd7, 0xff, 0xff, 
	0xff, 0xff, 0xf2, 0x56, 0x5b, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xc5, 0xad, 0xf2, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfa, 0x5a, 0xad, 0xdb, 0x7f, 0xff, 0xff, 0xff, 0xa1, 0xa5, 0x5b, 0xf7, 0xff, 0xff, 
	0xff, 0xff, 0xee, 0xaf, 0xf7, 0xdf, 0x7f, 0xff, 0xff, 0xff, 0xd9, 0x2b, 0x5e, 0xbe, 0xff, 0xff, 
	0xff, 0xff, 0xe2, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xbb, 0xfb, 0xdd, 0xff, 0xff, 
	0xff, 0xff, 0xc4, 0x5f, 0xef, 0xf7, 0x7f, 0xff, 0xff, 0xff, 0xbb, 0xf7, 0xff, 0x6e, 0xff, 0xff, 
	0xff, 0xff, 0xf4, 0xaf, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x7f, 0x7b, 0xed, 0x7f, 0xff, 
	0xff, 0xff, 0xfa, 0xeb, 0xef, 0x5a, 0xff, 0xff, 0xff, 0xff, 0xa6, 0xde, 0xfe, 0xf5, 0xff, 0xff, 
	0xff, 0xff, 0xfa, 0xbf, 0xbd, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xa4, 0x7b, 0xef, 0xb5, 0x7f, 0xff, 
	0xff, 0xff, 0xab, 0xc7, 0x7a, 0x6f, 0xff, 0xff, 0xff, 0xff, 0x58, 0x3d, 0xdf, 0xdb, 0x7f, 0xff, 
	0xff, 0xfe, 0xa5, 0xc6, 0xfd, 0x3e, 0xff, 0xff, 0xff, 0xfd, 0x3d, 0x2b, 0xda, 0xe5, 0xff, 0xff, 
	0xff, 0xff, 0x42, 0x5d, 0xb7, 0xbf, 0xff, 0xff, 0xff, 0xfe, 0x1c, 0xa2, 0x6d, 0x5f, 0xbf, 0xff, 
	0xff, 0xf8, 0xba, 0xa5, 0xdf, 0xfd, 0x7f, 0xff, 0xff, 0xff, 0x05, 0x2b, 0x7d, 0xab, 0xff, 0xff, 
	0xff, 0xfd, 0x5a, 0xc0, 0xdb, 0x7f, 0x7f, 0xff, 0xff, 0xfa, 0x25, 0x3f, 0xbe, 0xda, 0xff, 0xff, 
	0xff, 0xfc, 0x8e, 0xd1, 0x75, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x12, 0xbe, 0xae, 0xd2, 0xff, 0xff, 
	0xff, 0xfc, 0x4d, 0xd3, 0x7d, 0xbf, 0xff, 0xff, 0xff, 0xfe, 0x92, 0xbe, 0xdb, 0xf7, 0xff, 0xff, 
	0xff, 0xfd, 0x05, 0xfd, 0xb6, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x55, 0xb7, 0x6d, 0xf7, 0x7f, 0xff, 
	0xfe, 0xeb, 0x83, 0x7e, 0xb3, 0x5f, 0xff, 0xff, 0xff, 0x92, 0x10, 0xff, 0x6e, 0xef, 0xff, 0xff, 
	0xfd, 0x44, 0x45, 0xfe, 0x91, 0x3e, 0xff, 0xff, 0xfa, 0x01, 0x91, 0xfd, 0x6d, 0x5f, 0xff, 0xff, 
	0xff, 0x91, 0x22, 0xff, 0xa2, 0x5d, 0xff, 0xff, 0xfb, 0x52, 0x41, 0xbb, 0x29, 0x1f, 0xff, 0xff, 
	0xff, 0xf5, 0xa1, 0xff, 0x80, 0x37, 0xff, 0xff, 0xff, 0xff, 0xc2, 0xd7, 0x28, 0x2e, 0xbb, 0xff, 
	0xff, 0xff, 0xe3, 0xff, 0x42, 0xcf, 0xf7, 0xff, 0xff, 0xff, 0xdd, 0x35, 0xb9, 0x1e, 0xd7, 0xff, 
	0xff, 0xff, 0xf3, 0xcf, 0x7f, 0xfb, 0xbb, 0xff, 0xff, 0xff, 0xfd, 0x3a, 0xff, 0xd6, 0x67, 0xff, 
	0xff, 0xff, 0xff, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

    ssd1306_t ssd_bm;
    ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
*/

    while(true) {
        sleep_ms(1000);
    }

    return 0;
}