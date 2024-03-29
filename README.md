# MIPT_HOMEWORK

![GitHub repo size](https://img.shields.io/github/repo-size/derzhavin3016/MIPT_Homework?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/derzhavin3016/MIPT_Homework?color=red&style=for-the-badge)
![GitHub lines](https://img.shields.io/tokei/lines/github/derzhavin3016/MIPT_Homework?style=for-the-badge)

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
Ниже представлен пример программы, считающей факториал.
```
$fact( number )
{
  fi (number <= 1)
    rterun 1;
  rterun number * fact(number - 1);
}

$main()
{
 arv number = 228;
  gte(number);

 tup(fact(number));
 rterun;
}
```
Программа разбивается на токены, а затем 
[парсер](https://github.com/derzhavin3016/MIPT_Homework/tree/master/LNG/Parser), основанный на методе рекурсивного спуска, создаёт синтаксическое дерево программы и записывает его в файл.

### Frontend^-1

На вход программа получает файл, в котором описано синтаксическое дерево. Программа восстанавливает дерево из файла, а затем пишет исходный код программы, основываясь на этом дереве.


### Middle-end

На вход программа получает файл, в котором описано синтаксическое дерево. Программа восстанавливает дерево из файла, а затем ищет узлы с выражениями (`expression`), либо с оператором взятия производной (`rived`) и производит вычисления.


### Back-end

На вход программа получает файл, в котором описано синтаксическое дерево. Программа восстанавливает дерево из файла, после чего пишет код на ассемблере `adasm`, сохраняя его в текстовый файл.
