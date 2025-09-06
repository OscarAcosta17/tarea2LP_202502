#include "entidades.h"
#include "tablero.h"
#include "main.h"
#include "spawn.h"

#include <ctype.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/////////////////////////// Jugador /////////////////////////////
bool mover_jugador_izq(struct Juego *juego) {
    if (!juego || !juego->t) return false;
    if (juego->jugador_x > 0) {
        juego->jugador_x--;
        spawn_turno(juego);
        return true;
    }
    return false;
}

bool mover_jugador_der(struct Juego *juego) {
    if (!juego || !juego->t) return false;
    int max_x = juego->t->W - 1;
    if (juego->jugador_x < max_x) {
        juego->jugador_x++;
        spawn_turno(juego);
        return true;
    }
    return false;
}

bool mover_jugador(struct Juego *juego, char c) {
    if (!juego || !juego->t) return false;
    c = (char)tolower((unsigned char)c);
    if (c == 'a' || c == 'A') return mover_jugador_izq(juego);
    if (c == 'd'|| c == 'D') return mover_jugador_der(juego);
    return false; 
}

///////////////////////// aliens /////////////////////////////
void mover_aliens(struct Juego *juego) {
    if (!juego || !juego->t) return;
    if (juego->derrota) return;

    Tablero *t = juego->t;
    int H = t->H, W = t->W;

    bool *moved = (bool*)calloc(H * W, sizeof(bool));
    if (!moved) return;

    for (int i = H - 1; i >= 1; --i) {
        for (int j = 0; j < W; ++j) {
            if (moved[i*W + j]) continue;

            Celda *cel = (Celda*) t->celdas[i][j];
            if (!cel || !cel->alien) continue;

            Alien *a = cel->alien;
            int ni = i - 1; //una fila menos
            int nj = j;//misma columna

            if (a->tipo == 1) { //skater
                nj = j + a->dx;
                if (nj < 0) {
                    nj = 1;
                    a->dx = +1;
                } else if (nj >= W) {
                    nj = W - 2;
                    a->dx = -1;
                }

                if (!(t->celdas[ni][nj] == NULL && !moved[ni*W + nj])) {
                    nj = j;
                }
            }

            if (a->tipo == 2) { // Especial
                int movs[4][2] = {
                    {-2, 1}, {-2, -1}, {-1, 2}, {-1, -2}
                };

                int idx = rand() % 4;
                int ti = i + movs[idx][0];
                int tj = j + movs[idx][1];

                if (ti >= 0 && ti < H && tj >= 0 && tj < W &&
                    t->celdas[ti][tj] == NULL && !moved[ti*W + tj]) {
                    t->celdas[i][j] = NULL;
                    a->x = tj; a->y = ti;
                    t->celdas[ti][tj] = (void*)cel;
                    moved[ti*W + tj] = true;
                    continue;
                }
            }
            if (ni >= 0 && t->celdas[ni][nj] == NULL && !moved[ni*W + nj]) {//movimiento si ta libre la celda
                t->celdas[i][j] = NULL;
                a->x = nj; a->y = ni;
                t->celdas[ni][nj] = (void*)cel;
                moved[ni*W + nj] = true;
            }
        }
    }

    for (int x = 0; x < W; ++x) { // pa cachar la derrota
        Celda *c0 = (Celda*) t->celdas[0][x];
        if (c0 && c0->alien) { juego->derrota = 1; break; }
    }
    free(moved);
}

void resolver_danos(struct Juego *juego) {
    if (!juego || !juego->t) return;
    Tablero *t = juego->t;

    const int PROB_ESPECIAL = 10;   // 10% para la especial
    const int AMMO_ESPECIAL = 1;    // +1 bala especial

    for (int y = 0; y < t->H; ++y) {
        for (int x = 0; x < t->W; ++x) {
            Celda *cel = (Celda*) t->celdas[y][x];
            if (!cel || !cel->alien) continue;

            if (cel->dano_pend > 0) {
                cel->alien->hp -= cel->dano_pend;
                cel->dano_pend = 0;

                if (cel->alien->hp <= 0) {
                    const char *nombre_tipo =
                        (cel->alien->tipo == 0) ? "Drone" :
                        (cel->alien->tipo == 1) ? "Skater" : "Especial";

                    printf("*** %s destruido en (%d,%d)! ***\n", nombre_tipo, x, y);

                    int r = rand() % 100; //bonus
                    if (r < 25) {
                        juego->armas.ammo_perforador += 1;
                        printf("[BONUS] Munición Perforador +1 (ahora: %d)\n", juego->armas.ammo_perforador);

                    } else if (r < 25 + PROB_ESPECIAL) {
                        juego->armas.ammo_especial += AMMO_ESPECIAL;
                        printf("[BONUS] Munición Especial +%d (ahora: %d)\n", AMMO_ESPECIAL, juego->armas.ammo_especial);
                    }

                    free(cel->alien);
                    free(cel);
                    t->celdas[y][x] = NULL;
                    juego->vivos--;
                }
            }
        }
    }
    if (juego->vivos == 0 && //victoria
        juego->pool.drone == 0 &&
        juego->pool.skater == 0 &&
        juego->pool.especial == 0) {
        juego->derrota = 2;
    }
}