#ifndef RESOURCES_H
#define RESOURCES_H

#include "headers.h"

enum block_ids {
	block_air = 0,
	block_stone = 1,
	block_bedrock = 2,
};

enum textures_ids {
	texture_stone = 1,
	texture_bedrock = 2
};

struct argb_image_map texture_maps[100] = {
	{texture_stone, "resources/stone.png", NULL },
	{texture_bedrock, "resources/bedrock.png", NULL }
};

int texture_maps_length = 2;

struct oriented_rect_info stone_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_stone }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_stone }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_stone, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_stone }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_stone }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_stone }, // negz
};

struct oriented_rect_info bedrock_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_bedrock }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_bedrock }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_bedrock, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_bedrock }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_bedrock }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_bedrock }, // negz
};

struct block_render_info block_render_infos[] = {
	{block_air, 0, NULL, 0},
	{block_stone, BLOCK_OPAQUE, stone_block_textures, 6},
	{block_bedrock, BLOCK_OPAQUE, bedrock_block_textures, 6}
};

int block_render_infos_lenght = 3;



#endif // !RESOURCES_H