#ifndef SPAWN_H
#define SPAWN_H

#include <stdbool.h>

typedef struct {
    int especial; int skater; int drone;
    int vivos_tope; /* tope de aliens simultaneos en pantalla */
} PoolAliens;

struct Juego;
/*
 * Nombre: spawn_inicio
 * Parámetros:
 *      - struct Juego *j: puntero a la estructura principal del juego, que contiene el tablero, la dificultad y el inventario de aliens.
 * Retorno:
 *      - void (no retorna nada).
 * Descripción:
 *      Inicializa el inventario de aliens y coloca los primeros enemigos en el tablero al inicio de la partida.
 *      - Si la dificultad es fácil:
 *          • Inventario: 6 Drones, 4 Skaters, 5 Especiales (total 15).
 *          • Máximo simultáneo: 6 vivos.
 *          • Se colocan 2 aliens en la fila superior.
 *      - Si la dificultad es difícil:
 *          • Inventario: 8 Drones, 6 Skaters, 6 Especiales (total 20).
 *          • Máximo simultáneo: 8 vivos.
 *          • Se colocan 3 aliens en la fila superior.
 *      - Para cada alien inicial:
 *          • Se selecciona un tipo válido según el inventario con pick_tipo.
 *          • Se asignan puntos de vida (Drone y Especial = 2, Skater = 1).
 *          • Los Skaters reciben además una dirección horizontal inicial aleatoria.
 *          • Se intenta ubicar el alien en una columna aleatoria de la fila superior, si no hay espacio, se recorre la fila hasta encontrar un hueco.
 *          • Si el alien fue colocado con éxito, se descuenta del inventario.
 */
void spawn_inicio(struct Juego *juego);

/*
 * Nombre: spawn_turno
 * Parámetros:
 *      - struct Juego *j: puntero a la estructura principal del juego, que contiene el tablero, el inventario de aliens y el contador de vivos.
 * Retorno:
 *      - void (no retorna nada).
 * Descripción:
 *      Controla la aparición de nuevos aliens en cada turno del juego.
 *      - No hace nada si:
 *          • No quedan aliens en el inventario.
 *          • Ya se alcanzó el máximo de aliens simultáneos permitidos.
 *      - Intenta spawnear al menos 1 alien por turno, con 30% de probabilidad de generar un segundo.
 *      - Para cada intento:
 *          • Selecciona un tipo válido con pick_tipo según inventario.
 *          • Asigna puntos de vida (Drone y Especial = 2, Skater = 1).
 *          • Los Skaters reciben dirección horizontal inicial aleatoria.
 *          • Intenta colocarlo en una columna aleatoria de la fila superior, si no puede, recorre la fila hasta hallar un espacio libre.
 *          • Si logra colocarlo, descuenta del inventario y muestra un mensaje informando el tipo, columna y puntos de vida iniciales del alien.
 */
void spawn_turno(struct Juego *juego);

/*
 * Nombre: colocar_alien
 * Parámetros:
 *      - struct Juego *j: puntero a la estructura principal del juego, que contiene el tablero y el conteo de aliens vivos.
 *      - int x: columna donde se intentará colocar el alien.
 *      - int y: fila donde se intentará colocar el alien.
 *      - int tipo: tipo de alien (0 = Drone, 1 = Skater, 2 = Especial).
 *      - int hp: puntos de vida iniciales del alien.
 *      - int dx: dirección de movimiento horizontal del alien (ejemplo: -1 = izquierda, 1 = derecha).
 * Retorno:
 *      - bool:
 *          - true si el alien fue creado y colocado exitosamente.
 *          - false si la posición era inválida, estaba ocupada, o si falló la reserva de memoria.
 * Descripción:
 *      Intenta crear un alien en la posición especificada del tablero.
 *      - Valida que la posición (x, y) esté dentro de los límites y libre.
 *      - Reserva memoria para una celda y un alien.
 *      - Inicializa los atributos del alien (tipo, hp, posición y dirección).
 *      - Asocia el alien con la celda, coloca la celda en el tablero e incrementa el contador de aliens vivos en el juego.
 */
bool colocar_alien(struct Juego *j, int x, int y, int tipo, int hp, int dx);

/*
 * Nombre: pick_tipo
 * Parámetros:
 *      - struct Juego *j: puntero a la estructura principal del juego, que contiene el inventario de aliens disponibles en el pool.
 * Retorno:
 *      - int:
 *          - 0 si se selecciona un Drone.
 *          - 1 si se selecciona un Skater.
 *          - 2 si se selecciona un Especial.
 *          - -1 si no quedan aliens disponibles en el inventario.
 * Descripción:
 *      Selecciona aleatoriamente un tipo de alien entre los que aún tienen unidades disponibles en el inventario del juego. Construye una lista de tipos válidos (Drone, Skater, Especial) según el stock restante y devuelve uno al azar usando rand().
 */
int pick_tipo(struct Juego *j);

/*
 * Nombre: descuenta
 * Parámetros:
 *      - struct Juego *j: puntero a la estructura principal del juego, que contiene el inventario de aliens disponibles en el pool.
 *      - int tipo: identificador del tipo de alien a descontar (0 = Drone, 1 = Skater, 2 = Especial).
 * Retorno:
 *      - void (no retorna nada).
 * Descripción:
 *      Resta en uno el contador de aliens disponibles en el pool correspondiente al tipo especificado. Se utiliza cuando un alien es colocado en el tablero para reflejar el consumo del inventario.
 */
void descuenta(struct Juego *j, int tipo);
#endif