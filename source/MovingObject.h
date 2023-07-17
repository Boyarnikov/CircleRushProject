#pragma once
#include "Object.h"
#include "TimeManager.h"

class moving_object :
    public object
{
private: 
    tfm::transform transform_dt;
public:
    void act(data_time) override;
    moving_object(data_time, tfm::transform, tfm::transform, Tags);
    moving_object(data_time, tfm::transform, tfm::transform);
    moving_object(data_time, tfm::transform, Tags);
    moving_object(data_time, tfm::transform);
    moving_object(data_time, Tags);
    moving_object(data_time);

    const tfm::transform get_transform_dt();
    void set_transform_dt(tfm::transform);
};

