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
    for (unsigned int i = 0; i < _camera->width; i++) {
        for (unsigned int j = 0; j < _camera->height; j++) {
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
        struct v2dabs min = { (int)floor(min_3double(cords[0].x, cords[1].x, cords[2].x)), (int)floor(min_3double(cords[0].y, cords[1].y, cords[2].y)) };
        struct v2dabs max = { (int)floor(max_3double(cords[0].x, cords[1].x, cords[2].x)), (int)floor(max_3double(cords[0].y, cords[1].y, cords[2].y)) };

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

void camera_render_oriented_rect(struct camera* _camera, struct oriented_rect* _o_rect) {

    //struct oriented_rect transformed_rect = {};

    if (_camera->width <= 0 || _camera->height <= 0) return;
    
    struct v3d pos = _camera->position;
    struct v3d dir = _camera->direction_v3d;
    struct v3d tanx = _camera->xpixelpointer;
    struct v3d tany = _camera->ypixelpointer;

    struct v3d corners[4] = {
        {_o_rect->Origin.x + _o_rect->N.x, _o_rect->Origin.y + _o_rect->N.y, _o_rect->Origin.z + _o_rect->N.z},
        {_o_rect->Origin.x + _o_rect->N.x + _o_rect->B.x, _o_rect->Origin.y + _o_rect->N.y + _o_rect->B.y, _o_rect->Origin.z + _o_rect->N.z + _o_rect->B.z},
        {_o_rect->Origin.x + _o_rect->B.x, _o_rect->Origin.y + _o_rect->B.y, _o_rect->Origin.z + _o_rect->B.z},
        _o_rect->Origin,
        
    };

    

    struct v3d piercers[4];

    double factors[4];


    for (int i = 0; i < 4; i++) {
        piercers[i] = (struct v3d) {corners[i].x - pos.x, corners[i].y - pos.y, corners[i].z - pos.z};
        factors[i] = _camera->direction_sph3d.radius * _camera->direction_sph3d.radius / (_camera->direction_v3d.x * piercers[i].x + _camera->direction_v3d.y * piercers[i].y + _camera->direction_v3d.z * piercers[i].z);
    }

    struct v3d corners_on_screen[5];

    unsigned int corners_on_screen_length = 0;
    
    for (int i = 0; i < 4; i++) {

        unsigned int next_i = (i + 1) % 4;

        if (factors[i] <= 1 && factors[i] > 0) {
            corners_on_screen[corners_on_screen_length] = (struct v3d){ (piercers[i].x * tanx.x + piercers[i].y * tanx.y + piercers[i].z * tanx.z) * factors[i] / _camera->pixel_size + _camera->width / 2,
                                                                        (piercers[i].x * tany.x + piercers[i].y * tany.y + piercers[i].z * tany.z) * factors[i] / _camera->pixel_size + _camera->height / 2,
                                                                        0
            };
            corners_on_screen_length++;

            if (factors[next_i] > 1 || factors[next_i] < 0) {
                struct v3d diff = { corners[next_i].x - pos.x, corners[next_i].y - pos.y, corners[next_i].z - pos.z};
                
                struct v3d moved_origin = { diff.x * tanx.x + diff.y * tanx.y + diff.z * tanx.z, diff.x * tany.x + diff.y * tany.y + diff.z * tany.z, 0 };

                struct v3d line_piercer = { corners[i].x - corners[next_i].x, corners[i].y - corners[next_i].y, corners[i].z - corners[next_i].z };

                double scalar = 1 - (dir.x * diff.x + dir.y * diff.y + dir.z * diff.z) / (_camera->direction_sph3d.radius * _camera->direction_sph3d.radius);

                struct v3d new_dir = { dir.x * scalar, dir.y * scalar, dir.z * scalar };

                corners_on_screen[corners_on_screen_length] = (struct v3d){ ( ( line_piercer.x * tanx.x + line_piercer.y * tanx.y + line_piercer.z * tanx.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.x ) / _camera->pixel_size + _camera->width / 2,
                                                                            ( ( line_piercer.x * tany.x + line_piercer.y * tany.y + line_piercer.z * tany.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.y) / _camera->pixel_size + _camera->height / 2,
                                                                            0
                };
                corners_on_screen_length++;
            }

        }

        else if (factors[next_i] <= 1 && factors[next_i] > 0) {
            struct v3d diff = { corners[i].x - pos.x, corners[i].y - pos.y, corners[i].z - pos.z };

            struct v3d moved_origin = { diff.x * tanx.x + diff.y * tanx.y + diff.z * tanx.z, diff.x * tany.x + diff.y * tany.y + diff.z * tany.z, 0 };

            struct v3d line_piercer = { corners[next_i].x - corners[i].x, corners[next_i].y - corners[i].y, corners[next_i].z - corners[i].z };

            double scalar = 1 - (dir.x * diff.x + dir.y * diff.y + dir.z * diff.z) / (_camera->direction_sph3d.radius * _camera->direction_sph3d.radius);

            struct v3d new_dir = { dir.x * scalar, dir.y * scalar, dir.z * scalar };

            corners_on_screen[corners_on_screen_length] = (struct v3d){ ((line_piercer.x * tanx.x + line_piercer.y * tanx.y + line_piercer.z * tanx.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.x) / _camera->pixel_size + _camera->width / 2,
                                                                        ((line_piercer.x * tany.x + line_piercer.y * tany.y + line_piercer.z * tany.z) * (new_dir.x * new_dir.x + new_dir.y * new_dir.y + new_dir.z * new_dir.z) / (new_dir.x * line_piercer.x + new_dir.y * line_piercer.y + new_dir.z * line_piercer.z) + moved_origin.y) / _camera->pixel_size + _camera->height / 2,
                                                                        0
            };
            corners_on_screen_length++;
        }

        
    }

    
    int* frame = (unsigned int*)calloc(_camera->width * _camera->height, sizeof(unsigned int));

    for (int i = 0; i < (int)corners_on_screen_length; i++) {
        struct line l = { corners_on_screen[i], corners_on_screen[(i + 1) % corners_on_screen_length] };

        double dydx = (l.p[1].y - l.p[0].y) / (l.p[1].x - l.p[0].x);

        if (1 > dydx && -1 < dydx) {
            struct v3d left;
            struct v3d right;

            if (l.p[1].x - l.p[0].x > 0) {
                left = (struct v3d){ floor(l.p[0].x), floor(l.p[0].y), 0 };
                right = (struct v3d){ l.p[1].x, l.p[1].y, 0 };
            }

            else {
                left = (struct v3d){ l.p[1].x, l.p[1].y, 0 };
                right = (struct v3d){ l.p[0].x, l.p[0].y, 0 };
            }

            for (double x = floor(left.x + 0.5) + 0.5; x <= right.x; x++) {
                int next_pixel_index = clamp_int((int)floor(x), _camera->width - 1, 0) + _camera->width * clamp_int((int)(left.y + dydx * (x - left.x)), _camera->height - 1, 0);
                if (frame[next_pixel_index] != i + 1 && frame[next_pixel_index] != 0) frame[next_pixel_index] = -1;
                else frame[next_pixel_index] = i + 1;
            }

        }

        else {
            struct v3d down;
            struct v3d up;

            if (l.p[1].y - l.p[0].y > 0) {
                down = (struct v3d){ l.p[0].x, l.p[0].y, 0 };
                up = (struct v3d){ l.p[1].x, l.p[1].y, 0 };
            }

            else {
                down = (struct v3d){ l.p[1].x, l.p[1].y, 0 };
                up = (struct v3d){ l.p[0].x, l.p[0].y, 0 };
            }

            for (double y = floor(down.y + 0.5) + 0.5; y <= up.y; y++) {
                int next_pixel_index = clamp_int((int)(down.x + 1 / dydx * (y - down.y)), _camera->width - 1, 0) + _camera->width * clamp_int((int)floor(y), _camera->height - 1, 0);
                if (frame[next_pixel_index] != i + 1 && frame[next_pixel_index] != 0) frame[next_pixel_index] = -1;
                else frame[next_pixel_index] = i + 1;
            }


        }

        frame[clamp_int((int)floor(l.p[0].x), _camera->width - 1, 0) + _camera->width * clamp_int((int)floor(l.p[0].y), _camera->height - 1, 0)] = i + 1;

    }

    for (int j = 0; j < _camera->height; j++) {
        bool start = false;
        bool render = false;
        bool prev = false;
        for (int i = 0; i < _camera->width; i++) {
            
            if (frame[i + j * _camera->width] > 0) {
                if (start == false && render == false) start = true;
                render = true;
                prev = true;
            }

            else {
                if (start == false && render == true && prev == true) render = false;
                start = false;
                prev = false;
            }
            
            if(render) {
                frame[i + j * _camera->width] = 1;
            
                struct v3d screen_point = { pos.x + dir.x + (i - (int) _camera->width / 2 + 0.5) * _camera->pixel_size * tanx.x + (j - (int) _camera->height / 2 + 0.5) * _camera->pixel_size * tany.x,
                                            pos.y + dir.y + (i - (int) _camera->width / 2 + 0.5) * _camera->pixel_size * tanx.y + (j - (int) _camera->height / 2 + 0.5) * _camera->pixel_size * tany.y,
                                            pos.z + dir.z + (i - (int) _camera->width / 2 + 0.5) * _camera->pixel_size * tanx.z + (j - (int) _camera->height / 2 + 0.5) * _camera->pixel_size * tany.z
                };

                double scalar = -(_o_rect->T.x * (pos.x - _o_rect->Origin.x) + _o_rect->T.y * (pos.y - _o_rect->Origin.y) + _o_rect->T.z * (pos.z - _o_rect->Origin.z)) /
                                 (_o_rect->T.x * (screen_point.x - pos.x) + _o_rect->T.y * (screen_point.y - pos.y) + _o_rect->T.z * (screen_point.z - pos.z));
            


                struct v3d int3d = { pos.x + scalar * (screen_point.x - pos.x) - _o_rect->Origin.x, pos.y + scalar * (screen_point.y - pos.y) - _o_rect->Origin.y, pos.z + scalar * (screen_point.z - pos.z) - _o_rect->Origin.z };

                

                struct v3d int2d = { (int3d.x * _o_rect->N.x + int3d.y * _o_rect->N.y + int3d.z * _o_rect->N.z) * 16, (int3d.x * _o_rect->B.x + int3d.y * _o_rect->B.y + int3d.z * _o_rect->B.z) * 16, 0 };

                

                if (int2d.x >= 0 && int2d.x < _o_rect->image->width && int2d.y >= 0 && int2d.y < _o_rect->image->height) { 
                    _camera->pixels[i + j * _camera->width] = _o_rect->image->data[((int)int2d.x) + _o_rect->image->width * (_o_rect->image->height - (int)int2d.y - 1)].value; 
                }
                
            }


        }
    }
    


    free(frame);


    return;
}

void camera_render_cursor(struct camera* _camera) {

    struct v3d pos = _camera->position;
    struct v3d dir = _camera->direction_v3d;
    struct v3d tanx = _camera->xpixelpointer;
    struct v3d tany = _camera->ypixelpointer;

    struct line lines[3] = {
                             { (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z }, (struct v3d) { pos.x + dir.x + _camera->direction_sph3d.radius / 10 , pos.y + dir.y , pos.z + dir.z } },
                             { (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z }, (struct v3d) { pos.x + dir.x , pos.y + dir.y + _camera->direction_sph3d.radius / 10 , pos.z + dir.z } },
                             { (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z }, (struct v3d) { pos.x + dir.x , pos.y + dir.y , pos.z + dir.z + _camera->direction_sph3d.radius / 10 } }
    };

    

    for (int c = 0; c < 3; c++) {
        struct line line_screen;
        for (int i = 0; i < 2; i++) {
            struct v3d piercer = { lines[c].p[i].x - pos.x, lines[c].p[i].y - pos.y, lines[c].p[i].z - pos.z};
            double factor = _camera->direction_sph3d.radius * _camera->direction_sph3d.radius / (_camera->direction_v3d.x * piercer.x + _camera->direction_v3d.y * piercer.y + _camera->direction_v3d.z * piercer.z);
            line_screen.p[i] = (struct v3d){(piercer.x * tanx.x + piercer.y * tanx.y + piercer.z * tanx.z) * factor / _camera->pixel_size + _camera->width / 2,
                                                                            (piercer.x * tany.x + piercer.y * tany.y + piercer.z * tany.z) * factor / _camera->pixel_size + _camera->height / 2,
                                                                            0
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
    return;
}