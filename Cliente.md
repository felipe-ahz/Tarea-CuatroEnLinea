## README del Cliente
Este código implementa la parte del cliente para un juego de Cuatro en Línea. El cliente se conecta al servidor, recibe actualizaciones del estado del juego y del tablero, y envía sus movimientos al servidor. El objetivo es conectar cuatro fichas consecutivas para ganar el juego.

***Instrucciones para ejecutar el juego:***

## Compilación
- Abra una terminal en la carpeta del proyecto.
- Escriba el comando `make` y presione Enter.
- Esto compilará el servidor y el cliente del juego.

## Opción de Compilar sin Makefile
- Abra una nueva terminal en la carpeta del proyecto.
- Escriba el comando: `g++ -o cliente cliente.cpp -lws2_32`

## Ejecución
- Abra una nueva terminal en la carpeta del proyecto.
- Asegurarse que el servidor esté en funcionamiento y esté escuchando en su puerto.
- Escriba el comando `./cliente <dirección ip> <puerto>` y presione Enter.
- Esto conectará el cliente al servidor del juego en la IP y puerto especificados, por ejemplo, IP 127.0.0.1 y puerto 7777.

**Notas:**
- Antes de todo, el servidor debe estar escuchando en su puerto.
- Asegúrese de tener instalado el compilador, las bibliotecas necesarias para compilar el juego y de anotar bien la dirección IP para empezar a jugar.
- El cliente debe estar ejecutándose al mismo tiempo que el servidor para poder jugar.