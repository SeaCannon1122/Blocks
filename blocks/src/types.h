#ifndef TYPES_H
#define TYPES_H

#include "headers.h"

//2d

struct v2d {
	double x;
	double y;
};

struct v2dabs {
	int x;
	int y;
};

struct line2d {
	struct v2d p[2];
};

//3d

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

struct line3d {
	struct v3d p[2];
};

struct argb_color {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
};

union pixel {
	unsigned int value;
	struct argb_color color;
};

struct argb_image {
	unsigned int width;
	unsigned int height;
	union pixel* data;
};

struct oriented_rect {
	struct v3d Origin;
	struct v3d T;
	struct v3d N;
	struct v3d B;
	struct argb_image* image;
	double distance;
};

enum direction {
	POS_X = 0,
	NEG_X = 1,
	POS_Y = 2,
	NEG_Y = 3,
	POS_Z = 4,
	NEG_Z = 5
};

struct block {
	short id;
	char facing;
	char state;
};


#endif // !TYPES_H
