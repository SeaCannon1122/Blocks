#include "headers.h"

struct camera* new_camera(struct v3d _position, struct sph3d _direction, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int _render_distance) {
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
    camera_->render_distance = _render_distance;
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

void render_camera(struct camera* _camera, struct world* _world) {

    struct v3dabs cords = { 0, 0, 0 };

    double _sin_theta = sin(_camera->direction.theta);
    double _cos_theta = cos(_camera->direction.theta);
    double _sin_phi = sin(_camera->direction.phi);
    double _cos_phi = cos(_camera->direction.phi);

    struct v3d direction = {
                -_camera->direction.radius * _sin_theta * _cos_phi,
                _camera->direction.radius * _cos_theta * _cos_phi,
                _camera->direction.radius * _sin_phi
    };

    struct v3d xtangentcomp = { _cos_theta * _camera->pixel_size, _sin_theta * _camera->pixel_size, 0 };

    struct v3d ytangentcomp = { _sin_theta * _sin_phi * _camera->pixel_size, -_cos_theta * _sin_phi * _camera->pixel_size, _cos_phi * _camera->pixel_size };

    for (unsigned int i = 0; i < _camera->height; i++) {
        for (unsigned int j = 0; j < _camera->width; j++) {
            
            /*
            struct v3d xtangent = { _cos_theta, _sin_theta, 0 };

            struct v3d ytangent = { _sin_theta * _sin_phi, -_cos_theta * _sin_phi, _cos_phi };

            struct v3d xpixelpointer = scale_v3d(&xtangent, _camera->pixel_size * (0.5 + j - 0.5 * _camera->width));
            struct v3d ypixelpointer = scale_v3d(&ytangent, _camera->pixel_size * (0.5 + i - 0.5 * _camera->height));
            
            struct v3d pixelpointer = {
                xpixelpointer.x + ypixelpointer.x + direction.x,
                xpixelpointer.y + ypixelpointer.y + direction.y,
                xpixelpointer.z + ypixelpointer.z + direction.z
            };
            */

            //all in one

            struct v3d pixelpointer = {
                xtangentcomp.x * (0.5 + j - 0.5 * _camera->width) + ytangentcomp.x * (0.5 + i - 0.5 * _camera->height) + direction.x,
                xtangentcomp.y * (0.5 + j - 0.5 * _camera->width) + ytangentcomp.y * (0.5 + i - 0.5 * _camera->height) + direction.y,
                ytangentcomp.z * (0.5 + i - 0.5 * _camera->height) + direction.z
            };

            
            struct v3d time = { 0, 0, 0 };

            struct v3d inttime = { 0, 0, 0 };
            
            cords.x = -6;

            while (-6 <= cords.x && cords.x <= 4) {
                cords.x ++;
                time.x = (cords.x - _camera->position.x) / pixelpointer.x;
                if(time.x > 0) {
                    cords.y = floor_double(pixelpointer.y * time.x + _camera->position.y);
                    cords.z = floor_double(pixelpointer.z * time.x + _camera->position.z);

                    if (-5 <= cords.x && cords.x <= 4 && -5 <= cords.y && cords.y <= 4 && -5 <= cords.z && cords.z <= 4) {
                        if (get_block_ptr(_world, &cords)->id && (inttime.x == 0 || time.x < inttime.x)) { inttime.x = time.x; }
                        cords.x--;
                        if (get_block_ptr(_world, &cords)->id && (inttime.x == 0 || time.x < inttime.x)) { inttime.x = time.x; }
                        cords.x++;
                    }

                }    
            
            }
            
            cords.y = -6;

            while (-6 <= cords.y && cords.y <= 4) {
                cords.y++;
                time.y = (cords.y - _camera->position.y) / pixelpointer.y;
                if (time.y > 0) {
                    cords.x = floor_double(pixelpointer.x * time.y + _camera->position.x);
                    cords.z = floor_double(pixelpointer.z * time.y + _camera->position.z);

                    if (-5 <= cords.x && cords.x <= 4 && -5 <= cords.y && cords.y <= 4 && -5 <= cords.z && cords.z <= 4) {
                        if (get_block_ptr(_world, &cords)->id && (inttime.y == 0 || time.y < inttime.y)) { inttime.y = time.y; }
                        cords.y--;
                        if (get_block_ptr(_world, &cords)->id && (inttime.y == 0 || time.y < inttime.y)) { inttime.y = time.y; }
                        cords.y++;
                    }

                }

            }

            cords.z = -6;

            while (-6 <= cords.z && cords.z <= 4) {
                cords.z++;
                time.z = (cords.z - _camera->position.z) / pixelpointer.z;
                if (time.z > 0) {
                    cords.x = floor_double(pixelpointer.x * time.z + _camera->position.x);
                    cords.y = floor_double(pixelpointer.y * time.z + _camera->position.y);

                    if (-5 <= cords.x && cords.x <= 4 && -5 <= cords.y && cords.y <= 4 && -5 <= cords.z && cords.z <= 4) {
                        if (get_block_ptr(_world, &cords)->id && (inttime.z == 0 || time.z < inttime.z)) { inttime.z = time.z; }
                        cords.z--;
                        if (get_block_ptr(_world, &cords)->id && (inttime.z == 0 || time.z < inttime.z)) { inttime.z = time.z; }
                        cords.z++;
                    }

                }

            }

            _camera->pixels[i * _camera->width + j] = 0xe5e5e5;

            double min = -1;

            if (inttime.x > 0) {
                _camera->pixels[i * _camera->width + j] = 0x6f5134;
                min = inttime.x;
            }

            if (inttime.y > 0) {
                if (inttime.x == 0 || inttime.y < min) {
                    _camera->pixels[i * _camera->width + j] = 0x6f5134;
                    min = inttime.y;
                }
                
            }

            if (inttime.z > 0) {
                if ((inttime.x == 0 && inttime.y == 0) || inttime.z < min) {
                    _camera->pixels[i * _camera->width + j] = 0x68b33d;
                }
            }

        }
        
    }   

    printf("frame_done ");
}

void delete_camera(struct camera* _camera) {
    free(_camera->pixels);
    free(_camera);
    return;
}