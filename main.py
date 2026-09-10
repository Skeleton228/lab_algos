import random
from dataclasses import dataclass

i = 0
a = [3, 1, 2, 3, 4, 5, 6, 7, 8, 9]
new_mas = []
mas_in_key = []
y = 0
o = 0
matrix = [
    [1, 2], [5, 3],
    [3, 4], [8, 1]
]
summ = 0
# question 1
print("Задание 1")
mas = [int(x) for x in a]
print(max(mas) - min(mas))
# question 2
print("Задание 2")
while len(new_mas) != 10:
    new = random.randint(1, 10)
    if new not in new_mas:
        new_mas.append(new)
        a[y] = new
        y += 1
print(new_mas)
# question 3
print("Задание 3")
z = int(input('Введите колличество чисел в массиве:\n'))
while len(mas_in_key) != z:
    new = random.randint(1, 10)
    if new not in mas_in_key:
        mas_in_key.append(new)
        a[o] = new
        o += 1
print(mas_in_key)
# question 4
print("Задание 4")

m = int(input("Введите количество строк "))
n = int(input("Введите количество столбцов "))
if n != m:
    print("Это не квадратная матрица")
else:
    new_matrix = [[] * m for _ in range(n)]
    for i in range(m):
        for j in range(n):
            new = random.randint(-10, 30)
            new_matrix[j].append(new)
    summ = 0
    for i in range(n):
        summ += new_matrix[i][i]
    print(new_matrix)
    print(summ)

summs = []
for c in matrix:
    summ = sum(c)
    summs.append(summ)
print(summs)

# question 5
print("Задание 5")


@dataclass
class Student:
    Sername: str = ""
    name: str = ""
    facult: str = ""
    nomzach: int = 0


number = int(input("Введите количество студентов которых хотите занести в базу: "))
stud = [Student() for i in range(number)]
for i in range(number):
    Sername = input("Введите фамилию студента\n")
    stud[i].Sername = Sername
    name = input("Введите имя студента\n")
    stud[i].name = name
    facult = input("Введите название факультета студента\n")
    stud[i].facult = facult
    nomzach = int(input("Введите номер зачетной книжки студента студента\n"))
    stud[i].nomzach = nomzach
for student in stud:
    print("Студент", student.Sername, student.name, "обучается на факультете", student.facult, "номер зачетной книжки",
          student.nomzach)

find = input("\nВведите фамилию интересующего вас студента\n")
for student in stud:
    if (find.lower() in student.Sername.lower()) or (find.lower() in student.name.lower()):
        print("Студент", student.Sername, student.name, "обучается на факультете", student.facult,
              "номер зачетной книжки",
              student.nomzach)

    else:
        print("Такого студента нет в базе")
        break
