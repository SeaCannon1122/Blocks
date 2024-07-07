#ifndef WORLD_H
#define WORLD_H

#include "headers.h"

#define INDEX(world_ptr, cords) ((cords->x + world_ptr->size_x) + 2 * (world_ptr->size_x) * ((cords->y + world_ptr->size_y) + 2 * (world_ptr->size_y) * (cords->z + world_ptr->size_z)))



enum block_id
{
	AIR = 0,
	DIRT = 1,
	GRASS = 2,
	STONE = 3,
	OAK_LOG = 4,
	OAK_PLANKS = 5,
};

enum facing
{
	POS_X = 0,
	NEG_X = 1,
	POS_Y = 2,
	NEG_Y = 3,
	POS_Z = 4,
	NEG_Z = 5
};

struct block {
	int id;
	char facing;
	char breaking_status;
};

struct world {
	struct block* blocks;
	int size_x;
	int size_y;
	int size_z;
};

struct world* new_world(int _size_x, int _size_y, int _size_z);

struct block* get_block_ptr(struct world* _world, struct v3dabs* _cords);

#endif // !WORLD_H
