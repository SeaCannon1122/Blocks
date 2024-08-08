#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "headers.h"

struct argb_image_map {
	int texture_id;
	char path[256];
	struct argb_image* image;
};

struct resource_manager {
	struct argb_image_map* image_maps;
	int images_count;
};

struct oriented_rect_info {
	struct v3d Origin;
	struct v3d T;
	struct v3d N;
	struct v3d B; 
	struct argb_image* image;
	int texture_id;
};

#define BLOCK_OPAQUE 0b1

#define BLOCK_ORIENTED 0b10

#define BLOCK_FULL 0b100

#define BLOCK_SAME_TYPE_NO_BORDER 0b1000

struct block_render_info {
	int block_id;
	int type;
	struct oriented_rect_info* rect_infos;
	int rect_infos_length;
};

struct resource_manager* new_resource_manager();

struct argb_image* get_texture(struct resource_manager* _rm, int _texture_id);

struct block_render_info* get_block_render_info(struct resource_manager* _rm, int _block_id);
#endif // !RESOURCE_MANAGER_H