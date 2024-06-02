Daniel Gallegos y Felipe Henriquez
### Makefile

```makefile
# Makefile for Cuatro en Línea Cliente/Servidor

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Archivos de origen y ejecutables
SRCS = Cliente.cpp Servidor.cpp
OBJS = $(SRCS:.cpp=.o)
TARGETS = cliente servidor

# Reglas por defecto
all: $(TARGETS)

# Regla para el cliente
cliente: Cliente.cpp
	$(CXX) $(CXXFLAGS) Cliente.cpp -o cliente -lws2_32

# Regla para el servidor
servidor: Servidor.cpp
	$(CXX) $(CXXFLAGS) Servidor.cpp -o servidor -lws2_32

# Limpiar archivos compilados
clean:
	rm -f $(OBJS) $(TARGETS)
