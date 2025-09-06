#ifndef SPAWN_H
#define SPAWN_H

#include <stdbool.h>

typedef struct {
    int especial; int skater; int drone;
    int vivos_tope; /* tope de aliens simultaneos en pantalla */
} PoolAliens;

struct Juego;

void spawn_inicio(struct Juego *juego);
void spawn_turno(struct Juego *juego);

bool colocar_alien(struct Juego *j, int x, int y, int tipo, int hp, int dx);
int pick_tipo(struct Juego *j);
void descuenta(struct Juego *j, int tipo);
#endif