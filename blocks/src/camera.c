#include "headers.h"

struct camera* new_camera(struct v3d _position, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int _render_distance) {
    if (_pixel_size <= 0) return NULL;

    struct camera* camera_ = (struct camera*)malloc(sizeof(struct camera));
    if (camera_ == NULL) return NULL;

    camera_->pixels = (union pixel*)calloc( _height * _width, sizeof(union pixel));
    if (camera_->pixels == NULL) {
        free(camera_);
        return NULL;
    }

    camera_->position = _position;
    camera_->direction_sph3d = (struct sph3d){ 1, 0, 0 };
    camera_->direction_v3d = (struct v3d){ 0, 1, 0 };
    camera_->xpixelpointer = (struct v3d){ 1, 0, 0 };
    camera_->xpixelpointer = (struct v3d){ 0, 0, 1 };
    camera_->height = _height;
    camera_->width = _width;
    camera_->pixel_size = _pixel_size;
    camera_->render_distance = _render_distance;
    return camera_;
   
}

void set_camera_direction_v3d(struct camera* _camera, struct v3d _direction) {
   _camera->direction_v3d = _direction;
   _camera->direction_sph3d = (struct sph3d) {sqrt(_direction.x * _direction.x + _direction.y * _direction.y + _direction.z * _direction.z), atan(-_camera->direction_v3d.x / _camera->direction_v3d.y) , asin(_camera->direction_v3d.z / sqrt(_direction.x * _direction.x + _direction.y * _direction.y + _direction.z * _direction.z))};
   _camera->xpixelpointer = (struct v3d){ cos(_camera->direction_sph3d.theta), sin(_camera->direction_sph3d.theta), 0 };
   _camera->xpixelpointer = (struct v3d){ sin(_camera->direction_sph3d.theta) * sin(_camera->direction_sph3d.phi), -cos(_camera->direction_sph3d.theta) * sin(_camera->direction_sph3d.phi), cos(_camera->direction_sph3d.phi)};
}

void set_camera_direction_sph3d(struct camera* _camera, struct sph3d _sph3d) {
    _camera->direction_sph3d = _sph3d;
    _camera->direction_sph3d.phi = clamp(_camera->direction_sph3d.phi, -PI / 2, PI / 2);
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
    
    union pixel* temp = (union pixel*)calloc(_new_height * _new_width, sizeof(unsigned int));
    if (temp == NULL) return;

    if (_camera->pixels != NULL) free(_camera->pixels);

    _camera->pixels = temp;
    _camera->width = _new_width;
    _camera->height = _new_height;

    return;
}


void flash_camera_screen(struct camera* _camera) {
    for (unsigned int i = 0; i < _camera->width; i++) {
        for (unsigned int j = 0; j < _camera->height; j++) {
            _camera->pixels[i + j * _camera->width].value = 0x88b2ff;
        }
    }
}


void camera_render_cursor(struct camera* _camera) {

    struct v3d pos = _camera->position;
    struct v3d dir = _camera->direction_v3d;
    struct v3d tanx = _camera->xpixelpointer;
    struct v3d tany = _camera->ypixelpointer;

    struct line3d lines[3] = {
        { (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z }, (struct v3d) { pos.x + dir.x + _camera->direction_sph3d.radius / 10 , pos.y + dir.y , pos.z + dir.z } },
        { (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z }, (struct v3d) { pos.x + dir.x , pos.y + dir.y + _camera->direction_sph3d.radius / 10 , pos.z + dir.z } },
        { (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z }, (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z + _camera->direction_sph3d.radius / 10 } }
    };
    

    for (int c = 0; c < 3; c++) {
        struct line2d line_screen;
        for (int i = 0; i < 2; i++) {
            struct v3d piercer = { lines[c].p[i].x - pos.x, lines[c].p[i].y - pos.y, lines[c].p[i].z - pos.z};
            double factor = _camera->direction_sph3d.radius * _camera->direction_sph3d.radius / (_camera->direction_v3d.x * piercer.x + _camera->direction_v3d.y * piercer.y + _camera->direction_v3d.z * piercer.z);
            line_screen.p[i] = (struct v2d){(piercer.x * tanx.x + piercer.y * tanx.y + piercer.z * tanx.z) * factor / _camera->pixel_size + _camera->width / 2,
                                                                            (piercer.x * tany.x + piercer.y * tany.y + piercer.z * tany.z) * factor / _camera->pixel_size + _camera->height / 2
            };
        }
    
        unsigned int color;

        if (c == 0) color = 0xff0000;
        else if (c == 1) color = 0xff;
        else color = 0xff00;

        draw_line(_camera->pixels, _camera->width, _camera->height, &line_screen, color);

    }

    return;

}

struct v3dabs* get_block_cords_selected(struct camera* _camera, double _reach, int* blocks_length) {
    int x = (int)floor(_camera->position.x);
    int y = (int)floor(_camera->position.y);
    int z = (int)floor(_camera->position.z);

    double dx = _camera->direction_v3d.x / _camera->direction_sph3d.radius * _reach;
    double dy = _camera->direction_v3d.y / _camera->direction_sph3d.radius * _reach;
    double dz = _camera->direction_v3d.z / _camera->direction_sph3d.radius * _reach;

    int stepX = sign(_camera->direction_v3d.x);
    int stepY = sign(_camera->direction_v3d.y);
    int stepZ = sign(_camera->direction_v3d.z);

    double tMaxX = (stepX > 0 ? (x + 1 - _camera->position.x) : (_camera->position.x - x)) / fabs(dx);
    double tMaxY = (stepY > 0 ? (y + 1 - _camera->position.y) : (_camera->position.y - y)) / fabs(dy);
    double tMaxZ = (stepZ > 0 ? (z + 1 - _camera->position.z) : (_camera->position.z - z)) / fabs(dz);

    double tDeltaX = 1 / fabs(dx);
    double tDeltaY = 1 / fabs(dy);
    double tDeltaZ = 1 / fabs(dz);

    // Calculate the maximum possible t value when we reach the end of the vector
    double length = _reach;
    double tEnd = length / length; // This simplifies to 1.0 for normalized vector

    struct v3dabs* cords = malloc(_reach * 100 * sizeof(struct v3dabs) + 1);
    *blocks_length = 0;
    while (tMaxX <= tEnd || tMaxY <= tEnd || tMaxZ <= tEnd) {
        if (z >= 0 && z <= 255) {
            cords[*blocks_length] = (struct v3dabs){ x, y, z };
            (*blocks_length)++;
        }
        
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                if (tMaxX > tEnd) break;
                x += stepX;
                tMaxX += tDeltaX;
            }
            else {
                if (tMaxZ > tEnd) break;
                z += stepZ;
                tMaxZ += tDeltaZ;
            }
        }
        else {
            if (tMaxY < tMaxZ) {
                if (tMaxY > tEnd) break;
                y += stepY;
                tMaxY += tDeltaY;
            }
            else {
                if (tMaxZ > tEnd) break;
                z += stepZ;
                tMaxZ += tDeltaZ;
            }
        }
    }
    return cords;
}

void delete_camera(struct camera* _camera) {
    free(_camera->pixels);
    free(_camera);
}