#include "headers.h"

int clamp_int(int _value, int _min, int _max) {
    return (_value > _max ? _max : (_value < _min ? _min : _value));
}

double clamp(double _value, double _min, double _max) {
    return (_value > _max ? _max : (_value < _min ? _min : _value));
}

