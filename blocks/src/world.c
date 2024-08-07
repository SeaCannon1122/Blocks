#include "headers.h"
#include "platform.h"

struct world* new_world(char* _world_file_path, int _world_chunk_radius, int _seed) {

	int world_file_path_length = string_length(_world_file_path);

	void* buffer = malloc(sizeof(struct world) + world_file_path_length * sizeof(char) + 4 * _world_chunk_radius * sizeof(void*));
	if (buffer == NULL) goto _alloc_error;
	
	struct world* world = buffer;
	world->world_file_path = (unsigned long long) buffer + sizeof(struct world);
	world->chunk_pointer_table = (unsigned long long) buffer + sizeof(struct world) + world_file_path_length * sizeof(char);
	world->world_chunk_radius = _world_chunk_radius;
	world->rm = new_resource_manager();
	memcpy(world->world_file_path, _world_file_path, world_file_path_length * sizeof(char));

	return world;

_alloc_error:

	return NULL;
}

void play_world(struct world* _world) {
	
	struct window_state* window = create_window(100, 100, 700, 400, "blocks");

	struct camera* player_camera = new_camera( (struct v3d) {0, 0, 0}, window->window_width, window->window_height, 0.0002, 10);
	set_camera_direction_sph3d(player_camera, (struct sph3d) {0.07, 0, 0});




	while (1 == 1) sleep_for_ms(1000);





	return;
}