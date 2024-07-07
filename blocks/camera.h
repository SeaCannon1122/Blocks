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
    unsigned int render_distance;
};

struct camera* new_camera(struct v3d _eye_position, struct sph3d _direction, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int render_distance);

void set_camera_size(struct camera* _camera, unsigned int _new_width, unsigned int _new_height);

void render_camera(struct camera* _camera, struct world* _world);

void delete_camera(struct camera* _camera);
#endif // !CAMERA_H
