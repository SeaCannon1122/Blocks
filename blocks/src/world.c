#include "headers.h"
#include "platform.h"

struct world* new_world(char* _world_file_path, int _world_chunk_radius, int _seed) {

	unsigned int world_file_path_length = string_length(_world_file_path);

	void* buffer = calloc(1, sizeof(struct world) + 4 * _world_chunk_radius * _world_chunk_radius * sizeof(void*) + world_file_path_length * sizeof(char));
	if (buffer == NULL) goto _alloc_error;
	
	struct world* world = buffer;
	world->chunk_pointer_table = (unsigned long long) buffer + sizeof(struct world);
	world->world_file_path = (unsigned long long) buffer + sizeof(struct world) + 4 * _world_chunk_radius * _world_chunk_radius * sizeof(void*);
	world->world_chunk_radius = _world_chunk_radius;
	world->seed = _seed;
	memcpy(world->world_file_path, _world_file_path, world_file_path_length * sizeof(char));

	return world;

_alloc_error:

	return NULL;
}

struct control_thread_args {
	struct camera* camera;
	struct window_state* window;
};

void world_control_function(void* args) {
	struct camera* _camera = ((struct control_thread_args*) args)->camera;
	struct window_state* window = ((struct control_thread_args*)args)->window;

	double scalar = 0.2;

	while (is_window_active(window)) {

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
		if (get_key_state(KEY_SHIFT_L) & 0b0001) _camera->position.z -= 0.5 * scalar;

		if (get_key_state(KEY_ARROW_UP) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi + 0.04 });
		if (get_key_state(KEY_ARROW_DOWN) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta, _camera->direction_sph3d.phi - 0.04 });

		if (get_key_state(KEY_ARROW_LEFT) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta + 0.03, _camera->direction_sph3d.phi });
		if (get_key_state(KEY_ARROW_RIGHT) & 0b0001) set_camera_direction_sph3d(_camera, (struct sph3d) { _camera->direction_sph3d.radius, _camera->direction_sph3d.theta - 0.03, _camera->direction_sph3d.phi });

		//if (keystate('C')) *active = false;

		sleep_for_ms(20);
	}


}

void play_world(struct world* _world) {
	
	struct window_state* window = create_window(100, 100, 700, 400, "blocks");

	struct camera* player_camera = new_camera( (struct v3d) {0, 0, 0}, window->window_width, window->window_height, 0.0002, 5);
	set_camera_direction_sph3d(player_camera, (struct sph3d) {0.07, 0, 0});

	struct resource_manager* master_of_the_resource = new_resource_manager();

	struct camera player_camera_copy;

	struct control_thread_args args = { player_camera, window };

	void* control_thread = create_thread(world_control_function, &args);

	while (is_window_active(window)) {
		set_console_cursor_position(0 , 0);
		double start_frame_time = get_time();

		set_camera_size(player_camera, window->window_width, window->window_height);

		player_camera_copy = *player_camera;

		flash_camera_screen(&player_camera_copy);

		render_world(_world, &player_camera_copy, master_of_the_resource);

		camera_render_cursor(&player_camera_copy);

		draw_to_window(window, player_camera_copy.pixels, player_camera_copy.width, player_camera_copy.height);

		printf("frame took %fms                      \n", get_time() - start_frame_time);
		printf("pos: %f %f %f                         \n", player_camera_copy.position.x, player_camera_copy.position.y, player_camera_copy.position.z);


	}


	join_thread(control_thread);

	return;
}