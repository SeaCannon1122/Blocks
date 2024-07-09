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
    
    struct camera* _camera = new_camera((struct v3d) { 0, -2.0, 0 }, 500, 300, 0.002, 10);
    set_camera_direction_sph3d(_camera, (struct sph3d) { 01.4, 0, 00 });

    struct triangle triangles[12] = {
        { (struct v3d) { 0, 0, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, 0xff0000 },
        { (struct v3d) { 0, 1, 0 }, (struct v3d) { 1, 1, 0 }, (struct v3d) { 0, 1, 1 }, 0xff0000 },
        { (struct v3d) { 1, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, 0xff0000 },
        { (struct v3d) { 1, 1, 1 }, (struct v3d) { 1, 1, 0 }, (struct v3d) { 0, 1, 1 }, 0xff0000 },

        { (struct v3d) { 0, 0, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, 0xff },
        { (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 1 }, (struct v3d) { 0, 1, 1 }, 0xff },
        { (struct v3d) { 1, 1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, 0xff },
        { (struct v3d) { 1, 1, 1 }, (struct v3d) { 1, 0, 1 }, (struct v3d) { 0, 1, 1 }, 0xff },

        { (struct v3d) { 0, 0, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, 0xff00 },
        { (struct v3d) { 1, 0, 0 }, (struct v3d) { 1, 1, 0 }, (struct v3d) { 1, 0, 1 }, 0xff00 },
        { (struct v3d) { 0, 1, 1 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, 0xff00 },
        { (struct v3d) { 1, 1, 1 }, (struct v3d) { 1, 1, 0 }, (struct v3d) { 1, 0, 1 }, 0xff00 },

    };

    struct triangle t = { (struct v3d) { 0, 0, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, 0xff0000 };

    while (active) {

        double start_time = get_time();

        set_camera_size(_camera, render_state.buffer_width, render_state.buffer_height);

        flash_camera_screen(_camera);

        for (int i = 0; i < 12; i++) camera_render_triangle(_camera, &triangles[i]);
        
        //camera_render_triangle(_camera, &t);

        if (keystate('C')) active = false;

        drawWindow(_camera->pixels, _camera->width, _camera->height);

        if (keystate('W')) {
            _camera->position.x -= 0.05 * sin(_camera->direction_sph3d.theta);
            _camera->position.y += 0.05 * cos(_camera->direction_sph3d.theta);
        }

        if (keystate('S')) {
            _camera->position.x += 0.05 * sin(_camera->direction_sph3d.theta);
            _camera->position.y -= 0.05 * cos(_camera->direction_sph3d.theta);
        }

        if (keystate('D')) {
            _camera->position.x += 0.05 * cos(_camera->direction_sph3d.theta);
            _camera->position.y += 0.05 * sin(_camera->direction_sph3d.theta);
        }

        if (keystate('A')) {
            _camera->position.x -= 0.05 * cos(_camera->direction_sph3d.theta);
            _camera->position.y -= 0.05 * sin(_camera->direction_sph3d.theta);
        }

        if (keystate(VK_SPACE)) _camera->position.z += 0.05;
        if (keystate(VK_SHIFT)) _camera->position.z -= 0.05;

        if (keystate(VK_UP)) set_camera_direction_sph3d(_camera, (struct sph3d) {_camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi + 0.04});
        if (keystate(VK_DOWN)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi - 0.04 });

        if (keystate(VK_LEFT)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta + 0.025, _camera->direction_sph3d.phi });
        if (keystate(VK_RIGHT)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta - 0.025, _camera->direction_sph3d.phi });

        //sleepforms(10);

        double time_left= 20 + start_time - get_time();

        if(time_left > 0) sleepforms(time_left);

    }
    
return 0;
}