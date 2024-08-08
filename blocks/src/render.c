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
            i;

        }
    }

    return;
}

struct oriented_rect* get_chunk_rects(struct world* _world, int _posx, int _posy, struct camera* _camera, struct resource_manager* _rm, int* _length) {

    struct chunk* chunk = _world->chunk_pointer_table[(_posx + _world->world_chunk_radius) + (_posy + _world->world_chunk_radius) * 2 * _world->world_chunk_radius];

    *_length = 0;
    struct oriented_rect** block_rects = (struct oriented_rect**)malloc(16 * 16 * 256 * sizeof(void*));
    int* block_rect_lengths = calloc(16 * 16 * 256, sizeof(int));

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 256; k++) {
                

                if (chunk->blocks[i + j * 16 + k * 256].id == 0) {

                    goto _air;
                }

                struct block_render_info* info = get_block_render_info(_rm, chunk->blocks[i + j * 16 + k * 256].id);

                struct oriented_rect* single_block_rects = NULL;
                int single_block_rects_length = 0;

                if (info->type & BLOCK_OPAQUE) {
                    if (info->type & BLOCK_ORIENTED) {

                    }

                    else {

                        single_block_rects = (struct oriented_rect*) malloc(info->rect_infos_length * sizeof(struct oriented_rect));
                        
                        //POS_X

                        if (i == 15 && _posx != _world->world_chunk_radius - 1) {
                            if (!(get_block_render_info(_rm, _world->chunk_pointer_table[((_posx + 1) + _world->world_chunk_radius) + (_posy + _world->world_chunk_radius) * 2 * _world->world_chunk_radius]->blocks[ j * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                                single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[POS_X]);
                                struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                                cur_rect->Origin = (struct v3d){
                                    cur_rect->Origin.x + i + _posx * 16,
                                    cur_rect->Origin.y + j + _posy * 16,
                                    cur_rect->Origin.z + k
                                };

                                cur_rect->distance = sqrt(
                                    (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                    (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                    (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                                );
                                single_block_rects_length++;
                            }
                        }

                        else if (!(get_block_render_info(_rm, chunk->blocks[(i + 1) + j * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[POS_X]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }

                        //NEG_X

                        if (i == 0 && _posx != -_world->world_chunk_radius) {
                            if (!(get_block_render_info(_rm, _world->chunk_pointer_table[((_posx - 1) + _world->world_chunk_radius) + (_posy + _world->world_chunk_radius) * 2 * _world->world_chunk_radius]->blocks[15 + j * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                                single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[NEG_X]);
                                struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                                cur_rect->Origin = (struct v3d){
                                    cur_rect->Origin.x + i + _posx * 16,
                                    cur_rect->Origin.y + j + _posy * 16,
                                    cur_rect->Origin.z + k
                                };

                                cur_rect->distance = sqrt(
                                    (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                    (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                    (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                                );
                                single_block_rects_length++;
                            }
                        }

                        else if (!(get_block_render_info(_rm, chunk->blocks[(i - 1) + j * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[NEG_X]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }

                        //POS_Y

                        if (j == 15 && _posy != _world->world_chunk_radius - 1) {
                            if (!(get_block_render_info(_rm, _world->chunk_pointer_table[(_posx + _world->world_chunk_radius) + (_posy + 1 + _world->world_chunk_radius) * 2 * _world->world_chunk_radius]->blocks[i + k * 256].id)->type & BLOCK_OPAQUE)) {
                                single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[POS_Y]);
                                struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                                cur_rect->Origin = (struct v3d){
                                    cur_rect->Origin.x + i + _posx * 16,
                                    cur_rect->Origin.y + j + _posy * 16,
                                    cur_rect->Origin.z + k
                                };

                                cur_rect->distance = sqrt(
                                    (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                    (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                    (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                                );
                                single_block_rects_length++;
                            }
                        }

                        else if (!(get_block_render_info(_rm, chunk->blocks[i + (j + 1) * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[POS_Y]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }

                        //NEG_Y

                        if (j == 0 && _posy != -_world->world_chunk_radius) {
                            if (!(get_block_render_info(_rm, _world->chunk_pointer_table[(_posx + _world->world_chunk_radius) + (_posy - 1 + _world->world_chunk_radius) * 2 * _world->world_chunk_radius]->blocks[i + 15 * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                                single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[NEG_Y]);
                                struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                                cur_rect->Origin = (struct v3d){
                                    cur_rect->Origin.x + i + _posx * 16,
                                    cur_rect->Origin.y + j + _posy * 16,
                                    cur_rect->Origin.z + k
                                };

                                cur_rect->distance = sqrt(
                                    (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                    (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                    (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                                );
                                single_block_rects_length++;
                            }
                        }

                        else if (!(get_block_render_info(_rm, chunk->blocks[i + (j - 1) * 16 + k * 256].id)->type & BLOCK_OPAQUE)) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[NEG_Y]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }

                        //POS_Z

                        if (k == 255) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[POS_Z]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }
                        else if (!(get_block_render_info(_rm, chunk->blocks[i + j * 16 + (k + 1) * 256].id)->type & BLOCK_OPAQUE)) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[POS_Z]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }

                        //NEG_Z

                        if (k == 0) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[NEG_Z]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                           cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }
                        else if (!(get_block_render_info(_rm, chunk->blocks[i + j * 16 + (k - 1) * 256].id)->type & BLOCK_OPAQUE)) {
                            single_block_rects[single_block_rects_length] = *((struct oriented_rect*)&info->rect_infos[NEG_Z]);
                            struct oriented_rect* cur_rect = &single_block_rects[single_block_rects_length];

                            cur_rect->Origin = (struct v3d){
                                cur_rect->Origin.x + i + _posx * 16,
                                cur_rect->Origin.y + j + _posy * 16,
                                cur_rect->Origin.z + k
                            };

                            cur_rect->distance = sqrt(
                                (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) * (cur_rect->Origin.x + (double)cur_rect->image->width / 32 * cur_rect->N.x + (double)cur_rect->image->height / 32 * cur_rect->B.x - _camera->position.x) +
                                (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) * (cur_rect->Origin.y + (double)cur_rect->image->width / 32 * cur_rect->N.y + (double)cur_rect->image->height / 32 * cur_rect->B.y - _camera->position.y) +
                                (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z) * (cur_rect->Origin.z + (double)cur_rect->image->width / 32 * cur_rect->N.z + (double)cur_rect->image->height / 32 * cur_rect->B.z - _camera->position.z)
                            );
                            single_block_rects_length++;
                        }


                    }
                }

                block_rects[i + 16 * j + 256 * k] = single_block_rects;
                block_rect_lengths[i + 16 * j + 256 * k] = single_block_rects_length;

            }
        _air:
            1 == 1;
        }
    }

    int rects_length = 0;

    for (int i = 0; i < 16 * 16 * 256; i++) rects_length += block_rect_lengths[i];

    struct oriented_rect* rects = (struct oriented_rect*)malloc(rects_length * sizeof(struct oriented_rect));

    rects_length = 0;

    for (int block_i = 0; block_i < 16 * 16 * 256; block_i++) {

        if (block_rect_lengths[block_i] != 0) {
            for (int rect_i = 0; rect_i < block_rect_lengths[block_i]; rect_i++) {
                rects[rects_length] = block_rects[block_i][rect_i];
                rects_length++;
            }
            free(block_rects[block_i]);
        }
        
    }
    *_length = rects_length;
    free(block_rects);
    free(block_rect_lengths);

    return rects;
}

int compare_double_pointers(void* a, void* b) {
    double arg1 = (*(struct oriented_rect**)a)->distance;
    double arg2 = (*(struct oriented_rect**)b)->distance;
    return (arg1 < arg2) - (arg1 > arg2);
}

int render_world(struct world* _world, struct camera* _camera, struct resource_manager* _rm) {
    
    int chunk_min_x = clamp_int(floor(_camera->position.x / 16) - _camera->render_distance, -_world->world_chunk_radius, _world->world_chunk_radius);
    int chunk_max_x = clamp_int(floor(_camera->position.x / 16) + _camera->render_distance, -_world->world_chunk_radius, _world->world_chunk_radius);
    int chunk_min_y = clamp_int(floor(_camera->position.y / 16) - _camera->render_distance, -_world->world_chunk_radius, _world->world_chunk_radius);
    int chunk_max_y = clamp_int(floor(_camera->position.y / 16) + _camera->render_distance, -_world->world_chunk_radius, _world->world_chunk_radius);

    struct oriented_rect** chunk_rects = (struct oriented_rect**) malloc( (chunk_max_x - chunk_min_x + 1) * (chunk_max_y - chunk_min_y + 1) * sizeof(void*) );
    int* chunk_rects_lengths = (int*) malloc( (chunk_max_x - chunk_min_x + 1) * (chunk_max_y - chunk_min_y + 1) * sizeof(int) );
    int rects_length = 0;

    int i = 0;
    for (int cx = chunk_min_x; cx <= chunk_max_x ; cx++) {
        int j = 0;
        printf("chunks ");
        for (int cy = chunk_min_y; cy <= chunk_max_y; cy++) {
            int chunk_table_index = (cx + _world->world_chunk_radius) + (cy + _world->world_chunk_radius) * 2 * _world->world_chunk_radius;

            if (_world->chunk_pointer_table[chunk_table_index] == NULL) {
                _world->chunk_pointer_table[chunk_table_index] = new_chunk();
                generate_chunk(_world->chunk_pointer_table[chunk_table_index], cx, cy, _world->seed);
            }

            chunk_rects[i + (chunk_max_x - chunk_min_x + 1) * j] = get_chunk_rects(_world, cx, cy, _camera, _rm, &chunk_rects_lengths[i + j * (chunk_max_x - chunk_min_x + 1)]);
            printf("(%d|%d) ", cx, cy);
            rects_length += chunk_rects_lengths[i + j * (chunk_max_x - chunk_min_x + 1)]; 
            j++;
        }
        printf("done\n");
        i++;
    }

    struct oriented_rect** rects = (struct oriented_rect**) malloc(rects_length * sizeof(void*));
    rects_length = 0;

    for (int chunk_i = 0; chunk_i < (chunk_max_x - chunk_min_x + 1) * (chunk_max_y - chunk_min_y + 1); chunk_i++) {
        for (int rect_i = 0; rect_i < chunk_rects_lengths[chunk_i]; rect_i++) {
            rects[rects_length] = &(chunk_rects[chunk_i])[rect_i];
            rects_length++;
        }
    }
    

    if(rects_length > 1) qsort(rects, rects_length, sizeof(void*), compare_double_pointers);
    
    for (int i = 0; i < rects_length; i++) {
        if ((rects[i]->Origin.x - _camera->position.x) * rects[i]->T.x + (rects[i]->Origin.y - _camera->position.y) * rects[i]->T.y + (rects[i]->Origin.z - _camera->position.z) * rects[i]->T.z < 0) camera_render_oriented_rect(_camera, rects[i]);
    }

    for (int i = 0; i < (chunk_max_x - chunk_min_x + 1) * (chunk_max_y - chunk_min_y + 1); i++) {
        if(chunk_rects_lengths[i] != 0) free(chunk_rects[i]);
    }
    free(chunk_rects);
    free(chunk_rects_lengths);

    return 0;
}

