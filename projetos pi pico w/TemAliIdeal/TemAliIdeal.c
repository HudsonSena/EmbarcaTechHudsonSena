#include "pico/stdlib.h"       // Biblioteca padrão do SDK da Pico
#include "pico/cyw43_arch.h"   // Biblioteca para controle do Wi-Fi (cyw43)
#include "lwip/init.h"         // Inicialização da pilha LWIP
#include "lwip/ip_addr.h"      // Manipulação de endereços IP
#include "lwip/netif.h"        // Interface de rede
#include "lwip/sockets.h"      // Sockets (para TCP/UDP)
#include "lwip/dhcp.h"         // Para configurar IP automaticamente via DHCP
#include "string.h"            // Funções de string como snprintf

#define WIFI_SSID "Seu_SSID"
#define WIFI_PASS "Sua_Senha"
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_API_KEY "SUA_CHAVE_DE_API"
#define THINGSPEAK_PORT 80

// Função para conectar ao Wi-Fi
bool wifi_connect() {
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        return false;
    }
    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return false;
    }

    printf("Wi-Fi conectado!\n");
    return true;
}

// Função para enviar dados ao ThingSpeak
void send_data_to_thingspeak(float temperatura, float umidade) {
    int sock;
    struct sockaddr_in server_addr;
    char request[256];

    // Criar socket TCP
    sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Erro ao criar socket\n");
        return;
    }

    // Configurar servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(THINGSPEAK_PORT);
    server_addr.sin_addr.s_addr = inet_addr("34.117.59.81"); // IP do api.thingspeak.com

    // Conectar ao servidor
    if (lwip_connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erro ao conectar ao ThingSpeak\n");
        lwip_close(sock);
        return;
    }

    // Criar requisição HTTP GET
    snprintf(request, sizeof(request),
             "GET /update?api_key=%s&field1=%.2f&field2=%.2f HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             THINGSPEAK_API_KEY, temperatura, umidade, THINGSPEAK_HOST);

    // Enviar requisição
    lwip_send(sock, request, strlen(request), 0);

    // Fechar conexão
    lwip_close(sock);
    printf("Dados enviados: Temperatura=%.2f°C, Umidade=%.2f%%\n", temperatura, umidade);
}

int main() {
    stdio_init_all();
    sleep_ms(5000);

    if (!wifi_connect()) {
        return 1;
    }

    while (1) {
        float temperatura = 22.5; // Simulando leitura
        float umidade = 55.3;

        send_data_to_thingspeak(temperatura, umidade);
        sleep_ms(15000); // Enviar a cada 15 segundos
    }

    return 0;
}