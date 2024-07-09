#ifndef UTILS_H
#define UTILS_H

#include "headers.h"

#define PI 3.14159265358979323846

#define clamp(_value, _max, _min) (_value > _max ? _max : (_value < _min ? _min : _value))

#define clamp_int(_value, _max, _min) ((unsigned int) _value > (unsigned int)_max ? (unsigned int) _max : ((unsigned int)_value < (unsigned int)_min ? (unsigned int)_min : (unsigned int)_value))

#define floor_double(_value) (_value > 0 ? (int)_value : (_value == (double)(int)_value ? (int)_value : (int)_value - 1))
#define ceil_double(_value) (_value < 0 ? (int)_value : (_value == (double)(int)_value ? (int)_value : (int)_value + 1))


double min_3double(double _a, double _b, double _c);

double max_3double(double _a, double _b, double _c);

#endif // !UTILS_H
