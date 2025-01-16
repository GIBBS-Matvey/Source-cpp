## C. Расстояние

Рассмотрим целочисленный массив $a$ длины $n$. Назовём расстоянием от индекса $i$ до множества индексов $S$ величину
$dist(i, S) = \sum\limits_{j\in S}|a_i - a_j|$ .

Зафиксируем целое число $k$. Рассмотрим функцию $f(i) = min\text{ }dist(i, S)$, где минимум берётся по множествам $S$ размера $k$, не содержащим индекс $i$.
Определите значение $f(i)$ для всех $i$ от $1$ до $n$.

## Формат ввода

В первой строке заданы два целых числа $n$ и $k$ $(2\leq n\leq 300\text{ }000\text{ , }1\leq k < n)$, описанные в условии.

Во второй строке содержится n целых чисел $a_i\text{ }(1\leq a_i \leq 10^9)$ — элементы массива $a$.

## Формат вывода

Выведите $n$ целых чисел: значения $f(i)$ для $i = 1, i = 2, ..., i = n$.

<img src="https://github.com/GiBBS-Matvey/Source-cpp/raw/master/Distance/Images/ex_1_dist.PNG" width="700">
<img src="https://github.com/GiBBS-Matvey/Source-cpp/raw/master/Distance/Images/ex_2_dist.PNG" width="700">
