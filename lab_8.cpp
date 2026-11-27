#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <queue> 

// Структура 1: Граф на основе матрицы смежности
typedef struct {
    int N;
    int** mat;
} GraphMat;

// Узел для динамического списка смежности
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

typedef struct {
    struct node* head;
    struct node* tail;
} CustomQueue;

void q_init(CustomQueue* q) {
    q->head = NULL;
    q->tail = NULL;
}

int q_empty(CustomQueue* q) {
    return q->head == NULL;
}

// Добавление в конец очереди
void q_push(CustomQueue* q, int v) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->vertex = v;
    new_node->next = NULL;

    if (q->tail == NULL) {
        q->head = q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
}

// Извлечение из начала очереди
int q_pop(CustomQueue* q) {
    if (q->head == NULL) return -1;
    struct node* temp = q->head;
    int val = temp->vertex;
    q->head = temp->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    free(temp);
    return val;
}


int input(const char* msg, int min, int max) {
    int val;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &val) == 1 && val >= min && val <= max) return val;
        while (getchar() != '\n');
        printf("Ошибка. Введите число от %d до %d.\n", min, max);
    }
}

// Вывод одной пройденной компоненты связности
void print_component(int comp, int* order, int count) {
    printf("Компонента %d: ", comp);
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

// Генерация матрицы
void generate_mat(GraphMat* gMat, int N, int p) {
    free_mat(gMat);
    gMat->N = N;
    gMat->mat = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        gMat->mat[i] = (int*)calloc(N, sizeof(int));
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int edge = ((rand() % 100) < p) ? 1 : 0;
            gMat->mat[i][j] = gMat->mat[j][i] = edge;
        }
    }
}

// Построение динамических списков
void build_list_from_mat(GraphMat* gMat, GraphList* gList) {
    free_list(gList);
    int N = gMat->N;
    gList->N = N;
    gList->array = (ListNode**)malloc(N * sizeof(ListNode*));

    for (int i = 0; i < N; i++) {
        gList->array[i] = NULL;
        for (int j = N - 1; j >= 0; j--) {
            if (gMat->mat[i][j] == 1) {
                ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
                newNode->vertex = j;
                newNode->next = gList->array[i];
                gList->array[i] = newNode;
            }
        }
    }
}


// Вариант 1: Матрица смежности + queue
void bfs_mat_stl(GraphMat* g, int start, int* order, int show) {
    int* vis = (int*)calloc(g->N, sizeof(int));
    std::queue<int> Q;
    int comp = 0;

    for (int k = 0; k < g->N; k++) {
        int s = (start + k) % g->N;
        if (vis[s]) continue;

        int idx = 0;
        Q.push(s);
        vis[s] = 1;

        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            order[idx++] = v;

            for (int i = 0; i < g->N; i++) {
                if (g->mat[v][i] == 1 && !vis[i]) {
                    Q.push(i);
                    vis[i] = 1;
                }
            }
        }

        if (show) {
            comp++;
            print_component(comp, order, idx);
        }
    }

    free(vis);
}

// Вариант 2: Списки смежности + стандартный queue
void bfs_list_stl(GraphList* g, int start, int* order, int show) {
    int* vis = (int*)calloc(g->N, sizeof(int));
    std::queue<int> Q;
    int comp = 0;

    for (int k = 0; k < g->N; k++) {
        int s = (start + k) % g->N;
        if (vis[s]) continue;

        int idx = 0;
        Q.push(s);
        vis[s] = 1;

        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            order[idx++] = v;

            ListNode* curr = g->array[v];
            while (curr != NULL) {
                if (!vis[curr->vertex]) {
                    Q.push(curr->vertex);
                    vis[curr->vertex] = 1;
                }
                curr = curr->next;
            }
        }

        if (show) {
            comp++;
            print_component(comp, order, idx);
        }
    }

    free(vis);
}

// Вариант 3: Матрица смежности + своя очередь
void bfs_mat_custom(GraphMat* g, int start, int* order, int show) {
    int* vis = (int*)calloc(g->N, sizeof(int));
    CustomQueue Q;
    q_init(&Q);
    int comp = 0;

    for (int k = 0; k < g->N; k++) {
        int s = (start + k) % g->N;
        if (vis[s]) continue;

        int idx = 0;
        q_push(&Q, s);
        vis[s] = 1;

        while (!q_empty(&Q)) {
            int v = q_pop(&Q);
            order[idx++] = v;

            for (int i = 0; i < g->N; i++) {
                if (g->mat[v][i] == 1 && !vis[i]) {
                    q_push(&Q, i);
                    vis[i] = 1;
                }
            }
        }

        if (show) {
            comp++;
            print_component(comp, order, idx);
        }
    }

    free(vis);
}

// Вариант 4: Списки смежности + своя очередь
void bfs_list_custom(GraphList* g, int start, int* order, int show) {
    int* vis = (int*)calloc(g->N, sizeof(int));
    CustomQueue Q;
    q_init(&Q);
    int comp = 0;

    for (int k = 0; k < g->N; k++) {
        int s = (start + k) % g->N;
        if (vis[s]) continue;

        int idx = 0;
        q_push(&Q, s);
        vis[s] = 1;

        while (!q_empty(&Q)) {
            int v = q_pop(&Q);
            order[idx++] = v;

            ListNode* curr = g->array[v];
            while (curr != NULL) {
                if (!vis[curr->vertex]) {
                    q_push(&Q, curr->vertex);
                    vis[curr->vertex] = 1;
                }
                curr = curr->next;
            }
        }

        if (show) {
            comp++;
            print_component(comp, order, idx);
        }
    }

    free(vis);
}


void compare_time(int p) {
    int sizes[] = { 100, 500, 1000, 2000, 3000 };
    int num_tests = sizeof(sizes) / sizeof(sizes[0]);

    printf("\nСравнение времени работы BFS (вероятность %d%%):\n", p);
    printf("+------------+------------------+------------------+------------------+------------------+\n");
    printf("|   Вершин   |  Матр+queue (мс) | Матр+очередь (мс)| Спис+queue (мс)  | Спис+очередь (мс)|\n");
    printf("+------------+------------------+------------------+------------------+------------------+\n");

    for (int t = 0; t < num_tests; t++) {
        int N = sizes[t];
        GraphMat testMat = { 0, NULL };
        GraphList testList = { 0, NULL };
        generate_mat(&testMat, N, p);
        build_list_from_mat(&testMat, &testList);
        int* order = (int*)malloc(N * sizeof(int));

        clock_t t1 = clock();
        bfs_mat_stl(&testMat, 0, order, 0);
        clock_t t2 = clock();
        double time_mat_stl = (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC;

        clock_t t3 = clock();
        bfs_mat_custom(&testMat, 0, order, 0);
        clock_t t4 = clock();
        double time_mat_custom = (double)(t4 - t3) * 1000.0 / CLOCKS_PER_SEC;

        clock_t t5 = clock();
        bfs_list_stl(&testList, 0, order, 0);
        clock_t t6 = clock();
        double time_list_stl = (double)(t6 - t5) * 1000.0 / CLOCKS_PER_SEC;

        clock_t t7 = clock();
        bfs_list_custom(&testList, 0, order, 0);
        clock_t t8 = clock();
        double time_list_custom = (double)(t8 - t7) * 1000.0 / CLOCKS_PER_SEC;

        printf("| %10d | %16.3f | %16.3f | %16.3f | %16.3f |\n",
               N, time_mat_stl, time_mat_custom, time_list_stl, time_list_custom);

        free(order);
        free_list(&testList);
        free_mat(&testMat);
    }
    printf("+------------+------------------+------------------+------------------+------------------+\n");
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned)time(NULL));

    GraphMat gMat = { 0, NULL };
    GraphList gList = { 0, NULL };
    int p = 30;

    while (1) {
        printf("\n========== ЛАБОРАТОРНАЯ РАБОТА №8  ==========\n");
        printf("1. Создать случайный граф\n");
        printf("2. Вывести матрицу смежности\n");
        printf("3. Вывести списки смежности\n");
        printf("4. BFS по матрице (queue)\n");
        printf("5. BFS по спискам смежности (queue)\n");
        printf("6. BFS по матрице (очередь)\n");
        printf("7. BFS по спискам смежности (очередь)\n");
        printf("8. Сравнить время работы\n");
        printf("9. Изменить вероятность ребра\n");
        printf("0. Выход\n");

        int choice = input("Выберите пункт: ", 0, 9);

        if (choice == 0) {
            free_mat(&gMat);
            free_list(&gList);
            break;
        }

        if (choice == 1) {
            int N = input("Количество вершин (1-100): ", 1, 100);
            generate_mat(&gMat, N, p);
            build_list_from_mat(&gMat, &gList);
            choice = 2; 
        }

        if (gMat.N == 0 && choice >= 2 && choice <= 7) {
            printf("Сначала создайте граф!\n");
            continue;
        }

        int start = 0;
        int* order = NULL;
        clock_t t1, t2;

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
                printf("\nДинамические списки смежности:\n");
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

            case 4:
                start = input("Старт (1-N): ", 1, gMat.N) - 1;
                order = (int*)malloc(gMat.N * sizeof(int));
                printf("\nBFS по матрице:\n");
                t1 = clock();
                bfs_mat_stl(&gMat, start, order, 1);
                t2 = clock();
                printf("Время выполнения: %.3f мс\n", (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC);
                free(order);
                break;

            case 5:
                start = input("Старт (1-N): ", 1, gList.N) - 1;
                order = (int*)malloc(gList.N * sizeof(int));
                printf("\nBFS по спискам смежности:\n");
                t1 = clock();
                bfs_list_stl(&gList, start, order, 1);
                t2 = clock();
                printf("Время выполнения: %.3f мс\n", (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC);
                free(order);
                break;

            case 6:
                start = input("Старт (1-N): ", 1, gMat.N) - 1;
                order = (int*)malloc(gMat.N * sizeof(int));
                printf("\nBFS по матрице (очередь):\n");
                t1 = clock();
                bfs_mat_custom(&gMat, start, order, 1);
                t2 = clock();
                printf("Время выполнения: %.3f мс\n", (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC);
                free(order);
                break;

            case 7:
                start = input("Старт (1-N): ", 1, gList.N) - 1;
                order = (int*)malloc(gList.N * sizeof(int));
                printf("\nBFS по спискам смежности (очередь):\n");
                t1 = clock();
                bfs_list_custom(&gList, start, order, 1);
                t2 = clock();
                printf("Время выполнения: %.3f мс\n", (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC);
                free(order);
                break;

            case 8:
                compare_time(p);
                break;

            case 9:
                p = input("Вероятность (0-100): ", 0, 100);
                break;
        }
    }
    return 0;
}