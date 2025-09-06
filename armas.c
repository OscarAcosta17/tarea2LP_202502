#include "armas.h"
#include "main.h"
#include "tablero.h"
#include "entidades.h"
#include <stdio.h>
#include <stdbool.h>

bool disparar_armas(struct Juego *juego, int arma_id) {
    if (!juego) return false;
    if (arma_id < 0 || arma_id > 2) return false;

    if (arma_id == 1 && juego->armas.ammo_perforador <= 0) { 
        printf(">>> Sin munición perforadora!\n");
        return false;
    }
    if (arma_id == 2 && juego->armas.ammo_especial <= 0) {
        printf(">>> Sin munición especial!\n");
        return false;
    }

    bool ok = juego->armas.fn[arma_id](juego);

    if (ok) { // restar la municion
        if (arma_id == 1) juego->armas.ammo_perforador--;
        if (arma_id == 2) juego->armas.ammo_especial--;
    }
    return ok;
}

// ==================== ARMAS ====================
bool arma_normal(struct Juego *juego) {
    if (!juego || !juego->t) return false;

    int col = juego->jugador_x;

    for (int y = 0; y < juego->t->H; ++y) {
        Celda *cel = (Celda*) juego->t->celdas[y][col];
        if (cel && cel->alien) {
            cel->dano_pend += 1;

            int vida_restante = cel->alien->hp - cel->dano_pend; //calculo del daño
            if (vida_restante < 0) vida_restante = 0;

            printf(">>> [DISPARO NORMAL] Impacto en (%d,%d). Vida restante: %d <<<\n", col, y, vida_restante);
            return true;
        }
    }
    printf(">>> [DISPARO NORMAL] Falló: la columna está vacía <<<\n");
    return true;
}

bool arma_perforador(struct Juego *juego) {
    if (!juego || !juego->t) return false;

    int col = juego->jugador_x;
    int hits = 0;

    for (int y = 0; y < juego->t->H; ++y) {
        Celda *cel = (Celda*) juego->t->celdas[y][col];
        if (cel && cel->alien) {
            cel->dano_pend += 1;

            int vida_restante = cel->alien->hp - cel->dano_pend; //calculo del daño
            if (vida_restante < 0) vida_restante = 0;

            const char *nombre_tipo =
                (cel->alien->tipo == 0) ? "Drone" :
                (cel->alien->tipo == 1) ? "Skater" : "Especial";

            printf(">>> [PERFORADOR] Impacto a %s en (%d,%d). Vida restante: %d <<<\n",nombre_tipo, col, y, vida_restante);
            hits++;
        }
    }
    if (hits == 0) {
        printf(">>> [PERFORADOR] Falló: columna vacía <<<\n");
    }
    return true;
}

bool arma_especial(struct Juego *juego) {
    if (!juego || !juego->t) return false;

    int col = juego->jugador_x;
    int H = juego->t->H, W = juego->t->W;

    int hit_y = -1;
    for (int y = 0; y < H; ++y) {
        Celda *cel = (Celda*) juego->t->celdas[y][col];
        if (cel && cel->alien) { hit_y = y; break; }
    }
    if (hit_y < 0) {
        printf(">>> [LANZAFUEGOS] Falló: no había alien en la columna <<<\n");
        return true; // consume turno/munición igual
    }

    for (int dy = 0; dy <= 2; ++dy) {
        int y = hit_y + dy;                 // filas por encima del impacto (más lejos del jugador)
        if (y >= H) break;

        int half = dy;                      // 0,1,2  → 1,3,5 de ancho
        int x0 = col - half;
        int x1 = col + half;
        if (x0 < 0) x0 = 0;
        if (x1 >= W) x1 = W - 1;

        for (int x = x0; x <= x1; ++x) {
            Celda *c = (Celda*) juego->t->celdas[y][x];
            if (c && c->alien) {
                int dmg = (dy == 0 && x == col) ? 2 : 1;  // centro en impacto: 2; el resto: 1
                c->dano_pend += dmg;

                int vida_rest = c->alien->hp - c->dano_pend;
                if (vida_rest < 0) vida_rest = 0;

                const char *tipo =
                    (c->alien->tipo == 0) ? "Drone" :
                    (c->alien->tipo == 1) ? "Skater" : "Especial";

                printf(">>> [LANZAFUEGOS] %s en (%d,%d) recibe %d daño. Vida restante: %d <<<\n",
                       tipo, x, y, dmg, vida_rest);
            }
        }
    }
    return true;
}