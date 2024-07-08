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
    
    struct camera* _camera = new_camera((struct v3d) { 0, -10.0, 0 }, 500, 300, 0.01, 10);
    set_camera_direction_sph3d(_camera, (struct sph3d) { 0.07, 0, 00 });

    struct triangle triangles[8] = {0};

    triangles[0].p[0] = (struct v3d){ 0, 0, 0 };
    triangles[0].p[1] = (struct v3d){ 0, 0, 1 };
    triangles[0].p[2] = (struct v3d){ 1, 0, 0 };
    triangles[0].color = 0xff0000;

    triangles[2].p[0] = (struct v3d){ 0, 1, 0 };
    triangles[2].p[1] = (struct v3d){ 0, 1, 1 };
    triangles[2].p[2] = (struct v3d){ 1, 1, 0 };
    triangles[2].color = 0xff0000;


    triangles[4].p[0] = (struct v3d){ 0, 0, 0 };
    triangles[4].p[1] = (struct v3d){ 0, 1, 0 };
    triangles[4].p[2] = (struct v3d){ 1, 0, 0 };
    triangles[4].color = 0xff;


    triangles[6].p[0] = (struct v3d){ 0, 0, 1 };
    triangles[6].p[1] = (struct v3d){ 0, 1, 1 };
    triangles[6].p[2] = (struct v3d){ 1, 0, 1 };
    triangles[6].color = 0xff;

    while (active) {

        while (msgcheck) { sleepforms(1); }

        set_camera_size(_camera, render_state.buffer_width, render_state.buffer_height);

        flash_camera_screen(_camera);

        for (int i = 0; i < 8; i++) {
            camera_render_triangle(_camera, &triangles[i]);
        }
        

        if (keystate('C')) active = false;

        drawWindow(_camera->pixels, _camera->width, _camera->height);

        if (keystate('W')) {
            _camera->position.x -= 0.1 * sin(_camera->direction_sph3d.theta);
            _camera->position.y += 0.1 * cos(_camera->direction_sph3d.theta);
        }

        if (keystate('S')) {
            _camera->position.x += 0.1 * sin(_camera->direction_sph3d.theta);
            _camera->position.y -= 0.1 * cos(_camera->direction_sph3d.theta);
        }

        if (keystate('D')) {
            _camera->position.x += 0.1 * cos(_camera->direction_sph3d.theta);
            _camera->position.y += 0.1 * sin(_camera->direction_sph3d.theta);
        }

        if (keystate('A')) {
            _camera->position.x -= 0.1 * cos(_camera->direction_sph3d.theta);
            _camera->position.y -= 0.1 * sin(_camera->direction_sph3d.theta);
        }

        if (keystate(VK_SPACE)) _camera->position.z += 0.1;
        if (keystate(VK_SHIFT)) _camera->position.z -= 0.1;

        if (keystate(VK_UP)) set_camera_direction_sph3d(_camera, (struct sph3d) {_camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi + 0.1});
        if (keystate(VK_DOWN)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi - 0.1 });

        if (keystate(VK_LEFT)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta + 0.05, _camera->direction_sph3d.phi });
        if (keystate(VK_RIGHT)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta - 0.05, _camera->direction_sph3d.phi });

        sleepforms(100);

    }
    
return 0;
}