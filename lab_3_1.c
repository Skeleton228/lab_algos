#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Структура элемента приоритетной очереди
struct node
{
    char inf[256];      // Название задачи
    int priority;       // Приоритет (чем больше число, тем важнее)
    struct node *next;  // Указатель на следующий элемент
};

struct node *head = NULL; // Начало очереди

// Функция создания нового элемента
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
    printf("Введите приоритет: ");
    scanf("%d", &p->priority);
    p->next = NULL;
    return p;
}

// Добавление элемента с сортировкой по убыванию приоритета
void spstore(void)
{
    struct node *p = get_struct();

    // Если очередь пуста
    if (head == NULL) 
    {
        head = p;
    }
    // Если приоритет нового выше, чем у головы списка
    else if (p->priority > head->priority) 
    {
        p->next = head;
        head = p;
    }
    else 
    {
        // Ищем место: пропускаем все элементы с приоритетом больше либо равным нашему
        struct node *current = head;
        while (current->next != NULL && current->next->priority >= p->priority)
        {
            current = current->next;
        }
        // Вставляем новый элемент после найденного
        p->next = current->next;
        current->next = p;
    }
}

// Извлечение задачи с наивысшим приоритетом (первой в списке)
void serve(void)
{
    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }
    struct node *temp = head;
    head = head->next; // Сдвигаем голову
    printf("Извлечен объект: %s (Приоритет: %d)\n", temp->inf, temp->priority);
    free(temp); // Очищаем память удаленного элемента
}

// Просмотр всех элементов очереди
void review(void)
{
    struct node *struc = head;
    if (head == NULL) printf("Очередь пуста\n");
    
    while (struc != NULL)
    {
        printf("Объект: %s, Приоритет: %d\n", struc->inf, struc->priority);
        struc = struc->next;
    }
}

// Очистка всей памяти при выходе
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
    SetConsoleCP(65001);       // Настройка русской кодировки для Windows
    SetConsoleOutputCP(65001); 

    int choice;
    do {
        printf("\n--- ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ---\n");
        printf("1. Добавить объект (по приоритету)\n");
        printf("2. Извлечь объект (первый)\n");
        printf("3. Просмотр очереди\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: spstore(); break;
            case 2: serve(); break;
            case 3: review(); break;
            case 0: clear_list(); break;
            default: printf("Неверный ввод!\n");
        }
    } while (choice != 0);

    return 0;
}