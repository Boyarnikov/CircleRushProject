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
    double speed = 1.5;

    // player control fields
    bool is_clockwise = true;
    bool direction_button_pressed = false;

    // player hp fields
    int hp = 3;
    // player level fields 
    int score = 0; // full amount
    int level = 0; // left level border
    int level_count = 0; // right level border
    int next_level = 2; // next right level border

    continuous_value level_fullnes{0, 0, false, false, false };
    continuous_value player_hp{ 3, 3, false, false, false };
public:

    int get_level();
    int get_score();
    int get_hp();
    void add_score(int);
    void add_hp(int);

    tfm::transform get_ball_transform(int);
    
    void act(data_time) override;
    void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time) override;

    player_object(data_time, tfm::transform);
};

