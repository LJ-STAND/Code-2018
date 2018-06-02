#ifndef POINT_H
#define POINT_H 

#include <Arduino.h>
#include <Common.h>

class Point {
public:
    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    double getAngle() {
        return doubleMod(90 - radiansToDegrees(atan2(y, x)), 360);
    }

    double getMagnitude() {
        return sqrt(x * x + y * y);
    }

    Point& operator+=(const Point &rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    Point& operator-=(const Point &rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    Point operator+(const Point &rhs) {
        return Point(x + rhs.x, y + rhs.y);
    }

    Point operator-(const Point &rhs) {
        return Point(x - rhs.x, y - rhs.y);
    }

    double x = 0;
    double y = 0;
};

#endif // POINT_H