#include <stdlib.h>
#include <stdio.h>
#include "tablero.h"
#include "main.h"
#include "entidades.h"

Tablero* tablero_crear(int ancho, int alto){
    Tablero *tablero= malloc(sizeof(Tablero));
    tablero-> W= ancho;
    tablero -> H= alto;

    tablero -> celdas = malloc(tablero->H * sizeof(void***));

    for(int i=0 ; i < tablero->H; i++){
        tablero -> celdas[i] = malloc(tablero->W *sizeof(void*));
        for (int j=0; j< tablero->W; j++){
            tablero -> celdas[i][j]= NULL;
        }
    }
    return tablero;
}

void tablero_cerrar(Tablero *t) {
    if (!t) return;

    // libera cada Celda* y su Alien*
    for (int y = 0; y < t->H; ++y) {
        for (int x = 0; x < t->W; ++x) {
            Celda *cel = (Celda*) t->celdas[y][x];
            if (cel) {
                free(cel->alien);   // primero el Alien*
                free(cel);          // luego la Celda*
                t->celdas[y][x] = NULL;
            }
        }
        free(t->celdas[y]);         // libera la fila (void**)
    }

    free(t->celdas);                // libera el arreglo de filas
    free(t);                        // libera el Tablero
}

void tablero_imprimir(struct Juego *juego) {
    if (!juego || !juego->t) return;
    Tablero *t = juego->t;

    if (juego->jugador_x < 0) juego->jugador_x = 0;
    if (juego->jugador_x >= t->W) juego->jugador_x = t->W - 1;

    int ancho_borde = 3 * t->W + 2; // '|' + 3*W + '|'

    // borde superior
    for (int i = 0; i < ancho_borde; ++i) putchar('=');
    putchar('\n');

    // grilla (aliens si existen)
    for (int y = t->H - 1; y >= 0; --y) {
        for (int x = 0; x < t->W; ++x) {
            Celda *cel = (Celda*) t->celdas[y][x];
            if (cel && cel->alien) {
                char ch = (cel->alien->tipo == 0) ? 'D'
                         : (cel->alien->tipo == 1) ? 'S' : 'E';
                printf("[%c]", ch);
            } else {
                fputs("[.]", stdout);
            }
        }
        putchar('\n');
    }
    putchar('\n');

    int caret_pos = 1 + 3 * juego->jugador_x ; // centra bajo la celda
    for (int i = 0; i < caret_pos; ++i) putchar(' ');
    putchar('^');
    putchar('\n');
    for (int i = 0; i < ancho_borde; ++i) putchar('=');
    putchar('\n');
}

void hud_imprimir(struct Juego *j) {
    printf("Turno: %d | Vivos: %d | Jugador x=%d\n", j->turno, j->vivos, j->jugador_x);
}