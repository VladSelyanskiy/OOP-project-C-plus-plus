#pragma once

#include <string>
#include <vector>

using namespace std;

class Student {
private:
    string name;        // RAII-тип
    vector<int> grades; // RAII-тип
    int id;             // примитивный тип

public:
    // Конструктор
    Student(const string& n, const vector<int>& g, int i);
    // Деструктор
    ~Student();
    // Метод для добавления оценки
    void addGrade(int grade);
    // Метод для вычисления среднего
    double average() const;
    // Метод для вывода
    void print() const;

    // Мы НЕ пишем конструктор копирования — компилятор сделает сам!
    // Это и есть правило нуля
};

