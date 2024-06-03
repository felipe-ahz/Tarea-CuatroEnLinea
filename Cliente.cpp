#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib")

#define ROWS 6
#define COLS 7
#define EMPTY ' '

char board[ROWS][COLS];

// Inicializa el tablero
void initializeBoard() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

// Imprime el tablero
void printBoard() {
    std::cout << "TABLERO" << std::endl;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------" << std::endl;
    for (int j = 0; j < COLS; ++j) {
        std::cout << " " << (j + 1);
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <direccion IP> <puerto>" << std::endl;
        return -1;
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int valread;

    // Inicializar WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar WinSock" << std::endl;
        return -1;
    }

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket" << std::endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convertir direcciones IPv4 e IPv6 de texto a formato binario
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Direccion invalida / Direccion no compatible" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    std::cout << "Conectado al servidor" << std::endl;

    valread = recv(sock, buffer, 1024, 0);
    std::cout << buffer << std::endl;

    bool game_over = false;
    while (!game_over) {
        valread = recv(sock, buffer, 1024, 0);
        if (valread > 0) {
            buffer[valread] = '\0';
            std::cout << buffer << std::endl;
            if (strstr(buffer, "ha ganado") != nullptr) {
                game_over = true;
                continue;
            }

            if (strstr(buffer, "Turno del jugador C") != nullptr) {
                std::cout << "Ingrese su movimiento (1-7): ";
                std::string move;
                std::cin >> move;
                send(sock, move.c_str(), move.length(), 0);
            }
        }
    }

    // Cerrar el socket
    closesocket(sock);
    WSACleanup();
    return 0;
}
