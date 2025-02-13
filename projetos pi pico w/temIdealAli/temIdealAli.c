#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#define WIFI_SSID "login do wifi"  // Substitua pelo nome da sua rede Wi-Fi
#define WIFI_PASS "senha do wifi"     // Substitua pela senha da sua rede Wi-Fi
#define CHANNEL_API_KEY "sua api do thingspeak" // Substitua pela sua chave API de escrita

// Variáveis globais para armazenar os dados e a resposta do ThingSpeak
static float temperatura = 0.0;
static float umidade = 0.0;
static char thingspeak_response[512] = "Aguardando resposta do servidor...";

// Callback para processar a resposta do servidor
static err_t receive_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Armazena a resposta para exibição no servidor HTTP
    snprintf(thingspeak_response, sizeof(thingspeak_response), "Resposta do ThingSpeak: %.*s", p->len, (char *)p->payload);
    
    // Liberar o buffer e fechar conexão
    pbuf_free(p);
    tcp_close(tpcb);
    return ERR_OK;
}

// Função para enviar dados para o ThingSpeak e receber resposta
static void send_data_to_thingspeak() {
    char post_data[256];

    // Formato correto: "api_key=XXXX&field1=XX&field2=XX"
    snprintf(post_data, sizeof(post_data),
             "api_key=%s&field1=%.2f&field2=%.2f",
             CHANNEL_API_KEY, temperatura, umidade);

    // Criar conexão TCP
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB TCP\n");
        return;
    }

    // Endereço IP do ThingSpeak (api.thingspeak.com)
    ip_addr_t ip;
    IP4_ADDR(&ip, 184, 106, 153, 149);

    // Conectar ao servidor na porta 80
    err_t err = tcp_connect(pcb, &ip, 80, NULL);
    if (err != ERR_OK) {
        printf("Erro ao conectar ao ThingSpeak\n");
        tcp_close(pcb);
        return;
    }

    // Montar a requisição HTTP
    char request[512];
    snprintf(request, sizeof(request),
             "POST /update.json HTTP/1.1\r\n"
             "Host: api.thingspeak.com\r\n"
             "Connection: close\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %d\r\n\r\n"
             "%s",
             (int)strlen(post_data), post_data);

    // Enviar a requisição
    err_t send_err = tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    if (send_err != ERR_OK) {
        printf("Erro ao enviar os dados para o ThingSpeak\n");
        tcp_close(pcb);
        return;
    } else{
        printf("Dados enviados para ThingSpeak\n");        
    }

    // Garantir que os dados sejam enviados
    tcp_output(pcb);

    // Registrar callback para receber resposta
    tcp_recv(pcb, receive_callback);
}

// Callback para responder requisições HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Construir a resposta HTTP dinâmica com temperatura, umidade e resposta do ThingSpeak
    char http_response[1024];
    snprintf(http_response, sizeof(http_response),
             "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
             "<!DOCTYPE html><html><body>"
             "<h1>Monitoramento de Temperatura e Umidade</h1>"
             "<p><strong>Temperatura:</strong> %.2f°C</p>"
             "<p><strong>Umidade:</strong> %.2f%%</p>"
             "<p><strong>%s</strong></p>"
             "</body></html>\r\n",
             temperatura, umidade, thingspeak_response);

    // Envia a resposta HTTP
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

// Callback de conexão para o servidor HTTP
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
    return ERR_OK;
}

// Função de setup do servidor HTTP
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
    stdio_init_all();
    sleep_ms(20000);  // Tempo de inicialização

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

    // Inicia o servidor HTTP
    start_http_server();

    // Loop principal enviando dados a cada 16 segundos
    while (true) {
        temperatura = 31.5 + (rand() % 100) / 50.0;
        umidade = 61.5 + (rand() % 100) / 50.0;
        send_data_to_thingspeak();
        sleep_ms(60000); // Espera 16 segundos antes de enviar novamente
    }

    cyw43_arch_deinit();  // Nunca será chamado, pois o loop é infinito
    return 0;
}
