#pragma once
#include "Object.h"
#include <string>
class moving_object :
    public object
{
private: 
    tfm::transform transform_dt;
public:
    void act(double time, double df) override;
    moving_object(double time, tfm::transform, tfm::transform, std::string);
};

