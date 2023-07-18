#pragma once
#include "MovingObject.h"
class enemy_cube :
    public moving_object
{
private:
    int block_size = 20;
public:
    void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time) override;

    enemy_cube() = default;
    enemy_cube(data_time, tfm::transform, tfm::transform);
};

