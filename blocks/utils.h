#ifndef UTILS_H
#define UTILS_H

#include "headers.h"

#define PI 3.14159265358979323846

#define floor_double(_value) (_value > 0 ? (int)_value : (_value == (double)(int)_value ? (int)_value : (int)_value - 1))
#define ceil_double(_value) (_value < 0 ? (int)_value : (_value == (double)(int)_value ? (int)_value : (int)_value + 1))

int clamp_int(int _value, int _max, int _min);
double clamp(double _value, double _max, double _min);

double min_3double(double _a, double _b, double _c);
double max_3double(double _a, double _b, double _c);

double max_4double(double _a, double _b, double _c, double _d);
double min_4double(double _a, double _b, double _c, double _d);

#endif // !UTILS_H
