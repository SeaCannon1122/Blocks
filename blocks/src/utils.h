#ifndef UTILS_H
#define UTILS_H

#include "headers.h"

#define PI 3.14159265358979323846

int clamp_int(int _value, int _max, int _min);
double clamp(double _value, double _max, double _min);

int string_length(char* _str);

#endif // !UTILS_H
