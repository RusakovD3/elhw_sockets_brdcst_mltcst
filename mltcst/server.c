#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MULTICAST_IP "224.0.0.1"  // IP-адрес для мультикаст-группы
#define PORT 8888                // Порт для мультикаст-сообщений

int main() {
    int sock;
    struct sockaddr_in multicastAddr;
    char *message = "Hi!";

    // Создание UDP сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        exit(1);
    }

    // Подготовка структуры адреса для мультикаста
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr(MULTICAST_IP);
    multicastAddr.sin_port = htons(PORT);

    // Отправка сообщения
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&multicastAddr, sizeof(multicastAddr)) < 0) {
        perror("sendto() failed");
    } else {
        printf("Multicast message sent\n");
    }

    // Закрытие сокета
    close(sock);

    return 0;
}
