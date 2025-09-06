#ifndef TABLERO_H
#define TABLERO_H

struct Juego;  // forward declaration

typedef struct Tablero {
    int W, H;
    void ***celdas;
} Tablero;

Tablero* tablero_crear(int ancho, int alto);
void tablero_cerrar(Tablero *tablero);
void tablero_imprimir(struct Juego *juego);
void hud_imprimir(struct Juego *j);

#endif
