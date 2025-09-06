# Compilador y flags
CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2
DBGFLAGS= -g -O0                  # Para depurar/valgrind

# Archivos
OBJS    = main.o tablero.o entidades.o spawn.o armas.o
TARGET  = juego

# Valgrind
VALGRIND = valgrind
VG_OPTS  = --leak-check=full --show-leak-kinds=all --track-origins=yes
VG_STRICT= $(VG_OPTS) --errors-for-leak-kinds=all --error-exitcode=1

.PHONY: all debug run mem memv clean

all: $(TARGET)

# Binario optimizado (por defecto)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Binario con símbolos de depuración (para valgrind)
debug: CFLAGS := $(filter-out -O2,$(CFLAGS)) $(DBGFLAGS)
debug: clean $(TARGET)

# Reglas de compilación
main.o: main.c main.h tablero.h entidades.h spawn.h armas.h
	$(CC) $(CFLAGS) -c main.c

tablero.o: tablero.c tablero.h main.h
	$(CC) $(CFLAGS) -c tablero.c

entidades.o: entidades.c entidades.h main.h spawn.h tablero.h
	$(CC) $(CFLAGS) -c entidades.c

spawn.o: spawn.c spawn.h main.h entidades.h tablero.h
	$(CC) $(CFLAGS) -c spawn.c

armas.o: armas.c armas.h main.h
	$(CC) $(CFLAGS) -c armas.c

# Ejecutar normal
run: $(TARGET)
	./$(TARGET)

# Valgrind (leaks detallados)
mem: debug
	$(VALGRIND) $(VG_OPTS) ./$(TARGET)

# Valgrind estricto (falla si hay leaks)
memv: debug
	$(VALGRIND) $(VG_STRICT) ./$(TARGET)

# Limpieza
clean:
	rm -f $(OBJS) $(TARGET)
