#include "headers.h"

void Entry() {    

    show_console_window();

    struct world* world = new_world("world1", 100, 187);
    world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius] = new_chunk();

    world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius]->blocks[0].id = 2;
    world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius]->blocks[256].id = 2;
    world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius]->blocks[1].id = 1;
    world->chunk_pointer_table[world->world_chunk_radius + world->world_chunk_radius * 2 * world->world_chunk_radius]->blocks[3].id = 1;

    play_world(world);

    return;
}
