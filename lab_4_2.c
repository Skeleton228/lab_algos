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

// --- (ЗАДАНИЕ 3: Добавление без дубликатов) ---
struct Node *CreateTree(struct Node *root, struct Node *r, int data)
{
    if (r == NULL)
    {
        r = (struct Node *)malloc(sizeof(struct Node));
        if (r == NULL)
        {
            printf("Ошибка выделения памяти\n");
            exit(0);
        }
        
        r->left = NULL;
        r->right = NULL;
        r->data = data;
        if (root == NULL) return r;
        if (data > root->data)  root->left = r;
        else root->right = r;   
        return r;
    }
    
    // Большие числа идут в левую ветку
    if (data > r->data)
        CreateTree(r, r->left, data);
    // Меньшие числа идут в правую ветку
    else if (data < r->data)
        CreateTree(r, r->right, data);
    else
        printf(" -> Элемент %d уже существует!\n", data);
        
    return root;
}

// Вспомогательная функция: ищет узел с минимальным значением в поддереве
// Для твоей логики (где меньшие идут вправо) минимум находится в крайнем правом узле
struct Node *find_min(struct Node *r)
{
    while (r != NULL && r->right != NULL)
    {
        r = r->right;
    }
    return r;
}

// --- ФУНКЦИЯ УДАЛЕНИЯ УЗЛА С ПЕРЕРАСПРЕДЕЛЕНИЕМ ---
struct Node *delete_node(struct Node *r, int key, int *deleted)
{
    if (r == NULL)
    {
        return NULL; // Узел не найден
    }

    // 1. Поиск удаляемого элемента по логике CreateTree
    if (key > r->data)
    {
        r->left = delete_node(r->left, key, deleted);
    }
    else if (key < r->data)
    {
        r->right = delete_node(r->right, key, deleted);
    }
    else
    {
        // Элемент найден!
        *deleted = 1;

        // Случай 1 и 2: нет левого потомка
        if (r->left == NULL)
        {
            struct Node *temp = r->right;
            free(r);
            return temp;
        }
        // Случай 1 и 2: нет правого потомка
        else if (r->right == NULL)
        {
            struct Node *temp = r->left;
            free(r);
            return temp;
        }

        // Случай 3: у узла два потомка
        // Ищем замену в левом поддереве (минимальный среди бóльших)
        struct Node *temp = find_min(r->left);

        // Переносим значение на место текущего узла
        r->data = temp->data;

        // Удаляем узел-замену из левого поддерева
        r->left = delete_node(r->left, temp->data, deleted);
    }

    return r;
}

void print_tree(struct Node *r, int l)
{
    if (r == NULL)
    {
        return;
    }
    
    print_tree(r->right, l + 1);
    for (int i = 0; i < l; i++)
    {
        printf("    ");
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

    // --- УДАЛЕНИЕ УЗЛА ---
    int del_key;
    printf("\nВведите значение узла для удаления: ");
    scanf_s("%d", &del_key);

    int deleted = 0;
    root = delete_node(root, del_key, &deleted);

    if (deleted)
    {
        printf("Узел %d успешно удален\n\n", del_key);
        printf("Дерево после удаления:\n");
        print_tree(root, 0);
    }
    else
    {
        printf("Узел %d не найден в дереве!\n", del_key);
    }
    return 0;
}