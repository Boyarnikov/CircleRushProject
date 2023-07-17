#pragma once
#include "Object.h"
class player_object :
    public object
{
private:
    tfm::transform player_balls[2];
    double player_ball_size = 20.;
    double radius = 150.;
    double speed = 2.;
    bool is_clockwise = true;
public:
    void act(data_time) override;
    void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time) override;

    player_object(data_time, tfm::transform);
};

