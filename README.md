# Labs-Cpp
Архив лабораторных работ на С++.

Ко всем работам написаны тесты: doctest или bash-testing (опубликована лишь часть).

Всё протестировано на разных системах, компиляторах (GCC, Clang, MSVC, Apple Clang) и разных режимах запуска (ASan, Valgrind, Release).\
Дисклеймер: опубликованные CMakeLists протестированы только на MacOS

По мотивам ПМИ ВШЭ.

### Bigint
* Реализовал класс длинной арифметики для универсальной системы счисления

### Widgets-Position
* Реализовал эффективную архитектуру размещения виджетов на экране через класс-наследник, способный корректно отображать внутри себя любые виджеты

### Tic-Tac-Toe | <small>_boost.dll_</small>

* Разработал консольную игру в крестики-нолики 10x10 с динамической поддержкой плагинов отображения:
    * Компактный режим (символьное представление)
    * Графический режим (ASCII арт с псевдографикой)

### Ptrs
* Реализовал потокобезопасные шаблоны классов `unique_ptr<T, Deleter>`, `shared_ptr<T>` с поддержкой incomplete types

### My-Test 
* Разработал легковесную библиотеку для юнит-тестирования с поддержкой макросов `CHECK`, `TEST_CASE`, `SUBCASE`

### Interpreter assembler-like
* Разработал матричный интерпретатор со строгой гарантией исключений

* Реализовал класс матриц с поддержкой стандартных операций и загрузкой, сохранением в файлы

### Bank-Server | <small>_boost.asio_</small>

* Разработал многопоточный банковский сервер с независимой обработкой TCP-клиентов

* Реализовал потокобезопасные классы со сторогой гарантией: 
    * Гроссбух для управления пользователями
    * Пользователь с операциями и историей транзакций
    * Блокирующий итератор для отслеживания появляющихся транзакций

### BMP-Image | C
* Разработал класс со строгой гарантией для побайтовой работы с изображениями: сrop, rotate за O(ответа)

### Vector
...todo

### C-Calculator | C
* Реализовал на Си арифметическую библиотеку для точных вычислений (double) и для нее линейный парсер с функциями произвольной арности и обработкой ошибок.
