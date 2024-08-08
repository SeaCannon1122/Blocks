#include "headers.h"
#include "resources.h"

struct resource_manager* new_resource_manager() {
	struct resource_manager* rm = (struct resource_manager*) malloc(sizeof(struct resource_manager));
	rm->images_count = texture_maps_length;
	rm->image_maps = texture_maps;
	return rm;
}

struct argb_image* get_texture(struct resource_manager* _rm, int _texture_id) {
	for (int i = 0; i < _rm->images_count; i++) {
		if (_rm->image_maps[i].texture_id == _texture_id) {
			if (_rm->image_maps[i].image == NULL) {
				_rm->image_maps[i].image = load_png(_rm->image_maps[i].path);
			}
			
			return _rm->image_maps[i].image;		
		}
	}

}

struct block_render_info* get_block_render_info(struct resource_manager* _rm, int _block_id) {
	struct block_render_info elset;
	struct block_render_info* info = &elset;
	for (int i = 0; i < block_render_infos_lenght; i++) if (block_render_infos[i].block_id== _block_id) info = &block_render_infos[i];

	for (int i = 0; i < info->rect_infos_length; i++) {
		if (info->rect_infos[i].image == NULL) {
			info->rect_infos[i].image = get_texture(_rm, info->rect_infos[i].texture_id);
		}
	}

	return info;
}