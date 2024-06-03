## README del Servidor
Este código implementa un juego de Cuatro en Línea con funcionalidad de servidor. El servidor maneja las conexiones de los jugadores, el tablero y la lógica del juego. Además, notifica a los clientes sobre los cambios en el tablero y el estado del juego. El juego finaliza cuando un jugador conecta cuatro fichas consecutivas.

***Instrucciones para ejecutar el juego:***

## Compilación
- Abra una terminal en la carpeta del proyecto.
- Escriba el comando make y presione Enter.
- Esto compilará el servidor y el cliente del juego.

## Opción de Compilar sin Makefile
- Abra una nueva terminal en la carpeta del proyecto.
- Escriba el comando: g++ -o servidor servidor.cpp -lws2_32

## Ejecución
- Abra una nueva terminal en la carpeta del proyecto.
- Escriba el comando `./servidor <puerto>` y presione Enter
- Esto iniciará el servidor del juego en el puerto que usted ponga, como por ejemplo, puerto 7777.

**Notas:**
Asegúrese de tener instalado el compilador y las bibliotecas necesarias para compilar el juego.
Al momento de tener un ganador el servidor finalizara las conexiones y entregará un mensaje con el ganador.