#pragma once
#include "Object.h"

class moving_object :
    public object
{
private: 
    tfm::transform transform_dt;
public:
    void act(double time, double df) override;
    moving_object(double time, tfm::transform, tfm::transform, Tags);
    moving_object(double time, tfm::transform, tfm::transform);
    moving_object(double time, tfm::transform, Tags);
    moving_object(double time, tfm::transform);
    moving_object(double time, Tags);
    moving_object(double time);
};

