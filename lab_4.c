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
		printf(" ");
	}
	printf("%d\n", r->data);
	print_tree(r->left,   l+1);
}

// ==========================================
// ЗАДАНИЕ 1: Алгоритм поиска значения
// ==========================================
struct Node* search_value(struct Node *r, int key)
{
	if (r == NULL) return NULL; // Дошли до конца ветки, ничего не нашли
	
	if (r->data == key) return r; // Нашли нужное значение!

	if (key > r->data)
		return search_value(r->left, key); // Большие числа ищем слева
	else
		return search_value(r->right, key); // Меньшие - справа
}

// ==========================================
// ЗАДАНИЕ 2: Подсчет числа вхождений
// ==========================================
int count_occurrences(struct Node *r, int key)
{
	if (r == NULL) return 0;

	int count = 0;
	if (r->data == key)
	{
		// Если нашли совпадение, считаем его (1 + ...)
		// Поскольку дубликаты (одинаковые числа) уходят в правую ветку, продолжаем поиск там
		count = 1 + count_occurrences(r->right, key);
	}
	else if (key > r->data)
	{
		count = count_occurrences(r->left, key);
	}
	else
	{
		count = count_occurrences(r->right, key);
	}
	
	return count;
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
	
	printf("Ваше дерево:\n");
	print_tree(root,0);
	
	// --- ВЫПОЛНЕНИЕ ЗАДАНИЙ 1 И 2 ---
	int key;
	printf("\n--- ЗАДАНИЕ 1 И 2 ---\n");
	printf("Введите число для поиска и подсчета вхождений: ");
	scanf_s("%d", &key);
	
	// Задание 1
	struct Node *found = search_value(root, key);
	if (found != NULL)
		printf("Элемент %d успешно НАЙДЕН в дереве!\n", found->data);
	else
		printf("Элемент %d в дереве НЕ НАЙДЕН.\n", key);
		
	// Задание 2
	int count = count_occurrences(root, key);
	printf("Число вхождений элемента %d равно %d\n", key, count);

    
	return 0;
}