#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "sockets/sockets.h"

// Defina os parâmetros da rede Wi-Fi
#define WIFI_SSID "REDE"
#define WIFI_PASSWORD "SENHA"

// Defina os parâmetros do ThingSpeak
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80
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
    // Cria o payload HTTP para enviar os dados ao ThingSpeak
    char payload[256];
    snprintf(payload, sizeof(payload),
             "GET /update?api_key=%s&field1=%.2f&field2=%.2f HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "\r\n",
             THINGSPEAK_API_KEY, temperature, humidity, THINGSPEAK_HOST);

    printf("Enviando dados para o ThingSpeak:\n%s\n", payload);

    // Configura o socket TCP
    struct sockaddr_in server_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro ao criar o socket\n");
        return;
    }

    // Resolve o endereço do servidor ThingSpeak
    struct hostent *server = gethostbyname(THINGSPEAK_HOST);
    if (server == NULL) {
        printf("Erro ao resolver o host\n");
        close(sockfd);
        return;
    }

    // Preenche a estrutura do endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(THINGSPEAK_PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Conecta ao servidor ThingSpeak
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erro ao conectar ao servidor\n");
        close(sockfd);
        return;
    }

    // Envia os dados via socket
    if (send(sockfd, payload, strlen(payload), 0) < 0) {
        printf("Erro ao enviar dados\n");
    } else {
        printf("Dados enviados com sucesso\n");
    }

    // Fecha o socket
    close(sockfd);
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