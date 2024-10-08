### Задача "Пересекающиеся отрезки" (version 2).

Даны два массива `a` и `b` длины `N` из натуральных чисел. Рассмотрим множестве, состоящее из отрезков, соединяющих точки $(0, a_{i})$ и $(1, b_i)$ для $1 \leq i \leq N$. Найдите количество отрезков этого множества, которые не пересекаются с другими отрезками. Гарантируется, что все начала и концы отрезков **уникальны**.

![Иллюстрация к проекту](https://github.com/GiBBS-Matvey/Source-cpp/raw/master/Intersecting_segments(ver.2)/Images/problem_segments.jpg)
### Формат ввода:
В первой строке ввода находится единственное число $N\text{ }(1 \leq N \leq 3\cdot 10^5)$ - количество отрезков.

В следующих $N$ строках находится по два целых числа, разделённых пробелом - $a_i$ и $b_i$ $(1 \leq a_i\text{, } b_i \leq N)$, задающие координаты $i$-го отрезка. 

Гарантируется, что все начала и концы отрезков, заданные во вводе, **различны**, то есть при $i \neq j$ выполнены **оба** условия: $a_i \neq a_j\text{ и }b_i \neq b_j$.


### Формат вывода:
Выведите единственное число - количество отрезков, которые не пересекаются с другими.
