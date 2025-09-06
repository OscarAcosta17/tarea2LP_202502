#include "main.h"
#include "entidades.h"
#include <stdio.h>

int main(void) {
    Juego juego;
    juego.turno = 0;
    juego.vivos = 0;
    juego.derrota= 0;

    int opcion = 0;

    printf("Bienvenido\nSeleccione la dificultad:\n1. Facil\n2. Dificil\n> ");
    if (scanf("%d", &opcion) != 1) {
        fprintf(stderr, "Dificultad invalida.\n");
        return 1;
    }
    if (opcion == 1) {
        juego.dificultad = 0;
        juego.t = tablero_crear(5, 15);

    } else if (opcion == 2) {
        juego.dificultad = 1;
        juego.t = tablero_crear(7, 15);

    } else {
        fprintf(stderr, "Dificultad invalida.\n");
        return 1;
    }

    if (!juego.t) {
        fprintf(stderr, "Error al crear tablero\n");
        return 1;
    }

    juego.jugador_x = juego.t->W / 2;
    spawn_inicio(&juego);

    tablero_imprimir(&juego);
    while (true){
        hud_imprimir(&juego);
        printf("\n[a] izq, [d] der (otra = no hace nada :D )\n> ");

        int ch = getchar(); while (ch == '\n') ch = getchar();

        if (ch == EOF || ch == 'q' || ch == 'Q') { //salida
            puts("Saliendo...");
            break;
        }
        

        if (mover_jugador(&juego, (char)ch)) {
            juego.turno++;
            if ((juego.turno % 2) == 0) {
                mover_aliens(&juego);
                if(juego.derrota){
                    tablero_imprimir(&juego);
                    puts("DERROTA: un alien llegó a la base.");
                    break;
                }
            }
        }
        printf("====================");
        tablero_imprimir(&juego);
    }

    tablero_cerrar(juego.t);
    juego.t = NULL;

    return 0;
}
