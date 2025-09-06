#ifndef TABLERO_H
#define TABLERO_H

struct Juego;  // forward declaration

typedef struct Tablero {
    int W, H;
    void ***celdas;
} Tablero;

/*
 * Nombre: tablero_crear
 * Parámetros: 
 *      - int ancho: número de columnas del tablero.
 *      - int alto: número de filas del tablero.
 * Retorno: 
 *      - Tablero*: puntero a una estructura Tablero recién creada e inicializada.
 * Descripción: 
 *      Reserva memoria para un tablero de dimensiones `alto` x `ancho`. 
 *      Inicializa todas las celdas con NULL para indicar que están vacías.
 *      Devuelve un puntero al tablero creado.
 */
Tablero* tablero_crear(int ancho, int alto);

/*
 * Nombre: tablero_cerrar
 * Parámetros: 
 *      - Tablero *t: puntero al tablero que se desea liberar.
 * Retorno: 
 *      - void (no retorna nada).
 * Descripción: 
 *      Libera toda la memoria asociada al tablero `t`. 
 *      - Recorre cada celda del tablero y libera primero el alien (si existe) y luego la celda.
 *      - Libera cada fila del arreglo de celdas.
 *      - Finalmente libera el arreglo de celdas y la estructura Tablero.
 *      Deja el puntero `t` inválido, por lo que no debe usarse tras la llamada.
 */
void tablero_cerrar(Tablero *tablero);

/*
 * Nombre: tablero_imprimir
 * Parámetros: 
 *      - struct Juego *juego: puntero a la estructura principal del juego, que contiene el tablero y la posición del jugador.
 * Retorno: 
 *      - void (no retorna nada).
 * Descripción: 
 *      Imprime el estado actual del tablero en la consola. 
 *      - Muestra un borde superior e inferior con '='.
 *      - Recorre todas las filas y columnas del tablero desde arriba hacia abajo.
 *      - En cada celda imprime:
 *          - 'D' si hay un alien de tipo 0.
 *          - 'S' si hay un alien de tipo 1.
 *          - 'E' si hay un alien de tipo 2.
 *          - '.' si la celda está vacía.
 *      - Al final muestra la posición actual del jugador con el carácter '^'.
 *      Corrige automáticamente si la posición del jugador está fuera de los límites.
 */
void tablero_imprimir(struct Juego *juego);

/*
 * Nombre: hud_imprimir
 * Parámetros: 
 *      - struct Juego *j: puntero a la estructura principal del juego que contiene información de turno, aliens, inventario y armas.
 * Retorno: 
 *      - void (no retorna nada).
 * Descripción: 
 *      Imprime en la consola el HUD (Heads-Up Display) con información 
 *      relevante del estado actual del juego:
 *        - Número de turno actual.
 *        - Aliens vivos en el tablero en relación con el máximo permitido.
 *        - Inventario restante de cada tipo de alien (Drone, Skater, Especial).
 *        - Munición disponible para cada tipo de arma (Normal es infinita, Perforador y Especial son limitadas).
 *        - Columna en la que se encuentra el jugador.
 */
void hud_imprimir(struct Juego *j);
#endif