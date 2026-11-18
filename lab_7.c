#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// ==========================================
// 1. ДИНАМИЧЕСКИЕ СТРУКТУРЫ ГРАФОВ
// ==========================================

// Структура 1: Граф на основе матрицы смежности
typedef struct {
    int N;
    int** mat;
} GraphMat;

// Узел для списков смежности
typedef struct ListNode {
    int vertex;
    struct ListNode* next;
} ListNode;

// Структура 2: Граф на основе списков смежности
typedef struct {
    int N;
    ListNode** array;
} GraphList;

struct node {
    int vertex;
    struct node* next;
};

void push(struct node** top, int v) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->vertex = v;
    new_node->next = *top;
    *top = new_node;
}

int pop(struct node** top) {
    if (*top == NULL) return -1;
    struct node* temp = *top;
    int popped = temp->vertex;
    *top = temp->next;
    free(temp);
    return popped;
}


// ==========================================
// УТИЛИТЫ И ОЧИСТКА ПАМЯТИ
// ==========================================

int input(const char* msg, int min, int max) {
    int val;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &val) == 1 && val >= min && val <= max) return val;
        while (getchar() != '\n'); 
        printf("Ошибка. Введите число от %d до %d.\n", min, max);
    }
}

void print_order(const char* title, int* order, int count) {
    printf("\n%s\nПорядок посещения: ", title);
    for (int i = 0; i < count; i++) {
        printf("%d%s", order[i] + 1, (i < count - 1) ? " -> " : "");
    }
    printf("\n");
}

void free_mat(GraphMat* g) {
    if (g->mat == NULL) return;
    for (int i = 0; i < g->N; i++) free(g->mat[i]);
    free(g->mat);
    g->mat = NULL;
    g->N = 0;
}

void free_list(GraphList* g) {
    if (g->array == NULL) return;
    for (int i = 0; i < g->N; i++) {
        ListNode* curr = g->array[i];
        while (curr != NULL) {
            ListNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(g->array);
    g->array = NULL;
    g->N = 0;
}


// ==========================================
// 3. АЛГОРИТМЫ ОБХОДА
// ==========================================

void dfs_mat_rec(GraphMat* g, int v, int* vis, int* order, int* idx) {
    vis[v] = 1;
    order[(*idx)++] = v;
    for (int i = 0; i < g->N; i++) {
        if (g->mat[v][i] == 1 && !vis[i]) {
            dfs_mat_rec(g, i, vis, order, idx);
        }
    }
}

void dfs_list_rec(GraphList* g, int v, int* vis, int* order, int* idx) {
    vis[v] = 1;
    order[(*idx)++] = v;
    
    ListNode* curr = g->array[v];
    while (curr != NULL) {
        if (!vis[curr->vertex]) {
            dfs_list_rec(g, curr->vertex, vis, order, idx);
        }
        curr = curr->next;
    }
}

void dfs_iterative(GraphMat* g, int start) {
    int* vis = (int*)calloc(g->N, sizeof(int));
    int* order = (int*)malloc(g->N * sizeof(int));
    int idx = 0;
    
    struct node* top = NULL; 

    for (int i = 0; i < g->N; i++) {
        int v = (start + i) % g->N;
        if (vis[v]) continue;

        push(&top, v);

        while (top != NULL) {
            int cur = pop(&top);
            if (vis[cur]) continue;

            vis[cur] = 1;
            order[idx++] = cur;

            for (int j = g->N - 1; j >= 0; j--) {
                if (g->mat[cur][j] == 1 && !vis[j]) {
                    push(&top, j);
                }
            }
        }
    }
    
    print_order("Нерекурсивный обход: ", order, g->N);
    free(vis);
    free(order);
}


// ==========================================
// ГЛАВНОЕ МЕНЮ
// ==========================================
int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned)time(NULL));

    GraphMat gMat = { 0, NULL };
    GraphList gList = { 0, NULL };
    int p = 30;

    while (1) {
        printf("\n========== ЛАБОРАТОРНАЯ РАБОТА №7 ==========\n");
        printf("1. Создать случайный граф\n2. Вывести матрицу\n3. Рекурсивный обход (матрица)\n");
        printf("4. Обход по спискам смежности\n5. Нерекурсивный обход (стек)\n");
        printf("6. Вывести динамические списки\n7. Вероятность ребра\n0. Выход\n");

        int choice = input("Выберите пункт: ", 0, 7);
        if (choice == 0) {
            free_mat(&gMat);
            free_list(&gList);
            printf("Память очищена. Программа завершена.\n");
            break;
        }

        if (choice == 1) {
            int N = input("Количество вершин (1-100): ", 1, 100);
            
            free_mat(&gMat);
            free_list(&gList);

            gMat.N = N;
            gMat.mat = (int**)malloc(N * sizeof(int*));
            for (int i = 0; i < N; i++) {
                gMat.mat[i] = (int*)calloc(N, sizeof(int));
            }

            gList.N = N;
            gList.array = (ListNode**)malloc(N * sizeof(ListNode*));
            for (int i = 0; i < N; i++) {
                gList.array[i] = NULL;
            }

            for (int i = 0; i < N; i++) {
                for (int j = i + 1; j < N; j++) {
                    int edge = ((rand() % 100) < p) ? 1 : 0;
                    gMat.mat[i][j] = gMat.mat[j][i] = edge;
                }
            }

            for (int i = 0; i < N; i++) {
                for (int j = N - 1; j >= 0; j--) {
                    if (gMat.mat[i][j] == 1) {
                        ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
                        newNode->vertex = j;
                        newNode->next = gList.array[i];
                        gList.array[i] = newNode;
                    }
                }
            }
            printf("Граф создан (%d вершин, %d%% вероятность). \n", N, p);
            choice = 2;
        }

        if (gMat.N == 0 && choice >= 2 && choice <= 6) {
            printf("Сначала создайте граф!\n");
            continue;
        }

        int start = 0;
        int* vis = NULL;
        int* order = NULL;
        int idx = 0;

        switch (choice) {
            case 2:
                printf("\nМатрица смежности:\n    ");
                for (int j = 0; j < gMat.N; j++) printf("%3d", j + 1);
                for (int i = 0; i < gMat.N; i++) {
                    printf("\n%3d ", i + 1);
                    for (int j = 0; j < gMat.N; j++) printf("%3d", gMat.mat[i][j]);
                }
                printf("\n");
                break;

            case 3:
                start = input("Старт (1-N): ", 1, gMat.N) - 1;
                vis = (int*)calloc(gMat.N, sizeof(int));
                order = (int*)malloc(gMat.N * sizeof(int));
                
                dfs_mat_rec(&gMat, start, vis, order, &idx);
                for (int i = 0; i < gMat.N; i++) if (!vis[i]) dfs_mat_rec(&gMat, i, vis, order, &idx);
                
                print_order("Рекурсивный обход (матрица):", order, gMat.N);
                free(vis); free(order);
                break;

            case 4:
                start = input("Старт (1-N): ", 1, gMat.N) - 1;
                vis = (int*)calloc(gMat.N, sizeof(int));
                order = (int*)malloc(gMat.N * sizeof(int));
                
                dfs_list_rec(&gList, start, vis, order, &idx);
                for (int i = 0; i < gMat.N; i++) if (!vis[i]) dfs_list_rec(&gList, i, vis, order, &idx);
                
                print_order("Рекурсивный обход (динамические списки):", order, gMat.N);
                free(vis); free(order);
                break;

            case 5:
                start = input("Старт (1-N): ", 1, gMat.N) - 1;
                dfs_iterative(&gMat, start);
                break;

            case 6:
                printf("\nПоказать динамические списки :\n");
                for (int i = 0; i < gList.N; i++) {
                    printf("%d: ", i + 1);
                    ListNode* curr = gList.array[i];
                    if (curr == NULL) printf("нет соседей");
                    while (curr != NULL) {
                        printf("%d%s", curr->vertex + 1, (curr->next != NULL) ? " -> " : "");
                        curr = curr->next;
                    }
                    printf("\n");
                }
                break;

            case 7:
                p = input("Вероятность (0-100): ", 0, 100);
                break;
        }
    }
    return 0;
}