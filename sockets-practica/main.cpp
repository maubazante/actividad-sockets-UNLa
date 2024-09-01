#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

#define PORT 8080
#define MAX_CLIENTS 10

const char* vocales = "aeiou";
const char* cons = "bcdfghjklmnpqrstvwxyz";

void generarUsername(char* username, int longitud) {
    int isEmpiezaVocal = rand() % 2;
    for (int i = 0; i < longitud; i++) {
        if (isEmpiezaVocal) {
            username[i] = (i % 2 == 0) ? vocales[rand() % 5] : cons[rand() % 21];
        } else {
            username[i] = (i % 2 == 0) ? cons[rand() % 21] : vocales[rand() % 5];
        }
    }
    username[longitud] = '\0';
}

void generarPassword(char* password, int longitud) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < longitud; i++) {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[longitud] = '\0';
}

int main() {
    WSADATA wsa;
    SOCKET serv_sock, client_sock;
    struct sockaddr_in server, client;
    int client_len, recv_len;
    char buffer[1024] = {0};
    int isEjecutando = 1;

    srand(time(NULL));

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error en WSAStartup. Código de error: %d\n", WSAGetLastError());
        return 1;
    }

    if ((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Error al crear el socket. Código de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error en bind. Código de error: %d\n", WSAGetLastError());
        closesocket(serv_sock);
        WSACleanup();
        return 1;
    }

    listen(serv_sock, MAX_CLIENTS);
    printf("Servidor iniciado en el puerto %d\n", PORT);
    client_len = sizeof(struct sockaddr_in);

    while (1) {
        client_sock = accept(serv_sock, (struct sockaddr*)&client, &client_len);
        if (client_sock == INVALID_SOCKET) {
            printf("Error en accept. Código de error: %d\n", WSAGetLastError());
            continue;
        }

        isEjecutando = 1;
        while (isEjecutando) {
            memset(buffer, 0, sizeof(buffer));
            recv_len = recv(client_sock, buffer, 1024, 0);
            buffer[recv_len] = '\0';

            if (strcmp(buffer, "username") == 0) {
                recv_len = recv(client_sock, buffer, 1024, 0);
                buffer[recv_len] = '\0';
                int longitud = atoi(buffer);

                if (longitud < 5 || longitud > 15) {
                    send(client_sock, "Error: La longitud del nombre de usuario debe ser entre 5 y 15\n", strlen("Error: La longitud del nombre de usuario debe ser entre 5 y 15\n"), 0);
                } else {
                    char username[16];
                    generarUsername(username, longitud);
                    send(client_sock, username, strlen(username), 0);
                }
            } else if (strcmp(buffer, "password") == 0) {
                recv_len = recv(client_sock, buffer, 1024, 0);
                buffer[recv_len] = '\0';
                int longitud = atoi(buffer);

                if (longitud < 8 || longitud > 50) {
                    send(client_sock, "Error: La longitud de la contraseña debe ser entre 8 y 50\n", strlen("Error: La longitud de la contraseña debe ser entre 8 y 50\n"), 0);
                } else {
                    char password[51];
                    generarPassword(password, longitud);
                    send(client_sock, password, strlen(password), 0);
                }
            } else if (strcmp(buffer, "exit") == 0) {
                isEjecutando = 0;
                send(client_sock, "Desconectando...\n", strlen("Desconectando...\n"), 0);
            }
        }

        closesocket(client_sock);
    }

    closesocket(serv_sock);
    WSACleanup();
    return 0;
}
