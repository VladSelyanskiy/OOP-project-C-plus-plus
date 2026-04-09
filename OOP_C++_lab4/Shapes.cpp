#include "Shapes.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

//class Shape {
//public:
//    // Чисто виртуальные функции (слайд 3.6)
//    virtual double area() const = 0;
//    virtual double perimeter() const = 0;
//    virtual void print() const = 0;
//
//    // Виртуальный деструктор (слайд 3.7)
//    virtual ~Shape() {}
//};

Circle::Circle(double r) : radius(r) {}
// TODO: Переопределите виртуальные функции (слайд 3.4)
double Circle::area() const
{
    return acos(-1.0) * radius * radius; 
}
double Circle::perimeter() const
{ 
    return 2 * acos(-1.0) * radius; 
}
void Circle::print() const
{
    cout << "Круг: радиус = " << radius << 
        ", площадь = " << Circle::area() <<
        ", периметр = " << Circle::perimeter() <<
        endl;
}

Rectangle::Rectangle(double w, double h) : width(w), height(h) {}
// TODO: Переопределите виртуальные функции
double Rectangle::area() const
{ 
    return width * height; 
}
double Rectangle::perimeter() const
{ 
    return 2 * (width + height); 
}
void Rectangle::print() const
{
    cout << "Прямоугольник: " << width << " x " << height <<
        ", площадь = " << Rectangle::area() <<
        ", периметр = " << Rectangle::perimeter() <<
        endl;
}
