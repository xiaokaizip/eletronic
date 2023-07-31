//
// Created by 86136 on 2023/8/1.
//

#include "encoder.h"
#include "main.h"
#include "tim.h"
#include <stdio.h>
#include "motor.h"

#define ENCODER_FL

extern chassis_t chassis;

void encoder_init() {
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

}

float calculate_speed(int encoder, unsigned short interval_time) {
    float speed = (float) encoder / 52.0f / 30.0f * (1000.0f / (float) interval_time);
    return speed;
}


int count = 0;
int last_count = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        count = TIM1->CNT;
        chassis.motor[motor_FL].real_speed = calculate_speed(count - chassis.motor[motor_FL].last_count, 10);
        chassis.motor[motor_FL].last_count = count;

        count = TIM2->CNT;
        chassis.motor[motor_FR].real_speed = calculate_speed(count - chassis.motor[motor_FR].last_count, 10);
        chassis.motor[motor_FR].last_count = count;

        count = TIM3->CNT;
        chassis.motor[motor_BL].real_speed = calculate_speed(count - chassis.motor[motor_BL].last_count, 10);
        chassis.motor[motor_BL].last_count = count;

        count = TIM5->CNT;
        chassis.motor[motor_BR].real_speed = calculate_speed(count - chassis.motor[motor_BR].last_count, 10);
        chassis.motor[motor_BR].last_count = count;
    }

}