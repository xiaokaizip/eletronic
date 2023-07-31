//
// Created by 86180 on 2023/7/30.
//

#ifndef INTELLIGENT_CAR_CONTROL_MOTOR_H
#define INTELLIGENT_CAR_CONTROL_MOTOR_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "math.h"
#include "tim.h"

typedef enum {
    motor_forward, motor_back
} motor_direction_e;

typedef enum {
    motor_FL, motor_FR, motor_BL, motor_BR
} motor_type_e;


typedef struct {
    float set_speed;
    float real_speed;
    int direction;
    float motor_pid_out;
    motor_type_e motor_type;
    int last_count;
} motor_t;


typedef enum {
    chassis_forward, chassis_back, chassis_turn_left, chassis_turn_right
} chassis_direction_e;

typedef struct {
    motor_t motor[4];
    float chassis_speed;
    chassis_direction_e chassis_direction;
} chassis_t;


void chassis_init(void);

void set_chassis_direction_speed(chassis_t *chassis, chassis_direction_e chassis_direction, int speed);

void set_motor_direction(chassis_t *chassis);


#endif //INTELLIGENT_CAR_CONTROL_MOTOR_H
