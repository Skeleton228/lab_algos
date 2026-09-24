#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Структура для обычной очереди (приоритет не нужен)
struct node
{
    char inf[256];
    struct node *next;
};

struct node *head = NULL; // Отсюда забираем элементы
struct node *last = NULL; // Сюда добавляем элементы

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

// Постановка объекта строго в конец очереди
void spstore(void)
{
    struct node *p = get_struct();

    if (head == NULL) // Если очередь была пуста
    {
        head = p;
        last = p;
    }
    else // Привязываем к последнему элементу и обновляем last
    {
        last->next = p;
        last = p;
    }
}

// Извлечение объекта строго из начала очереди
void serve(void)
{
    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }
    struct node *temp = head;
    head = head->next; // Сдвигаем начало
    
    // Если очередь после удаления стала пустой
    if (head == NULL) last = NULL; 
    
    printf("Извлечен объект: %s\n", temp->inf);
    free(temp);
}

void review(void)
{
    struct node *struc = head;
    if (head == NULL) printf("Очередь пуста\n");
    
    while (struc != NULL)
    {
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
    last = NULL;
}

int main(void)
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001); 

    int choice;
    do {
        printf("\n--- ОБЫЧНАЯ ОЧЕРЕДЬ (FIFO) ---\n");
        printf("1. Добавить объект (в конец)\n");
        printf("2. Извлечь объект (из начала)\n");
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