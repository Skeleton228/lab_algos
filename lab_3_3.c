#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Структура для стека (указатель last не нужен)
struct node
{
    char inf[256];
    struct node *next;
};

struct node *head = NULL; // Вершина стека

struct node *get_struct(void)
{
    struct node *p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL)
    {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    printf("Введите название объекта: ");
    scanf("%255s", p->inf);
    p->next = NULL;
    return p;
}

// Добавление объекта на вершину стека
void push(void)
{
    struct node *p = get_struct();
    
    // Новый элемент ссылается на бывшую вершину
    p->next = head; 
    // И сам становится новой вершиной
    head = p;       
}

// Снятие объекта с вершины стека
void pop(void)
{
    if (head == NULL)
    {
        printf("Стек пуст\n");
        return;
    }
    struct node *temp = head;
    head = head->next; // Вершиной становится нижележащий элемент
    
    printf("Извлечен объект: %s\n", temp->inf);
    free(temp);
}

void review(void)
{
    struct node *struc = head;
    if (head == NULL) printf("Стек пуст\n");
    
    while (struc != NULL)
    {
        if (struc == head) 
            printf("Объект: %s <-- вершина\n", struc->inf);
        else 
            printf("Объект: %s\n", struc->inf);
            
        struc = struc->next;
    }
}

void clear_list(void)
{
    struct node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001); 

    int choice;
    do {
        printf("\n--- СТЕК (LIFO) ---\n");
        printf("1. Добавить объект (на вершину)\n");
        printf("2. Извлечь объект (с вершины)\n");
        printf("3. Просмотр стека\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: push(); break;
            case 2: pop(); break;
            case 3: review(); break;
            case 0: clear_list(); break;
            default: printf("Неверный ввод!\n");
        }
    } while (choice != 0);

    return 0;
}