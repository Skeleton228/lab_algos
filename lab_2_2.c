#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000


void shell(int *items, int count)
{
    int i, j, gap, k;
    int x, a[5];
    
    a[0]=9; a[1]=5; a[2]=3; a[3]=2; a[4]=1;
    
    for(k=0; k < 5; k++) {
        gap = a[k];
        for(i=gap; i < count; ++i) {
            x = items[i];
            for(j=i-gap; (x < items[j]) && (j >= 0); j=j-gap)
                items[j+gap] = items[j];
            items[j+gap] = x;
        }
    }
}

void qs(int *items, int left, int right)
{
    int i, j;
    int x, y;
    
    i = left; j = right;
    x = items[(left+right)/2];
    
    do {
        while((items[i] < x) && (i < right)) i++;
        while((x < items[j]) && (j > left)) j--;
        
        if(i <= j) {
            y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
    } while(i <= j);
    
    if(left < j) qs(items, left, j);
    if(i < right) qs(items, i, right);
}

int compare(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

int main(void)
{
    int orig[N], temp[N];
    clock_t start, end;

    
    srand(time(NULL));    
    for (int type = 1; type <= 4; type++) {
        if (type == 1) {
            printf("\n--- 1. random mass ---\n");
            for (int i = 0; i < N; i++) orig[i] = rand() % 10000;
        }
        else if (type == 2) {
            printf("\n--- 2. small_to_big ---\n");
            for (int i = 0; i < N; i++) orig[i] = i;
        }
        else if (type == 3) {
            printf("\n--- 3. big_to_small ---\n");
            for (int i = 0; i < N; i++) orig[i] = N - i;
        }
        else if (type == 4) {
            printf("\n--- 4. 1/2 big., 1/2 small. ---\n");
            for (int i = 0; i < N / 2; i++) orig[i] = i;
            for (int i = N / 2; i < N; i++) orig[i] = N - i;
        }

        
        for (int i = 0; i < N; i++) temp[i] = orig[i];
        start = clock();
        shell(temp, N);
        end = clock();
        printf("Shell sort: %.3f s\n",
               (double)(end - start)  / CLOCKS_PER_SEC);

        
        for (int i = 0; i < N; i++) temp[i] = orig[i];
        start = clock();
        qs(temp, 0, N - 1);
        end = clock();
        printf("QuickSort : %.3f s\n",
               (double)(end - start)  / CLOCKS_PER_SEC);

        
        for (int i = 0; i < N; i++) temp[i] = orig[i];
        start = clock();
        qsort(temp, N, sizeof(int), compare);
        end = clock();
        printf("qsort : %.3f s\n",
               (double)(end - start)  / CLOCKS_PER_SEC);
    }

    return 0;
}
