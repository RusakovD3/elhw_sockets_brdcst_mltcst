#include <stdio.h>      // Подключает стандартную библиотеку ввода-вывода
#include <stdlib.h>     // Подключает библиотеку стандартных функций
#include <string.h>     // Подключает библиотеку функций для работы со строками
#include <unistd.h>     // Подключает функции для работы с системными вызовами Unix
#include <arpa/inet.h>  // Подключает функции для работы с адресами интернет-протоколов
#include <sys/socket.h> // Подключает функции для работы с сокетами

#define BROADCAST_IP "255.255.255.255" // Определяет IP-адрес для броадкаста
#define PORT 8888                      // Определяет номер порта


int main() {
    int sock; // Дескриптор сокета
    struct sockaddr_in broadcastAddr; // Структура для адреса броадкаста
    char *message = "Hello";          // Сообщение для отправки
    int broadcastPermission = 1;      // Переменная для разрешения броадкаста

    // Создание UDP сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket() failed");   // Выводит описание ошибки, если создание сокета не удалось
        exit(1);
    }

    // Установка разрешения на отправку броадкастов
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST|SO_REUSEADDR, &broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("setsockopt() failed"); // Выводит описание ошибки, если установка опции не удалась
        close(sock);
        exit(1);
    }

    // Заполнение структуры адреса для броадкаста
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    broadcastAddr.sin_port = htons(PORT);
    
    // Отправка сообщения
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0) {
        perror("sendto() failed"); // Выводит описание ошибки, если отправка не удалась
    } else {
        printf("Broadcast message sent\n");
    }

    close(sock); // Закрытие сокета

    return 0;
}