#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdbool.h>
typedef struct {
    int tipo; /* tipo de alien */
    int hp; /* vida actual */
    int x, y; /* posicion en el tablero */
    int dx; /* direccion horizontal (zig-zag), p.ej., -1/+1 */
} Alien;

typedef struct {
    Alien *alien; /* NULL si la celda esta vacia */
    int dano_pend; /* danio marcado a consolidar */
} Celda;

struct Juego; /* forward-declaration para punteros a funcion (armas) */
void mover_aliens(struct Juego *juego);
void resolver_danos(struct Juego *juego);

/*
 * Nombre: mover_jugador_izq
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene el tablero y la posición del jugador.
 * Retorno:
 *      - bool: 
 *          - true si el jugador se movió exitosamente a la izquierda.
 *          - false si no se movió (porque está en el borde o el puntero es inválido).
 * Descripción:
 *      Mueve al jugador una columna a la izquierda en el tablero, siempre que no se encuentre ya en la posición más a la izquierda (columna 0). 
 *      Al realizar un movimiento válido, también ejecuta la función `spawn_turno` para procesar el avance del turno (aparición de aliens, etc.).
 */
bool mover_jugador_izq(struct Juego *juego);

/*
 * Nombre: mover_jugador_der
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene el tablero y la posición del jugador.
 * Retorno:
 *      - bool:
 *          - true si el jugador se movió exitosamente a la derecha.
 *          - false si no se movió (porque está en el borde derecho o el puntero es inválido).
 * Descripción:
 *      Mueve al jugador una columna a la derecha en el tablero, siempre que no se encuentre ya en la última columna disponible. 
 *      Al realizar un movimiento válido, también ejecuta la función `spawn_turno` para procesar el avance del turno (aparición de aliens, etc.).
 */

bool mover_jugador_der(struct Juego *juego);

/*
 * Nombre: mover_jugador
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego,que contiene el tablero y la posición del jugador.
 *      - char c: carácter ingresado por el usuario que indica la dirección del movimiento ('a' o 'A' para izquierda, 'd' o 'D' para derecha).
 * Retorno:
 *      - bool:
 *          - true si el jugador se movió exitosamente en la dirección indicada.
 *          - false si no se movió (carácter inválido, puntero inválido, o jugador en el borde del tablero).
 * Descripción:
 *      Controla el movimiento del jugador según la tecla presionada. 
 *      Si se recibe 'a'/'A', llama a `mover_jugador_izq`. 
 *      Si se recibe 'd'/'D', llama a `mover_jugador_der`. 
 *      Cualquier otro carácter no produce movimiento. 
 *      Esta función centraliza la lógica de entrada para movimientos horizontales.
 */

bool mover_jugador(struct Juego *juego, char tecla);
#endif