#include "headers.h"

struct v3d scale_v3d(struct v3d* _v3d, double _scalar) {
	return (struct v3d) { _v3d->x * _scalar, _v3d->y * _scalar, _v3d->z * _scalar };
}

struct v3d add_v3d(struct v3d* _addend1, struct v3d* _addend2) {
	return (struct v3d) {_addend1->x + _addend2->x, _addend1->y + _addend2->y, _addend1->z + _addend2->z};
}

struct v3d sub_v3d(struct v3d* _minuend, struct v3d* _subtrahend) {
	return (struct v3d) { _minuend->x - _subtrahend->x, _minuend->y - _subtrahend->y, _minuend->z - _subtrahend->z };
}

double magnitude_v3d(struct v3d* _v3d) {
	return sqrt(_v3d->x * _v3d->x + _v3d->y * _v3d->y + _v3d->z * _v3d->z);
}

struct v3d unit_v3d(struct v3d* _v3d) {
	double magnitude = magnitude_v3d(_v3d);
	return (struct v3d) { _v3d->x / magnitude, _v3d->y / magnitude, _v3d->z / magnitude };
}

double dot_v3d(struct v3d* _factor1, struct v3d* _factor2) {
	return _factor1->x * _factor2->x + _factor1->y * _factor2->y + _factor1->z * _factor2->z;
}

struct v3d cross_v3d(struct v3d* _factor1, struct v3d* _factor2) {
	return (struct v3d) {
		_factor1->y * _factor2->z - _factor1->z * _factor2->y,
		_factor1->z * _factor2->x - _factor1->x * _factor2->z,
		_factor1->x * _factor2->y - _factor1->y * _factor2->x
	};
}

