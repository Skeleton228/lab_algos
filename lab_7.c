#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_V 100


typedef struct {
    int N;
    int mat[MAX_V][MAX_V];
    int list[MAX_V][MAX_V];      // Списки смежности (соседи каждой вершины)
    int list_len[MAX_V];
} Graph;

int input(const char* msg, int min, int max) {
    int val;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &val) == 1 && val >= min && val <= max) {
            return val;
        }
        while (getchar() != '\n');
        printf("Ошибка ввода. Введите число от %d до %d.\n", min, max);
    }
}

// Форматированный вывод порядка обхода
void print_order(const char* title, int order[], int count) {
    printf("\n%s\nПорядок посещения: ", title);
    for (int i = 0; i < count; i++) {
        printf("%d%s", order[i] + 1, (i < count - 1) ? " -> " : "");
    }
    printf("\n");
}

// 1. Рекурсивный обход по матрице смежности
void dfs_mat_rec(Graph* g, int v, int vis[], int order[], int* idx) {
    vis[v] = 1;
    order[(*idx)++] = v;
    for (int i = 0; i < g->N; i++) {
        if (g->mat[v][i] == 1 && !vis[i]) {
            dfs_mat_rec(g, i, vis, order, idx);
        }
    }
}

// 2. Рекурсивный обход по спискам смежности
void dfs_list_rec(Graph* g, int v, int vis[], int order[], int* idx) {
    vis[v] = 1;
    order[(*idx)++] = v;
    for (int i = 0; i < g->list_len[v]; i++) {
        int neighbor = g->list[v][i];
        if (!vis[neighbor]) {
            dfs_list_rec(g, neighbor, vis, order, idx);
        }
    }
}

// 3. Нерекурсивный обход со стеком
void dfs_iterative(Graph* g, int start) {
    int vis[MAX_V] = { 0 }, order[MAX_V], stack[MAX_V];
    int idx = 0, top = -1;

    // Цикл для прохода по несвязным кускам графа
    for (int i = 0; i < g->N; i++) {
        int v = (start + i) % g->N;
        if (vis[v]) continue;

        stack[++top] = v;
        while (top >= 0) {
            int cur = stack[top--];
            if (vis[cur]) continue;

            vis[cur] = 1;
            order[idx++] = cur;

            // Добавляем соседей в обратном порядке, чтобы меньшие номера обрабатывались первыми
            for (int j = g->N - 1; j >= 0; j--) {
                if (g->mat[cur][j] == 1 && !vis[j]) {
                    stack[++top] = j;
                }
            }
        }
    }
    print_order("Нерекурсивный обход: ", order, g->N);
}


int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned)time(NULL));

    Graph g = { 0 };
    int p = 50; 

    while (1) {
        printf("1. Создать случайный граф\n");
        printf("2. Вывести матрицу смежности\n");
        printf("3. Рекурсивный обход по матрице\n");
        printf("4. Обход по спискам смежности\n");
        printf("5. Нерекурсивный обход\n");
        printf("6. Вывести списки смежности\n");
        printf("7. Указать вероятность ребра\n");
        printf("0. Выход\n");

        int choice = input("Выберите пункт меню: ", 0, 7);

        if (choice == 0) {
            printf("Работа программы завершена.\n");
            break;
        }

        if (choice == 1) {
            g.N = input("Количество вершин (1-100): ", 1, MAX_V);
            for (int i = 0; i < g.N; i++) g.list_len[i] = 0;
            for (int i = 0; i < g.N; i++) {
                g.mat[i][i] = 0;
                for (int j = i + 1; j < g.N; j++) {
                    int edge = ((rand() % 100) < p) ? 1 : 0;
                    g.mat[i][j] = edge;
                    g.mat[j][i] = edge;
                }
            }

            for (int i = 0; i < g.N; i++) {
                for (int j = 0; j < g.N; j++) {
                    if (g.mat[i][j] == 1) {
                        g.list[i][g.list_len[i]++] = j;
                    }
                }
            }
            printf("Граф создан: вершин — %d, вероятность каждого ребра — %d%%.\n", g.N, p);
            choice = 2; 
        }

        if (g.N == 0 && choice >= 2 && choice <= 6) {
            printf("Сначала создайте граф (пункт 1).\n");
            continue;
        }

        int vis[MAX_V] = { 0 }, order[MAX_V], idx = 0, start = 0;

        switch (choice) {
            case 2:
                printf("\nМатрица смежности:\n    ");
                for (int j = 0; j < g.N; j++) printf("%3d", j + 1);
                for (int i = 0; i < g.N; i++) {
                    printf("\n%3d ", i + 1);
                    for (int j = 0; j < g.N; j++) {
                        printf("%3d", g.mat[i][j]);
                    }
                }
                printf("\n");
                break;

            case 3:
                start = input("Введите начальную вершину (1-N): ", 1, g.N) - 1;
                dfs_mat_rec(&g, start, vis, order, &idx);
                // Допроход несвязных компонент
                for (int i = 0; i < g.N; i++) {
                    if (!vis[i]) dfs_mat_rec(&g, i, vis, order, &idx);
                }
                print_order("Рекурсивный обход по матрице смежности:", order, g.N);
                break;

            case 4:
                start = input("Введите начальную вершину (1-N): ", 1, g.N) - 1;
                dfs_list_rec(&g, start, vis, order, &idx);
                // Допроход несвязных компонент
                for (int i = 0; i < g.N; i++) {
                    if (!vis[i]) dfs_list_rec(&g, i, vis, order, &idx);
                }
                print_order("Рекурсивный обход по спискам смежности:", order, g.N);
                break;

            case 5:
                start = input("Введите начальную вершину (1-N): ", 1, g.N) - 1;
                dfs_iterative(&g, start);
                break;

            case 6:
                printf("\nСписки смежности:\n");
                for (int i = 0; i < g.N; i++) {
                    printf("%d: ", i + 1);
                    if (g.list_len[i] == 0) printf("нет смежных вершин");
                    for (int j = 0; j < g.list_len[i]; j++) {
                        printf("%d%s", g.list[i][j] + 1, (j < g.list_len[i] - 1) ? ", " : "");
                    }
                    printf("\n");
                }
                break;

            case 7:
                p = input("Вероятность появления ребра (0-100%): ", 0, 100);
                printf("Установлена вероятность %d%%. \n", p);
                break;
        }
    }
    return 0;
}