#include <stdlib.h>
#include <stdbool.h>
#include "spawn.h"
#include "tablero.h"
#include "entidades.h"
#include "main.h"

/* Inventario y tope: internos a spawn.c para no tocar main.h */
static int inv_drone = 0, inv_skater = 0, inv_especial = 0;
static int vivos_tope = 0;

static bool colocar_alien(struct Juego *j, int x, int y, int tipo, int hp, int dx) {
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

/* elige un tipo disponible entre los que quedan en inventario */
static int pick_tipo(void) {
    int tipos[3], n = 0;
    if (inv_drone   > 0) tipos[n++] = 0;
    if (inv_skater  > 0) tipos[n++] = 1;
    if (inv_especial> 0) tipos[n++] = 2;
    if (n == 0) return -1;
    return tipos[rand() % n];
}
static void descuenta(int tipo) {
    if      (tipo == 0) inv_drone--;
    else if (tipo == 1) inv_skater--;
    else                inv_especial--;
}

/* Siembra inicial + seteo de inventario/tope (mínimos de la pauta) */
void spawn_inicio(struct Juego *j) {
    if (!j || !j->t) return;

    if (j->dificultad == 0) { // Fácil: total 15, >=6 drones, >=4 skaters
        inv_drone = 6; inv_skater = 4; inv_especial = 5;
        vivos_tope = 6;
    } else {                  // Difícil: total 20, >=8 drones, >=6 skaters
        inv_drone = 8; inv_skater = 6; inv_especial = 6;
        vivos_tope = 8;
    }

    int sembrar = (j->dificultad == 0) ? 2 : 3;
    int y = j->t->H - 1;

    for (int i = 0; i < sembrar; ++i) {
        if (j->vivos >= vivos_tope) break;
        int tipo = pick_tipo(); if (tipo < 0) break;
        int hp = (tipo == 0) ? 2 : 1;
        int dx = (tipo == 1) ? (rand()%2 ? 1 : -1) : 0;

        int col = rand() % j->t->W;
        bool ok = colocar_alien(j, col, y, tipo, hp, dx);
        if (!ok) {
            for (int x = 0; x < j->t->W && !ok; ++x)
                ok = colocar_alien(j, x, y, tipo, hp, dx);
        }
        if (ok) descuenta(tipo);
        else break; // fila superior llena -> pospone
    }
}

/* 1 intento por turno +, con 30%, un segundo; respeta inventario y tope */
void spawn_turno(struct Juego *j) {
    if (!j || !j->t) return;

    int total_inv = inv_drone + inv_skater + inv_especial;
    if (total_inv <= 0) return;
    if (j->vivos >= vivos_tope) return;

    int intentos = 1 + ((rand()%100) < 30);  // 1 o 2
    int y = j->t->H - 1;

    for (int k = 0; k < intentos; ++k) {
        if (j->vivos >= vivos_tope) break;
        total_inv = inv_drone + inv_skater + inv_especial;
        if (total_inv <= 0) break;

        int tipo = pick_tipo(); if (tipo < 0) break;
        int hp = (tipo == 0) ? 2 : 1;
        int dx = (tipo == 1) ? (rand()%2 ? 1 : -1) : 0;

        int col = rand() % j->t->W;
        bool ok = colocar_alien(j, col, y, tipo, hp, dx);
        if (!ok) {
            for (int x = 0; x < j->t->W && !ok; ++x)
                ok = colocar_alien(j, x, y, tipo, hp, dx);
        }
        if (ok) descuenta(tipo);
        else break; // fila superior llena -> detén el resto de intentos
    }
}
