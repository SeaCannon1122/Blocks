#include "headers.h"

void draw_line(union pixel* _buffer, unsigned int _width, unsigned int _height, struct line2d* _line, unsigned int color);

void camera_render_oriented_rect(struct camera* _camera, struct oriented_rect* _oriented_rect);

int render_world(struct world* _world, struct camera* _camera, struct resource_manager* _rm);

struct oriented_rect* render_chunk(struct chunk* _chunk, int _posx, int _posy, struct camera* _camera, struct resource_manager* _rm);