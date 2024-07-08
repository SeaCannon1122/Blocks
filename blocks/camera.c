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



void camera_render_world(struct camera* _camera, struct world* _world) {

    struct v3dabs cords = { 0, 0, 0 };

    double _sin_theta = sin(_camera->direction_sph3d.theta);
    double _cos_theta = cos(_camera->direction_sph3d.theta);
    double _sin_phi = sin(_camera->direction_sph3d.phi);
    double _cos_phi = cos(_camera->direction_sph3d.phi);

    struct v3d direction = {
                -_camera->direction_sph3d.radius * _sin_theta * _cos_phi,
                _camera->direction_sph3d.radius * _cos_theta * _cos_phi,
                _camera->direction_sph3d.radius * _sin_phi
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

            _camera->pixels[i * _camera->width + j] = 0x88b2ff;

            double min = -1;

            if (inttime.x > 0) {
                _camera->pixels[i * _camera->width + j] = 0xff0000;
                min = inttime.x;
            }

            if (inttime.y > 0) {
                if (inttime.x == 0 || inttime.y < min) {
                    _camera->pixels[i * _camera->width + j] = 0xff00;
                    min = inttime.y;
                }
                
            }

            if (inttime.z > 0) {
                if ((inttime.x == 0 && inttime.y == 0) || inttime.z < min) {
                    _camera->pixels[i * _camera->width + j] = 0xff;
                }
            }

        }
        
    }   

    printf("frame_done ");
}

void flash_camera_screen(struct camera* _camera) {
    for (int i = 0; i < _camera->width; i++) {
        for (int j = 0; j < _camera->height; j++) {
            _camera->pixels[i + j * _camera->width] = 0x88b2ff;
        }
    }
}

double min_3double(double _a, double _b, double _c) {
    double min = _a;
    if (_b < min) min = _b;
    if (_c < min) min = _c;
    return min;
}

double max_3double(double _a, double _b, double _c) {
    double max = _a;
    if (_b > max) max = _b;
    if (_c > max) max = _c;
    return max;
}

void camera_render_triangle(struct camera* _camera, struct triangle* _tri) {

    struct v2d cords[3];

    for (int i = 0; i < 3; i++) {
        struct v3d piercer = { _tri->p[i].x - _camera->position.x, _tri->p[i].y - _camera->position.y, _tri->p[i].z - _camera->position.z };
        printf("p %llf %llf %llf\n", piercer.x, piercer.y, piercer.z);
        double factor = _camera->direction_sph3d.radius * _camera->direction_sph3d.radius / (_camera->direction_v3d.x * piercer.x + _camera->direction_v3d.y * piercer.y + _camera->direction_v3d.z * piercer.z);
        printf("factor %llf\n", factor);
        if (factor <= 0 || factor >= 1) return;

        struct v3d comb = { piercer.x * factor - _camera->direction_v3d.x, piercer.y * factor - _camera->direction_v3d.y, piercer.z * factor - _camera->direction_v3d.z };
        cords[i] = (struct v2d) {
            (piercer.x * _camera->xpixelpointer.x + piercer.y * _camera->xpixelpointer.y + piercer.z * _camera->xpixelpointer.z) / _camera->pixel_size + _camera->width / 2, 
            (piercer.x * _camera->ypixelpointer.x + piercer.y * _camera->ypixelpointer.y + piercer.z * _camera->ypixelpointer.z) / _camera->pixel_size + _camera->height / 2 };
    }

    

    struct v2dabs min = { (int) floor(min_3double(cords[0].x, cords[1].x, cords[2].x)), (int) floor(min_3double(cords[0].y, cords[1].y, cords[2].y, 0))};
    struct v2dabs max = { (int)floor(max_3double(cords[0].x, cords[1].x, cords[2].x, _camera->width-1)), (int)floor(max_3double(cords[0].y, cords[1].y, cords[2].y, _camera->height-1)) };

    min.x = clamp(min.x, _camera->width - 1, 0);
    min.y = clamp(min.y, _camera->height - 1, 0);
    max.x = clamp(max.x, _camera->width - 1, 0);
    max.y = clamp(max.y, _camera->height - 1, 0);


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


    return;
};

void delete_camera(struct camera* _camera) {
    free(_camera->pixels);
    free(_camera);
    return;
}