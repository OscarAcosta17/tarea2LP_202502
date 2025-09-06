#ifndef ARMAS_H
#define ARMAS_H
#include <stdbool.h>

struct Juego; /* declaracion adelantada para usar la siguiente... */
typedef bool (*FuncArmas)(struct Juego* j);

typedef struct {
    int ammo_perforador; /* arma 2 */
    int ammo_especial; /* arma 3 */
    FuncArmas fn[3]; /* 0..2 mapean las 3 armas */
} Armas;

bool disparar_armas(struct Juego *juego, int arma_id); /* Tres armas */
bool arma_normal (struct Juego *juego); /* id 0 */

bool arma_perforador (struct Juego *juego); /* id 1 */
bool arma_especial (struct Juego *juego); /* id 2 */

#endif