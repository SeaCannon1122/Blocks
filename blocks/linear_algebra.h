#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "headers.h"

struct v3d {
	double x;
	double y;
	double z;
};

struct v3dabs {
	int x;
	int y;
	int z;
};

struct sph3d {
	double radius;
	double theta;
	double phi;
};

struct v2d {
	double x;
	double y;
};

struct v3d scale_v3d(struct v3d* _v3d, double _scalar);

struct v3d add_v3d(struct v3d* _addend1, struct v3d* _addend2);

struct v3d sub_v3d(struct v3d* _minuend, struct v3d* _subtrahend);

double magnitude_v3d(struct v3d* _v3d);

struct v3d unit_v3d(struct v3d* _v3d);

double dot_v3d(struct v3d* _factor1, struct v3d* _factor2);

struct v3d cross_v3d(struct v3d* _factor1, struct v3d* _factor2);


#endif // !LINEAR_ALGEBRA_H
