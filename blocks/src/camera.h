#ifndef CAMERA_H
#define CAMERA_H

#include "headers.h"

struct camera {
    struct v3d position;
    struct sph3d direction_sph3d;
    struct v3d direction_v3d;
    struct v3d xpixelpointer;
    struct v3d ypixelpointer;
    unsigned int* pixels;
    unsigned int height;
    unsigned int width;
    double pixel_size;
    unsigned int render_distance;
};

struct triangle {
    struct v3d p[3];
    unsigned int color;
};

struct oriented_rect {
    struct v3d Origin;
    struct v3d T;
    struct v3d N;
    struct v3d B;
    struct argb_image* image;
};

struct line {
    struct v3d p[2];
};

struct camera* new_camera(struct v3d _position, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int _render_distance);

void set_camera_direction_v3d(struct camera* _camera, struct v3d _v3d);
void set_camera_direction_sph3d(struct camera* _camera, struct sph3d _sph3d);

void set_camera_size(struct camera* _camera, unsigned int _new_width, unsigned int _new_height);

void camera_render_oriented_rect(struct camera* _camera, struct oriented_rect* _oriented_rect);

void delete_camera(struct camera* _camera);

void camera_render_cursor(struct camera* _camera);

void flash_camera_screen(struct camera* _camera);

#endif // !CAMERA_H
