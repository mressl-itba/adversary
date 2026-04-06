# Level 3: El Adversario

En esta práctica vas a **diseñar y programar una inteligencia artificial** capaz de jugar **Reversi** (también conocido como **Othello**) de manera competitiva.

## Contexto: La Sombra en el Tablero

![Logo](img/logo.jpg)

**Año 2026.** Durante décadas, la humanidad ha confiado en simulaciones y algoritmos para resolver problemas complejos: trayectorias espaciales, identificación de lenguajes, predicción de fenómenos.

Pero ahora surge un nuevo desafío, uno que no se resuelve con física ni con estadística de trigramas.

Un misterioso programa llamado **El Adversario** ha aparecido en la red oscura de la universidad. Nadie sabe quién lo creó. Algunos dicen que es un proyecto abandonado de un exalumno genio; otros murmuran que es una entidad que simplemente **aprendió a no tolerar la derrota**.

Lo único cierto es que desafía a cualquiera a enfrentarlo en un tablero de Reversi 8×8 y, hasta ahora, ha derrotado a todos los que lo han intentado.

**Tu misión**: construir una IA capaz de plantarle cara… y, si es posible, derrotarlo.

No alcanza con hacer jugadas válidas. Debes pensar como un adversario real: anticipar movimientos, evaluar posiciones y tomar decisiones bajo presión de tiempo.

Si logras vencerlo, el programa revelará un mensaje oculto… y quizás un secreto sobre su origen. Si fallas, simplemente te recordará que “la perfección no existe… todavía”.

El tablero te espera.

El Adversario ya hizo su jugada.

## Reglas del Reversi (recordatorio rápido)

- Tablero de **8×8**.
- Dos jugadores: fichas **negras** y **blancas**.
- Posición inicial: 4 fichas en el centro, en forma cruzada.
- Juega primero el jugador **negro**.
- Un movimiento es válido si **flanquea** fichas del oponente:

  - Debe existir una línea recta (horizontal, vertical o diagonal)
  - con fichas del oponente entre la ficha colocada y otra ficha propia.
  - Todas las fichas flanqueadas se voltean.

- Si un jugador no tiene movimientos válidos, **pasa turno**.
- La partida termina cuando:

  - el tablero está lleno, o
  - ninguno de los jugadores puede mover.

- Gana quien tenga **más fichas** al final.

El repositorio incluye una implementación completa del juego para que puedas probar tu agente contra humanos o contra otro agente adversario.

## Tu misión

Debes desarrollar un módulo que represente tu **agente inteligente** (dale un nombre memorable).

El framework del repositorio te permitirá enfrentarlo contra otros jugadores, incluido El Adversario.

### 1. Punto de partida

Parte de uno de los agentes provistos:

- `Starter agent`: juega la primera jugada válida disponible.
- `Greedy agent`: elige la jugada que maximiza la ganancia inmediata.

### 2. Búsqueda adversaria (obligatorio)

Implementa **Minimax con poda Alpha-Beta**:

- Debes manejar profundidad de búsqueda limitada.
- Usa **iterative deepening** para aprovechar el tiempo disponible.
- Control de interrupción:

  - Tu algoritmo debe llamar periódicamente a la función `StopRequested`.
  - Si devuelve `true`, debes **interrumpir inmediatamente** la búsqueda y devolver la mejor jugada encontrada hasta el momento.

### 3. Función de evaluación heurística (la clave)

Cuando no puedas explorar hasta el final de la partida, necesitas evaluar qué tan “buena” es una posición intermedia.

Diseña una función de evaluación que combine varios factores. Algunas ideas:

- **Diferencia de fichas** (más relevante en el endgame).
- **Movilidad:** cantidad de jugadas disponibles (propias vs. del oponente).
- **Estabilidad:**

  - Esquinas: extremadamente valiosas.
  - Bordes: generalmente favorables.

- **Posiciones peligrosas:** evita jugadas que habiliten esquinas al oponente.
* **Control por fases**:

  * Early game: prioriza movilidad.
  * Mid game: equilibrio.
  * Endgame: maximiza fichas.

* **Patrones conocidos**:

  * Penaliza configuraciones peligrosas (por ejemplo, casillas tipo “X” cerca de esquinas).

Puedes hacer que la evaluación sea **dinámica** según la fase de la partida (cantidad de fichas en el tablero).

### 4. Optimizaciones (para destacar)

- **Ordenamiento de movimientos**: prueba primero los movimientos que parecen más prometedores (usando una heurística rápida) para mejorar la poda.
- **Tabla de transposición** (opcional avanzado): guarda evaluaciones de posiciones ya vistas para evitar recalcular.
- **Evaluación más sofisticada**: experimenta con ajuste de pesos, combinaciones no lineales, etc.
- **Gestión de tiempo**: tu IA debe estar preparada para ser interrumpida en cualquier momento.

### 5. Entrega

Debes entregar:

- El código de tu agente.
- Un archivo `ENTREGA.md` donde documentes:
  - Descripción de tu algoritmo (Minimax + Alpha-Beta).
  - Detalles de tu función de evaluación (qué factores consideraste, por qué los elegiste, cómo los ponderaste).
  - Resultados de pruebas que hayas realizado.
  - Estadísticas: profundidad promedio, nodos explorados, tiempo por jugada.
  - Dificultades encontradas y cómo las resolviste.
  - Reflexión crítica: ¿qué tan bien escala tu IA? ¿Qué debilidades posee frente a estrategias humanas? ¿Qué mejorarías con más tiempo?

## Bonus points 🚀 (sorpréndenos)

- Implementa **Monte Carlo Tree Search (MCTS)**.
- Integra **aprendizaje por refuerzo** haciendo que tu IA juegue miles de partidas contra sí misma para optimizar sus pesos y evolucionar.
- Soporta **tableros de tamaño variable** (6×6, 8×8, 10×10 y más).
- Evalúa y compara tu IA contra motores clásicos de referencia como **Edax** y **Zebra**.
- Permite ajustar la **personalidad** de la IA (agresiva, defensiva, posicional, equilibrada, etc.) mediante parámetros configurables.

## Recomendaciones

- Usa estructuras de datos eficientes (tablero como array 2D o bitboards para velocidad avanzada).
- Avanza en forma incremental: primero movimientos válidos, luego Minimax sin poda, luego Alpha-Beta, luego heurística.
- **No modifiques** el motor de juego.
- Usa Git. Commitea frecuentemente.
* Antes de pedir ayuda, ¡investiga y experimenta!

## Epílogo

Después de muchas noches afinando podas y ajustando pesos, tu IA finalmente logró vencer a El Adversario.

La pantalla parpadea.

Aparece un mensaje:

> “Interesante… No eres como los demás.  
> Quizás el verdadero adversario nunca fue el tablero.  
> Bienvenido al siguiente nivel.”

El código fuente del Adversario se desbloquea parcialmente.

Pero eso… será parte de otra historia.
