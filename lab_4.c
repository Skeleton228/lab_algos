#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

struct Node {
	int data;
	struct Node *left;
	struct Node *right;
};
struct Node *root;

struct Node *CreateTree(struct Node *root, struct Node *r, int data)
{
	if (r == NULL)
	{
		r = (struct Node *)malloc(sizeof(struct Node));
		if (r == NULL)
		{
			printf("Ошибка выделения памяти");
			exit(0);
		}
		
		r->left = NULL;
		r->right = NULL;
		r->data = data;
		if (root == NULL) return r;
		if (data > root->data)	root->left = r;
		else root->right = r;	
		return r;
	}
	if (data > r->data)
		CreateTree(r, r->left, data);
	else
		CreateTree(r, r->right, data);
	return root;
}

void print_tree(struct Node *r, int l)
{
	
	if (r == NULL)
	{
		return;
	}
	
	print_tree(r->right, l + 1);
	for(int i = 0; i < l; i++)
	{
		printf("    ");
	}
	printf("%d\n", r->data);
	print_tree(r->left,   l+1);
}


void find_levels_to_array(struct Node *r, int key, int level, int levels[], int *count)
{
    if (r == NULL) return;

    if (r->data == key)
    {
        // Записываем найденный индекс (уровень) в массив
        levels[*count] = level;
        (*count)++;

        // Продолжаем искать дубликаты справа (по логике методички)
        find_levels_to_array(r->right, key, level + 1, levels, count);
    }
    else if (key > r->data)
    {
        find_levels_to_array(r->left, key, level + 1, levels, count);
    }
    else
    {
        find_levels_to_array(r->right, key, level + 1, levels, count);
    }
}


int main()
{
    SetConsoleCP(65001);       
    SetConsoleOutputCP(65001);
    
    int D, start = 1;
    root = NULL;
    printf("-1 - окончание построения дерева\n");
    while (start)
    {
        printf("Введите число: ");
        scanf_s("%d", &D);
        if (D == -1)
        {
            printf("Построение дерева окончено\n\n");
            start = 0;
        }
        else
            root = CreateTree(root, root, D);
    }
    
    printf("Ваше дерево:\n\n");
    print_tree(root, 0);
    
    // --- ВЫПОЛНЕНИЕ ЗАДАНИЙ 1 И 2 (С ИНДЕКСАМИ УРОВНЕЙ) ---
    int key;
    printf("\n--- ЗАДАНИЕ 1 И 2 ---\n");
    printf("Введите число для поиска: ");
    scanf_s("%d", &key);
    
    int levels[100]; // Массив для сохранения индексов уровней
    int count = 0;   // Количество найденных вхождений

    // Вызываем поиск с записью уровней в массив
    find_levels_to_array(root, key, 0, levels, &count);

    if (count == 0)
    {
        printf("Элемент %d в дереве НЕ НАЙДЕН.\n", key);
    }
    else
    {
        printf("Элемент %d успешно НАЙДЕН в дереве!\n", key);
        printf("Число вхождений элемента %d равно: %d\n", key, count);
        
        printf("Индексы уровней: ");
        for (int i = 0; i < count; i++)
        {
            printf("%d ", levels[i]);
        }
        printf("\n");
    }
    return 0;
}