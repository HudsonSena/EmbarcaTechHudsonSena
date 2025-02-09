#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Definição do pino do buzzer
#define BUZZER_PIN 21

// Notas musicais para a música tema de Star Wars
extern const uint star_wars_notes[];
extern const uint note_duration[];

// Função para inicializar o PWM do buzzer
void pwm_init_buzzer(uint pin);

// Função para tocar uma nota com frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms);

// Função para tocar a música Star Wars
void play_star_wars(uint pin, bool *in_loop);

#endif // BUZZER_H
