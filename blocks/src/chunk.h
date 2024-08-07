#ifndef CHUNK_H
#define CHUNK_H

#include "headers.h"

struct chunk {
	struct block blocks[256 * 16 * 16];
};

struct chunk* new_chunk();

void generate_chunk(struct chunk* _chunk, int _posx, int _posy, int _seed);

#endif // !CHUNK_H
