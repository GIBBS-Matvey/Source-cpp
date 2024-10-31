### Идея решения:
Словарь отсортирован в лексикографическом порядке, а это значит, что для каждого запроса достаточно находить первое слово, начинающегося с заданного префикса (так называемый `lower_bound`), и отсчитывать от него слово с заданным порядковым номером. Ассимптотика `lower_bound` - это O($\log N$). Помимо этого на каждой итерации вызывается `operator<` для строк, он работает в худшем случае за O($|p_i|$). Всего у нас `Q` запросов. 

**Complexity**: O($Q\cdot max|p_i|\cdot \log N$)