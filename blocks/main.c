#include "headers.h"
    
void draw_quadrilateral(struct quadrilateral* _quadrilateral, struct camera* _camera) {
}

void fps_counting(void* args) {
    while (active) {
        printf("fps: %d\n", **(int**)args);
        **(int**)args = 0;
        sleepforms(1000);
    }
}

void controling(struct camera* _camera) {
    while (active) {

        if (keystate('W')) {
            _camera->position.x -= 0.02 * sin(_camera->direction_sph3d.theta);
            _camera->position.y += 0.02 * cos(_camera->direction_sph3d.theta);
        }

        if (keystate('S')) {
            _camera->position.x += 0.02 * sin(_camera->direction_sph3d.theta);
            _camera->position.y -= 0.02 * cos(_camera->direction_sph3d.theta);
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

        if (keystate(VK_UP)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi + 0.04 });
        if (keystate(VK_DOWN)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi - 0.04 });

        if (keystate(VK_LEFT)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta + 0.025, _camera->direction_sph3d.phi });
        if (keystate(VK_RIGHT)) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta - 0.025, _camera->direction_sph3d.phi });


        sleepforms(20);
    }
}

int Entry() {

    struct camera* _camera = new_camera((struct v3d) { 0, -2.0, -0.05 }, 500, 300, 0.0002, 10);
    struct camera* _camera_copy = new_camera((struct v3d) { 0, -2.0, 0 }, 500, 300, 0.0002, 10);

    set_camera_direction_sph3d(_camera, (struct sph3d) { 0.07, 0, 0 });

    unsigned int diamond_ore_pixels[256] = { 0xff8f8f8f, 0xff7f7f7f, 0xff7f7f7f, 0xff8f8f8f, 0xff7f7f7f, 0xff747474, 0xff747474, 0xff7f7f7f, 0xff747474, 0xff676767, 0xff747474, 0xff747474, 0xff7f7f7f, 0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f,
0xff7f7f7f, 0xff7f7f7f, 0xff747474, 0xff7f7f7f, 0xff676767, 0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f, 0xff7f7f7f, 0xff7f7f7f, 0xff747474, 0xff676767, 0xff676767, 0xff747474, 0xff7f7f7f,
0xff7f7f7f, 0xff747474, 0xff676767, 0xff676767, 0xff1ed0d6, 0xff8f8f8f, 0xff7f7f7f, 0xff676767, 0xff676767, 0xff747474, 0xff747474, 0xff676767, 0xff77e7d1, 0xff239698, 0xff7f7f7f, 0xff7f7f7f,
0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f, 0xff7f7f7f, 0xff8dadb1, 0xff8f8f8f, 0xff676767, 0xff77e7d1, 0xff239698, 0xff8dadb1, 0xff8f8f8f, 0xff7f7f7f, 0xff8dadb1, 0xff8f8f8f, 0xff747474, 0xff747474,
0xff747474, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff747474, 0xff676767, 0xff676767, 0xff8f8f8f, 0xff8dadb1, 0xff7f7f7f, 0xff747474, 0xff7f7f7f, 0xff8f8f8f, 0xff747474, 0xff7f7f7f, 0xff7f7f7f,
0xff7f7f7f, 0xff8f8f8f, 0xff7f7f7f, 0xff747474, 0xff676767, 0xffd5fff6, 0xff77e7d1, 0xff676767, 0xff7f7f7f, 0xff747474, 0xffd5fff6, 0xff77e7d1, 0xff747474, 0xff676767, 0xff747474, 0xff8f8f8f,
0xff747474, 0xff747474, 0xff8f8f8f, 0xffd5fff6, 0xff77e7d1, 0xff1ed0d6, 0xff1ed0d6, 0xff239698, 0xff8dadb1, 0xff7f7f7f, 0xff1ed0d6, 0xff239698, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f,
0xff8f8f8f, 0xff676767, 0xff676767, 0xff8f8f8f, 0xff8dadb1, 0xffa2a2a2, 0xff8dadb1, 0xff8dadb1, 0xff676767, 0xff676767, 0xff8f8f8f, 0xff8dadb1, 0xff7f7f7f, 0xff8f8f8f, 0xffa2a2a2, 0xffa2a2a2,
0xff676767, 0xff77e7d1, 0xff1ed0d6, 0xff8dadb1, 0xff8f8f8f, 0xff7f7f7f, 0xff747474, 0xff676767, 0xffd5fff6, 0xff77e7d1, 0xff747474, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff8f8f8f, 0xff747474,
0xff7f7f7f, 0xff8dadb1, 0xff8dadb1, 0xff7f7f7f, 0xff676767, 0xff7f7f7f, 0xff747474, 0xff77e7d1, 0xff1ed0d6, 0xff239698, 0xff239698, 0xff8dadb1, 0xff8f8f8f, 0xffa2a2a2, 0xffa2a2a2, 0xff7f7f7f,
0xff676767, 0xff7f7f7f, 0xff747474, 0xff7f7f7f, 0xff1ed0d6, 0xff747474, 0xff676767, 0xff8f8f8f, 0xff8dadb1, 0xff8dadb1, 0xff8f8f8f, 0xff676767, 0xff676767, 0xff747474, 0xff747474, 0xff747474,
0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f, 0xff747474, 0xff676767, 0xff77e7d1, 0xff239698, 0xff8f8f8f, 0xff8f8f8f, 0xff7f7f7f,
0xff7f7f7f, 0xff747474, 0xff8f8f8f, 0xff676767, 0xff676767, 0xff7f7f7f, 0xff8f8f8f, 0xff747474, 0xff77e7d1, 0xffd5fff6, 0xff77e7d1, 0xff1ed0d6, 0xff1ed0d6, 0xff239698, 0xff8f8f8f, 0xff747474,
0xff8f8f8f, 0xff8f8f8f, 0xff8dadb1, 0xff77e7d1, 0xff239698, 0xff676767, 0xff747474, 0xff7f7f7f, 0xffa2a2a2, 0xff239698, 0xff1ed0d6, 0xff8dadb1, 0xff8dadb1, 0xff8f8f8f, 0xff7f7f7f, 0xff7f7f7f,
0xff7f7f7f, 0xff747474, 0xff747474, 0xff8dadb1, 0xff8f8f8f, 0xff8f8f8f, 0xff8f8f8f, 0xff7f7f7f, 0xff7f7f7f, 0xff8f8f8f, 0xff8dadb1, 0xff7f7f7f, 0xff747474, 0xff747474, 0xff747474, 0xff7f7f7f,
0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff747474, 0xff8f8f8f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff7f7f7f, 0xff8f8f8f, 0xff8f8f8f
    };


    struct argb_image diamond_ore = { 16, 16, diamond_ore_pixels };

    struct oriented_rect sides[6] = {
        { (struct v3d) { 0, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 1, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 0, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 0, 0, 1 }, (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, &diamond_ore },
        { (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, -1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, -1, 0 }, &diamond_ore },
    };

    struct oriented_rect testrect = { (struct v3d) { 0, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, & diamond_ore };
    
    int fps_counter = 0;
    int* fps_counter_address = &fps_counter;


    int fps_counter_threadID;

    void* fps_counter_thread = create_thread(fps_counting, &fps_counter_address, &fps_counter_threadID);

    int control_threadID;

    void* control_thread = create_thread(controling, _camera, &control_threadID);

    while (active) {

        set_camera_size(_camera, render_state.buffer_width, render_state.buffer_height);

        flash_camera_screen(_camera);
        
        *_camera_copy = *_camera;

        for (int i = 0; i < 6; i++) {
            //if ((sides[i].Origin.x - _camera_copy->position.x) * sides[i].T.x + (sides[i].Origin.y - _camera_copy->position.y) * sides[i].T.y + (sides[i].Origin.z - _camera_copy->position.z) * sides[i].T.z < 0) camera_render_oriented_rect(_camera_copy, &sides[i]);
        }

        camera_render_oriented_rect(_camera_copy, &testrect);

        camera_render_cursor(_camera);

        if (keystate('C')) active = false;

        drawWindow(_camera->pixels, _camera->width, _camera->height);

        fps_counter++;
    }
    
    join_thread(fps_counter_thread);
    join_thread(control_thread);


return 0;
}