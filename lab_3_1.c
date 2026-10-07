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

// 4. ИЗМЕНЕНИЕ ПРИОРИТЕТА У ВСЕХ ОБЪЕКТОВ С ЗАДАННЫМ ИМЕНЕМ
void change_priority(char *name)
{
    struct node *struc = head;
    int count = 0;

    // ШАГ 1: Считаем, есть ли такие объекты и сколько их
    while (struc != NULL)
    {
        if (strcmp(struc->inf, name) == 0) count++;
        struc = struc->next;
    }

    if (count == 0)
    {
        printf("Объекты с именем '%s' не найдены!\n", name);
        return;
    }

    // ШАГ 2: Запрашиваем новый приоритет (один раз для всех)
    int new_priority;
    do {
        printf("Найдено объектов: %d. Введите для них новый приоритет: ", count);
        scanf("%d", &new_priority);
        if (new_priority < 1)
        {
            printf("Ошибка! Приоритет не может быть меньше 1.\n");
        }
    } while (new_priority < 1);

    // ШАГ 3: Вырезаем все нужные объекты во временный список (temp_list)
    struc = head;
    struct node *prev = NULL;
    struct node *temp_list = NULL; 

    while (struc != NULL)
    {
        if (strcmp(struc->inf, name) == 0)
        {
            struct node *target = struc;
            
            // Отсоединяем от основного списка
            if (prev == NULL) 
                head = head->next; 
            else 
                prev->next = struc->next; 
                
            struc = struc->next; // Шагаем дальше по основному списку
            
            // Прикрепляем вырезанный элемент во временный список
            target->next = temp_list;
            temp_list = target;
        }
        else
        {
            prev = struc;
            struc = struc->next;
        }
    }

    // ШАГ 4: Вставляем элементы из временного списка обратно (уже с новыми правилами)
    while (temp_list != NULL)
    {
        struct node *target = temp_list;
        temp_list = temp_list->next; // Берем элемент из временной стопки

        target->priority = new_priority; // Меняем приоритет
        target->next = NULL; 

        // Стандартный алгоритм вставки (как в spstore)
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
    }

    printf("Приоритет успешно изменен у %d объектов!\n", count);
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
        printf("2. Извлечь объект\n");
        printf("3. Просмотр очереди\n");
        printf("4. Изменить приоритет по имени\n");
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