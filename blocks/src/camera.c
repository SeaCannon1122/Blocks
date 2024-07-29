#include "headers.h"

struct camera* new_camera(struct v3d _position, unsigned int _width, unsigned int _height, double _pixel_size, unsigned int _render_distance) {
    if (_pixel_size <= 0) return NULL;

    struct camera* camera_ = (struct camera*)malloc(sizeof(struct camera));
    if (camera_ == NULL) return NULL;

    camera_->pixels = (union pixel*)malloc(_height * _width * sizeof(union pixel));
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

void camera_render_oriented_rect(struct camera* _camera, struct oriented_rect* _o_rect) {

    if (_camera->width <= 0 || _camera->height <= 0) return;
    
    struct v3d pos = _camera->position;
    struct v3d dir = _camera->direction_v3d;
    struct v3d tanx = _camera->xpixelpointer;
    struct v3d tany = _camera->ypixelpointer;
    int width = _camera->width;
    int height = _camera->height;

    struct v3d corners[4] = {
        {_o_rect->Origin.x + _o_rect->N.x, _o_rect->Origin.y + _o_rect->N.y, _o_rect->Origin.z + _o_rect->N.z},
        {_o_rect->Origin.x + _o_rect->N.x + _o_rect->B.x, _o_rect->Origin.y + _o_rect->N.y + _o_rect->B.y, _o_rect->Origin.z + _o_rect->N.z + _o_rect->B.z},
        {_o_rect->Origin.x + _o_rect->B.x, _o_rect->Origin.y + _o_rect->B.y, _o_rect->Origin.z + _o_rect->B.z},
        _o_rect->Origin
    };

    struct v3d piercers[4];

    double factors[4];


    for (int i = 0; i < 4; i++) {
        piercers[i] = (struct v3d) {corners[i].x - pos.x, corners[i].y - pos.y, corners[i].z - pos.z};
        factors[i] = (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) / (dir.x * piercers[i].x + dir.y * piercers[i].y + dir.z * piercers[i].z);
    }

    struct v2d corners_on_screen[5];

    int corners_on_screen_length = 0;
    
    for (int i = 0; i < 4; i++) {

        unsigned int next_i = (i + 1) % 4;

        if (factors[i] <= 1 && factors[i] > 0) {
            corners_on_screen[corners_on_screen_length] = (struct v2d){ 
                (piercers[i].x * tanx.x + piercers[i].y * tanx.y + piercers[i].z * tanx.z) * factors[i] / _camera->pixel_size + _camera->width / 2,
                (piercers[i].x * tany.x + piercers[i].y * tany.y + piercers[i].z * tany.z) * factors[i] / _camera->pixel_size + _camera->height / 2
            };
            corners_on_screen_length++;

            if (factors[next_i] > 1 || factors[next_i] < 0) {
                struct v3d diff = { corners[next_i].x - pos.x, corners[next_i].y - pos.y, corners[next_i].z - pos.z};
                
                struct v2d moved_origin = { diff.x * tanx.x + diff.y * tanx.y + diff.z * tanx.z, diff.x * tany.x + diff.y * tany.y + diff.z * tany.z };

                struct v3d line_piercer = { corners[i].x - corners[next_i].x, corners[i].y - corners[next_i].y, corners[i].z - corners[next_i].z };

                double scalar = 1 - (dir.x * diff.x + dir.y * diff.y + dir.z * diff.z) / (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

                struct v3d new_dir = { dir.x * scalar, dir.y * scalar, dir.z * scalar };

                corners_on_screen[corners_on_screen_length] = (struct v2d){ 
                    ( ( line_piercer.x * tanx.x + line_piercer.y * tanx.y + line_piercer.z * tanx.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.x ) / _camera->pixel_size + _camera->width / 2,
                    ( ( line_piercer.x * tany.x + line_piercer.y * tany.y + line_piercer.z * tany.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.y ) / _camera->pixel_size + _camera->height / 2
                };
                corners_on_screen_length++;
            }

        }

        else if (factors[next_i] <= 1 && factors[next_i] > 0) {
            struct v3d diff = { corners[i].x - pos.x, corners[i].y - pos.y, corners[i].z - pos.z };

            struct v2d moved_origin = { diff.x * tanx.x + diff.y * tanx.y + diff.z * tanx.z, diff.x * tany.x + diff.y * tany.y + diff.z * tany.z };

            struct v3d line_piercer = { corners[next_i].x - corners[i].x, corners[next_i].y - corners[i].y, corners[next_i].z - corners[i].z };

            double scalar = 1 - (dir.x * diff.x + dir.y * diff.y + dir.z * diff.z) / (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

            struct v3d new_dir = { dir.x * scalar, dir.y * scalar, dir.z * scalar };

            corners_on_screen[corners_on_screen_length] = (struct v2d){ 
                ( ( line_piercer.x * tanx.x + line_piercer.y * tanx.y + line_piercer.z * tanx.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.x ) / _camera->pixel_size + _camera->width / 2,
                ( ( line_piercer.x * tany.x + line_piercer.y * tany.y + line_piercer.z * tany.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.y ) / _camera->pixel_size + _camera->height / 2
            };
            corners_on_screen_length++;
        }

        
    }

    if (corners_on_screen_length == 0) return;

    struct v2dabs screen_bound_min = { width, height };
    struct v2dabs screen_bound_max = {-1, -1};

    struct v2d normals[5];

    for (int i = 0; i < corners_on_screen_length; i++) {
        normals[i] = (struct v2d){ corners_on_screen[i].y - corners_on_screen[(i + 1) % corners_on_screen_length].y, corners_on_screen[(i + 1) % corners_on_screen_length].x - corners_on_screen[i].x };
        struct v2dabs abs = { clamp_int( (int)corners_on_screen[i].x, 0, width - 1), clamp_int((int)corners_on_screen[i].y, 0, height - 1) };
        
        if (abs.x > screen_bound_max.x) screen_bound_max.x = abs.x;
        if (abs.x < screen_bound_min.x) screen_bound_min.x = abs.x;
        if (abs.y > screen_bound_max.y) screen_bound_max.y = abs.y;
        if (abs.y < screen_bound_min.y) screen_bound_min.y = abs.y;
    }
    
    for (int j = screen_bound_min.y; j <= screen_bound_max.y; j++) {

        for (int i = screen_bound_min.x; i <= screen_bound_max.x; i++) { 

            struct v3d screen_point = { 
                pos.x + dir.x + (i - width / 2 + 0.5) * _camera->pixel_size * tanx.x + (j - height / 2 + 0.5) * _camera->pixel_size * tany.x,
                pos.y + dir.y + (i - width / 2 + 0.5) * _camera->pixel_size * tanx.y + (j - height / 2 + 0.5) * _camera->pixel_size * tany.y,
                pos.z + dir.z + (i - width / 2 + 0.5) * _camera->pixel_size * tanx.z + (j - height / 2 + 0.5) * _camera->pixel_size * tany.z
            };

            double scalar = -(_o_rect->T.x * (pos.x - _o_rect->Origin.x) + _o_rect->T.y * (pos.y - _o_rect->Origin.y) + _o_rect->T.z * (pos.z - _o_rect->Origin.z)) /
                (_o_rect->T.x * (screen_point.x - pos.x) + _o_rect->T.y * (screen_point.y - pos.y) + _o_rect->T.z * (screen_point.z - pos.z));

            struct v3d int3d = { pos.x + scalar * (screen_point.x - pos.x) - _o_rect->Origin.x, pos.y + scalar * (screen_point.y - pos.y) - _o_rect->Origin.y, pos.z + scalar * (screen_point.z - pos.z) - _o_rect->Origin.z };

            struct v2d int2d = { (int3d.x * _o_rect->N.x + int3d.y * _o_rect->N.y + int3d.z * _o_rect->N.z) * 16, (int3d.x * _o_rect->B.x + int3d.y * _o_rect->B.y + int3d.z * _o_rect->B.z) * 16 };

            if (int2d.x >= 0 && int2d.x < _o_rect->image->width && int2d.y >= 0 && int2d.y < _o_rect->image->height) {

                union pixel top;
                top.value = _o_rect->image->data[((int)int2d.x) + _o_rect->image->width * (_o_rect->image->height - (int)int2d.y - 1)].value;
                union pixel bottom;
                bottom.value = _camera->pixels[i + j * width].value;

                _camera->pixels[i + j * width].color.r = (unsigned char)(top.color.r * top.color.a / 255.0f + (1.0f - top.color.a / 255.0f) * bottom.color.r);
                _camera->pixels[i + j * width].color.g = (unsigned char)(top.color.g * top.color.a / 255.0f + (1.0f - top.color.a / 255.0f) * bottom.color.g);
                _camera->pixels[i + j * width].color.b = (unsigned char)(top.color.b * top.color.a / 255.0f + (1.0f - top.color.a / 255.0f) * bottom.color.b);

            }

        }
    }

    return;
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

void delete_camera(struct camera* _camera) {
    free(_camera->pixels);
    free(_camera);
}