#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MULTICAST_IP "224.0.0.1"  // IP-адрес для мультикаст-группы
#define PORT 8888                // Порт для мультикаст-сообщений
#define BUFSIZE 1024
int main() {
    int sock;
    struct sockaddr_in localAddr;
    struct ip_mreq group;
    char buffer[BUFSIZE];
    int recvBytes;

    // Создание UDP сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        exit(1);
    }

    // Заполнение структуры адреса
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(sock, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0) {
        perror("bind() failed");
        close(sock);
        exit(1);
    }

    // Присоединение к мультикаст-группе
    group.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    group.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        perror("setsockopt() failed");
        close(sock);
        exit(1);
    }

    printf("Waiting for messages...\n");
    // Чтение входящих сообщений
    recvBytes = recvfrom(sock, buffer, BUFSIZE, 0, NULL, NULL);
    if (recvBytes < 0) {
        perror("No recieved data");
        setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&group, sizeof(group));
        close(sock);
        exit(1);
    }
    buffer[recvBytes] = '\0';  // Обеспечиваем корректное завершение строки
    printf("Received: %s\n", buffer);

    // Покидание мультикаст-группы и закрытие сокета
    setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&group, sizeof(group));
    close(sock);

    return 0;
}
