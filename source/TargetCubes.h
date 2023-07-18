#pragma once
#include "MovingObject.h"
// high-end gameobject that follows straight path and revards player with score if touched
class target_cube :
    public moving_object
{
private:
    int block_size = 20;
public:
    void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time) override;

    target_cube() = default;
    target_cube(data_time, tfm::transform, tfm::transform);
};

