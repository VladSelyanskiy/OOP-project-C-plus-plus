#include <iostream>
#include <cmath>

#include "Point.h"

using namespace std;


// TODO: Конструктор с параметрами (слайд 2.2)
// Должен инициализировать x и y через список инициализации
Point::Point(double x_val, double y_val) : x(x_val), y(y_val)
{
    cout << "Создана точка (" << x << ", " << y << ")" << endl;
}

// TODO: Конструктор по умолчанию (слайд 2.1)
// Должен создавать точку (0, 0)
Point::Point() : x(0), y(0)
{
    cout << "Создана точка (0,0)" << endl;
}

// TODO: Делегирующий конструктор (слайд 2.4)
// Конструктор с одним параметром (создаёт точку (val, val))
// Должен делегировать конструктору с двумя параметрами
Point::Point(double val) : Point(val, val) {}

// TODO: Геттеры (слайд 1.5)
// Должны быть константными
double Point::getX() const { return x; }

double Point::getY() const { return y; }

// TODO: Сеттеры (слайд 1.5)
// setX и setY (без проверок пока)
void Point::setX(double newX)
{
    x = newX;
}

void Point::setY(double newY)
{
    y = newY;
}

// Константный метод для вывода (слайд 1.6)
void Point::print() const {
    cout << "(" << x << ", " << y << ")";
}

// Метод для вычисления расстояния
double Point::distanceTo(const Point& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return sqrt(dx * dx + dy * dy);
};
