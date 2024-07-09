#include "headers.h"

struct camera* new_camera(struct v3d _position, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int _render_distance) {
    if (_pixel_size <= 0) return NULL;

    struct camera* camera_ = (struct camera*)malloc(sizeof(struct camera));
    if (camera_ == NULL) return NULL;

    camera_->pixels = (unsigned int*)malloc(_height * _width * sizeof(unsigned int));
    if (camera_->pixels == NULL) {
        free(camera_);
        return NULL;
    }

    camera_->position = _position;
    camera_->direction_sph3d = (struct sph3d){ 1, 0, 0 };
    camera_->direction_v3d = (struct v3d){ 0, 1, 0 };
    camera_->xpixelpointer = (struct v3d){ _pixel_size, 0, 0 };
    camera_->xpixelpointer = (struct v3d){ 0, 0, _pixel_size };
    camera_->height = _height;
    camera_->width = _width;
    camera_->pixel_size = _pixel_size;
    camera_->render_distance = _render_distance;
    return camera_;
   
}

void set_camera_direction_v3d(struct camera* _camera, struct v3d _v3d) {
   _camera->direction_v3d = _v3d;
   _camera->direction_sph3d = (struct sph3d) {magnitude_v3d(_v3d), atan(- _camera->direction_v3d.x / _camera->direction_v3d.y) , asin(_camera->direction_v3d.z / magnitude_v3d(_v3d))};
   _camera->xpixelpointer = (struct v3d){ cos(_camera->direction_sph3d.theta), sin(_camera->direction_sph3d.theta), 0 };
   _camera->xpixelpointer = (struct v3d){ sin(_camera->direction_sph3d.theta) * sin(_camera->direction_sph3d.phi), -cos(_camera->direction_sph3d.theta) * sin(_camera->direction_sph3d.phi), cos(_camera->direction_sph3d.phi)};
}

void set_camera_direction_sph3d(struct camera* _camera, struct sph3d _sph3d) {
    _camera->direction_sph3d = _sph3d;
    _camera->direction_sph3d.phi = clamp(_camera->direction_sph3d.phi, PI / 2, -PI / 2);
    _camera->direction_v3d = (struct v3d) { 
                -_camera->direction_sph3d.radius * sin(_camera->direction_sph3d.theta) * cos(_camera->direction_sph3d.phi),
                _camera->direction_sph3d.radius * cos(_camera->direction_sph3d.theta) * cos(_camera->direction_sph3d.phi),
                _camera->direction_sph3d.radius * sin(_camera->direction_sph3d.phi)
    };

    _camera->xpixelpointer = (struct v3d){ cos(_camera->direction_sph3d.theta), sin(_camera->direction_sph3d.theta), 0 };
    _camera->ypixelpointer = (struct v3d){ sin(_camera->direction_sph3d.theta) * sin(_camera->direction_sph3d.phi), -cos(_camera->direction_sph3d.theta) * sin(_camera->direction_sph3d.phi), cos(_camera->direction_sph3d.phi) };
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


void flash_camera_screen(struct camera* _camera) {
    for (int i = 0; i < _camera->width; i++) {
        for (int j = 0; j < _camera->height; j++) {
            _camera->pixels[i + j * _camera->width] = 0x88b2ff;
        }
    }
}

void camera_render_triangle(struct camera* _camera, struct triangle* _tri) {

    struct v2d cords[3];
    int cords_length = 0;
    struct v2d out_cords[3];
    int out_cords_length = 0;

    for (int i = 0; i < 3; i++) {
        struct v3d piercer = { _tri->p[i].x - _camera->position.x, _tri->p[i].y - _camera->position.y, _tri->p[i].z - _camera->position.z };
        double factor = _camera->direction_sph3d.radius * _camera->direction_sph3d.radius / (_camera->direction_v3d.x * piercer.x + _camera->direction_v3d.y * piercer.y + _camera->direction_v3d.z * piercer.z);
        
        struct v3d comb = { piercer.x * factor - _camera->direction_v3d.x, piercer.y * factor - _camera->direction_v3d.y, piercer.z * factor - _camera->direction_v3d.z };
        struct v2d cordinates = {
            (comb.x * _camera->xpixelpointer.x + comb.y * _camera->xpixelpointer.y + comb.z * _camera->xpixelpointer.z) / _camera->pixel_size + _camera->width / 2,
            (comb.x * _camera->ypixelpointer.x + comb.y * _camera->ypixelpointer.y + comb.z * _camera->ypixelpointer.z) / _camera->pixel_size + _camera->height / 2 };
        
        if (factor < 0) {
            out_cords[out_cords_length] = cordinates;
            out_cords_length++;
        }

        else if (factor > 0) {
            cords[cords_length] = cordinates;
            cords_length++;
        }

    }

    if (out_cords_length == 0) {
        struct v2dabs min = { (int)floor(min_3double(cords[0].x, cords[1].x, cords[2].x)), (int)floor(min_3double(cords[0].y, cords[1].y, cords[2].y, 0)) };
        struct v2dabs max = { (int)floor(max_3double(cords[0].x, cords[1].x, cords[2].x, _camera->width - 1)), (int)floor(max_3double(cords[0].y, cords[1].y, cords[2].y, _camera->height - 1)) };

        min.x = (min.x > (signed int)_camera->width - 1 ? (signed int)_camera->width - 1 : (min.x < 0 ? 0 : min.x));
        min.y = (min.y > (signed int)_camera->height - 1 ? (signed int)_camera->height - 1 : (min.y < 0 ? 0 : min.y));
        max.x = (max.x > (signed int)_camera->width - 1 ? (signed int)_camera->width - 1 : (max.x < 0 ? 0 : max.x));
        max.y = (max.y > (signed int)_camera->height - 1 ? (signed int)_camera->height - 1 : (max.y < 0 ? 0 : max.y));


        for (int i = min.x; i <= max.x; i++) {
            for (int j = min.y; j <= max.y; j++) {
                double v0x = cords[2].x - cords[0].x;
                double v0y = cords[2].y - cords[0].y;
                double v1x = cords[1].x - cords[0].x;
                double v1y = cords[1].y - cords[0].y;
                double v2x = i + 0.5 - cords[0].x;
                double v2y = j + 0.5 - cords[0].y;

                // Compute dot products
                double dot00 = v0x * v0x + v0y * v0y;
                double dot01 = v0x * v1x + v0y * v1y;
                double dot02 = v0x * v2x + v0y * v2y;
                double dot11 = v1x * v1x + v1y * v1y;
                double dot12 = v1x * v2x + v1y * v2y;

                // Compute barycentric coordinates
                double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
                double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
                double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

                // Check if point is in triangle
                if ((u >= 0) && (v >= 0) && (u + v <= 1)) _camera->pixels[i + _camera->width * j] = _tri->color;


            }
        }
    }

    else if (out_cords_length == 1) {
        

    }

    else if (out_cords_length == 2) {

    }

    return;
};

void delete_camera(struct camera* _camera) {
    free(_camera->pixels);
    free(_camera);
    return;
}