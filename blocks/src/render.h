#ifndef RENDER_H
#define RENDER_H

#include "headers.h"

void draw_line(union pixel* _buffer, unsigned int _width, unsigned int _height, struct line2d* _line, unsigned int color);

void camera_render_oriented_rect(struct camera* _camera, struct oriented_rect* _o_rect);

int render_world(struct world* _world, struct camera* _camera, struct resource_manager* _rm);

#endif