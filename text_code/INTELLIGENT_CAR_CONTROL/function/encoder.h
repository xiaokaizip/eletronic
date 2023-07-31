//
// Created by 86136 on 2023/8/1.
//

#ifndef INTELLIGENT_CAR_CONTROL_ENCODER_H
#define INTELLIGENT_CAR_CONTROL_ENCODER_H


void encoder_init();

float calculate_speed(int encoder, unsigned short interval_time);

#endif //INTELLIGENT_CAR_CONTROL_ENCODER_H
