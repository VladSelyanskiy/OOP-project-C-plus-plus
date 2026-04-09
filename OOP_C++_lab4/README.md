# LAB4

## Подробное описание каждого задания

### Задание 1: Класс `Point` (Геометрическая точка)

**Файлы:** `Point.h`, `Point.cpp`

**Цель:** Демонстрация конструкторов и инкапсуляции

**Основные концепции:**
- 🔹 **Конструктор с параметрами** — создание точки (x, y)
- 🔹 **Конструктор по умолчанию** — создание точки (0, 0)
- 🔹 **Делегирующий конструктор** — конструктор с одним параметром
- 🔹 **Геттеры и сеттеры** — доступ к приватным членам
- 🔹 **Константные методы** — вычисление расстояния

**Главные методы класса:**

```cpp
class Point {
private:
    double x, y;
public:
    Point(double x_val, double y_val);  // Основной конструктор
    Point();                             // Конструктор по умолчанию
    Point(double val);                   // Делегирующий конструктор
    
    // Геттеры
    double getX() const;
    double getY() const;
    
    // Сеттеры
    void setX(double newX);
    void setY(double newY);
    
    // Методы
    void print() const;                  // Вывод координат
    double distanceTo(const Point& other) const;  // Евклидово расстояние
};
```

**Пример использования:**

```
Входные данные:
1 (или "point")

Вывод:
p1 = (3, 4)
p2 = (0, 0)
p3 = (5, 5)
Расстояние p1-p2: 5

p2 после set: (1, 1)
Новое расстояние: 3.60555
```

**Формула расстояния:**
$$d = \sqrt{(x_1 - x_2)^2 + (y_1 - y_2)^2}$$

---

### Задание 2: Класс `Student` (Студент)

**Файлы:** `Student.h`, `Student.cpp`

**Цель:** Демонстрация работы с контейнерами, копирования и **правила нуля**

**Основные концепции:**
- 🔹 **Конструктор** — инициализация студента
- 🔹 **Правило нуля** — класс использует `std::vector<int>` для оценок
- 🔹 **Копирование и перемещение** — компилятор генерирует методы автоматически
- 🔹 **STL контейнеры** — работа с vector
- 🔹 **Константные методы** — вычисление среднего

**Структура класса:**

```cpp
class Student {
private:
    std::string name;
    std::vector<int> grades;
    int id;
public:
    Student(const string& n, const vector<int>& g, int i);
    ~Student();
    
    void addGrade(int grade);
    double average() const;              // Средний балл
    void print() const;
};
```

**Пример использования:**

```
Входные данные:
2 (или "student")

Вывод:
=== ЧАСТЬ 1: Создание объектов ===
ID: 12345, Имя: Иван Петров, Оценки: 5 4 5 4 (среднее: 4.5)

=== ЧАСТЬ 2: Копирование (правило нуля!) ===
... копирование и перемещение объектов ...

=== ЧАСТЬ 6: Вывод группы ===
ID: 12345, Имя: Иван Петров, Оценки: 5 4 5 4 (среднее: 4.5)
ID: 54321, Имя: Петр Сидоров, Оценки: 3 4 3 (среднее: 3.33)

=== ЧАСТЬ 7: Завершение программы ===
```

**Ключевая концепция — Правило нуля:**
```cpp
// Мы НЕ пишем конструктор копирования — компилятор сделает сам!
// Мы НЕ пишем оператор присваивания — компилятор сделает сам!
// Мы НЕ пишем деструктор — std::vector позаботится о памяти!

// Это называется "Rule of Zero"
```

---

### Задание 3: Полиморфизм `Shapes` (Фигуры)

**Файлы:** `Shapes.h`, `Shapes.cpp`

**Цель:** Демонстрация наследования и виртуальных функций

**Основные концепции:**
- 🔹 **Базовый класс** — `Shape` с чисто виртуальными методами
- 🔹 **Наследование** — `Circle` и `Rectangle` наследуют `Shape`
- 🔹 **Виртуальные функции** — переопределение методов `area()`, `perimeter()`, `print()`
- 🔹 **Полиморфный контейнер** — `vector<unique_ptr<Shape>>`
- 🔹 **Умные указатели** — `unique_ptr` для управления памятью

**Иерархия классов:**

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void print() const = 0;
    virtual ~Shape() {}  // Виртуальный деструктор!
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r);
    double area() const override;
    double perimeter() const override;
    void print() const override;
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h);
    double area() const override;
    double perimeter() const override;
    void print() const override;
};
```

**Формулы:**

| Фигура | Площадь | Периметр |
|--------|---------|----------|
| Круг | πr² | 2πr |
| Прямоугольник | w × h | 2(w + h) |

**Пример использования:**

```
Входные данные:
3 (или "shapes")

Вывод:
=== Все фигуры ===
1: Круг: радиус = 5, площадь = 78.539816, периметр = 31.415927
2: Прямоугольник: 3 x 3, площадь = 9, периметр = 12
3: Круг: радиус = 2.5, площадь = 19.634954, периметр = 15.707963

Number of shape: 0, shape: Круг: радиус = 5, площадь = 78.539816
```

**Особенность:** Нахождение фигуры с максимальной площадью через полиморфные вызовы:

```cpp
vector<unique_ptr<Shape>> shapes;
shapes.push_back(make_unique<Circle>(5));
shapes.push_back(make_unique<Rectangle>(3, 3));

// Полиморфный вызов через base class pointer
for (auto& shape : shapes) {
    shape->print();  // Вызовет правильный метод!
}
```

---

### Задание 4: Класс `BankAccount` (Банковский счёт)

**Файлы:** `BankAccount.h`, `BankAccount.cpp`

**Цель:** Демонстрация обработки исключений и валидации

**Основные концепции:**
- 🔹 **Обработка исключений** — `try-catch` блоки
- 🔹 **Выброс исключений** — `throw std::exception`
- 🔹 **Валидация входных данных** — проверки перед операциями
- 🔹 **Инкапсуляция** — защита состояния объекта

**Структура класса:**

```cpp
class BankAccount {
private:
    string owner;
    double balance;
public:
    BankAccount(const string& name, double initial);
    ~BankAccount();
    
    void deposit(double amount);     // Пополнение счёта
    void withdraw(double amount);    // Снятие со счёта
    void print() const;              // Вывод информации
};
```

**Тесты с обработкой исключений:**

**Тест 1 — Нормальная работа:**
```cpp
try {
    BankAccount acc("Иван", 1000);
    acc.deposit(500);      // Баланс = 1500
    acc.withdraw(200);     // Баланс = 1300
    acc.print();
}
catch (const exception& e) {
    cout << "Ошибка: " << e.what() << endl;
}

Вывод:
Хозяин: Иван
Баланс: 1300
```

**Тест 2 — Ошибка: снятие больше баланса**
```cpp
try {
    BankAccount acc("Петр", 100);
    acc.withdraw(200);  // Ошибка!
}
catch (const exception& e) {
    cout << "Ошибка: " << e.what() << endl;
}

Вывод:
Ошибка: Insufficient funds!
```

**Тест 3 — Ошибка: отрицательный начальный баланс**
```cpp
try {
    BankAccount acc("Толя", -42);  // Ошибка!
}
catch (const exception& e) {
    cout << "Ошибка: " << e.what() << endl;
}

Вывод:
Ошибка: Initial balance cannot be negative!
```

---

## Образовательная ценность LAB4

| Задание | Ключевые навыки |
|---------|-----------------|
| **Point** | Конструкторы, геттеры/сеттеры, инкапсуляция |
| **Student** | STL контейнеры, правило нуля, RAII |
| **Shapes** | Наследование, полиморфизм, виртуальные функции, умные указатели |
| **BankAccount** | Исключения, валидация, обработка ошибок |

---

## Структура файлов LAB4

```
OOP_C++_lab4/
├── OOP_C++_lab4.cpp          # Главная программа с меню
├── Point.h / Point.cpp        # Геометрическая точка
├── Student.h / Student.cpp    # Студент
├── Shapes.h / Shapes.cpp      # Фигуры (Circle, Rectangle)
└── BankAccount.h / BankAccount.cpp  # Банковский счёт
```

---
