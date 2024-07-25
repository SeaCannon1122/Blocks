#ifndef IMAGE_H
#define IMAGE_H

struct color {
	char b;
	char g;
	char r;
	char a;
};

union pixel {
	unsigned int value;
	struct color color;
};

struct argb_image {
	unsigned int width;
	unsigned int height;
	union pixel* data;
};



#endif // !IMAGE_H
