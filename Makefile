# Makefile para compilar y ejecutar Servidor.cpp y Cliente.cpp en Windows con MinGW

# Compilador y opciones
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lws2_32

# Archivos fuente y ejecutables
SERVIDOR_SRC = Servidor.cpp
CLIENTE_SRC = Cliente.cpp
SERVIDOR_EXE = servidor.exe
CLIENTE_EXE = cliente.exe

# Reglas de compilación
all: $(SERVIDOR_EXE) $(CLIENTE_EXE)

$(SERVIDOR_EXE): $(SERVIDOR_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(SERVIDOR_EXE) $(SERVIDOR_SRC)

$(CLIENTE_EXE): $(CLIENTE_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(CLIENTE_EXE) $(CLIENTE_SRC)

# Regla para compilar y ejecutar el servidor
run-servidor: $(SERVIDOR_EXE)
	./$(SERVIDOR_EXE)

# Regla para compilar y ejecutar el cliente
run-cliente: $(CLIENTE_EXE)
	./$(CLIENTE_EXE)

# Limpieza
clean:
	rm -f $(SERVIDOR_EXE) $(CLIENTE_EXE)

# Para ejecución en Windows
run: run-servidor run-cliente