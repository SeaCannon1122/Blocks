#include "headers.h"

struct chunk* new_chunk() {
	struct chunk* chunk = calloc(1, sizeof(struct chunk));

	return chunk;
}
void generate_chunk(struct chunk* _chunk, int _posx, int _posy, int _seed) {
	//for (int i = 0; i < 256; i++) _chunk->blocks[i].id = 1;
}