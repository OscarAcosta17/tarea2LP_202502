#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "spawn.h"
#include "tablero.h"
#include "entidades.h"
#include "main.h"

bool colocar_alien(struct Juego *j, int x, int y, int tipo, int hp, int dx) {
    Tablero *t = j->t;
    if (!t) return false;
    if (x < 0 || x >= t->W || y < 0 || y >= t->H) return false;
    if (t->celdas[y][x] != NULL) return false;

    Celda *cel = (Celda*)malloc(sizeof(Celda));
    if (!cel) return false;
    Alien *a = (Alien*)malloc(sizeof(Alien));
    if (!a) { free(cel); return false; }

    a->tipo = tipo; a->hp = hp; a->x = x; a->y = y; a->dx = dx;
    cel->alien = a; cel->dano_pend = 0;

    t->celdas[y][x] = (void*)cel;
    j->vivos += 1;
    return true;
}

int pick_tipo(struct Juego *j) {
    int tipos[3], n = 0;
    if (j->pool.drone    > 0) tipos[n++] = 0;
    if (j->pool.skater   > 0) tipos[n++] = 1;
    if (j->pool.especial > 0) tipos[n++] = 2;
    if (n == 0) return -1;
    return tipos[rand() % n];
}

void descuenta(struct Juego *j, int tipo) {
    if      (tipo == 0) j->pool.drone--;
    else if (tipo == 1) j->pool.skater--;
    else                j->pool.especial--;
}

void spawn_inicio(struct Juego *j) {
    if (!j || !j->t) return;

    if (j->dificultad == 0) { // Fácil: total 15, >=6 drones, >=4 skaters
        j->pool.drone = 6; j->pool.skater = 4; j->pool.especial = 5;
        j->pool.vivos_tope = 6;

    } else {                  // Difícil: total 20, >=8 drones, >=6 skaters
        j->pool.drone = 8; j->pool.skater = 6; j->pool.especial = 6;
        j->pool.vivos_tope = 8;
    }

    int sembrar = (j->dificultad == 0) ? 2 : 3;
    int y = j->t->H - 1;

    for (int i = 0; i < sembrar; ++i) {
        if (j->vivos >= j->pool.vivos_tope) break;

        int tipo = pick_tipo(j); if (tipo < 0) break;
        int hp = (tipo == 0) ? 2 : (tipo == 2 ? 2 : 1);
        int dx = (tipo == 1) ? (rand()%2 ? 1 : -1) : 0;

        int col = rand() % j->t->W;
        bool ok = colocar_alien(j, col, y, tipo, hp, dx);
        if (!ok) {
            for (int x = 0; x < j->t->W && !ok; ++x)
                ok = colocar_alien(j, x, y, tipo, hp, dx);
        }
        if (ok) descuenta(j, tipo);
        else break;
    }
}

void spawn_turno(struct Juego *j) {
    if (!j || !j->t) return;

    int total_inv = j->pool.drone + j->pool.skater + j->pool.especial;
    if (total_inv <= 0) return;
    if (j->vivos >= j->pool.vivos_tope) return;

    int intentos = 1 + ( (rand() / (RAND_MAX + 1.0)) < 0.30 );
    int y = j->t->H - 1;

    for (int k = 0; k < intentos; ++k) {
        if (j->vivos >= j->pool.vivos_tope) break;

        total_inv = j->pool.drone + j->pool.skater + j->pool.especial;
        if (total_inv <= 0) break;

        int tipo = pick_tipo(j); if (tipo < 0) break;
        int hp = (tipo == 0) ? 2 : (tipo == 2 ? 2 : 1);
        int dx = (tipo == 1) ? (rand()%2 ? 1 : -1) : 0;

        int col = rand() % j->t->W;
        bool ok = colocar_alien(j, col, y, tipo, hp, dx);
        if (!ok) {
            for (int x = 0; x < j->t->W && !ok; ++x)
                ok = colocar_alien(j, x, y, tipo, hp, dx);
        }
        if (ok) {
            descuenta(j, tipo);
            const char *nombre_tipo =
                (tipo == 0) ? "Drone" :
                (tipo == 1) ? "Skater" : "Especial";

            printf("[SPAWN] Apareció un %s en columna %d (hp=%d)\n",
                   nombre_tipo, col, hp);
        } else {
            break;
        }
    }
}