#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_DIMENSION 13
#define MAX_DIMENSION 100
#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[MAX_DIMENSION * MAX_DIMENSION];
    int front, rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

void enqueue(Queue *q, Point p) {
    q->points[q->rear++] = p;
}

Point dequeue(Queue *q) {
    return q->points[q->front++];
}

// Generar un laberinto aleatorio más confuso
void generateMaze(char maze[MAX_DIMENSION][MAX_DIMENSION], int rows, int cols) {
    srand(time(NULL));

    // Determine entry side (0: top, 1: bottom, 2: left, 3: right)
    int entry_side = rand() % 4;
    int exit_side = (entry_side + 2) % 4; // Opposite side of entry

    // Place entry and exit
    Point entry, exit;
    switch (entry_side) {
        case 0: // Top side
            entry.x = rand() % cols;
            entry.y = 0;
            break;
        case 1: // Bottom side
            entry.x = rand() % cols;
            entry.y = rows - 1;
            break;
        case 2: // Left side
            entry.x = 0;
            entry.y = rand() % rows;
            break;
        case 3: // Right side
            entry.x = cols - 1;
            entry.y = rand() % rows;
            break;
    }

    switch (exit_side) {
        case 0: // Top side
            exit.x = rand() % cols;
            exit.y = 0;
            break;
        case 1: // Bottom side
            exit.x = rand() % cols;
            exit.y = rows - 1;
            break;
        case 2: // Left side
            exit.x = 0;
            exit.y = rand() % rows;
            break;
        case 3: // Right side
            exit.x = cols - 1;
            exit.y = rand() % rows;
            break;
    }

    // Initialize maze with walls and paths
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                maze[i][j] = WALL;  // Border wall
            } else {
                maze[i][j] = (rand() % 2 == 0) ? WALL : PATH;  // 1/2 probability of wall
            }
        }
    }

    maze[entry.y][entry.x] = START;  // Place start
    maze[exit.y][exit.x] = END;  // Place end
}

// Mostrar el laberinto
void printMaze(char maze[MAX_DIMENSION][MAX_DIMENSION], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

// Buscar el camino más corto con BFS
int bfs(char maze[MAX_DIMENSION][MAX_DIMENSION], int rows, int cols, Point start, Point end) {
    int steps[MAX_DIMENSION][MAX_DIMENSION] = {0};
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    Queue queue;
    initQueue(&queue);
    enqueue(&queue, start);

    while (!isEmpty(&queue)) {
        Point p = dequeue(&queue);

        if (p.x == end.x && p.y == end.y) {
            return steps[p.y][p.x];
        }

        for (int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < cols && ny < rows &&
                maze[ny][nx] != WALL && steps[ny][nx] == 0) {
                steps[ny][nx] = steps[p.y][p.x] + 1;
                enqueue(&queue, (Point){nx, ny});
            }
        }
    }
    return -1;  // No se encontró camino
}

int main() {
    int rows, cols;

    do {
        printf("Ingrese el número de filas (entre %d y %d): ", MIN_DIMENSION, MAX_DIMENSION);
        scanf("%d", &rows);
        printf("Ingrese el número de columnas (entre %d y %d): ", MIN_DIMENSION, MAX_DIMENSION);
        scanf("%d", &cols);

        if (rows < MIN_DIMENSION || rows > MAX_DIMENSION || cols < MIN_DIMENSION || cols > MAX_DIMENSION) {
            printf("Dimensiones no válidas. Por favor, intente nuevamente.\n");
        }
    } while (rows < MIN_DIMENSION || rows > MAX_DIMENSION || cols < MIN_DIMENSION || cols > MAX_DIMENSION);

    char maze[MAX_DIMENSION][MAX_DIMENSION];
    generateMaze(maze, rows, cols);

    Point start, end;
    // Find start (S) and end (E) positions
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == START) {
                start.x = j;
                start.y = i;
            } else if (maze[i][j] == END) {
                end.x = j;
                end.y = i;
            }
        }
    }

    printf("Laberinto generado:\n");
    printMaze(maze, rows, cols);

    int shortestPath = bfs(maze, rows, cols, start, end);
    if (shortestPath != -1) {
        printf("El camino más corto tiene %d pasos.\n", shortestPath);
    } else {
        printf("No se encontró un camino del inicio al fin.\n");
    }

    return 0;
}
