#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#define LED_PIN 12          // Define o pino do LED
#define WIFI_SSID "POCO X3 Pro"  // Substitua pelo nome da sua rede Wi-Fi
#define WIFI_PASS "930315dd" // Substitua pela senha da sua rede Wi-Fi

// Buffer para respostas HTTP
#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" \
                      "<!DOCTYPE html><html><body>" \
                      "<h1>Controle do LED</h1>" \
                      "<p><a href=\"/led/on\">Ligar LED</a></p>" \
                      "<p><a href=\"/led/off\">Desligar LED</a></p>" \
                      "</body></html>\r\n"

// Dados simulados (temperatura e umidade)
#define THINGSPEAK_URL "http://api.thingspeak.com/update"
#define CHANNEL_API_KEY "CCMWPYJM527SXJNM" // Substitua pela sua chave API de escrita

// Função para enviar dados simulados para o ThingSpeak no formato JSON
static void send_data_to_thingspeak(float temperatura, float umidade) {
    char post_data[256];
    
    // Formato JSON para enviar os dados
    snprintf(post_data, sizeof(post_data),
             "{\"api_key\":\"%s\",\"field1\":%.2f,\"field2\":%.2f}",
             CHANNEL_API_KEY, temperatura, umidade);

    // Configuração da conexão TCP
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar o PCB\n");
        return;
    }

    // Conectar ao ThingSpeak (porta HTTP padrão 80)
    ip_addr_t ip;
    IP4_ADDR(&ip, 184, 106, 153, 149);  // IP do ThingSpeak (api.thingspeak.com)
    
    err_t err = tcp_connect(pcb, &ip, 80, NULL);
    if (err != ERR_OK) {
        printf("Erro ao conectar ao ThingSpeak\n");
        return;
    }

    // Montar e enviar a requisição POST com formato JSON
    char request[512];
    snprintf(request, sizeof(request),
             "POST /update HTTP/1.1\r\n"
             "Host: api.thingspeak.com\r\n"
             "Connection: close\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n\r\n"
             "%s", strlen(post_data), post_data);

    // Enviar a requisição
    tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);

    // Esperar a resposta do ThingSpeak (geralmente o status 200 OK)
    tcp_poll(pcb, NULL, 1);  // Chama tcp_poll sem atribuir a nenhum valor

    // Fechar a conexão após o envio
    tcp_close(pcb);
    printf("Dados enviados para o ThingSpeak: Temp=%.2f, Hum=%.2f\n", temperatura, umidade);
}

// Função de callback para processar requisições HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Cliente fechou a conexão
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Processa a requisição HTTP
    char *request = (char *)p->payload;

    if (strstr(request, "GET /led/on")) {
        gpio_put(LED_PIN, 1);  // Liga o LED
    } else if (strstr(request, "GET /led/off")) {
        gpio_put(LED_PIN, 0);  // Desliga o LED
    }

    // Envia a resposta HTTP
    tcp_write(tpcb, HTTP_RESPONSE, strlen(HTTP_RESPONSE), TCP_WRITE_FLAG_COPY);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

// Callback de conexão: associa o http_callback à conexão
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
    return ERR_OK;
}

// Função de setup do servidor TCP
static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    // Liga o servidor na porta 80
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao ligar o servidor na porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);  // Coloca o PCB em modo de escuta
    tcp_accept(pcb, connection_callback);  // Associa o callback de conexão

    printf("Servidor HTTP rodando na porta 80...\n");
}

int main() {
    stdio_init_all();  // Inicializa a saída padrão
    sleep_ms(20000);
    printf("Iniciando servidor HTTP\n");

    // Inicializa o Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return 1;
    } else {
        printf("Conectado ao Wi-Fi!\n");
    }

    // Configura o LED como saída
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Inicia o servidor HTTP
    start_http_server();
    
    // Simula dados de temperatura e umidade
    float temperatura = 22.5 + (rand() % 100) / 50.0;
    float umidade = 55.0 + (rand() % 100) / 50.0;
    
    // Envia os dados para o ThingSpeak
    send_data_to_thingspeak(temperatura, umidade);

    // Loop principal
    while (true) {
        cyw43_arch_poll();  // Necessário para manter o Wi-Fi ativo
        sleep_ms(100);
    }

    cyw43_arch_deinit();  // Desliga o Wi-Fi (não será chamado, pois o loop é infinito)
    return 0;
}
