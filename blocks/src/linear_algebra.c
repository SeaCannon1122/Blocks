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