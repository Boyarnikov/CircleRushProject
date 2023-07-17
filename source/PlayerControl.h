#pragma once
#include "Object.h"
#include "ContinuousValue.h"

class player_object :
    public object
{
private:
    // player balls fields
    tfm::transform player_balls[2];
    double player_ball_size = 20.;
    double radius = 150.;
    double speed = 2.;

    // player control fields
    bool is_clockwise = true;
    bool direction_button_pressed = false;

    // player level fields
    int score = 0; // full amount
    int level = 0; // left level border
    int level_count = 0; // right level border
    int next_level = 2; // next right level border
    continuous_value level_fullnes{0, 0, false, false, false };

public:
    void act(data_time) override;
    void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time) override;



    player_object(data_time, tfm::transform);
};

