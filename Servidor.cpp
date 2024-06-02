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

// Inserta una ficha en la columna dada
bool makeMove(int col, char player) {
    if (col < 0 || col >= COLS || board[0][col] != EMPTY) {
        return false;
    }

    for (int i = ROWS - 1; i >= 0; --i) {
        if (board[i][col] == EMPTY) {
            board[i][col] = player;
            return true;
        }
    }
    return false;
}

// Verifica si hay cuatro en línea
bool checkWin(char player) {
    // Check horizontal
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS - 3; ++j) {
            if (board[i][j] == player && board[i][j + 1] == player && board[i][j + 2] == player && board[i][j + 3] == player) {
                return true;
            }
        }
    }

    // Check vertical
    for (int i = 0; i < ROWS - 3; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (board[i][j] == player && board[i + 1][j] == player && board[i + 2][j] == player && board[i + 3][j] == player) {
                return true;
            }
        }
    }

    // Check diagonal (bottom-left to top-right)
    for (int i = 3; i < ROWS; ++i) {
        for (int j = 0; j < COLS - 3; ++j) {
            if (board[i][j] == player && board[i - 1][j + 1] == player && board[i - 2][j + 2] == player && board[i - 3][j + 3] == player) {
                return true;
            }
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i < ROWS - 3; ++i) {
        for (int j = 0; j < COLS - 3; ++j) {
            if (board[i][j] == player && board[i + 1][j + 1] == player && board[i + 2][j + 2] == player && board[i + 3][j + 3] == player) {
                return true;
            }
        }
    }

    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>" << std::endl;
        return -1;
    }

    int port = atoi(argv[1]);
    WSADATA wsaData;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* welcome = "Bienvenido al juego de Cuatro en Linea. Eres el jugador 1 (S)\n";

    // Inicializar WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar WinSock" << std::endl;
        return -1;
    }

    // Crear descriptor de archivo de socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket" << std::endl;
        WSACleanup();
        return -1;
    }
    std::cout << "Socket creado exitosamente" << std::endl;

    // Adjuntar el socket al puerto
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        std::cerr << "Error en setsockopt" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }
    std::cout << "setsockopt configurado exitosamente" << std::endl;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Vincular el socket a la dirección de red y al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Error al vincular el socket" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }
    std::cout << "Vinculado al puerto " << port << " exitosamente" << std::endl;

    // Comenzar a escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error al escuchar" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }
    std::cout << "Escuchando en el puerto " << port << std::endl;

    // Aceptar una conexión entrante
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (int*)&addrlen)) == INVALID_SOCKET) {
        std::cerr << "Error al aceptar la conexión" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }
    std::cout << "Conexión aceptada" << std::endl;

    // Enviar mensaje de bienvenida
    send(new_socket, welcome, strlen(welcome), 0);
    initializeBoard();
    printBoard();

    char player = 'S';
    bool game_over = false;

    while (!game_over) {
        // Imprimir tablero y enviar al cliente
        std::string board_str = "TABLERO\n";
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                board_str += board[i][j];
                board_str += ' ';
            }
            board_str += '\n';
        }
        board_str += "-------------\n";
        for (int j = 0; j < COLS; ++j) {
            board_str += ' ';
            board_str += std::to_string(j + 1);
        }
        board_str += '\n';
        send(new_socket, board_str.c_str(), board_str.length(), 0);

        if (player == 'S') {
            // Leer movimiento del servidor (jugador S)
            int move;
            std::cout << "Ingrese su movimiento (1-7): ";
            std::cin >> move;
            move -= 1; // Convertir a índice basado en 0

            if (makeMove(move, player)) {
                printBoard();
                if (checkWin(player)) {
                    std::string win_msg = std::string(1, player) + " ha ganado!\n";
                    send(new_socket, win_msg.c_str(), win_msg.length(), 0);
                    game_over = true;
                } else {
                    player = 'C'; // Cambiar a jugador C
                    std::string msg = "Turno del jugador C\n";
                    send(new_socket, msg.c_str(), msg.length(), 0);
                }
            } else {
                std::cout << "Movimiento inválido. Intente de nuevo.\n";
            }
        } else {
            // Leer movimiento del cliente (jugador C)
            valread = recv(new_socket, buffer, 1024, 0);
            if (valread <= 0) {
                std::cerr << "Error de lectura o el cliente se desconectó" << std::endl;
                break;
            }

            int move = atoi(buffer) - 1; // Convertir a índice basado en 0
            if (makeMove(move, player)) {
                printBoard();
                if (checkWin(player)) {
                    std::string win_msg = std::string(1, player) + " ha ganado!\n";
                    send(new_socket, win_msg.c_str(), win_msg.length(), 0);
                    game_over = true;
                } else {
                    player = 'S'; // Cambiar a jugador S
                    std::string msg = "Turno del jugador S\n";
                    send(new_socket, msg.c_str(), msg.length(), 0);
                }
            } else {
                std::string msg = "Movimiento inválido. Intente de nuevo.\n";
                send(new_socket, msg.c_str(), msg.length(), 0);
            }
        }
    }

    // Cerrar el socket
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
