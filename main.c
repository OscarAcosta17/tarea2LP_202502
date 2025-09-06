#include "main.h"
#include "armas.h"
#include "entidades.h"
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    Juego juego;
    juego.turno = 0;
    juego.vivos = 0;
    juego.derrota= 0;

    juego.armas.fn[0] = arma_normal;
    juego.armas.fn[1] = arma_perforador;
    juego.armas.fn[2] = arma_especial;   // todavía no implementada

    juego.armas.ammo_perforador = 7;

    int opcion = 0;
    printf("Bienvenido\nSeleccione la dificultad:\n1. Facil\n2. Dificil\n> ");

    if (scanf("%d", &opcion) != 1) {
        fprintf(stderr, "Dificultad invalida.\n");
        return 1;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (opcion == 1) {
        juego.dificultad = 0;
        juego.armas.ammo_especial = 3;
        juego.t = tablero_crear(5, 15);

    } else if (opcion == 2) {
        juego.dificultad = 1;
        juego.armas.ammo_especial = 2;
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

    while (true) {
        hud_imprimir(&juego);
        printf("\nAcciones:");
        printf("\n[a] izq | [d] der | [1] Normal | [2] Perforador | [3] Especial | [q] Salir (otra = no hace nada :D )\n> ");

        int ch = getchar(); 
        int exceso;
        while ((exceso = getchar()) != '\n' && exceso != EOF);

        if (ch == EOF || ch == 'q' || ch == 'Q') { // Rendirse
            puts("Saliendo...");
            break;
        }
        bool accion_valida = false;
        printf("\nEventos:\n");
        if (ch == '1' || ch == '2' || ch == '3') { // Disparo
            int id = ch - '1';
            accion_valida = disparar_armas(&juego, id);
        }
    
        if (mover_jugador(&juego, (char)ch)) { // Movimiento
            accion_valida = true;
        }

        if (accion_valida) {
            resolver_danos(&juego);
            if (juego.derrota == 2) { //Victoria
                tablero_imprimir(&juego);
                puts("\nVICTORIA: eliminaste todos los aliens!");
                break;
            }
            juego.turno++;
            spawn_turno(&juego);
            
            if ((juego.turno % 2) == 0) {
                mover_aliens(&juego);
                if (juego.derrota) { //Derrota
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