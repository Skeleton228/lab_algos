#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define V 6 // Число вершин графа (от 1 до 6)

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned int)time(NULL));

    int adj[V][V] = { 0 };

    // ==========================================
    // ЗАДАНИЕ 1: МАТРИЦА СМЕЖНОСТИ
    // ==========================================
    printf("================ ЗАДАНИЕ 1 ================\n");

    // 1.1 Генерация матрицы смежности (неориентированный граф, без петель)
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            int edge = rand() % 2; // 0 или 1
            adj[i][j] = edge;
            adj[j][i] = edge; // Симметричность
        }
    }

    printf("1. Матрица смежности (%dx%d):\n", V, V);
    printf("     ");
    for (int j = 0; j < V; j++) printf("v%-2d ", j + 1); 
    printf("\n    ");
    for (int j = 0; j < V; j++) printf("----");
    printf("\n");

    for (int i = 0; i < V; i++)
    {
        printf("v%-2d| ", i + 1); 
        for (int j = 0; j < V; j++)
        {
            printf("%-3d ", adj[i][j]);
        }
        printf("\n");
    }

    // 1.2 Определение размера графа (число рёбер) по матрице смежности
    int edge_count_adj = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (adj[i][j] == 1)
            {
                edge_count_adj++;
            }
        }
    }
    printf("\n2. Размер графа (число рёбер E): %d\n", edge_count_adj);

    // 1.3 Поиск изолированных, концевых и доминирующих вершин
    printf("\n3. Анализ вершин (по матрице смежности):\n");
    for (int i = 0; i < V; i++)
    {
        int deg = 0;
        for (int j = 0; j < V; j++)
        {
            deg += adj[i][j];
        }

        printf("  Вершина v%d (степень %d): ", i + 1, deg);
        if (deg == 0)
        {
            printf("ИЗОЛИРОВАННАЯ\n");
        }
        else if (deg == 1)
        {
            printf("КОНЦЕВАЯ (висячая)\n");
        }
        else if (deg == V - 1)
        {
            printf("ДОМИНИРУЮЩАЯ\n");
        }
        else
        {
            printf("обычная\n");
        }
    }

    // ==========================================
    // ЗАДАНИЕ 2: МАТРИЦА ИНЦИДЕНТНОСТИ
    // ==========================================
    printf("\n================ ЗАДАНИЕ 2 ================\n");

    int E = edge_count_adj;

    if (E == 0)
    {
        printf("В графе нет рёбер (все вершины изолированы). Матрица инцидентности пуста.\n");
        return 0;
    }

    int inc[V][100] = { 0 };

    // 2.1 Построение матрицы инцидентности
    int current_edge = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (adj[i][j] == 1)
            {
                inc[i][current_edge] = 1;
                inc[j][current_edge] = 1;
                current_edge++;
            }
        }
    }

    printf("1. Матрица инцидентности (%dx%d):\n", V, E);
    printf("     ");
    for (int e = 0; e < E; e++) printf("e%-2d ", e + 1); // Нумерация рёбер с 1
    printf("\n    ");
    for (int e = 0; e < E; e++) printf("----");
    printf("\n");

    for (int i = 0; i < V; i++)
    {
        printf("v%-2d| ", i + 1); // Нумерация вершин с 1
        for (int e = 0; e < E; e++)
        {
            printf("%-3d ", inc[i][e]);
        }
        printf("\n");
    }

    // 2.2 Определение размера графа по матрице инцидентности
    int sum_inc = 0;
    for (int i = 0; i < V; i++)
    {
        for (int e = 0; e < E; e++)
        {
            sum_inc += inc[i][e];
        }
    }
    int edge_count_inc = sum_inc / 2;
    printf("\n2. Размер графа (по матрице инцидентности = сумма / 2): %d\n", edge_count_inc);

    // 2.3 Поиск вершин по матрице инцидентности
    printf("\n3. Анализ вершин (по строкам матрицы инцидентности):\n");
    for (int i = 0; i < V; i++)
    {
        int deg = 0;
        for (int e = 0; e < E; e++)
        {
            deg += inc[i][e];
        }

        printf("  Вершина v%d (степень %d): ", i + 1, deg);
        if (deg == 0)
        {
            printf("ИЗОЛИРОВАННАЯ\n");
        }
        else if (deg == 1)
        {
            printf("КОНЦЕВАЯ (висячая)\n");
        }
        else if (deg == V - 1)
        {
            printf("ДОМИНИРУЮЩАЯ\n");
        }
        else
        {
            printf("обычная\n");
        }
    }

    return 0;
}