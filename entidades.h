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

bool mover_jugador_izq(struct Juego *juego);
bool mover_jugador_der(struct Juego *juego);
bool mover_jugador(struct Juego *juego, char tecla);

#endif