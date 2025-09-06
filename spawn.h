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
#endif