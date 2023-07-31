//
// Created by 58286 on 2023/7/31.
//

#ifndef GIMBALTEST_HARDWARE_GIMBAL_H_
#define GIMBALTEST_HARDWARE_GIMBAL_H_
/*
 * 0.1s/60 degree
 * max motor speed: 600 degree/s
 * */
#define GIMBAL_MAX_VELOCITY 50.0f

#define GIMBAL_PWM_MAX_COUNTER 2879
#define GIMBAL_YAW_MAX_DEGREE 150
#define GIMBAL_YAW_MIN_DEGREE 30
#define GIMBAL_PITCH_MAX_DEGREE 100
#define GIMBAL_PITCH_MIN_DEGREE 50

typedef struct {
  float yaw;
  float pitch;
}GimbalAngle_t;

typedef struct {
  float yaw;
  float pitch;
}GimbalVelocity_t;

typedef struct {
  float yaw;
  float pitch;
}GimbalAcceleration_t;

typedef struct {
  short yaw;
  short pitch;
}GimbalDirection_t;


typedef struct {
  float yaw;
  float pitch;
}GimbalTargetAngle_t;

typedef struct {
  GimbalVelocity_t velocity;
  GimbalAcceleration_t acceleration;
  GimbalAngle_t angle;
  GimbalDirection_t direction;
  GimbalTargetAngle_t target_angle;
  unsigned char is_done;
}Gimbal_t;

void GimbalInit(float initial_yaw_angle,float initial_pitch_angle);
void GimbalSetTarget(float yaw,float pitch);
void GimbalSetYawTarget(float yaw);
void GimbalSetPitchTarget(float pitch);
unsigned char GimbalIsMovementDone();

//Put it into 1ms TIM interrupt
void GimbalUpdate();


#endif // GIMBALTEST_HARDWARE_GIMBAL_H_
