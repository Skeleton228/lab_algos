#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MIN_V 2
#define MAX_V 10


int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned int)time(NULL));

    int V = 0;
    int prob = 0;
    int directed = 0;

    do
    {
        printf("Введите количество вершин (%d-%d): ", MIN_V, MAX_V);
        scanf("%d", &V);
    } while (V < MIN_V || V > MAX_V);

    do
    {
        printf("Введите вероятность создания ребра (0-100 %%): ");
        scanf("%d", &prob);
    } while (prob < 0 || prob > 100);

    int type = 0;
    do
    {
        printf("Выберите тип графа (1 - неориентированный, 2 - ориентированный): ");
        scanf("%d", &type);
    } while (type != 1 && type != 2);
    directed = (type == 2);

    int adj[MAX_V][MAX_V] = { 0 };

    // ==========================================
    // ЗАДАНИЕ 1: МАТРИЦА СМЕЖНОСТИ
    // ==========================================
    printf("\n================ ЗАДАНИЕ 1 ================\n");

    // 1.1 Генерация матрицы смежности
    if (!directed)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = i + 1; j < V; j++)
            {
                int edge = (rand() % 100 < prob) ? 1 : 0;
                adj[i][j] = edge;
                adj[j][i] = edge;
            }
        }
    }
    else
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (i == j) continue;
                adj[i][j] = (rand() % 100 < prob) ? 1 : 0;
            }
        }
    }

    printf("Тип графа: %s\n", directed ? "ориентированный" : "неориентированный");
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

    // 1.2 Определение размера графа (число рёбер/дуг) по матрице смежности
    int edge_count_adj = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = directed ? 0 : i + 1; j < V; j++)
        {
            if (adj[i][j] == 1)
            {
                edge_count_adj++;
            }
        }
    }
    printf("\n2. Размер графа: %d\n", edge_count_adj);

    // 1.3 Поиск изолированных, концевых и доминирующих вершин
    printf("\n3. Анализ вершин:\n");
    for (int i = 0; i < V; i++)
    {
        int deg = 0;     // для ориентированного: полустепень исхода + захода
        int out_deg = 0; // полустепень исхода
        for (int j = 0; j < V; j++)
        {
            out_deg += adj[i][j];
            deg += adj[i][j];
            if (directed) deg += adj[j][i];
        }

        if (directed)
            printf("  Вершина v%d (степень %d, исход %d): ", i + 1, deg, out_deg);
        else
            printf("  Вершина v%d (степень %d): ", i + 1, deg);

        if (deg == 0)
        {
            printf("ИЗОЛИРОВАННАЯ\n");
        }
        else if (deg == 1)
        {
            printf("КОНЦЕВАЯ\n");
        }
        else if (out_deg == V - 1)
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
        printf("В графе нет рёбер\n");
        return 0;
    }

    int inc[MAX_V][100] = { 0 };

    // 2.1 Построение матрицы инцидентности
    // неориентированный: 1 у обоих концов; ориентированный: +1 начало, -1 конец
    int current_edge = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = directed ? 0 : i + 1; j < V; j++)
        {
            if (adj[i][j] == 1)
            {
                if (!directed)
                {
                    inc[i][current_edge] = 1;
                    inc[j][current_edge] = 1;
                }
                else
                {
                    inc[i][current_edge] = 1;
                    inc[j][current_edge] = -1;
                }
                current_edge++;
            }
        }
    }

    printf("1. Матрица инцидентности (%dx%d):\n", V, E);
    printf("     ");
    for (int e = 0; e < E; e++) printf("e%-2d ", e + 1);
    printf("\n    ");
    for (int e = 0; e < E; e++) printf("----");
    printf("\n");

    for (int i = 0; i < V; i++)
    {
        printf("v%-2d| ", i + 1);
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
            if (directed)
                sum_inc += (inc[i][e] == 1) ? 1 : 0; 
            else
                sum_inc += inc[i][e];
        }
    }
    int edge_count_inc = directed ? sum_inc : sum_inc / 2;
    printf("\n2. Размер графа: %d\n", edge_count_inc);

    // 2.3 Поиск вершин по матрице инцидентности
    printf("\n3. Анализ вершин:\n");
    for (int i = 0; i < V; i++)
    {
        int deg = 0;
        int out_deg = 0;
        for (int e = 0; e < E; e++)
        {
            if (inc[i][e] != 0) deg++;
            if (inc[i][e] == 1) out_deg++;
        }

        if (directed)
            printf("  Вершина v%d (степень %d, исход %d): ", i + 1, deg, out_deg);
        else
            printf("  Вершина v%d (степень %d): ", i + 1, deg);

        if (deg == 0)
        {
            printf("ИЗОЛИРОВАННАЯ\n");
        }
        else if (deg == 1)
        {
            printf("КОНЦЕВАЯ\n");
        }
        else if ((!directed && deg == V - 1) || (directed && out_deg == V - 1))
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