//
// Created by 86180 on 2023/7/30.
//
#include "motor.h"

#define SET_MOTORFL_SPEED(SPEED) __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, SPEED)
#define SET_MOTORFR_SPEED(SPEED) __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, SPEED)
#define SET_MOTORBL_SPEED(SPEED) __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, SPEED)
#define SET_MOTORBR_SPEED(SPEED) __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, SPEED)


void set_chassis_speed(chassis_t *chassis) {
    SET_MOTORFL_SPEED(chassis->motor[motor_FL].set_speed);
    SET_MOTORFR_SPEED(chassis->motor[motor_FR].set_speed);
    SET_MOTORBL_SPEED(chassis->motor[motor_BL].set_speed);
    SET_MOTORBR_SPEED(chassis->motor[motor_BR].set_speed);
}

void set_chassis_direction_speed(chassis_t *chassis, chassis_direction_e chassis_direction, int speed) {
    chassis->chassis_direction = chassis_direction;
    if (chassis->chassis_direction == chassis_forward) {
        chassis->motor[motor_FL].direction = 1;
        chassis->motor[motor_FR].direction = 1;
        chassis->motor[motor_BL].direction = 1;
        chassis->motor[motor_BR].direction = 1;
    } else if (chassis->chassis_direction == chassis_back) {
        chassis->motor[motor_FL].direction = -1;
        chassis->motor[motor_FR].direction = -1;
        chassis->motor[motor_BL].direction = -1;
        chassis->motor[motor_BR].direction = -1;
    } else if (chassis->chassis_direction == chassis_turn_left) {
        chassis->motor[motor_FL].direction = -1;
        chassis->motor[motor_FR].direction = -1;
        chassis->motor[motor_BL].direction = 1;
        chassis->motor[motor_BR].direction = 1;
    } else if (chassis->chassis_direction == chassis_turn_right) {
        chassis->motor[motor_FL].direction = 1;
        chassis->motor[motor_FR].direction = 1;
        chassis->motor[motor_BL].direction = -1;
        chassis->motor[motor_BR].direction = -1;
    }
    set_motor_direction(chassis);


    chassis->motor[motor_FL].set_speed = speed;
    chassis->motor[motor_FR].set_speed = speed;
    chassis->motor[motor_BL].set_speed = speed;
    chassis->motor[motor_BR].set_speed = speed;

    set_chassis_speed(chassis);
}

void chassis_init(void) {
    HAL_GPIO_WritePin(FLIN1_GPIO_Port, FLIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FLIN2_GPIO_Port, FLIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FRIN1_GPIO_Port, FRIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FRIN2_GPIO_Port, FRIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLIN1_GPIO_Port, BLIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLIN2_GPIO_Port, BLIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BRIN1_GPIO_Port, BRIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BRIN2_GPIO_Port, BRIN2_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);


}


void set_motor_direction(chassis_t *chassis) {
    if (chassis->motor[motor_FL].direction == 1) {
        HAL_GPIO_WritePin(FLIN1_GPIO_Port, FLIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FLIN2_GPIO_Port, FLIN2_Pin, GPIO_PIN_RESET);
    } else if (chassis->motor[motor_FL].direction == -1) {
        HAL_GPIO_WritePin(FLIN1_GPIO_Port, FLIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FLIN2_GPIO_Port, FLIN2_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(FLIN1_GPIO_Port, FLIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FLIN2_GPIO_Port, FLIN2_Pin, GPIO_PIN_RESET);
    }
    if (chassis->motor[motor_FR].direction == 1) {
        HAL_GPIO_WritePin(FRIN1_GPIO_Port, FRIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FRIN2_GPIO_Port, FRIN2_Pin, GPIO_PIN_RESET);
    } else if (chassis->motor[motor_FR].direction == -1) {
        HAL_GPIO_WritePin(FRIN1_GPIO_Port, FRIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FRIN2_GPIO_Port, FRIN2_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(FRIN1_GPIO_Port, FRIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FRIN2_GPIO_Port, FRIN2_Pin, GPIO_PIN_RESET);
    }
    if (chassis->motor[motor_BL].direction == 1) {
        HAL_GPIO_WritePin(BLIN1_GPIO_Port, BLIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BLIN2_GPIO_Port, BLIN2_Pin, GPIO_PIN_SET);
    } else if (chassis->motor[motor_BL].direction == -1) {
        HAL_GPIO_WritePin(BLIN1_GPIO_Port, BLIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BLIN2_GPIO_Port, BLIN2_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(BLIN1_GPIO_Port, BLIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BLIN2_GPIO_Port, BLIN2_Pin, GPIO_PIN_RESET);
    }
    if (chassis->motor[motor_BR].direction == 1) {
        HAL_GPIO_WritePin(BRIN1_GPIO_Port, BRIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRIN2_GPIO_Port, BRIN2_Pin, GPIO_PIN_SET);
    } else if (chassis->motor[motor_BR].direction == -1) {
        HAL_GPIO_WritePin(BRIN1_GPIO_Port, BRIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRIN2_GPIO_Port, BRIN2_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(BRIN1_GPIO_Port, BRIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRIN2_GPIO_Port, BRIN2_Pin, GPIO_PIN_RESET);
    }
}
