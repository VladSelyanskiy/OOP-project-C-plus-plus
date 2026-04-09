#pragma once

class Shape {
public:
    // Чисто виртуальные функции (слайд 3.6)
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void print() const = 0;

    // Виртуальный деструктор (слайд 3.7)
    virtual ~Shape() {}
};

class Circle : public Shape {  // отношение IS-A (слайд 3.1)
private:
    double radius;

public:
    Circle(double r);
    double area() const;
    double perimeter() const;
    void print() const;
};


class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h);
    double area() const;
    double perimeter() const;
    void print() const;
};
