#include <iostream>
#include <cmath>
#include <string>
#include <memory>
#include <locale.h>

#include "Point.h"
#include "Student.h"
#include "Shapes.h"
#include "BankAccount.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    string exercise;

    while (true)
    {
        cout << endl << "Enter number or name of exercise (1-4):" << endl;
        cin >> exercise;
        if (exercise == "point" || exercise == "1")
        {
            // TODO: Создайте точку p1 с координатами (3, 4)
            Point p1(3, 4);
            // TODO: Создайте точку p2 с координатами (0, 0) используя конструктор по умолчанию
            Point p2;
            // TODO: Создайте точку p3 с координатами (5, 5) используя делегирующий конструктор
            Point p3(5);

            cout << "p1 = "; p1.print(); cout << endl;
            cout << "p2 = "; p2.print(); cout << endl;
            cout << "p3 = "; p3.print(); cout << endl;

            cout << "Расстояние p1-p2: " << p1.distanceTo(p2) << endl;

            // TODO: Измените p2 через сеттеры на (1, 1)
            p2.setX(1);
            p2.setY(1);

            cout << "p2 после set: "; p2.print(); cout << endl;
            cout << "Новое расстояние: " << p1.distanceTo(p2) << endl;
        }
        else if (exercise == "student" || exercise == "2")
        {
            cout << "=== ЧАСТЬ 1: Создание объектов ===" << endl;
            Student s1("Иван Петров", { 5, 4, 5, 4 }, 12345);

            cout << "\n=== ЧАСТЬ 2: Копирование (правило нуля!) ===" << endl;
            Student s2 = s1;  // вызывается конструктор копирования (сгенерированный компилятором)

            cout << "\n=== ЧАСТЬ 3: После копирования ===" << endl;
            cout << "s1: "; s1.print();
            cout << "s2: "; s2.print();

            cout << "\n=== ЧАСТЬ 4: Изменяем копию ===" << endl;
            s2.addGrade(3);
            cout << "После изменения s2:" << endl;
            cout << "s1: "; s1.print();  // не должна измениться!
            cout << "s2: "; s2.print();  // должна измениться

            cout << "\n=== ЧАСТЬ 5: Вектор студентов ===" << endl;
            vector<Student> group;
            group.push_back(s1);
            group.push_back(Student("Петр Сидоров", { 3, 4, 3 }, 54321));

            cout << "\n=== ЧАСТЬ 6: Вывод группы ===" << endl;
            for (const auto& s : group)
            {
                s.print();
            }

            cout << "\n=== ЧАСТЬ 7: Завершение программы ===" << endl;
        }
        else if (exercise == "shapes" || exercise == "3")
        {
            // Полиморфный контейнер (слайд 3.8)
            vector<unique_ptr<Shape>> shapes;

            // Добавляем фигуры
            shapes.push_back(make_unique<Circle>(5));
            shapes.push_back(make_unique<Rectangle>(3, 3));
            shapes.push_back(make_unique<Circle>(2.5));

            cout << "=== Все фигуры ===" << endl;
            for (size_t i = 0; i < shapes.size(); i++)
            {
                cout << i + 1 << ": ";
                shapes[i]->print();  // полиморфный вызов (слайд 3.4)
                cout << endl;
            }

            // TODO: Найдите фигуру с максимальной площадью
            // Подсказка: используйте shapes[i]->area()
            size_t num_max = -1;
            double max_area = -1;
            for (size_t i = 0; i < shapes.size(); i++)
            {
                double curr_area = shapes[i]->area();
                if (curr_area > max_area)
                {
                    max_area = curr_area;
                    num_max = i;
                }
            }
            cout << "Number of shape: " << num_max << ", shape: ";
            shapes[num_max]->print();
            cout << "Area: " << shapes[num_max]->area() << endl;
        }
        else if (exercise == "bank account" || exercise == "4")
        {
            int test_number = -1;
            cout << "Enter test number (1-3):" << endl;
            cin >> test_number;
            if (test_number == 1)
            {
                // Тест 1: нормальная работа
                try
                {
                    BankAccount acc("Иван", 1000);
                    acc.deposit(500);
                    acc.withdraw(200);
                    acc.print();
                }
                catch (const exception& e)
                {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
            if (test_number == 2)
            {
                // Тест 2: снятие больше баланса
                try
                {
                    BankAccount acc("Петр", 100);
                    acc.withdraw(200);
                }
                catch (const exception& e)
                {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
            if (test_number == 3)
            {
                // Тест 3: отрицательный начальный баланс
                try
                {
                    BankAccount acc("Толя", -42);
                }
                catch (const exception& e)
                {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
        }
        else { break; }
    }

    return 0;
}
