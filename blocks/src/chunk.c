#include "headers.h"

#define leaves(x, y, z) (_chunk->blocks[(x) + 16 * (y) + 256 * (z)].id = 6)

struct chunk* new_chunk() {
	struct chunk* chunk = calloc(1, sizeof(struct chunk));

	return chunk;
}
void generate_chunk(struct chunk* _chunk, int _posx, int _posy, int _seed) {
	for (int i = 0; i < 256; i++) _chunk->blocks[i].id = 2;
	for (int i = 256; i < 5 * 256; i++) _chunk->blocks[i].id = 1;
	for (int i = 4 * 256; i < 5 * 256; i++) _chunk->blocks[i].id = 3;
	for (int i = 5 * 256; i < 6 * 256; i++) _chunk->blocks[i].id = 4;

	for (int i = 2; i < 14; i++) {
		for (int j = 2; i < 14; i++) {
			if (40 * (double)rand() / RAND_MAX > 39) {
				_chunk->blocks[i + 16 * j + 6 * 256].id = 5;
				_chunk->blocks[i + 16 * j + 7 * 256].id = 5;
				leaves(i - 1, j + 2, 8); leaves(i, j + 2, 8); leaves(i + 1, j + 2, 8);
				leaves(i - 2, j + 1, 8); leaves(i - 1, j + 1, 8); leaves(i, j + 1, 8); leaves(i + 1, j + 1, 8); leaves(i + 2, j + 1, 8);
				leaves(i - 2, j, 8); leaves(i - 1, j, 8); leaves(i, j, 8); leaves(i + 1, j, 8); leaves(i + 2, j, 8);
				leaves(i - 2, j - 1, 8); leaves(i - 1, j - 1, 8); leaves(i, j - 1, 8); leaves(i + 1, j - 1, 8); leaves(i + 2, j - 1, 8);
				leaves(i - 1, j - 2, 8); leaves(i, j - 2, 8); leaves(i + 1, j - 2, 8);

				leaves(i - 1, j + 2, 9); leaves(i, j + 2, 9); leaves(i + 1, j + 2, 9);
				leaves(i - 2, j + 1, 9); leaves(i - 1, j + 1, 9); leaves(i, j + 1, 9); leaves(i + 1, j + 1, 9); leaves(i + 2, j + 1, 9);
				leaves(i - 2, j, 9); leaves(i - 1, j, 9); leaves(i, j, 9); leaves(i + 1, j, 9); leaves(i + 2, j, 9);
				leaves(i - 2, j - 1, 9); leaves(i - 1, j - 1, 9); leaves(i, j - 1, 9); leaves(i + 1, j - 1, 9); leaves(i + 2, j - 1, 9);
				leaves(i - 1, j - 2, 9); leaves(i, j - 2, 9); leaves(i + 1, j - 2, 9);

				leaves(i, j + 1, 10);
				leaves(i - 1, j, 10); leaves(i, j, 10); leaves(i + 1, j, 10);
				leaves(i, j - 1, 10);

			}
		}
	}





}