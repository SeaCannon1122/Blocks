#include "headers.h"

struct world* new_world(int _size_x, int _size_y, int _size_z) {

    struct world* world_ = (struct camera*)malloc(sizeof(struct world));
    if (world_ == NULL) return NULL;

    world_->blocks = (struct block*)calloc(8 * _size_x * _size_y * _size_z, sizeof(struct block));
    if (world_->blocks == NULL) {
        free(world_);
        return NULL;
    }

    world_->size_x = _size_x;
    world_->size_y = _size_y;
    world_->size_z = _size_z;
    return world_;
}

struct block* get_block_ptr(struct world* _world, struct v3dabs* _cords) {
    if (_cords->x >= _world->size_x || _cords->x < -_world->size_x || _cords->y >= _world->size_y || _cords->y < -_world->size_y || _cords->z >= _world->size_z || _cords->z < -_world->size_z) return NULL;
    return &_world->blocks[INDEX(_world, _cords)];
}