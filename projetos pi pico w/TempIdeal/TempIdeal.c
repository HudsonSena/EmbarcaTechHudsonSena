#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "http_client/http_client.h"
#include "lwip/apps/http_client.h"
#include <string.h>

// Defina os parâmetros da rede Wi-Fi
#define WIFI_SSID "REDE"
#define WIFI_PASSWORD "SENHA"

// Defina os parâmetros do ThingSpeak
#define THINGSPEAK_API_KEY "SUA_CHAVE_API"
#define THINGSPEAK_CHANNEL_ID "SEU_CHANNEL_ID"

bool wifi_connect() {
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar o Wi-Fi\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return false;
    }

    printf("Conectado ao Wi-Fi: %s\n", WIFI_SSID);
    printf("IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    return true;
}

void send_to_thingspeak(float temperature, float humidity) {
    char url[256];
    snprintf(url, sizeof(url), "https://api.thingspeak.com/update?api_key=%s&field1=%.2f&field2=%.2f", THINGSPEAK_API_KEY, temperature, humidity);

    printf("Enviando dados para o ThingSpeak: %s\n", url);

    struct http_client *client = http_client_init();
    if (!client) {
        printf("Falha ao inicializar o cliente HTTP\n");
        return;
    }

    http_client_get(client, url, NULL, NULL);
    http_client_close(client);
}

// Função para simular a leitura de temperatura e umidade
void simulate_sensor_data(float *temperature, float *humidity) {
    // Simula valores de temperatura entre 20.0 e 30.0 °C
    *temperature = 20.0 + (rand() % 100) / 10.0;

    // Simula valores de umidade entre 40.0 e 80.0 %
    *humidity = 40.0 + (rand() % 400) / 10.0;
}

int main() {
    stdio_init_all();

    if (!wifi_connect()) {
        return 1;
    }

    while (true) {
        float temperature, humidity;

        // Simula a leitura dos dados do sensor
        simulate_sensor_data(&temperature, &humidity);

        printf("Temperatura simulada: %.2f °C\n", temperature);
        printf("Umidade simulada: %.2f %%\n", humidity);

        // Envia os dados simulados para o ThingSpeak
        send_to_thingspeak(temperature, humidity);

        sleep_ms(20000); // Espera 20 segundos
    }

    return 0;
}