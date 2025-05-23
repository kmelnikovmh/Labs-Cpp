# Widgets-Position

Реализована модульная иерархия виджетов с поддержкой как простых элементов (кнопка, иконка-шарик), так и контейнеров с различными стратегиями размещения дочерних компонентов (горизонтально, вертикально, по сетке). Все виджеты обладают ограничивающим прямоугольником и взаимодействуют друг с другом через систему координат, относительно своего родителя.

Проект ориентирован на модульную компоновку UI-элементов, с корректным управлением геометрией и родительско-дочерними связями. Контейнеры реализуют логику обновления размеров и размещения, оставляя ответственность на вызов update_layout() на вызывающей стороне для гибкости при массовых изменениях.

Особенности:
* Единый базовый класс widget, от которого наследуются все UI-элементы.
* Поддержка контейнеров (container), владеющих своими детьми и автоматически обновляющих их размещение при изменении состава.
* Дочерние виджеты знают о своём родителе, но не могут изменять его напрямую — обеспечена безопасность через скрытый интерфейс изменения parent.
* Точное поведение child_at(x, y), определяющее принадлежность пикселя неконтейнерному элементу.
* Реализована поддержка нулевых размеров, не влияющих на размещение, но учитываемых при обходе и проверках.
* Мутируемость виджетов, с автоматическим пересчётом размеров при изменении состояния (например, текста, радиуса иконки и т.п.).
* Явное управление геометрией через update_layout() — родитель не обновляется автоматически при изменении дочернего элемента.

Основные компоненты:
* `widget` (базовый виртуальный класс)
    * Хранит bounding box: width(), height().
    * Виртуальный метод child_at(x, y) — определяет принадлежность пикселя неконтейнерному виджету.
    * Метод parent() — возвращает родительский контейнер при наличии.
* `button` - простой виджет с текстовой надписью. Размер автоматически обновляется при изменении текста.
* `ball_icon` - круглая иконка радиусом radius, площадь 2*radius+1.
    * Метод radius() / radius(new_radius) для получения и установки размера.
    * child_at() работает по евклидову расстоянию.
* `container` - базовый абстрактный контейнер: владеет дочерними элементами.
    * Обновление layout при изменении набора детей.
    * Метод update_layout() пересчитывает размеры и позиции детей.
* `box` - контейнер с горизонтальным или вертикальным размещением (box::kind::HORIZONTAL / VERTICAL). Упорядоченные дети, размещение вплотную.
    * size(), get(index)
    * add(widget), remove(index)
    * update_layout() пересчитывает размеры за O(n)
    * child_at(x, y) — за O(n) + 1 рекурсивный вызов
* `grid` - контейнер с сеточным размещением (rows, columns фиксированы). Максимум один элемент на ячейку. Выравнивание по левому верхнему углу ячейки.
    * rows(), columns()
    * get(row, col), add(child, row, col), remove(row, col)
    * update_layout() за O(rows*columns)
    * child_at(x, y) — за O(rows + columns) + 1 вызов

## Инструкция по запуску

В данном проекте реализована лишь архитектура классов, отвечающих за расположения виджетов. Компиляция данной архитектуры без дополнительных обвесов не несет смысла.

Но имеет смысл тестирование данной архитектуры.

**Тестирование**\
Из папки test запустить тесты:
```bash
./run-test-data.sh
```

К проекту опубликована лишь часть тестов.\
Добавить свои юнит-тесты можно в соответствующий cpp файл в папке test. Править скрипт или команды не требуется