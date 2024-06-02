# Cuatro en Línea - Juego Cliente/Servidor

Este proyecto implementa un juego de "Cuatro en Línea" utilizando una arquitectura cliente-servidor en C++. El servidor escucha las conexiones entrantes y maneja la lógica del juego, mientras que el cliente se conecta al servidor para jugar.

## Requisitos

- Sistema operativo: Windows
- Compilador: g++ (Mingw-w64)
- Biblioteca WinSock2 (Ws2_32.lib)

## Compilación

Para compilar los archivos del servidor y del cliente, puedes utilizar el archivo `Makefile` proporcionado.

## Ejecución

Primero, ejecuta el servidor, para luego ejecutar cliente en otra terminal:

```bash
./servidor <puerto>

./cliente <direccion IP> <puerto>

