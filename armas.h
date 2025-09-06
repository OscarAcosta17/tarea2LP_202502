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

/*
 * Nombre: disparar_armas
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene las armas, munición y funciones de disparo.
 *      - int arma_id: identificador del arma a disparar(0 = normal, 1 = perforador, 2 = especial).
 * Retorno:
 *      - bool:
 *          - true si el disparo fue realizado exitosamente.
 *          - false si no se disparó (puntero inválido, arma_id inválido, o falta de munición en armas especiales).
 * Descripción:
 *      Ejecuta el disparo de un arma seleccionada por el jugador. 
 *      - Verifica que el puntero y el identificador sean válidos.
 *      - Si el arma requiere munición limitada (perforador o especial),valida que haya cartuchos disponibles; de lo contrario, muestra un mensaje de advertencia y cancela la acción.
 *      - Llama a la función de disparo correspondiente desde el arreglo de punteros a función de armas.
 *      - Si el disparo fue exitoso, descuenta la munición correspondiente.
 */
bool disparar_armas(struct Juego *juego, int arma_id); /* Tres armas */

/*
 * Nombre: arma_normal
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene el tablero, la posición del jugador y las celdas con aliens.
 * Retorno:
 *      - bool:
 *          - true si el disparo fue ejecutado (con impacto o no).
 *          - false si el puntero al juego o al tablero es inválido.
 * Descripción:
 *      Simula el disparo con el arma normal (munición infinita).
 *      - Recorre la columna actual del jugador desde la fila inferior hasta la superior.
 *      - Si encuentra un alien en la trayectoria, incrementa el daño pendiente de la celda, calcula la vida restante del alien y muestra un mensaje del impacto.
 *      - Si no se encuentra ningún alien en toda la columna, se muestra un mensaje de fallo.
 *      El arma normal no consume munición y siempre retorna true cuando se ejecuta.
 */
bool arma_normal (struct Juego *juego); /* id 0 */

/*
 * Nombre: arma_perforador
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene el tablero, la posición del jugador y las celdas con aliens.
 * Retorno:
 *      - bool:
 *          - true si el disparo fue ejecutado (impactando a uno o varios enemigos, o fallando si la columna estaba vacía).
 *          - false si el puntero al juego o al tablero es inválido.
 * Descripción:
 *      Simula el disparo con el arma perforadora.
 *      - Recorre toda la columna actual del jugador de abajo hacia arriba.
 *      - A diferencia del arma normal, este proyectil atraviesa múltiples aliens, por lo que aplica daño a todos los que encuentre en la trayectoria.
 *      - Por cada impacto incrementa el daño pendiente en la celda, calcula la vida restante del alien y muestra un mensaje con el tipo de alien afectado 
 *        (Drone, Skater o Especial).
 *      - Si no había aliens en la columna, muestra un mensaje indicando fallo.
 *      El arma perforadora consume una unidad de munición al usarse.
 */
bool arma_perforador (struct Juego *juego); /* id 1 */

/*
 * Nombre: arma_especial
 * Parámetros:
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene el tablero, la posición del jugador y las celdas con aliens.
 * Retorno:
 *      - bool:
 *          - true si el disparo fue ejecutado (con impacto o fallo).
 *          - false si el puntero al juego o al tablero es inválido.
 * Descripción:
 *      Simula el disparo con el arma especial tipo "lanzafuegos".
 *      - Busca el primer alien en la columna actual del jugador; si no lo encuentra, muestra un mensaje de fallo pero consume igualmente turno y munición.
 *      - Si encuentra un alien, aplica un patrón de daño en forma de cono:
 *          - En la celda de impacto (columna del jugador) inflige 2 de daño.
 *          - En las filas superiores inmediatas, inflige 1 de daño a un rango cada vez mayor:
 *              • fila +1 → ancho de 3 celdas
 *              • fila +2 → ancho de 5 celdas
 *      - Por cada alien alcanzado, se suma el daño pendiente, se calcula la vida restante y se imprime un mensaje indicando tipo, posición e impacto.
 *      El arma especial consume una unidad de munición al usarse.
 */
bool arma_especial (struct Juego *juego); /* id 2 */
#endif