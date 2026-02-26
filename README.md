# DoublyLinkedList

Реализация сериализации и десериализации двусвязного списка с произвольными указателями (`rand`) на C++17.

## Задача

Дана структура узла:

```cpp
struct ListNode {
    ListNode* prev = nullptr;  // указатель на предыдущий элемент
    ListNode* next = nullptr;  // указатель на следующий элемент
    ListNode* rand = nullptr;  // указатель на произвольный элемент списка или nullptr
    std::string data;          // пользовательские данные
};
```

Программа считывает текстовое описание списка из файла `data/inlet.in`, строит связанный список и сериализует его в бинарный файл `outlet.out`. Поддерживается обратная операция — десериализация из бинарного файла с записью результата обратно в текстовый формат.

## Клонирование

```bash
git clone https://github.com/MaxGrinderAfk/DoublyLinkedList
cd DoublyLinkedList
```

## Сборка

Требования: CMake ≥ 3.16, компилятор с поддержкой C++17.

```bash
cmake -B build
cmake --build build
```

Исполняемый файл: `build/task1Intership` (Linux/macOS) или `build/task1Intership.exe` (Windows).

## Использование

Запускайте программу **из корня репозитория** — пути к файлам указаны относительно рабочей директории:

```bash
./build/task1Intership
```

```
Enter 0 to load data from inlet.in and serialize it to outlet.out
Enter 1 to deserialize from outlet.out and write it to deserializeTest.in
Enter any digit or character to leave

Choice:
```

- **0** — читает `data/inlet.in` → строит список → сериализует в `outlet.out`
- **1** — десериализует `outlet.out` → записывает в `deserializeTest.in` → сравнивает с `data/inlet.in`

## Формат входного файла `data/inlet.in`

Каждая строка описывает один узел:

```
<data>;<rand_index>
```

- `<data>` — строка в кодировке UTF-8, может содержать пробелы и спецсимволы
- `<rand_index>` — индекс узла (0-based), на который указывает `rand`, либо `-1` если `rand == nullptr`
- Символы `\n`, `\r`, `\` в поле `data` экранируются как `\\n`, `\\r`, `\\`

Пример:

```
apple;2
banana;-1
carrot;1
```

| Узел | data       | rand      |
|------|------------|-----------|
| 0    | `"apple"`  | → узел 2  |
| 1    | `"banana"` | `nullptr` |
| 2    | `"carrot"` | → узел 1  |

## Формат бинарного файла `outlet.out`

```
[uint32] count           — число узлов
для каждого узла:
    [uint32] data_len    — длина строки data в байтах
    [bytes]  data        — байты строки
    [int32]  rand_index  — индекс rand-узла или -1
```

Порядок байт: native (платформенный). Файл предназначен для чтения той же программой на той же платформе.

## Ограничения

| Параметр                | Значение      |
|-------------------------|---------------|
| Максимум узлов          | 10⁶           |
| Максимальная длина data | 1000 символов |

При превышении ограничений выбрасывается `std::runtime_error`.

## Структура проекта

```
.
├── data/
│   └── inlet.in                      — пример входных данных
├── main.cpp                          — точка входа
├── doublylinkedlistStarter.h         — функция appStarter(), меню программы
├── doublylinkedlistrandaccess.h/.cpp — класс списка, Rule of Five
├── doublylinkedlistserializer.h/.cpp — сериализация: LoadFromText, SaveToText, Serialize, Deserialize
├── listutils.h/.cpp                  — утилиты: EscapeData, UnescapeData, CompareTextFiles
├── .gitignore
└── CMakeLists.txt
```

### Описание классов

**`DoublyLinkedListRandAccess`**  
Хранит двусвязный список с `rand`-указателями. Реализует Rule of Five (copy/move конструкторы и операторы присваивания, деструктор). Не знает ничего о файлах — только структура данных. Приватные методы `BuildChain` и `CommitChain` доступны сериализатору через `friend`.

**`DoublyLinkedListSerializer`**  
Статический класс (не инстанцируется). Отвечает за весь файловый I/O: чтение из текста, запись в текст, бинарная сериализация и десериализация. Валидирует ограничения на число узлов и длину `data`. Обеспечивает strong exception safety — при ошибке состояние переданного списка не изменяется.

**`ListUtils`**  
Статический класс вспомогательных функций, не связанных со структурой списка: экранирование/деэкранирование спецсимволов в `data`, побайтовое сравнение текстовых файлов с отчётом о расхождениях.

## .gitignore

```
# This file is used to ignore files which are generated
# ----------------------------------------------------------------------------

*~
*.autosave
*.a
*.core
*.moc
*.o
*.obj
*.orig
*.rej
*.so
*.so.*
*_pch.h.cpp
*_resource.rc
*.qm
.#*
*.*#
core
!core/
tags
.DS_Store
.directory
*.debug
Makefile*
*.prl
*.app
moc_*.cpp
ui_*.h
qrc_*.cpp
Thumbs.db
*.res
*.rc
/.qmake.cache
/.qmake.stash

# qtcreator generated files
*.pro.user*
*.qbs.user*
CMakeLists.txt.user*

# xemacs temporary files
*.flc

# Vim temporary files
.*.swp

# Visual Studio generated files
*.ib_pdb_index
*.idb
*.ilk
*.pdb
*.sln
*.suo
*.vcproj
*vcproj.*.*.user
*.ncb
*.sdf
*.opensdf
*.vcxproj
*vcxproj.*

# MinGW generated files
*.Debug
*.Release

# Python byte code
*.pyc

# Binaries
# --------
*.dll
*.exe

# Directories with generated files
.moc/
.obj/
.pch/
.rcc/
.uic/
/build*/
```

## Пример кода

```cpp
#include "doublylinkedlistrandaccess.h"
#include "doublylinkedlistserializer.h"
#include "listutils.h"

DoublyLinkedListRandAccess list;

// Загрузить из текста и сериализовать
DoublyLinkedListSerializer::LoadFromText(list, "data/inlet.in");
DoublyLinkedListSerializer::Serialize(list, "outlet.out");

// Десериализовать и сохранить обратно в текст
DoublyLinkedListRandAccess restored;
DoublyLinkedListSerializer::Deserialize(restored, "outlet.out");
DoublyLinkedListSerializer::SaveToText(restored, "restored.in");

// Проверить совпадение
ListUtils::CompareTextFiles("data/inlet.in", "restored.in");
```
