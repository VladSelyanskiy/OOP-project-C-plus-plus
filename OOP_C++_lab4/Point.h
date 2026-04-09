#pragma once

class Point
{
private:
	double x, y;
public:
	Point(double x_val, double y_val);
	Point();
	Point(double val);
	double getX() const;
	double getY() const;
	void setX(double newX);
	void setY(double newY);
	void print() const;
	double distanceTo(const Point& other) const;
};
