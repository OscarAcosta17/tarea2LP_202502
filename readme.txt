Nombre: Oscar Acosta
Rol: 202373511-6


INSTRUCCIONES:
    - estar en un entorno linux, yo en mi caso desarrollé esta tarea desde windows usando WSL : 6.6.87.2-microsoft-standard-WSL2 y en el compilador g++ : g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    - Se compila usando: make run 
    EXTRA: 
        le añadi valgrid al makefile para yo mismo revisar asi que lo dejare por si les sirve, usando el comando "make mem" se puede ver y con "make memv" con modo estricto 

SUPUESTOS:
    - si los aliens no pueden moverse porque su casilla estaba usada, entonces bajan 1 casilla.
    - el movimiento del alien especial es medianamente aleatorio (tiene un set de movimientos y se elige al azar) lo hice así pa hacerlo más dificil
    - añadi una variable al main.h en el struct de juego que sirve para ver la derrota, 0 si estas jugando, 1 si perdiste y 2 si ganaste
    - el codigo considera que el jugador no se puede salir del tablero y esta acción no cuenta como turno
    - el movimiento consume 1 turno
    - el disparar consume 1 turno
    - el HUD no es igual al que se mostro en la ayudantia pero realmente era feo y lo hice mas bonito
    - cuando disparas y fallas se consume un turno igualmente

Movimiento de los aliens:
Drone (tipo 0): "D"
    -Movimiento simple: baja recto una fila en cada turno de movimiento.
    -No cambia de columna, no rebota.
    Vida: 2 puntos.

Skater (tipo 1): "S"
    -Movimiento zig zag diagonal:
        -Cada turno baja una fila y se desplaza una columna a la izquierda o a la derecha según su dirección actual (dx).
        -Si toca un borde del tablero, rebota: invierte la dirección horizontal y sigue bajando en diagonal
        -Si la casilla diagonal está ocupada, entonces baja recto en su misma columna.
    -Vida: 1 punto

Especial (tipo 2): "E"
    -Movimiento tipo caballo de ajedrez hacia abajo:
        -Intenta moverse en forma de “L” (dos filas abajo y 1 columna al lado, o una fila abajo y 2 columnas al lado).
        -Elige aleatoriamente entre las 4 variantes posibles lo que hace más dificil saber donde estará
        -Si no encuentra posición válida, baja recto una fila como un Drone.
    -Vida: 2 puntos

Armas del juego:
Normal (id 1):
    -Dispara un láser recto hacia arriba en la columna del jugador.
    -Impacta solo al primer alien que encuentre en esa columna.
    -Daño: 1 punto.
    -Munición: infinita

Perforador (id 2):
    -Dispara un rayo que atraviesa toda la columna del jugador
    -Impacta a todos los aliens en esa columna.
    -Daño: 1 punto por alien impactado.
    -Munición: limitada a 7 

Especial= LANZAFUEGOS (id 3): 
    -basicamente es la lanzafuegos del clash royale :D

    -Dispara hacia arriba en la columna en donde esta el jugador
    -Busca el primer alien que encuentre en esa columna.
    -Al impactar, la bala se dispersa en un cono invertido que cubre hasta 2 filas más arriba, de la siguiente forma:

    xxxxx   ← 1 de daño
     xxx    ← 1 de daño
      x     ← 2 de daño (impacto directo)
      ^
    jugador

    -Daño:
        - Impacto directo (centro): 2 puntos de daño.
        -Fila siguiente (3 casillas): 1 punto de daño.
        -Fila siguiente (5 casillas): 1 punto de daño.
    -Munición: 3 en modo facil y 2 en modo dificil
    
BONUS:
-25% de tener +1 bala perforadora al matar un alien
-10% de tener +1 bala especuial al matar un alien