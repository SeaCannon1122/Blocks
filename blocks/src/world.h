#ifndef WORLD_H
#define WORLD_H

#include "headers.h"

struct world {
	char* world_file_path;
	int seed;
	struct chunk** chunk_pointer_table;
	int world_chunk_radius;
};

struct world* new_world(char* _world_file_path, int _world_chunk_radius, int _seed);

void play_world(struct world* _world);

#endif // !WORLD_H