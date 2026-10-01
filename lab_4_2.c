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

// --- (ЗАДАНИЕ 3) ---
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
	
	// Если число больше - идем в левую ветку
	if (data > r->data)
		CreateTree(r, r->left, data);
	// Если число строго меньше - идем в правую ветку
	else if (data < r->data)
		CreateTree(r, r->right, data);
	else
		printf(" -> Элемент %d уже существует!\n", data);
		
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
	print_tree(r->left, l + 1);
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
	print_tree(root, 0);
	return 0;
}