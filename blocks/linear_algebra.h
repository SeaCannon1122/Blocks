#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "headers.h"

#define magnitude_v3d(_v3d) (sqrt(_v3d.x * _v3d.x + _v3d.y * _v3d.y + _v3d.z * _v3d.z))

#define scale_v3d(_v3d, _scalar) ((struct v3d) {_v3d.x * c, _v3d.y * c, _v3d.z * c,})

#define add_v3d(_a, _b) ((struct v3d) {_a.x + _b.x, _a.y + _b.y, _a.z + _b.z})

#define sub_v3d(_a, _b) ((struct v3d) {_a.x - _b.x, _a.y - _b.y, _a.z - _b.z})

#define unit_v3d(_v3d) (scale_v3d(_v3d, magnitude_v3d(_v3d)))

#define dot_v3d(_a, _b) (_a.x *_b.x + _a.y + _b.y + _a.z * _b.z)

#define dot_v2d(_a, _b) (_a.x *_b.x + _a.y + _b.y)

#define cross_v3d(_a, _b) ((struct v3d) {_a.y * _b.z - _a.z *_b.y, _a.z * _b * x - _a.x * _b.z, _a.x * _b.y - _a.y * _b.x})

#define cos_v3d(_a, _b) (dot_v3d(_a, _b)/(magnitude_v3d(_a) * magnitude_v3d(_b)))

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

struct v2dabs {
	int x;
	int y;
};

void draw_line(unsigned int* _buffer, unsigned int _width, unsigned int _height, struct line* _line, unsigned int color);

#endif // !LINEAR_ALGEBRA_H
