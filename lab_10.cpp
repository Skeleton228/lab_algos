#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <queue>

typedef struct {
    int N;
    int** mat;
    int is_weighted; // 1 - взвешенный, 0 - невзвешенный
    int is_directed; // 1 - ориентированный, 0 - неориентированный
} Graph;

typedef struct {
    int is_weighted;
    int is_directed;
    int N;
    int p;
    int start;
    int cli_mode;
} CliConfig;

int input(const char* msg, int min, int max) {
    int val;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &val) == 1 && val >= min && val <= max) return val;
        while (getchar() != '\n');
        printf("Ошибка. Введите число от %d до %d.\n", min, max);
    }
}

void free_graph(Graph* g) {
    if (g->mat == NULL) return;
    for (int i = 0; i < g->N; i++) free(g->mat[i]);
    free(g->mat);
    g->mat = NULL;
    g->N = 0;
}

// Генерация матрицы смежности (учитывает взвешенность и ориентацию)
void generate_graph(Graph* g, int N, int p, int is_weighted, int is_directed) {
    free_graph(g);
    g->N = N;
    g->is_weighted = is_weighted;
    g->is_directed = is_directed;
    g->mat = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        g->mat[i] = (int*)calloc(N, sizeof(int));
    }

    for (int i = 0; i < N; i++) {
        int j_start = is_directed ? 0 : i + 1;
        for (int j = j_start; j < N; j++) {
            if (i == j) continue;
            if ((rand() % 100) < p) {
                int weight = is_weighted ? (1 + rand() % 9) : 1; // веса 1..9 или 1
                g->mat[i][j] = weight;
                if (!is_directed) {
                    g->mat[j][i] = weight;
                }
            }
        }
    }
}

void print_matrix(Graph* g) {
    printf("\nМатрица смежности (%s, %s):\n    ",
           g->is_weighted ? "взвешенный" : "невзвешенный",
           g->is_directed ? "ориентированный" : "неориентированный");
    for (int j = 0; j < g->N; j++) printf("%3d", j + 1);
    for (int i = 0; i < g->N; i++) {
        printf("\n%3d ", i + 1);
        for (int j = 0; j < g->N; j++) printf("%3d", g->mat[i][j]);
    }
    printf("\n");
}

// Алгоритм BFSD
// show = 1: выводить ход обхода и вектор DIST; show = 0: тихий режим для расчёта характеристик
void bfsd(Graph* g, int start, int* dist, int show) {
    for (int i = 0; i < g->N; i++) {
        dist[i] = -1; // п. 1.1: не посещена
    }

    std::queue<int> Q;
    Q.push(start);     // п. 2.2
    dist[start] = 0;   // п. 2.3

    if (show) printf("\nПорядок извлечения из очереди: ");

    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();
        if (show) printf("%d ", v + 1); // п. 2.7

        for (int i = 0; i < g->N; i++) {
            if (g->mat[v][i] > 0 && dist[i] == -1) { // п. 2.9
                Q.push(i);
                dist[i] = dist[v] + g->mat[v][i];    // п. 2.12
            }
        }
    }

    if (show) {
        printf("\nВектор расстояний DIST от вершины %d:\n", start + 1);
        for (int i = 0; i < g->N; i++) {
            if (dist[i] == -1)
                printf("До %d: нет пути (-1)\n", i + 1);
            else
                printf("До %d: %d\n", i + 1, dist[i]);
        }
    }
}

// Задание 2: Эксцентриситеты, радиус, диаметр, центральные и периферийные вершины
void analyze_graph(Graph* g) {
    int N = g->N;
    int* ecc = (int*)malloc(N * sizeof(int));
    int* dist = (int*)malloc(N * sizeof(int));

    int radius = 1000000000;
    int diameter = -1;
    int valid_count = 0;

    printf("\nМатрица расстояний и эксцентриситеты:\n");
    for (int v = 0; v < N; v++) {
        bfsd(g, v, dist, 0);

        int max_d = 0;
        int disconnected = 0;

        printf("Вершина %2d | DIST: [", v + 1);
        for (int j = 0; j < N; j++) {
            printf("%3d", dist[j]);
            if (dist[j] == -1) {
                disconnected = 1;
            } else if (dist[j] > max_d) {
                max_d = dist[j];
            }
        }
        printf(" ] | ");

        if (disconnected) {
            ecc[v] = -1; // бесконечность (не все вершины достижимы)
            printf("e(%d) = inf (не все вершины достижимы)\n", v + 1);
        } else {
            ecc[v] = max_d;
            printf("e(%d) = %d\n", v + 1, ecc[v]);
            if (ecc[v] < radius) radius = ecc[v];
            if (ecc[v] > diameter) diameter = ecc[v];
            valid_count++;
        }
    }

    if (valid_count == 0) {
        printf("\nНи из одной вершины недостижимы все остальные.\n");
        printf("Радиус и диаметр равны бесконечности.\n");
    } else {
        printf("\nРадиус графа r(G) = %d\n", radius);
        printf("Диаметр графа D(G) = %d\n", diameter);

        printf("Центральные вершины (e(v) = r(G)): ");
        for (int i = 0; i < N; i++) {
            if (ecc[i] == radius) printf("%d ", i + 1);
        }
        printf("\n");

        printf("Периферийные вершины (e(v) = D(G)): ");
        for (int i = 0; i < N; i++) {
            if (ecc[i] == diameter) printf("%d ", i + 1);
        }
        printf("\n");
    }

    free(ecc);
    free(dist);
}

// Разбор параметров командной строки
// Пример: .\lab_10.exe -w 1 -d 0 -n 6 -p 45 -s 1
void parse_args(int argc, char* argv[], CliConfig* cfg) {
    cfg->is_weighted = 1;
    cfg->is_directed = 0;
    cfg->N = 6;
    cfg->p = 45;
    cfg->start = 1;
    cfg->cli_mode = (argc > 1) ? 1 : 0;

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "-weighted") == 0) && i + 1 < argc) {
            cfg->is_weighted = atoi(argv[i + 1]) ? 1 : 0;
            i++;
        } else if ((strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-directed") == 0) && i + 1 < argc) {
            cfg->is_directed = atoi(argv[i + 1]) ? 1 : 0;
            i++;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            cfg->N = atoi(argv[i + 1]);
            if (cfg->N < 1) cfg->N = 1;
            if (cfg->N > 50) cfg->N = 50;
            i++;
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            cfg->p = atoi(argv[i + 1]);
            if (cfg->p < 0) cfg->p = 0;
            if (cfg->p > 100) cfg->p = 100;
            i++;
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            cfg->start = atoi(argv[i + 1]);
            i++;
        }
    }

    if (cfg->start < 1) cfg->start = 1;
    if (cfg->start > cfg->N) cfg->start = cfg->N;
}

// Полный неинтерактивный прогон при запуске с аргументами из консоли
void run_cli_batch(Graph* g, const CliConfig* cfg) {
    printf("Параметры запуска: [%s | %s | N = %d | p = %d%% | старт = %d]\n",
           cfg->is_weighted ? "Взвешенный" : "Невзвешенный",
           cfg->is_directed ? "Ориентированный" : "Неориентированный",
           cfg->N, cfg->p, cfg->start);

    generate_graph(g, cfg->N, cfg->p, cfg->is_weighted, cfg->is_directed);
    print_matrix(g);

    int* dist = (int*)malloc(g->N * sizeof(int));
    bfsd(g, cfg->start - 1, dist, 1);
    free(dist);

    analyze_graph(g);
    free_graph(g);
}

int main(int argc, char* argv[]) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned)time(NULL));

    CliConfig cfg;
    parse_args(argc, argv, &cfg);

    Graph g = { 0, NULL, cfg.is_weighted, cfg.is_directed };
    int p = cfg.p;

    if (cfg.cli_mode) {
        run_cli_batch(&g, &cfg);
        return 0;
    }

    while (1) {
        printf("\n========== ЛАБОРАТОРНАЯ РАБОТА №10 ==========\n");
        printf("Текущий режим: [%s | %s | p = %d%%]\n",
               g.is_weighted ? "Взвешенный" : "Невзвешенный",
               g.is_directed ? "Ориентированный" : "Неориентированный", p);
        printf("1. Сгенерировать граф\n");
        printf("2. Вывести матрицу смежности\n");
        printf("3. Поиск расстояний BFSD от заданной вершины\n");
        printf("4. Найти радиус, диаметр, центр и периферию\n");
        printf("5. Переключить тип (взвешенный / невзвешенный)\n");
        printf("6. Переключить ориентацию (ориентированный / неориентированный)\n");
        printf("7. Изменить вероятность ребра\n");
        printf("0. Выход\n");

        int choice = input("Выберите пункт: ", 0, 7);

        if (choice == 0) {
            free_graph(&g);
            break;
        }

        if (choice == 1) {
            int N = input("Количество вершин (1-50): ", 1, 50);
            generate_graph(&g, N, p, g.is_weighted, g.is_directed);
            print_matrix(&g);
            continue;
        }

        if (g.N == 0 && choice >= 2 && choice <= 4) {
            printf("Сначала сгенерируйте граф\n");
            continue;
        }

        switch (choice) {
            case 2:
                print_matrix(&g);
                break;

            case 3: {
                int start = input("Исходная вершина (1-N): ", 1, g.N) - 1;
                int* dist = (int*)malloc(g.N * sizeof(int));
                bfsd(&g, start, dist, 1);
                free(dist);
                break;
            }

            case 4:
                analyze_graph(&g);
                break;

            case 5:
                g.is_weighted = !g.is_weighted;
                printf("Режим изменён. Пересоздайте граф чтобы обновить матрицу.\n");
                break;

            case 6:
                g.is_directed = !g.is_directed;
                printf("Ориентация изменена. Пересоздайте граф чтобы обновить матрицу.\n");
                break;

            case 7:
                p = input("Вероятность ребра (0-100): ", 0, 100);
                break;
        }
    }
    return 0;
}