#include "headers.h"

enum block_ids {
	block_air = 0,
	block_stone = 1,
	block_bedrock = 2,
	block_dirt = 3,
	block_grass = 4,
	block_oak_log = 5,
	block_oak_leaves = 6,
	block_oak_planks = 7,
	block_sand = 8,
	block_diamond_ore = 9
};

enum textures_ids {
	texture_stone = 1,
	texture_bedrock = 2,
	texture_dirt = 3,
	texture_grass_block_side = 4,
	texture_grass_block_top = 5,
	texture_oak_log = 6,
	texture_oak_log_top = 7,
	texture_oak_leaves = 8,
	texture_oak_planks = 9,
	texture_sand = 10,
	texture_diamond_ore = 11
};

struct argb_image_map texture_maps[] = {
	{texture_stone, "resources/stone.png", NULL },
	{texture_bedrock, "resources/bedrock.png", NULL },
	{texture_dirt, "resources/dirt.png", NULL },
	{texture_grass_block_side, "resources/grass_block_side.png", NULL },
	{texture_grass_block_top, "resources/grass_block_top.png", NULL },
	{texture_oak_log, "resources/oak_log.png", NULL },
	{texture_oak_log_top, "resources/oak_log_top.png", NULL },
	{texture_oak_leaves, "resources/oak_leaves.png", NULL },
	{texture_oak_planks, "resources/oak_planks.png", NULL },
	{texture_sand, "resources/sand.png", NULL },
	{texture_diamond_ore, "resources/diamond_ore.png", NULL },
};

int texture_maps_length = sizeof(texture_maps) / sizeof(struct argb_image_map);

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

struct oriented_rect_info dirt_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_dirt }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_dirt }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_dirt, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_dirt }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_dirt }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_dirt }, // negz
};

struct oriented_rect_info grass_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_grass_block_side }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_grass_block_side }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_grass_block_side, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_grass_block_side }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_grass_block_top }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_grass_block_top }, // negz
};

struct oriented_rect_info oak_log_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_oak_log }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_oak_log }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_oak_log, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_oak_log }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_oak_log_top }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_oak_log_top }, // negz
};

struct oriented_rect_info oak_leaves_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_oak_leaves }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_oak_leaves }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_oak_leaves, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_oak_leaves }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_oak_leaves }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_oak_leaves }, // negz
};

struct oriented_rect_info oak_leaves_planks_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_oak_planks }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_oak_planks }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_oak_planks, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_oak_planks }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_oak_planks }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_oak_planks }, // negz
};

struct oriented_rect_info sand_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_sand }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_sand }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_sand, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_sand }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_sand }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_sand }, // negz
};

struct oriented_rect_info diamond_ore_block_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, NULL, texture_diamond_ore }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, NULL, texture_diamond_ore }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, NULL, texture_diamond_ore, }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, NULL, texture_diamond_ore }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, NULL, texture_diamond_ore }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, NULL, texture_diamond_ore }, // negz
};

struct block_render_info block_render_infos[] = {
	{block_air, 0, NULL, 0},
	{block_stone, BLOCK_OPAQUE, stone_block_textures, 6},
	{block_bedrock, BLOCK_OPAQUE, bedrock_block_textures, 6},
	{block_dirt, BLOCK_OPAQUE, dirt_block_textures, 6},
	{block_grass, BLOCK_OPAQUE, grass_block_textures, 6},
	{block_oak_log, BLOCK_OPAQUE, oak_log_block_textures, 6},
	{block_oak_leaves, BLOCK_FULL, oak_leaves_block_textures, 6},
	{block_oak_planks, BLOCK_FULL, oak_leaves_planks_textures, 6},
	{block_sand, BLOCK_FULL, sand_block_textures, 6},
	{block_diamond_ore, BLOCK_FULL, diamond_ore_block_textures, 6}
};

int block_render_infos_lenght = sizeof(block_render_infos) / sizeof(struct block_render_info);

union pixel texture_not_found_pixels[256] = { 
	0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xff000000, 0xff000000, 0xff000000, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xff000000, 0xff000000, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xff000000, 0xff000000, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xfffc32e0, 0xff000000, 0xff000000, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
	0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0, 0xfffc32e0,
};

union pixel block_not_found_pixels[256] = {
	0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xff000000, 0xff000000, 0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xff000000, 0xff000000, 0xff000000, 0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xfffff200,
	0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xff000000, 0xfffff200, 0xff000000, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
	0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200, 0xfffff200,
};

struct argb_image texture_not_found_image = { 16, 16, &texture_not_found_pixels };
struct argb_image block_not_found_image = { 16, 16, &block_not_found_pixels };

struct oriented_rect_info block_not_found_textures[] = {
	{ { 1, 0, 0 }, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, &block_not_found_image }, //posx
	{ { 1, 1, 0 }, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, &block_not_found_image }, // posy
	{ { 0, 1, 0 }, {-1, 0, 0}, {0, -1, 0}, {0, 0, 1}, &block_not_found_image }, //negx
	{ { 0, 0, 0 }, {0, -1, 0}, {1, 0, 0}, {0, 0, 1}, &block_not_found_image }, // negy
	{ { 0, 0, 1 }, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, &block_not_found_image }, //posz
	{ { 0, 1, 0 }, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}, &block_not_found_image }, // negz
};

struct block_render_info block_not_found_render_info = { -1, BLOCK_OPAQUE, block_not_found_textures, 6 };

struct resource_manager* new_resource_manager() {
	struct resource_manager* rm = (struct resource_manager*) malloc(sizeof(struct resource_manager));
	rm->images_count = texture_maps_length;
	rm->image_maps = texture_maps;
	return rm;
}

struct argb_image* get_texture(struct resource_manager* _rm, int _texture_id) {
	for (int i = 0; i < _rm->images_count; i++) {
		if (_rm->image_maps[i].texture_id == _texture_id) {
			if (_rm->image_maps[i].image == NULL) _rm->image_maps[i].image = load_png(_rm->image_maps[i].path);
			if (_rm->image_maps[i].image == NULL) return &texture_not_found_image;
			return _rm->image_maps[i].image;
		}
	}

	return &texture_not_found_image;
}

struct block_render_info* get_block_render_info(struct resource_manager* _rm, int _block_id) {
	struct block_render_info* info = &block_not_found_render_info;
	for (int i = 0; i < block_render_infos_lenght; i++) if (block_render_infos[i].block_id== _block_id) info = &block_render_infos[i];

	for (int i = 0; i < info->rect_infos_length; i++) {
		if (info->rect_infos[i].image == NULL) {
			info->rect_infos[i].image = get_texture(_rm, info->rect_infos[i].texture_id);
		}
	}

	return info;
}