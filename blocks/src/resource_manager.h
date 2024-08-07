#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "headers.h"

struct argb_image_map {
	int id;
	char path[256];
	struct argb_image* image;
};

struct resource_manager {
	struct argb_image_map* image_maps;
	int images_count;
};

struct oriented_object {
	struct v3d Origin;
	struct v3d T;
	struct v3d N;
	struct v3d B;
	int resource_id;
	struct argb_image* image;
};

#define BLOCK_OPAQUE 0b1

#define BLOCK_ORIENTED 0b10

#define BLOCK_FULL 0b100

#define BLOCK_SAME_TYPE_NO_BORDER 0b

//#define

struct block_render_info {
	int id;
	bool oriented;
	int type;
	struct oriented_object* resources;
	int resources_length;
};

struct resource_manager* new_resource_manager();

struct argb_image* get_image(struct resource_manager* _rm, int _id);

struct block_render_info* get_block_render_info(struct resource_manager* _rm, int _id);

#endif // !RESOURCE_MANAGER_H