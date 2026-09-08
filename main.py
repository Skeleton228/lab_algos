import random
from dataclasses import dataclass

n = 10
i = 0
a = [3, 1, 2, 3, 4, 5, 6, 7, 8, 9]
max_mas = 0
min_mas = 0
new_mas = []
mas_in_key = []
y = 0
o = 0
matrix = [
    [1, 2],
    [3, 4]
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


stud = [Student() for i in range(3)]
for i in range(3):
    Sername = input("Введите фамилию студента\n")
    stud[i].famil = Sername
    name = input("Введите имя студента\n")
    stud[i].name = name
    facult = input("Введите название факультета студента\n")
    stud[i].facult = facult
    nomzach = input("Введите номер зачетной книжки студента студента\n")
    stud[i].nomzach = nomzach
for student in stud:
    print("Студент", student.Sername, student.name, "обучается на факультете", student.facult, "номер зачетной книжки",
          student.nomzach)

find = input("Введите фамилию интересующего вас студента\n")
for student in stud:
    if find == student.famil:
        print("Студент", student.famil, student.name, "обучается на факультете", student.facult,
              "номер зачетной книжки",
              student.nomzach)
        break
    else:
        print("Такого студента нет в базе")
        break
