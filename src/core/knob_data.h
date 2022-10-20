#pragma once

#include <Arduino.h>

struct KnobConfig {
    //可以运动的个数
    int32_t num_positions;
    //初始位置
    int32_t position;
    //位置宽度弧度 或者是每一步的度数
    float position_width_radians;
    //制动强度
    float detent_strength_unit;
    //end stop强度 
    float endstop_strength_unit;
    //中断点 
    float snap_point;
    // int32_t 设备类型,缺省 10，Computer 11;
    int32_t device_type;
    // int32_t 操作类型,缺省 10，Volume 11、UpDown 12、Play/Pause 13,Switch 14, Zoom 15;
    int32_t device_operate;
    // char 功能描述, 50个字符;
    char descriptor[50];
};

struct KnobState {
    int32_t current_position;
    float sub_position_unit;
    KnobConfig config;
};
