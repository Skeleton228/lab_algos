#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAXV 300   // Максимум вершин (с запасом для Декартова произведения 15x15 = 225)
#define MAXNAME 32

double P = 0.5;

typedef struct {
    int n;
    char names[MAXV][MAXNAME];
    int mat[MAXV][MAXV];
} Graph;


void generate_graph(Graph* g, int n, double p) {
    g->n = n;
    for (int i = 0; i < n; i++) {
        snprintf(g->names[i], MAXNAME, "v%d", i + 1);
        for (int j = 0; j < n; j++) g->mat[i][j] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((double)rand() / RAND_MAX < p) {
                g->mat[i][j] = 1;
                g->mat[j][i] = 1;
            }
        }
    }
}

void print_matrix(Graph* g) {
    if (g->n == 0) { printf("  (пустой граф)\n"); return; }
    printf("%6s", "");
    for (int j = 0; j < g->n; j++) printf("%8s", g->names[j]);
    printf("\n");
    for (int i = 0; i < g->n; i++) {
        printf("%6s", g->names[i]);
        for (int j = 0; j < g->n; j++) printf("%8d", g->mat[i][j]);
        printf("\n");
    }
}

void print_adj_list(Graph* g) {
    if (g->n == 0) { printf("  (пустой граф)\n"); return; }
    for (int i = 0; i < g->n; i++) {
        printf("  %s: ", g->names[i]);
        int first = 1;
        for (int j = 0; j < g->n; j++) {
            if (g->mat[i][j]) {
                if (!first) printf(", ");
                printf("%s", g->names[j]);
                first = 0;
            }
        }
        printf("\n");
    }
}


// ==========================================
// УНАРНЫЕ ОПЕРАЦИИ
// ==========================================

void identify_vertices(Graph* in, int u, int v, Graph* out) {
    int map[MAXV], idx = 0;
    for (int i = 0; i < in->n; i++) {
        if (i == u || i == v) map[i] = -1;
        else map[i] = idx++;
    }
    int z = idx;
    int total = idx + 1;

    out->n = total;
    for (int i = 0; i < total; i++)
        for (int j = 0; j < total; j++) out->mat[i][j] = 0;

    for (int i = 0; i < in->n; i++) {
        if (map[i] != -1) strcpy(out->names[map[i]], in->names[i]);
    }
    snprintf(out->names[z], MAXNAME, "%s%s", in->names[u], in->names[v]);

    for (int i = 0; i < in->n; i++) {
        if (map[i] == -1) continue;
        for (int j = 0; j < in->n; j++) {
            if (map[j] == -1) continue;
            if (in->mat[i][j]) out->mat[map[i]][map[j]] = 1;
        }
    }
    for (int w = 0; w < in->n; w++) {
        if (map[w] == -1) continue;
        if (in->mat[w][u] || in->mat[w][v]) {
            out->mat[map[w]][z] = 1;
            out->mat[z][map[w]] = 1;
        }
    }
    if (in->mat[u][v] || in->mat[u][u] || in->mat[v][v]) out->mat[z][z] = 1;
}

int contract_edge(Graph* in, int u, int v, Graph* out) {
    if (in->mat[u][v] == 0) return 0; // Ребра нет, стягивать нельзя

    // Создаем временную копию
    Graph tmp = *in;
    tmp.mat[u][v] = 0;
    tmp.mat[v][u] = 0;

    identify_vertices(&tmp, u, v, out);
    snprintf(out->names[out->n - 1], MAXNAME, "(%s %s)", in->names[u], in->names[v]);
    return 1;
}

void split_vertex(Graph* in, int v, Graph* out) {
    int total = in->n + 1, w = in->n;
    out->n = total;

    for (int i = 0; i < total; i++)
        for (int j = 0; j < total; j++) out->mat[i][j] = 0;

    for (int i = 0; i < in->n; i++) {
        strcpy(out->names[i], in->names[i]);
        for (int j = 0; j < in->n; j++) out->mat[i][j] = in->mat[i][j];
    }

    snprintf(out->names[w], MAXNAME, "%s*", in->names[v]);

    for (int i = 0; i < in->n; i++) {
        if (in->mat[v][i]) {
            out->mat[w][i] = 1;
            out->mat[i][w] = 1;
        }
    }
    out->mat[v][w] = 1;
    out->mat[w][v] = 1;
}


// ==========================================
// БИНАРНЫЕ ОПЕРАЦИИ И ДЕКАРТОВО ПРОИЗВЕДЕНИЕ
// ==========================================

void union_graphs(Graph* g1, Graph* g2, Graph* out) {
    int n = (g1->n > g2->n) ? g1->n : g2->n;
    out->n = n;
    for (int i = 0; i < n; i++) {
        snprintf(out->names[i], MAXNAME, "v%d", i + 1);
        for (int j = 0; j < n; j++) {
            int e1 = (i < g1->n && j < g1->n) ? g1->mat[i][j] : 0;
            int e2 = (i < g2->n && j < g2->n) ? g2->mat[i][j] : 0;
            out->mat[i][j] = e1 | e2;
        }
    }
}

void intersect_graphs(Graph* g1, Graph* g2, Graph* out) {
    int n = (g1->n < g2->n) ? g1->n : g2->n;
    out->n = n;
    for (int i = 0; i < n; i++) {
        snprintf(out->names[i], MAXNAME, "v%d", i + 1);
        for (int j = 0; j < n; j++) {
            out->mat[i][j] = g1->mat[i][j] & g2->mat[i][j];
        }
    }
}

void ring_sum(Graph* g1, Graph* g2, Graph* out) {
    int n = (g1->n > g2->n) ? g1->n : g2->n;
    int tmp[MAXV][MAXV] = { 0 };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int e1 = (i < g1->n && j < g1->n) ? g1->mat[i][j] : 0;
            int e2 = (i < g2->n && j < g2->n) ? g2->mat[i][j] : 0;
            tmp[i][j] = e1 ^ e2;
        }
    }

    // Оставляем только неизолированные вершины
    int map[MAXV], k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tmp[i][j]) {
                map[k++] = i;
                break;
            }
        }
    }

    out->n = k;
    for (int i = 0; i < k; i++) {
        snprintf(out->names[i], MAXNAME, "v%d", map[i] + 1);
        for (int j = 0; j < k; j++) {
            out->mat[i][j] = tmp[map[i]][map[j]];
        }
    }
}

void cartesian_product(Graph* g1, Graph* g2, Graph* out) {
    int n1 = g1->n, n2 = g2->n;
    out->n = n1 * n2;

    for (int i = 0; i < n1; i++) {
        for (int k = 0; k < n2; k++) {
            int a = i * n2 + k;
            snprintf(out->names[a], MAXNAME, "(%d,%d)", i + 1, k + 1);
            for (int j = 0; j < n1; j++) {
                for (int l = 0; l < n2; l++) {
                    int b = j * n2 + l;
                    out->mat[a][b] = 0;
                    if (a != b) {
                        if ((i == j && g2->mat[k][l]) || (k == l && g1->mat[i][j])) {
                            out->mat[a][b] = 1;
                        }
                    }
                }
            }
        }
    }
}


// ==========================================
// МЕНЮ
// ==========================================

void show_graphs(Graph* g1, Graph* g2) {
    printf("\nМатрица смежности G1:\n");
    print_matrix(g1);
    printf("\nМатрица смежности G2:\n");
    print_matrix(g2);
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned)time(NULL));

    static Graph g1, g2, res;
    g1.n = 0; g2.n = 0; res.n = 0;

    while (1) {
        printf("\n========== ЛАБОРАТОРНАЯ РАБОТА №6 ==========\n");
        printf("1. Сгенерировать графы\n");
        printf("2. Показать матрицы смежности\n");
        printf("3. Показать списки смежности\n");
        printf("4. Унарные операции: отождествление / стягивание / расщепление\n");
        printf("5. Бинарные операции: объединение, пересечение, кольцевая сумма\n");
        printf("6. Декартово произведение\n");
        printf("7. Поменять вероятность\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); continue;
        }

        if (choice == 0) {
            printf("Выход.\n");
            break;
        }

        if (choice != 1 && choice != 7 && g1.n == 0) {
            printf("Сначала сгенерируйте графы!\n");
            continue;
        }

        if (choice == 1) {
            int n1, n2;
            printf("Число вершин G1 (1..15): "); scanf("%d", &n1);
            printf("Число вершин G2 (1..15): "); scanf("%d", &n2);

            if (n1 < 1 || n1 > 15 || n2 < 1 || n2 > 15) {
                printf("Ошибка: допустимый размер от 1 до 15.\n");
            } else {
                generate_graph(&g1, n1, P);
                generate_graph(&g2, n2, P);
                show_graphs(&g1, &g2);
            }
        }
        else if (choice == 2) {
            show_graphs(&g1, &g2);
        }
        else if (choice == 3) {
            printf("\nСписки смежности G1:\n"); print_adj_list(&g1);
            printf("\nСписки смежности G2:\n"); print_adj_list(&g2);
        }
        else if (choice == 4) {
            int g_num;
            printf("\nС каким графом работать? (1 - G1, 2 - G2): ");
            scanf("%d", &g_num);

            Graph* target = (g_num == 1) ? &g1 : ((g_num == 2) ? &g2 : NULL);
            if (!target) { printf("Неверный выбор графа.\n"); continue; }

            printf("\nГраф G%d:\n", g_num);
            print_matrix(target);

            printf("\n  1. Отождествление вершин\n  2. Стягивание ребра\n  3. Расщепление вершины\nОперация: ");
            int op;
            scanf("%d", &op);

            if (op == 1 || op == 2) {
                int u, v;
                printf("Вершина u (число 1..%d): ", target->n); scanf("%d", &u);
                printf("Вершина v (число 1..%d): ", target->n); scanf("%d", &v);
                u--; v--; // Перевод в индексы от 0

                if (u < 0 || u >= target->n || v < 0 || v >= target->n || u == v) {
                    printf("Ошибка ввода вершин.\n");
                }
                else if (op == 1) {
                    identify_vertices(target, u, v, &res);
                    printf("\nРезультат отождествления:\n");
                    print_matrix(&res);
                }
                else {
                    if (contract_edge(target, u, v, &res)) {
                        printf("\nРезультат стягивания ребра:\n");
                        print_matrix(&res);
                    } else {
                        printf("Ошибка: вершины %s и %s не смежны.\n", target->names[u], target->names[v]);
                    }
                }
            }
            else if (op == 3) {
                int v;
                printf("Вершина для расщепления (число 1..%d): ", target->n); scanf("%d", &v);
                v--;
                if (v >= 0 && v < target->n) {
                    split_vertex(target, v, &res);
                    printf("\nРезультат расщепления:\n");
                    print_matrix(&res);
                }
            }
        }
        else if (choice == 5) {
            show_graphs(&g1, &g2);
            printf("\n  1. Объединение G1 U G2\n  2. Пересечение G1 ^ G2\n  3. Кольцевая сумма G1 (+) G2\nОперация: ");
            int op;
            scanf("%d", &op);

            if (op == 1) {
                printf("\nG1 U G2:\n"); union_graphs(&g1, &g2, &res); print_matrix(&res);
            }
            else if (op == 2) {
                printf("\nG1 ^ G2:\n"); intersect_graphs(&g1, &g2, &res); print_matrix(&res);
            }
            else if (op == 3) {
                printf("\nG1 (+) G2 (без изолированных вершин):\n"); ring_sum(&g1, &g2, &res); print_matrix(&res);
            }
        }
        else if (choice == 6) {
            if (g1.n * g2.n > MAXV) {
                printf("Результат слишком велик. Сгенерируйте графы поменьше.\n");
                continue;
            }
            show_graphs(&g1, &g2);
            printf("\nG1 x G2:\n");
            cartesian_product(&g1, &g2, &res);
            print_matrix(&res);
        }
        else if (choice == 7) {
            printf("\nВведите вероятность (0.01 - 1): ");
            scanf("%lf", &P);
            printf("Установлена вероятность %.2f\n", P);
        }
    }
    return 0;
}