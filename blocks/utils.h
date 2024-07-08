#ifndef UTILS_H
#define UTILS_H

#include "headers.h"

#define PI 3.14159265358979323846

#define max(a, b, c) ((a > b && a > c) ? a : (b > c ? b : c))
#define min(a, b, c) ((a < b && a < c) ? a : (b < c ? b : c))

#define clamp(_value, _max, _min) (_value > _max ? _max : (_value < _min ? _min : _value))

#define floor_double(_value) (_value > 0 ? (int)_value : (_value == (double)(int)_value ? (int)_value : (int)_value - 1))
#define ceil_double(_value) (_value < 0 ? (int)_value : (_value == (double)(int)_value ? (int)_value : (int)_value + 1))

#endif // !UTILS_H
