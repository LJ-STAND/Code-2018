#ifndef LINE_DATA_H
#define LINE_DATA_H

// Struct for line angle and size
typedef struct LineData {
    double angle;
    double size;
    bool onField;

    LineData() {}
    LineData(double a, double s, bool o) : angle(a), size(s), onField(o) {}

    bool operator !=(const LineData &d) {
        return angle != d.angle || size != d.size || onField != d.onField;
    }
} LineData;

#endif // LINE_DATA_H
