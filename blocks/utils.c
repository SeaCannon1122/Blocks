#include "headers.h"

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