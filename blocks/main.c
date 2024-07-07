#include "headers.h"

struct quadrilateral {
    struct v2d p0;
    struct v2d p1;
    struct v2d p2;
    struct v2d p3;
};
    
void draw_quadrilateral(struct quadrilateral* _quadrilateral, struct camera* _camera) {
}

int Entry() {
    struct world* _world = new_world(10, 10, 10);
    struct v3dabs c1 = { 0, 0, 0 };
    struct v3dabs c2 = { 0, 0, 1 };
    struct v3dabs c3 = { 1, 1, 0 };
    struct v3dabs c4 = { 3, 2, -4 };
    struct v3dabs c5 = { 1, -2, 2 };

    get_block_ptr(_world, &c1)->id = 1;
    get_block_ptr(_world, &c2)->id = 1;
    get_block_ptr(_world, &c3)->id = 1;
    get_block_ptr(_world, &c4)->id = 1;
    get_block_ptr(_world, &c5)->id = 1;
    
    struct camera* _camera = new_camera((struct v3d) { -0.5, 0, 0 }, (struct sph3d) { 0.07, 0, 0 }, 500, 300, 0.0001, 10);
        

    while (active) {

        while (msgcheck) { sleepforms(1); }

        set_camera_size(_camera, render_state.buffer_width, render_state.buffer_height);

        
        render_camera(_camera, _world);
        

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

        if (keystate(VK_UP)) _camera->direction.phi += 0.1;
        if (keystate(VK_DOWN)) _camera->direction.phi -= 0.1;
        _camera->direction.phi = clamp(_camera->direction.phi, PI/2, -PI/2);

        if (keystate(VK_LEFT)) _camera->direction.theta += 0.05;
        if (keystate(VK_RIGHT)) _camera->direction.theta -= 0.05;

        sleepforms(10);

    }
    
return 0;
}