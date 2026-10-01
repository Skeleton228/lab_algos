#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct node
{
    char inf[256];
    int priority;
    struct node *next;
};

struct node *head = NULL; 

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
    
    do {
        printf("Введите приоритет: ");
        scanf("%d", &p->priority);
        if (p->priority < 1)
        {
            printf("Ошибка! Приоритет не может быть меньше 1. Попробуйте снова.\n");
        }
    } while (p->priority < 1);

    p->next = NULL;
    return p;
}

// 1. Добавление элемента
void spstore(void)
{
    struct node *p = get_struct();

    if (head == NULL) 
    {
        head = p;
    }
    else if (p->priority < head->priority) 
    {
        p->next = head;
        head = p;
    }
    else 
    {
        struct node *current = head;
        while (current->next != NULL && current->next->priority <= p->priority)
        {
            current = current->next;
        }
        p->next = current->next;
        current->next = p;
    }
}

// 2. Извлечение первого элемента (самого важного)
void serve(void)
{
    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }
    struct node *temp = head;
    head = head->next; 
    printf("Извлечен объект: %s (Приоритет: %d)\n", temp->inf, temp->priority);
    free(temp); 
}

// 3. Просмотр списка
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

// 4. Изменение приоритета
void change_priority(char *name)
{
    struct node *struc = head;
    struct node *prev = NULL;
    struct node *target = NULL;
    int new_priority;

    // Ищем элемент и вырезаем его из списка
    while (struc != NULL)
    {
        if (strcmp(struc->inf, name) == 0)
        {
            target = struc;
            
            if (prev == NULL) 
                head = head->next; 
            else 
                prev->next = struc->next; 
                
            break; 
        }
        prev = struc;
        struc = struc->next;
    }

    if (target == NULL)
    {
        printf("Объект '%s' не найден!\n", name);
        return;
    }

    do {
        printf("Введите новый приоритет: ");
        scanf("%d", &new_priority);
        if (new_priority < 1)
        {
            printf("Ошибка!\n");
        }
    } while (new_priority < 1);

    target->priority = new_priority;
    target->next = NULL; 

    // Вставляем обратно с новыми правилами (знаки перевернуты)
    if (head == NULL) 
    {
        head = target;
    }
    else if (target->priority < head->priority) 
    {
        target->next = head;
        head = target;
    }
    else 
    {
        struct node *current = head;
        while (current->next != NULL && current->next->priority <= target->priority)
        {
            current = current->next;
        }
        target->next = current->next;
        current->next = target;
    }

    printf("Приоритет успешно изменен!\n");
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
    char name[256];

    do {
        printf("\n--- ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ---\n");
        printf("1. Добавить объект\n");
        printf("2. Удалить объект\n");
        printf("3. Просмотр очереди\n");
        printf("4. Изменить приоритет объекта\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: spstore(); break;
            case 2: serve(); break;
            case 3: review(); break;
            case 4:
                printf("Введите имя объекта: ");
                scanf("%255s", name);
                change_priority(name);
                break;
            case 0: clear_list(); break;
            default: printf("Неверный ввод!\n");
        }
    } while (choice != 0);

    return 0;
}