#ifndef RESOURCES_H
#define RESOURCES_H

#include "headers.h"

enum blocks {
	block_air = 0,
	block_stone = 1,
};

enum textures_ids {
	texture_stone = 1,
};

struct argb_image_map texture_maps[100] = {
	{texture_stone, "stone.png", NULL },
};

int texture_maps_length = 1;

struct oriented_object stone_block_textures[] = {
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, texture_stone, NULL },
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, texture_stone, NULL },
	{ { 1, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, texture_stone, NULL },
	{ { 1, 1, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, texture_stone, NULL },
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, texture_stone, NULL },
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, texture_stone, NULL },
};

struct block_render_info block_render_infos[] = {
	{block_stone, false, true, stone_block_textures, 6}
};

int block_render_infos_lenght = 1;



#endif // !RESOURCES_H