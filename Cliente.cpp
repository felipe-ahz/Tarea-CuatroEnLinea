#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Enlazar la biblioteca de Winsock

int main(int argc, char *argv[]) {
    WSADATA wsa_data;
    SOCKET sockfd;
    struct sockaddr_in direccion_servidor;
    char buffer[1024];
    int puerto;

    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <dirección IP> <puerto>\n";
        return 1;
    }

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Error al inicializar Winsock: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    puerto = atoi(argv[2]);
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(puerto);
    direccion_servidor.sin_addr.s_addr = inet_addr(argv[1]);

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar con el servidor: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado al servidor. Esperando el juego...\n";

    // Comunicación con el servidor
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sockfd, buffer, 1024, 0);
        if (bytesReceived > 0) {
            std::cout << buffer;
            if (strncmp("Ganaste", buffer, 7) == 0 || strncmp("Perdiste", buffer, 8) == 0) {
                break;
            }
        } else if (bytesReceived == 0) {
            std::cout << "El servidor cerró la conexión.\n";
            break;
        } else {
            std::cerr << "Error al recibir datos: " << WSAGetLastError() << std::endl;
            break;
        }

        int columna;
        std::cout << "Tu turno - Ingresa el número de columna (1-7): ";
        std::cin >> columna;
        // Validar la entrada de columna
        if (columna < 1 || columna > 7) {
            std::cout << "Número de columna no válido. Inténtalo de nuevo.\n";
            continue;
        }

        memset(buffer, 0, sizeof(buffer));
        _itoa_s(columna, buffer, 10);
        send(sockfd, buffer, strlen(buffer), 0);
    }

    // Cerrar el socket y limpiar Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
