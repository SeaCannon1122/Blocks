#include "headers.h"

enum ConsoleMode
{
    DEBUG = 1,
    GAMEINFO = 0,
};


int consolemode = DEBUG;

void controlling(struct camera* _camera) {
    
    double scalar = 0.14;
    
    while (active) {

        if (get_key_state('W') & 0b0001) {
            _camera->position.x -= 0.5 * sin(_camera->direction_sph3d.theta) * scalar;
            _camera->position.y += 0.5 * cos(_camera->direction_sph3d.theta) * scalar;
        }

        if (get_key_state('S') & 0b0001) {
            _camera->position.x += 0.5 * sin(_camera->direction_sph3d.theta) * scalar;
            _camera->position.y -= 0.5 * cos(_camera->direction_sph3d.theta) * scalar;
        }

        if (get_key_state('D') & 0b0001) {
            _camera->position.x += 0.5 * cos(_camera->direction_sph3d.theta) * scalar;
            _camera->position.y += 0.5 * sin(_camera->direction_sph3d.theta) * scalar;
        }

        if (get_key_state('A') & 0b0001) {
            _camera->position.x -= 0.5 * cos(_camera->direction_sph3d.theta) * scalar;
            _camera->position.y -= 0.5 * sin(_camera->direction_sph3d.theta) * scalar;
        }

        if (get_key_state(KEY_SPACE) & 0b0001) _camera->position.z += 0.5 * scalar;
        if (get_key_state(KEY_SHIFT) & 0b0001) _camera->position.z -= 0.5 * scalar;

        if (get_key_state(KEY_ARROW_UP) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi + 0.04});
        if (get_key_state(KEY_ARROW_DOWN) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi - 0.04});

        if (get_key_state(KEY_ARROW_LEFT) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta + 0.03, _camera->direction_sph3d.phi });
        if (get_key_state(KEY_ARROW_RIGHT) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta - 0.03, _camera->direction_sph3d.phi });

        //if (keystate('C')) active = false;

        if ((get_key_state('P') & 0b0011) == 0b0011) {
            if (consolemode == GAMEINFO) consolemode = DEBUG;
            else consolemode = GAMEINFO;
        }

        sleep_for_ms(20);
    }
}


void Entry() {

    

    double last_frame_time;

    struct camera* _camera = new_camera((struct v3d) { 0, -2.0, -0.05 }, 500, 300, 0.0002, 10);
    struct camera* _camera_copy = new_camera((struct v3d) { 0, -2.0, 0 }, 500, 300, 0.0002, 10);

    set_camera_direction_sph3d(_camera, (struct sph3d) { 0.07, 0, 0 });

    struct argb_image diamond_ore;

    load_png("resources/stone.png", &diamond_ore);

    struct oriented_rect sides[24] = {
        { (struct v3d) { 0, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 1, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 0, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 0, 0, 1 }, (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, &diamond_ore },
        { (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, -1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, -1, 0 }, &diamond_ore },

        { (struct v3d) { 0, 1, 1 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 0, 2, 1 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 2, 1 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 1, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 0, 1, 2 }, (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, &diamond_ore },
        { (struct v3d) { 0, 2, 1 }, (struct v3d) { 0, 0, -1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, -1, 0 }, &diamond_ore },

        { (struct v3d) { 1, 0, 1 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 1, 1 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 2, 1, 1 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 2, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 0, 2 }, (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, &diamond_ore },
        { (struct v3d) { 1, 1, 1 }, (struct v3d) { 0, 0, -1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, -1, 0 }, &diamond_ore },

        { (struct v3d) { 1, 1, 2 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 2, 2 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 2, 2, 2 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { -1, 0, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 2, 1, 2 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, (struct v3d) { 0, 0, 1 }, &diamond_ore },
        { (struct v3d) { 1, 1, 3 }, (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, &diamond_ore },
        { (struct v3d) { 1, 2, 2 }, (struct v3d) { 0, 0, -1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, -1, 0 }, &diamond_ore },
    };

    struct oriented_rect testrect = { (struct v3d) { 0, 0, 0 }, (struct v3d) { 0, -1, 0 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 0, 1 }, & diamond_ore };
    


    struct oriented_rect* floors = (struct oriented_rect*) malloc(160000 * sizeof(struct oriented_rect));

    //for (int i = 0; i < 160000; i++) floors[i] = (struct oriented_rect){ (struct v3d) { i % 400, (int)(i / 400), 0 }, (struct v3d) { 0, 0, 1 }, (struct v3d) { 1, 0, 0 }, (struct v3d) { 0, 1, 0 }, &diamond_ore};

    void* control_thread = create_thread((void*) controlling, _camera);

    last_frame_time = get_time();

    int count = 0; 

    while (active) {
        count++;
        set_camera_size(_camera, window_state.window_width, window_state.window_height);

        flash_camera_screen(_camera);
        
        *_camera_copy = *_camera;

        for (int i = 0; i < 6; i++) {
            if ((sides[i].Origin.x - _camera_copy->position.x) * sides[i].T.x + (sides[i].Origin.y - _camera_copy->position.y) * sides[i].T.y + (sides[i].Origin.z - _camera_copy->position.z) * sides[i].T.z < 0) camera_render_oriented_rect(_camera_copy, &sides[i]);
        }

        //if ((testrect.Origin.x - _camera_copy->position.x) * testrect.T.x + (testrect.Origin.y - _camera_copy->position.y) * testrect.T.y + (testrect.Origin.z - _camera_copy->position.z) * testrect.T.z < 0) camera_render_oriented_rect(_camera_copy, &testrect);
        
        for (int i = 0; i < 0; i++) {
            if ((floors[i].Origin.x - _camera_copy->position.x) * floors[i].T.x + (floors[i].Origin.y - _camera_copy->position.y) * floors[i].T.y + (floors[i].Origin.z - _camera_copy->position.z) * floors[i].T.z < 0) camera_render_oriented_rect(_camera_copy, &floors[i]);
        }
        
        

        camera_render_cursor(_camera);

        draw_to_window((unsigned int*) _camera->pixels, _camera->width, _camera->height);

        if (consolemode == GAMEINFO) {
            set_console_cursor_position(0, 0);
            printf("time taken: %fms           \n", get_time() - last_frame_time);
            printf("position: %f %f %f     \n", _camera->position.x, _camera->position.y, _camera->position.z);
            printf("heading: %f %f           \n", _camera->direction_sph3d.theta, _camera->direction_sph3d.phi);
            
        }

        last_frame_time = get_time();
    }
    
    join_thread(control_thread);


return;
}
