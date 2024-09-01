#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[1024] = {0};
    char option[10], length_str[10];
    int isEjecutando = 1;

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error en WSAStartup. Código de error: %d\n", WSAGetLastError());
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Error al crear el socket. Código de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Error en la conexión\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (isEjecutando) {
        printf("Menú:\n1. Generar Nombre de Usuario\n2. Generar Contraseña\n3. Salir\nIntroduce tu opción: ");
        fgets(option, sizeof(option), stdin);
        option[strcspn(option, "\n")] = 0;

        if (strcmp(option, "1") == 0) {
            printf("Introduce la longitud del nombre de usuario (5-15): ");
            fgets(length_str, sizeof(length_str), stdin);
            send(sock, "username", strlen("username"), 0);
            send(sock, length_str, strlen(length_str), 0);
        } else if (strcmp(option, "2") == 0) {
            printf("Introduce la longitud de la contraseña (8-50): ");
            fgets(length_str, sizeof(length_str), stdin);
            send(sock, "password", strlen("password"), 0);
            send(sock, length_str, strlen(length_str), 0);
        } else if (strcmp(option, "3") == 0) {
            send(sock, "exit", strlen("exit"), 0);
            isEjecutando = 0;
        } else {
            printf("Opción inválida, por favor intenta de nuevo.\n");
            continue;
        }

        recv(sock, buffer, sizeof(buffer), 0);
        printf("Respuesta del servidor: %s\n", buffer);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
