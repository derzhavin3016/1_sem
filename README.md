# MIPT_HOMEWORK

Этот репозиторий содежит задания из моего первого семестра обучения на ФРКТ. Далее следует краткий обзор некоторых задач.

## Язык

Си - подобный язык, основные ключевые слова которого можно менять с помощью файла [lang.h](https://github.com/derzhavin3016/MIPT_Homework/blob/master/LNG/lang.h).  
В языке используются следующие ключевые слова (текущая конфигурация - "непонятный язык"):
- `fi` -> `if`
- `esel` -> `else`
- `hewli` -> `while`
- `rterun` -> `return`
-  `gte`   -> `get(val)` считывание числа в переменную
- `tup`    -> `put(val)` вывод аргумента на экран
- `mani`   -> `main` главная функция программы
- `arv`    -> `var` объявление переменной
- `rived`  -> `deriv(expression, variable)` дифференцирование `expression` по переменной `variable` 
____

Данный проект включает в себя 4 программы:
- [Frontend](https://github.com/derzhavin3016/MIPT_Homework/tree/master/LNG/Frontend) - перевод из текстового файла в синтаксическое дерево.
- [Frontend^-1](https://github.com/derzhavin3016/MIPT_Homework/tree/master/LNG/Frontend%5E-1) - перевод из синтаксического дерева в текстовый файл.
- [Middle-end](https://github.com/derzhavin3016/MIPT_Homework/tree/master/LNG/Mid) - упрощение выражений и взятие производных в синтаксическом дереве.
- [Backend](https://github.com/derzhavin3016/MIPT_Homework/tree/master/LNG/Backend) - перевод из синтаксического дерева в собственный ассемблер `adasm`.  

Разберём подробнее каждый этап.

### Frontend

На вход программа получает текстовый файл с кодом программы, написанной на языке.
