#include "entidades.h"
#include "tablero.h"
#include "main.h"
#include "spawn.h"

#include <ctype.h> 
#include <stdbool.h>
#include <stdlib.h>

// --- JUGADOR ---
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

#include <stdlib.h>   // calloc, free
#include <stdbool.h>

void mover_aliens(struct Juego *juego){
    if (!juego || !juego->t) return;
    if (juego->derrota) return;
    if ((juego->turno % 2) != 0) return;   // solo en turnos pares

    Tablero *t = juego->t;
    int H = t->H, W = t->W;

    // marca: moved[y*W + x] == true si una celda llegó aquí este turno
    bool *moved = (bool*)calloc(H * W, sizeof(bool));
    if (!moved) return;

    for (int i = H - 1; i >= 1; --i) {
        for (int j = 0; j < W; ++j) {
            // si aquí ya llegó alguien este turno, no muevas ese contenido otra vez
            if (moved[i*W + j]) continue;

            Celda *cel = (Celda*) t->celdas[i][j];
            if (!cel || !cel->alien) continue;

            Alien *a = cel->alien;
            int nj = j;
            if (a->tipo == 1) {                 // Skater: paso horizontal en la misma fila
                int hj = j + a->dx;
                if (hj >= 0 && hj < W && t->celdas[i][hj] == NULL && !moved[i*W + hj]) {
                    nj = hj;
                } else {
                    a->dx = -a->dx;             // rebote, baja recto
                }
            }

            int ni = i - 1;

            // mover solo si el destino está libre y aún no recibió alguien este turno
            if (t->celdas[ni][nj] == NULL && !moved[ni*W + nj]) {
                t->celdas[i][j] = NULL;
                a->x = nj; a->y = ni;
                t->celdas[ni][nj] = (void*)cel;

                // marca que en (ni,nj) ya llegó alguien este turno -> evita doble-movimiento
                moved[ni*W + nj] = true;
            }
        }
    }

    // derrota: cualquier cosa en la base (y==0)
    for (int x = 0; x < W; ++x) {
        Celda *c0 = (Celda*) t->celdas[0][x];
        if (c0 && c0->alien) { juego->derrota = 1; break; }
    }

    free(moved);
}

