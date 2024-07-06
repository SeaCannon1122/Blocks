#include "headers.h"

struct quadrilateral {
    struct v2d p0;
    struct v2d p1;
    struct v2d p2;
    struct v2d p3;
};

struct block {
    struct v3d cordinates;
};

struct world {
    unsigned int x_size;
    unsigned int y_size;
    unsigned int z_size;

    int* data;

};

struct world get_world(unsigned int x, unsigned int y, unsigned int z) {

    return (struct world) { x, y, z, (int*)calloc(8 * x * y * z, sizeof(int)) };
}

void setblock(struct world* _world, int x, int y, int z) {
    _world->data[x + _world->x_size + (y + _world->y_size) * 2 * _world->x_size + (z + _world->z_size) * 4 * _world->x_size * _world->y_size] = 1;
}

int getblock(struct world* _world, int x, int y, int z) {
    return _world->data[x + _world->x_size + (y + _world->y_size) * 2 * _world->x_size + (z + _world->z_size) * 4 * _world->x_size * _world->y_size];
}
    
void draw_quadrilateral(struct quadrilateral* _quadrilateral, struct camera* _camera) {
}

int Entry() {
    struct world _world = get_world(10, 10, 10);

    setblock(&_world, 0, 2, 1);
    setblock(&_world, 0, 4, 0);
    //setblock(&_world, -1, 1, 0);
    
    struct camera* _camera = new_camera((struct v3d) { -0.5, 0, 0 }, (struct sph3d) { 0.05, 0, 0.1 }, 500, 300, 0.0001);
        

    while (active) {

        while (msgcheck) { sleepforms(1); }

        set_camera_size(_camera, render_state.buffer_width, render_state.buffer_height);

        for (int i = 0; i < _camera->height; i++) {
            for (int j = 0; j < _camera->width; j++) {

                _camera->pixels[i * _camera->width + j] = 0xe5e5e5;

                struct v3d xtangent = { cos(_camera->direction.theta), sin(_camera->direction.theta), 0 };

                struct v3d ytangent = { sin(_camera->direction.theta) * sin(_camera->direction.phi), -cos(_camera->direction.theta) * sin(_camera->direction.phi), cos(_camera->direction.phi) };

                struct v3d xpixelpointer = scale_v3d(&xtangent, _camera->pixel_size * (0.5 + j - 0.5 * _camera->width));
                struct v3d ypixelpointer = scale_v3d(&ytangent, _camera->pixel_size * (0.5 + i - 0.5 * _camera->height));

                struct v3d pixelpointer = {
                    xpixelpointer.x + ypixelpointer.x - _camera->direction.radius * sin(_camera->direction.theta) * cos(_camera->direction.phi),
                    xpixelpointer.y + ypixelpointer.y + _camera->direction.radius * cos(_camera->direction.theta) * cos(_camera->direction.phi),
                    xpixelpointer.z + ypixelpointer.z + _camera->direction.radius * sin(_camera->direction.phi)
                };

                int xcord = 0;
                int ycord = 0;
                int zcord = 0;

                xcord = -5;
                ycord = floor_double(pixelpointer.y / pixelpointer.x * (xcord - _camera->position.x) + _camera->position.y);
                zcord = floor_double(pixelpointer.z / pixelpointer.x * (xcord - _camera->position.x) + _camera->position.z);

                while (-5 <= xcord && xcord <= 4) {
                    if (-5 <= xcord && xcord <= 4 && -5 <= ycord && ycord <= 4 && -5 <= zcord && zcord <= 4) {
                        if (getblock(&_world, xcord, ycord, zcord)) _camera->pixels[i * _camera->width + j] = 0xff0000;
                    }
                    if (-5 <= xcord && xcord <= 4 && -5 <= ycord && ycord <= 4 && -5 <= zcord && zcord <= 4) {
                        if (getblock(&_world, xcord - 1, ycord, zcord)) _camera->pixels[i * _camera->width + j] = 0xff0000;
                    }
                    xcord++;
                    ycord = floor_double(pixelpointer.y / pixelpointer.x * (xcord - _camera->position.x) + _camera->position.y);
                    zcord = floor_double(pixelpointer.z / pixelpointer.x * (xcord - _camera->position.x) + _camera->position.z);
                }




                ycord = -5;
                xcord = floor_double(pixelpointer.x / pixelpointer.y * (ycord - _camera->position.y) + _camera->position.x);
                zcord = floor_double(pixelpointer.z / pixelpointer.y * (ycord - _camera->position.y) + _camera->position.z);

                while (-5 <= ycord && ycord <= 4) {
                    if (-5 <= xcord && xcord <= 4 && -5 <= ycord && ycord <= 4 && -5 <= zcord && zcord <= 4) {
                        if (getblock(&_world, xcord, ycord, zcord)) { _camera->pixels[i * _camera->width + j] = 0xff00; }
                    }
                    if (-5 <= xcord && xcord <= 4 && -5 <= ycord && ycord <= 4 && -5 <= zcord && zcord <= 4) {
                        if (getblock(&_world, xcord, ycord - 1, zcord)) { _camera->pixels[i * _camera->width + j] = 0xff00; }
                    }
                    ycord++;
                    xcord = floor_double(pixelpointer.x / pixelpointer.y * (ycord - _camera->position.y) + _camera->position.x);
                    zcord = floor_double(pixelpointer.z / pixelpointer.y * (ycord - _camera->position.y) + _camera->position.z);
                }




                zcord = -5;
                xcord = floor_double(pixelpointer.x / pixelpointer.z * (zcord - _camera->position.z) + _camera->position.x);
                ycord = floor_double(pixelpointer.y / pixelpointer.z * (zcord - _camera->position.z) + _camera->position.y);


                while (-5 <= zcord && zcord <= 4) {
                    if (-5 <= xcord && xcord <= 4 && -5 <= ycord && ycord <= 4 && -5 <= zcord && zcord <= 4) {
                        if (getblock(&_world, xcord, ycord, zcord)) _camera->pixels[i * _camera->width + j] = 0xff;
                    }
                    if (-5 <= xcord && xcord <= 4 && -5 <= ycord && ycord <= 4 && -5 <= zcord && zcord <= 4) {
                        if (getblock(&_world, xcord, ycord, zcord - 1)) _camera->pixels[i * _camera->width + j] = 0xff;
                    }

                    zcord++;
                    xcord = floor_double(pixelpointer.x / pixelpointer.z * (zcord - _camera->position.z) + _camera->position.x);
                    ycord = floor_double(pixelpointer.y / pixelpointer.z * (zcord - _camera->position.z) + _camera->position.y);
                }

            }

        }

        

        if (keystate('C')) active = false;

        drawWindow(_camera->pixels, _camera->width, _camera->height);

        if (keystate('W')) {
            _camera->position.x -= 0.1 * sin(_camera->direction.theta);
            _camera->position.y += 0.1 * cos(_camera->direction.theta);
        }

        if (keystate('S')) {
            _camera->position.x += 0.1 * sin(_camera->direction.theta);
            _camera->position.y -= 0.1 * cos(_camera->direction.theta);
        }

        if (keystate('D')) {
            _camera->position.x += 0.1 * cos(_camera->direction.theta);
            _camera->position.y += 0.1 * sin(_camera->direction.theta);
        }

        if (keystate('A')) {
            _camera->position.x -= 0.1 * cos(_camera->direction.theta);
            _camera->position.y -= 0.1 * sin(_camera->direction.theta);
        }

        if (keystate(VK_SPACE)) _camera->position.z += 0.1;
        if (keystate(VK_SHIFT)) _camera->position.z -= 0.1;

        if (keystate(VK_UP)) _camera->direction.phi += 0.025;
        if (keystate(VK_DOWN)) _camera->direction.phi -= 0.025;

        if (keystate(VK_LEFT)) _camera->direction.theta += 0.05;
        if (keystate(VK_RIGHT)) _camera->direction.theta -= 0.05;

        sleepforms(10);

    }
    
return 0;
}