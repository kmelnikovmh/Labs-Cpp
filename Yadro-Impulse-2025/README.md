# Yadro-Project
Цель: ...

**Дисклеймер:** не успел до 00:00 пройтись санитайзерами и написать полноценные юнит тесты.\
Спустя час добавил основные юнит тесты и слегка прошелся санитайзерами

Реализовано:
* Класс Tape - имитирующий работу ленты и настраиваемый файлом `src/config.txt`. Работает с файлами, которые представляют из себя подряд побайтово записанные числа int
    * create_tape - создает файл из побайтово подряд записанных int длины нужного размера (вне класса)
    * read - считывает число, которое находит под магнитной головкой ленты
    * write - перезаписывает число, которое находится под магнитной головкой ленты
    * next_pos - перемещает магнитную головку ленты на одно число вперед
    * rewind - возвращает магнитную головку на исходную позицию
    * size - возвращает количество элементов на магнитной ленте
    * is_eof - позволяет узнать, прочлили мы до этого последнее значение на ленте
* Класс SortTape, реализующий сортировку с ограничениями по объему оперативной памяти (max_ram) и числу одновременно открытых файлов (max_files).

    Входной файл читается порциями, каждая из которых помещается в память, сортируется и сохраняется как отдельный временный файл (лента). Эти временные ленты накапливаются в пул, пока их количество не достигнет max_files.

    После этого применяется нерекурсивная версия алгоритма слияния (mergesort): временные ленты сливаются попарно в несколько проходов, постепенно сокращая их число до одной объединённой ленты. Этот процесс повторяется до полной обработки всех данных. В конце оставшиеся ленты также объединяются в одну, которая записывается в итоговый выходной файл (output_path)      

    Замечание: 
    1) Алгоритм старается сперва мерджить файлы меньшего размера, чтобы случайно не уйти в квадрат, когда мы к большому файлу будем добавлять маленькие по очереди. Но есть физические ограничения по количеству допустимых файлов. Чем больше можно, тем лучше. Данный параметр можно менять (см ниже)
    2) Можно облегчить размер занимаемой памяти, если сразу удалять считанные данные. По ТЗ не реализовано

* Обработка исключений
* Изменение параметров работы (максимальное количество файлов, объем оперативной памяти, задержки у магнитной ленты)

## Инструкция

Сброка проекта в папку build:
```shell
mkdir build && cd build && cmake ../ && make
```

Запуск приложения с конфигурационным файлом, который настраивает эмулятор ленты (задержки по ее операциям в мс)
```shell
./tape-sort ../tape_config.txt
```

Работа с приложением:
* Загрузка файла с данными, эмулирующего данные на ленте, и выгрузка результата:
    ```shell
    sort <input_file_path> <output_file_path>
    ```

    Входной файл представляет из себя массив чисел: побайтово подряд идущие int числа
* Смена режима работы:
    * Установка максимального числа временных лент, степень двойки (по умолчанию 4) (+1 файл для работы mergesort):
        ```shell
        max-files <int>
        ```
    * Установка ограничения по использованию оперативной памяти (по умолчанию 32 числа):
        ```shell
        max-ram <int>
        ```
* Выход из приложения:
    ```shell
    exit
    ```

### Тестирование
Собрать тесты, запустив из папки build:
```shell
make tests
```

Создано простое приложение `make_tape`, которое создает ленты нужного размера из случайных чисел. Запуск из папки build:
```shell
./../test/tape-make
```

Ленты хранятся в папке `/test/data-test`. Смотреть данные удобно либо через побайтовый вывод функцией из `tape_make.cpp` (не реализовано), либо через "hex view"

Юнит тесты лежат в `test/main_test.cpp`. Их запуск из папки build:
```shell
./../test/run-test-data.sh
```