#ifndef POINT_H
#define POINT_H 

#include <Arduino.h>
#include <Common.h>

class Point {
public:
    Point() {
        x = 0;
        y = 0;
    }
    
    Point(double x, double y) : x(x), y(y) {}

    void setAngleMagnitude(double angle, double magnitude) {
        double polarAngle = doubleMod(90 - angle, 360);

        x = magnitude * cos(degreesToRadians(polarAngle));
        y = magnitude * sin(degreesToRadians(polarAngle));
    }

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

    bool operator !=(const Point &p) {
        return x != p.x || y != p.y;
    }

    double x;
    double y;
};

#endif // POINT_H