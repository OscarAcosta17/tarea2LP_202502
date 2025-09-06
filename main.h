#ifndef MAIN_H
#define MAIN_H

#include "tablero.h"
#include "armas.h"
#include "spawn.h"
#include "entidades.h"

typedef struct Juego {
    Tablero *t; /* encapsula W/H y la politica de memoria/render */
    Armas armas;
    PoolAliens pool;
    int dificultad;
    int turno, vivos, jugador_x;

    int derrota; // esta es nueva 
} Juego;

#endif