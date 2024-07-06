#ifndef CAMERA_H
#define CAMERA_H

#include "headers.h"

struct camera {
    struct v3d position;
    struct sph3d direction;
    unsigned int* pixels;
    unsigned int height;
    unsigned int width;
    double pixel_size;
};

struct camera* new_camera(struct v3d _eye_position, struct sph3d _direction, unsigned int _width, unsigned int _height, double _pixel_size);

void set_camera_size(struct camera* _camera, unsigned int _new_width, unsigned int _new_height);

#endif // !CAMERA_H
