#include "Common.h"

int mod(int x, int m) {
    int r = x % m;
    return r < 0 ? r + m : r;
}

double doubleMod(double x, double m) {
    double r = fmod(x, m);
    return r < 0 ? r + m : r;
}

bool angleIsInside(double angleBoundCounterClockwise, double angleBoundClockwise, double angleCheck) {
    if (angleBoundCounterClockwise < angleBoundClockwise) {
        return (angleBoundCounterClockwise < angleCheck && angleCheck < angleBoundClockwise);
    } else {
        return (angleBoundCounterClockwise < angleCheck || angleCheck < angleBoundClockwise);
    }
}

double angleBetween(double angleCounterClockwise, double angleClockwise) {
    return mod(angleClockwise - angleCounterClockwise, 360);
}

double smallestAngleBetween(double angle1, double angle2) {
    double ang = angleBetween(angle1, angle2);
    return fmin(ang, 360 - ang);
}

double midAngleBetween(double angleCounterClockwise, double angleClockwise) {
    return mod(angleCounterClockwise + angleBetween(angleCounterClockwise, angleClockwise) / 2.0, 360);
}

double distanceBetween(double x1, double x2, double y1, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int sign(int value) {
    return value >= 0 ? 1 : -1;
}

int sign(double value) {
    return value >= 0 ? 1 : -1;
}

double degreesToRadians(double degrees) {
    return degrees * TO_RADIANS;
}

double radiansToDegrees(double radians) {
    return radians * TO_DEGREES;
}

double doubleAbs(double value) {
    return value * sign(value);
}
