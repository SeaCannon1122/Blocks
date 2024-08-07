#include "headers.h"
#include "resources.h"

struct resource_manager* new_resource_manager() {
	struct resource_manager* rm = (struct resource_manager*) malloc(sizeof(struct resource_manager));
	rm->images_count = 0;
	rm->image_maps = texture_maps;
	return rm;
}

struct argb_image* get_image(struct resource_manager* _rm, int _id) {
	for (int i = 0; i < _rm->images_count; i++) {
		if (_rm->image_maps[i].id == _id) {
			if (_rm->image_maps[i].image == NULL) {
				_rm->image_maps[i].image = load_png(_rm->image_maps[i].path);
			}
			
			return _rm->image_maps[i].image;		
		}
	}

}

struct block_render_info* get_block_render_info(struct resource_manager* _rm, int _id) {
	struct block_render_info* info;
	for (int i = 0; i < block_render_infos_lenght; i++) if (block_render_infos[i].id == _id) info = &block_render_infos[i];

	for (int i = 0; i < info->resources_length; i++) {
		if (info->resources[i].image == NULL) {
			info->resources[i].image = get_image(_rm, info->resources[i].resource_id);
		}
	}

	return info;
}