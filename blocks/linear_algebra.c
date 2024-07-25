#include "headers.h"

void draw_line(unsigned int* _buffer, unsigned int _width, unsigned int _height, struct line* _line, unsigned int color) {

    if (_width <= 0 || _height <= 0) return;

    double dydx = (_line->p[1].y - _line->p[0].y) / (_line->p[1].x - _line->p[0].x);

    if (1 > dydx && -1 < dydx) {
        struct v3d left;
        struct v3d right;

        if (_line->p[1].x - _line->p[0].x > 0) {
            left = (struct v3d){ _line->p[0].x, _line->p[0].y, 0 };
            right = (struct v3d){ _line->p[1].x, _line->p[1].y, 0 };
        }

        else {
            left = (struct v3d){ _line->p[1].x, _line->p[1].y, 0 };
            right = (struct v3d){ _line->p[0].x, _line->p[0].y, 0 };
        }

        for (double x = floor(left.x + 0.5) + 0.5; x <= right.x; x++) _buffer[clamp_int((int)floor(x), _width - 1, 0) + _width * clamp_int((int)(left.y + dydx * (x - left.x)), _height - 1, 0)] = color;

    }

    else {
        struct v3d down;
        struct v3d up;

        if (_line->p[1].y - _line->p[0].y > 0) {
            down = (struct v3d){ _line->p[0].x, _line->p[0].y, 0 };
            up = (struct v3d){ _line->p[1].x, _line->p[1].y, 0 };
        }

        else {
            down = (struct v3d){ _line->p[1].x, _line->p[1].y, 0 };
            up = (struct v3d){ _line->p[0].x, _line->p[0].y, 0 };
        }

        for (double y = floor(down.y + 0.5) + 0.5; y <= up.y; y++) _buffer[clamp_int((int)(down.x + 1 / dydx * (y - down.y)), _width - 1, 0) + _width * clamp_int((int)floor(y), _height - 1, 0)] = color;

    }

    return;
}