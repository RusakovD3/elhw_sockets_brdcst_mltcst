#include <stdio.h>      // Подключает стандартную библиотеку ввода-вывода
#include <stdlib.h>     // Подключает библиотеку стандартных функций
#include <string.h>     // Подключает библиотеку функций для работы со строками
#include <unistd.h>     // Подключает функции для работы с системными вызовами Unix
#include <arpa/inet.h>  // Подключает функции для работы с адресами интернет-протоколов
#include <sys/socket.h> // Подключает функции для работы с сокетами

#define PORT 8888       // Определяет номер порта
#define BROADCAST_IP "255.255.255.255"

int main() {
    int sock;                        // Дескриптор сокета
    struct sockaddr_in serverAddr;   // Структура для адреса сервера
    char buffer[1024];               // Буфер для приема данных
    int recvBytes;                   // Количество полученных байт

    // Создание UDP сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket() failed");   // Выводит описание ошибки, если создание сокета не удалось
        exit(1);
    }

    // Заполнение структуры адреса сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);;
    serverAddr.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind() failed"); // Выводит описание ошибки, если привязка не удалась
        close(sock);
        exit(1);
    }

    printf("Waiting for messages...\n");
    // Получение данных
    recvBytes = recvfrom(sock, buffer, 1024, 0, NULL, NULL);
    if (recvBytes < 0){
        perror("No recived data");
        close(sock);
        exit(1);
    }
    buffer[recvBytes] = '\0'; // Превращаем данные в строку
    printf("Received: %s\n", buffer);

    close(sock); // Закрытие сокета
    return 0;
}