#include "headers.h"

void draw_line(union pixel* _buffer, unsigned int _width, unsigned int _height, struct line2d* _line, unsigned int color) {

    if (_width <= 0 || _height <= 0) return;

    double dydx = (_line->p[1].y - _line->p[0].y) / (_line->p[1].x - _line->p[0].x);

    if (1 > dydx && -1 < dydx) {
        struct v2d left;
        struct v2d right;

        if (_line->p[1].x - _line->p[0].x > 0) {
            left = (struct v2d){ _line->p[0].x, _line->p[0].y };
            right = (struct v2d){ _line->p[1].x, _line->p[1].y };
        }

        else {
            left = (struct v2d){ _line->p[1].x, _line->p[1].y };
            right = (struct v2d){ _line->p[0].x, _line->p[0].y };
        }

        for (double x = floor(left.x + 0.5) + 0.5; x <= right.x; x++) _buffer[clamp_int((int)floor(x), 0, _width - 1) + _width * clamp_int((int)(left.y + dydx * (x - left.x)), 0, _height - 1)].value = color;

    }

    else {
        struct v2d down;
        struct v2d up;

        if (_line->p[1].y - _line->p[0].y > 0) {
            down = (struct v2d){ _line->p[0].x, _line->p[0].y };
            up = (struct v2d){ _line->p[1].x, _line->p[1].y };
        }

        else {
            down = (struct v2d){ _line->p[1].x, _line->p[1].y };
            up = (struct v2d){ _line->p[0].x, _line->p[0].y };
        }

        for (double y = floor(down.y + 0.5) + 0.5; y <= up.y; y++) _buffer[clamp_int((int)(down.x + 1 / dydx * (y - down.y)), 0, _width - 1) + _width * clamp_int((int)floor(y), 0, _height - 1)].value = color;

    }

    return;
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
        piercers[i] = (struct v3d){ corners[i].x - pos.x, corners[i].y - pos.y, corners[i].z - pos.z };
        factors[i] = (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) / (dir.x * piercers[i].x + dir.y * piercers[i].y + dir.z * piercers[i].z);
    }

    struct v2d corners_on_screen[5];

    int corners_on_screen_length = 0;

    for (int i = 0; i < 4; i++) {

        unsigned int next_i = (i + 1) % 4;

        if (factors[i] <= 1 && factors[i] > 0) {
            corners_on_screen[corners_on_screen_length] = (struct v2d){
                _camera->width / 2 + (piercers[i].x * tanx.x + piercers[i].y * tanx.y + piercers[i].z * tanx.z) * factors[i] / _camera->pixel_size,
                _camera->height / 2 + (piercers[i].x * tany.x + piercers[i].y * tany.y + piercers[i].z * tany.z) * factors[i] / _camera->pixel_size
            };


            corners_on_screen_length++;

            if (factors[next_i] > 1 || factors[next_i] < 0) {

                double scalar = -(dir.x * (corners[next_i].x - pos.x - dir.x) + dir.y * (corners[next_i].y - pos.y - dir.y) + dir.z * (corners[next_i].z - pos.z - dir.z)) / (dir.x * (corners[i].x - corners[next_i].x) + dir.y * (corners[i].y - corners[next_i].y) + dir.z * (corners[i].z - corners[next_i].z));

                struct v3d int3d = {
                    corners[next_i].x + scalar * (corners[i].x - corners[next_i].x),
                    corners[next_i].y + scalar * (corners[i].y - corners[next_i].y),
                    corners[next_i].z + scalar * (corners[i].z - corners[next_i].z) };

                corners_on_screen[corners_on_screen_length] = (struct v2d){
                    (double)(width / 2) + ((int3d.x - pos.x) * tanx.x + (int3d.y - pos.y) * tanx.y + (int3d.z - pos.z) * tanx.z) / _camera->pixel_size,
                    (double)(height / 2) + ((int3d.x - pos.x) * tany.x + (int3d.y - pos.y) * tany.y + (int3d.z - pos.z) * tany.z) / _camera->pixel_size
                };
                corners_on_screen_length++;

            }

        }

        else if (factors[next_i] <= 1 && factors[next_i] > 0) {

            double scalar = -(dir.x * (corners[i].x - pos.x - dir.x) + dir.y * (corners[i].y - pos.y - dir.y) + dir.z * (corners[i].z - pos.z - dir.z)) / (dir.x * (corners[next_i].x - corners[i].x) + dir.y * (corners[next_i].y - corners[i].y) + dir.z * (corners[next_i].z - corners[i].z));

            struct v3d int3d = {
                corners[i].x + scalar * (corners[next_i].x - corners[i].x),
                corners[i].y + scalar * (corners[next_i].y - corners[i].y),
                corners[i].z + scalar * (corners[next_i].z - corners[i].z)
            };

            corners_on_screen[corners_on_screen_length] = (struct v2d){
                (double)(width / 2) + ((int3d.x - pos.x) * tanx.x + (int3d.y - pos.y) * tanx.y + (int3d.z - pos.z) * tanx.z) / _camera->pixel_size,
                (double)(height / 2) + ((int3d.x - pos.x) * tany.x + (int3d.y - pos.y) * tany.y + (int3d.z - pos.z) * tany.z) / _camera->pixel_size
            };
            corners_on_screen_length++;
        }


    }

    if (corners_on_screen_length == 0) return;

    struct v2dabs screen_bound_min = { width, height };
    struct v2dabs screen_bound_max = { -1, -1 };

    struct v2d normals[5];

    for (int i = 0; i < corners_on_screen_length; i++) {

        normals[i] = (struct v2d){ corners_on_screen[i].y - corners_on_screen[(i + 1) % corners_on_screen_length].y, corners_on_screen[(i + 1) % corners_on_screen_length].x - corners_on_screen[i].x };
        struct v2dabs abs = { clamp_int((int)corners_on_screen[i].x, 0, width - 1), clamp_int((int)corners_on_screen[i].y, 0, height - 1) };

        if (abs.x > screen_bound_max.x) screen_bound_max.x = abs.x;
        if (abs.x < screen_bound_min.x) screen_bound_min.x = abs.x;
        if (abs.y > screen_bound_max.y) screen_bound_max.y = abs.y;
        if (abs.y < screen_bound_min.y) screen_bound_min.y = abs.y;
    }

    for (int j = screen_bound_min.y; j <= screen_bound_max.y; j++) {

        for (int i = screen_bound_min.x; i <= screen_bound_max.x; i++) {

            for (int c = 0; c < corners_on_screen_length; c++) {
                if ((normals[c].x * (i + 0.5 - corners_on_screen[c].x) + normals[c].y * (j + 0.5 - corners_on_screen[c].y)) < 0) goto _skip;
            }


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

        _skip:
            i = i;

        }
    }

    return;
}

int render_world(struct world* _world, struct camera* _camera, struct resource_manager* _rm) {
    
    struct oriented_rect** chunk_rects = (struct oriented_rect**) malloc((2 * _camera->render_distance + 1) * (2 * _camera->render_distance + 1) * sizeof(void*));
    _world;
    for (int i = clamp_int(floor(_camera->position.x / 16) - _camera->render_distance, 0, 0); i <= floor(_camera->position.x / 16) + _camera->render_distance; i++) {
        for (int j = floor(_camera->position.y / 16) - _camera->render_distance; j <= floor(_camera->position.y / 16) + _camera->render_distance; j++) {
            //chunk_rects
        }
    }

    struct oriented_rect* rects;
    int rects_length;
    //for (int i = 0, i < rects_length, i++) camera_render_oriented_rect(_camera, &rects[i]);
    


}

struct oriented_rect* render_chunk(struct chunk* _chunk, int _posx, int _posy, struct camera* _camera, struct resource_manager* _rm) {
    
    int* oriented_rect_count = malloc(26 * 26 * 256 * sizeof(int));

    struct oriented_rect** rects = malloc(26 * 26 * 256 * sizeof(void*));

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 256; k++) {
                struct block_render_info* info = get_block_render_info(_rm, _chunk->blocks->id);

                //if()


            }
        }
    }
}