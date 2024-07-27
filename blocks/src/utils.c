#include "headers.h"

int clamp_int(int _value, int _max, int _min) {
    return (_value > _max ? _max : (_value < _min ? _min : _value));
}

double clamp(double _value, double _max, double _min) {
    return (_value > _max ? _max : (_value < _min ? _min : _value));
}

double min_3double(double _a, double _b, double _c) {
    double min = _a;
    if (_b < min) min = _b;
    if (_c < min) min = _c;
    return min;
}

double max_3double(double _a, double _b, double _c) {
    double max = _a;
    if (_b > max) max = _b;
    if (_c > max) max = _c;
    return max;
}

double min_4double(double _a, double _b, double _c, double _d) {
    double min = _a;
    if (_b < min) min = _b;
    if (_c < min) min = _c;
    if (_d < min) min = _d;
    return min;
}

double max_4double(double _a, double _b, double _c, double _d) {
    double max = _a;
    if (_b > max) max = _b;
    if (_c > max) max = _c;
    if (_d > max) max = _d;
    return max;
}