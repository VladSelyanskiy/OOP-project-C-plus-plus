#include "Student.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Конструктор (слайд 2.6)
Student::Student(const string& n, const vector<int>& g, int i)
    : name(n), grades(g), id(i)
{
    cout << "КОНСТРУКТОР: " << name << endl;
}

// TODO: Добавьте деструктор с выводом (для наблюдения)
// ~Student() { cout << "ДЕСТРУКТОР: " << name << endl; }
Student::~Student()
{
    cout << "ДЕСТРУКТОР: " << name << endl;
}

// Метод для добавления оценки
void Student::addGrade(int grade) {
    if (grade >= 2 && grade <= 5) {
        grades.push_back(grade);
    }
}

// Метод для вычисления среднего
double Student::average() const {
    if (grades.empty()) return 0;
    double sum = 0;
    for (int g : grades) sum += g;
    return sum / grades.size();
}

// Метод для вывода
void Student::print() const {
    cout << "ID: " << id << ", Имя: " << name << ", Оценки: ";
    for (int g : grades) cout << g << " ";
    cout << "(среднее: " << average() << ")" << endl;
}

// Мы НЕ пишем конструктор копирования — компилятор сделает сам!
// Это и есть правило нуля
