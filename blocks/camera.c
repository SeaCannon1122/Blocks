#include "headers.h"

struct camera* new_camera(struct v3d _position, struct sph3d _direction, unsigned int _width, unsigned int _height, double _pixel_size) {
    if (_pixel_size <= 0) return NULL;

    struct camera* camera_ = (struct camera*)malloc(sizeof(struct camera));
    if (camera_ == NULL) return NULL;

    camera_->pixels = (unsigned int*)malloc(_height * _width * sizeof(unsigned int));
    if (camera_->pixels == NULL) {
        free(camera_);
        return NULL;
    }

    camera_->position = _position;
    camera_->direction = _direction;
    camera_->height = _height;
    camera_->width = _width;
    camera_->pixel_size = _pixel_size;
    return camera_;
   
}

void set_camera_size(struct camera* _camera, unsigned int _new_width, unsigned int _new_height) {
    if (_new_width == _camera->width && _new_height == _camera->height) return;
    
    unsigned int* temp = (unsigned int*)malloc(_new_height * _new_width * sizeof(unsigned int));
    if (temp == NULL) return;

    if (_camera->pixels != NULL) free(_camera->pixels);

    _camera->pixels = temp;
    _camera->width = _new_width;
    _camera->height = _new_height;

    return;
}

