/* Library of common functions and macros.
 */

#ifndef COMMON_H
#define COMMON_H

#include <math.h>

#define MATH_E 2.7182818284590452353602874713527

#define TO_RADIANS 0.01745329251994329576923690768489
#define TO_DEGREES 57.295779513082320876798154814105

#define ARRAYLENGTH(array) (sizeof(array) / sizeof(array[0]))

#define ARRAYSHIFTDOWN(a, lower, upper){          \
    if (upper == (sizeof(a)/sizeof(a[0])) - 1){   \
        for (int q = upper - 1; q >= lower; q--){ \
            *(a + q + 1) = *(a + q); }            \
    } else{                                       \
        for (int q = upper; q >= lower; q--){     \
            *(a + q + 1) = *(a + q); }}}

int mod(int x, int m);
double doubleMod(double x, double max);

bool angleIsInside(double angleBoundCounterClockwise, double angleBoundClockwise, double angleCheck);
double angleBetween(double angleCounterClockwise, double angleClockwise);
double smallestAngleBetween(double angle1, double angle2);
double midAngleBetween(double angleCounterClockwise, double angleClockwise);

double degreesToRadians(double degrees);
double radiansToDegrees(double radians);

int sign(int value);
int sign(double value);

double doubleAbs(double value);

double distanceBetween(double x1, double y1, double x2, double y2);

struct Vector2D {
    double x;
    double y;
};

struct Vector3D {
    double x;
    double y;
    double z;
};

#endif  // COMMON_H
