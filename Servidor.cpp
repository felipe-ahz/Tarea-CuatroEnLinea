#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Enlazar la biblioteca de Winsock

#define TAMANO_TABLERO 6
#define NUM_COLUMNAS 7
#define PUERTO 7777

std::mutex cout_mutex;

void manejar_cliente(SOCKET socket_cliente);
void imprimir_tablero(const char tablero[TAMANO_TABLERO][NUM_COLUMNAS]);
bool jugar_en_columna(char tablero[TAMANO_TABLERO][NUM_COLUMNAS], int columna, char ficha);
bool verificar_ganador(const char tablero[TAMANO_TABLERO][NUM_COLUMNAS], char ficha);
void inicializar_tablero(char tablero[TAMANO_TABLERO][NUM_COLUMNAS]);
void enviar_tablero(const char tablero[TAMANO_TABLERO][NUM_COLUMNAS], SOCKET sock);
bool recibir_jugada(SOCKET sock, int& columna);

int main(int argc, char *argv[]) {
    WSADATA wsa_data;
    SOCKET sockfd, new_sock;
    struct sockaddr_in direccion_servidor, direccion_cliente;
    int tamano_direccion = sizeof(struct sockaddr_in);

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Error al inicializar Winsock." << std::endl;
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creando el socket." << std::endl;
        return 1;
    }

    int puerto = (argc > 1) ? atoi(argv[1]) : PUERTO;
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(puerto);
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) == SOCKET_ERROR) {
        std::cerr << "Error al enlazar el socket." << std::endl;
        return 1;
    }

    listen(sockfd, 5);
    std::cout << "Esperando conexiones en el puerto " << puerto << "..." << std::endl;

    while (true) {
        new_sock = accept(sockfd, (struct sockaddr *)&direccion_cliente, &tamano_direccion);
        if (new_sock == INVALID_SOCKET) {
            std::cerr << "Error al aceptar conexión." << std::endl;
            continue;
        }
        std::thread(manejar_cliente, new_sock).detach();
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

void manejar_cliente(SOCKET socket_cliente) {
    char tablero[TAMANO_TABLERO][NUM_COLUMNAS];
    inicializar_tablero(tablero);
    int columna;
    char jugador_actual = 'C'; // Comienza el cliente

    enviar_tablero(tablero, socket_cliente);
    while (true) {
        if (jugador_actual == 'C') {
            if (!recibir_jugada(socket_cliente, columna)) break;
            if (jugar_en_columna(tablero, columna, 'C')) {
                if (verificar_ganador(tablero, 'C')) {
                    enviar_tablero(tablero, socket_cliente);
                    send(socket_cliente, "Ganaste\n", 9, 0);
                    break;
                }
                jugador_actual = 'S';
            }
        } else {
            std::cout << "Turno del servidor (ingresa la columna): ";
            std::cin >> columna;
            if (jugar_en_columna(tablero, columna, 'S')) {
                if (verificar_ganador(tablero, 'S')) {
                    enviar_tablero(tablero, socket_cliente);
                    send(socket_cliente, "Perdiste\n", 10, 0);
                    break;
                }
                jugador_actual = 'C';
            }
        }
        enviar_tablero(tablero, socket_cliente);
    }
    closesocket(socket_cliente);
}

void inicializar_tablero(char tablero[TAMANO_TABLERO][NUM_COLUMNAS]) {
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        for (int j = 0; j < NUM_COLUMNAS; ++j) {
            tablero[i][j] = ' ';
        }
    }
}

void imprimir_tablero(const char tablero[TAMANO_TABLERO][NUM_COLUMNAS]) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        for (int j = 0; j < NUM_COLUMNAS; ++j) {
            std::cout << '[' << tablero[i][j] << ']';
        }
        std::cout << '\n';
    }
    std::cout << "-------------\n";
    for (int j = 1; j <= NUM_COLUMNAS; ++j) {
        std::cout << " " << j << " ";
    }
    std::cout << '\n';
}

bool jugar_en_columna(char tablero[TAMANO_TABLERO][NUM_COLUMNAS], int columna, char ficha) {
    if (columna < 0 || columna >= NUM_COLUMNAS) {
        return false;
    }
    for (int i = TAMANO_TABLERO - 1; i >= 0; --i) {
        if (tablero[i][columna] == ' ') {
            tablero[i][columna] = ficha;
            return true;
        }
    }
    return false;
}

bool verificar_ganador(const char tablero[TAMANO_TABLERO][NUM_COLUMNAS], char ficha) {
    // Verificar horizontalmente
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        for (int j = 0; j <= NUM_COLUMNAS - 4; ++j) {
            if (tablero[i][j] == ficha && tablero[i][j+1] == ficha && tablero[i][j+2] == ficha && tablero[i][j+3] == ficha) {
                return true;
            }
        }
    }
    // Verificar verticalmente
    for (int j = 0; j < NUM_COLUMNAS; ++j) {
        for (int i = 0; i <= TAMANO_TABLERO - 4; ++i) {
            if (tablero[i][j] == ficha && tablero[i+1][j] == ficha && tablero[i+2][j] == ficha && tablero[i+3][j] == ficha) {
                return true;
            }
        }
    }
    // Verificar diagonalmente (descendente)
    for (int i = 0; i <= TAMANO_TABLERO - 4; ++i) {
        for (int j = 0; j <= NUM_COLUMNAS - 4; ++j) {
            if (tablero[i][j] == ficha && tablero[i+1][j+1] == ficha && tablero[i+2][j+2] == ficha && tablero[i+3][j+3] == ficha) {
                return true;
            }
        }
    }
    // Verificar diagonalmente (ascendente)
    for (int i = 3; i < TAMANO_TABLERO; ++i) {
        for (int j = 0; j <= NUM_COLUMNAS - 4; ++j) {
            if (tablero[i][j] == ficha && tablero[i-1][j+1] == ficha && tablero[i-2][j+2] == ficha && tablero[i-3][j+3] == ficha) {
                return true;
            }
        }
    }
    return false;
}

void enviar_tablero(const char tablero[TAMANO_TABLERO][NUM_COLUMNAS], SOCKET sock) {
    std::string buffer = "Tablero actual:\n";
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        for (int j = 0; j < NUM_COLUMNAS; ++j) {
            buffer += '[';
            buffer += tablero[i][j];
            buffer += ']';
        }
        buffer += '\n';
    }
    send(sock, buffer.c_str(), buffer.length(), 0);
}

bool recibir_jugada(SOCKET sock, int& columna) {
    char buffer[1024];
    int bytes_recibidos = recv(sock, buffer, 1024, 0);
    if (bytes_recibidos > 0) {
        buffer[bytes_recibidos] = '\0';
        columna = atoi(buffer) - 1; // ajuste para índice base 0
        return true;
    }
    return false;
}
