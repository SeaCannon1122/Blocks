#include "headers.h"

#define block(x, y, z) world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius]->blocks[x + 16 * y + 256 * z].id
#define chunc


void Entry() {    

    show_console_window();

    struct world* world = new_world("world1", 100, 187);
    int chunkdex = 
    world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius] = new_chunk();

    block(7, 7, 6) = 4;
    block(7, 7, 7) = 4;

    for (int i = -1; i <= 1; i++) {
        for (int j = -2; j <= 2; j++) {
            block((7 + i), (7 + j), 8) = 5;
        }
    }

    for (int i = -2; i <= 2; i++) {
        for (int j = -1; j <= 1; j++) {
            block((7 + i), (7 + j), 8) = 5;
        }
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -2; j <= 2; j++) {
            block((7 + i), (7 + j), 9) = 5;
        }
    }

    for (int i = -2; i <= 2; i++) {
        for (int j = -1; j <= 1; j++) {
            block((7 + i), (7 + j), 9) = 5;
        }
    }
   
    block(7, 7, 10) = 5;
    block(7, 8, 10) = 5;
    block(7, 6, 10) = 5;
    block(8, 7, 10) = 5;
    block(6, 7, 10) = 5;

    play_world(world);

    return;
}
