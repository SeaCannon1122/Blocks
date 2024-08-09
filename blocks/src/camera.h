#ifndef CAMERA_H
#define CAMERA_H

#include "headers.h"

struct camera {
    struct v3d position;
    struct sph3d direction_sph3d;
    struct v3d direction_v3d;
    struct v3d xpixelpointer;
    struct v3d ypixelpointer;
    union pixel* pixels;
    unsigned int height;
    unsigned int width;
    double pixel_size;
    unsigned int render_distance;
};

struct camera* new_camera(struct v3d _position, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int _render_distance);

void set_camera_direction_v3d(struct camera* _camera, struct v3d _v3d);
void set_camera_direction_sph3d(struct camera* _camera, struct sph3d _sph3d);

void set_camera_size(struct camera* _camera, unsigned int _new_width, unsigned int _new_height);

void flash_camera_screen(struct camera* _camera);

void camera_render_cursor(struct camera* _camera);

struct v3dabs* get_block_cords_selected(struct camera* _camera, double _reach, int* blcoks_length);

void delete_camera(struct camera* _camera);





#endif // !CAMERA_H
