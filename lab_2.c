#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

int main(void)
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    clock_t start, end;
    int i = 0, j = 0, r;
    int elem_c;

    
    static int a[N][N], b[N][N], c[N][N];

    srand(time(NULL));
    while (i < N)
    {
        j = 0;
        while (j < N)
        {
            a[i][j] = rand() % 100 + 1;
            j++;
        }
        i++;
    }

    srand(time(NULL));
    i = 0; j = 0;
    while (i < N)
    {
        j = 0;
        while (j < N)
        {
            b[i][j] = rand() % 100 + 1;
            j++;
        }
        i++;
    }

    
    start = clock();

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            elem_c = 0;
            for (r = 0; r < N; r++)
            {
                elem_c = elem_c + a[i][r] * b[r][j];
            }
            c[i][j] = elem_c;
        }
    }

    
    end = clock();

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time: %.3f cek\n", time_taken);
    return 0;
}
