#pragma once
#include "MovingObject.h"
class health_bonus :
    public moving_object
{
private:
    int block_size = 20;
public:
    void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time) override;

    health_bonus() = default;
    health_bonus(data_time, tfm::transform, tfm::transform);
};

