//
// Created by 58286 on 2023/7/31.
//
#include "Gimbal.h"
#include "main.h"
#include "tim.h"

#define PWM_MAX_ROTATION_COUNTER (2.5f/20.0f*(float)GIMBAL_PWM_MAX_COUNTER)
#define PWM_MIN_ROTATION_COUNTER (0.5f/20.0f*(float)GIMBAL_PWM_MAX_COUNTER)
#define PWM_ROTATION_DIFFERENCE (PWM_MAX_ROTATION_COUNTER-PWM_MIN_ROTATION_COUNTER)
#define TRANSFER_COEFFICIENT (PWM_ROTATION_DIFFERENCE/180.0f)
#define TIME_STEP 0.001f

Gimbal_t gimbal;

inline static short Sign(float x)
{
  if(x>0) return 1;
  else if(x<0) return -1;
  return 0;
}

inline static float ABS(float x)
{
  return (x>0)?x:-x;
}

inline static unsigned short GimbalDegree2Counter(float degree)
{
  return (unsigned short)(degree*TRANSFER_COEFFICIENT+PWM_MIN_ROTATION_COUNTER);
}

static void GimbalSetYawDegree(float degree)
{
  if(degree>GIMBAL_YAW_MAX_DEGREE) degree = GIMBAL_YAW_MAX_DEGREE;
  if(degree<GIMBAL_YAW_MIN_DEGREE) degree = GIMBAL_YAW_MIN_DEGREE;
  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1, GimbalDegree2Counter(degree));
}

static void GimbalSetPitchDegree(float degree)
{
  if(degree>GIMBAL_PITCH_MAX_DEGREE) degree = GIMBAL_PITCH_MAX_DEGREE;
  if(degree<GIMBAL_PITCH_MIN_DEGREE) degree = GIMBAL_PITCH_MIN_DEGREE;
  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2, GimbalDegree2Counter(degree));
}

void GimbalInit(float initial_yaw_angle,float initial_pitch_angle)
{
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  gimbal.is_done = TRUE;
  gimbal.angle.yaw = initial_yaw_angle;
  gimbal.angle.pitch = initial_pitch_angle;
  gimbal.target_angle.yaw = initial_yaw_angle;
  gimbal.target_angle.pitch = initial_pitch_angle;
  GimbalSetYawDegree(gimbal.angle.yaw);
  GimbalSetPitchDegree(gimbal.angle.pitch);
}

void GimbalSetTarget(float yaw,float pitch)
{
  if(yaw>GIMBAL_YAW_MAX_DEGREE) yaw = GIMBAL_YAW_MAX_DEGREE;
  if(yaw<GIMBAL_YAW_MIN_DEGREE) yaw = GIMBAL_YAW_MIN_DEGREE;
  if(pitch>GIMBAL_PITCH_MAX_DEGREE) pitch = GIMBAL_PITCH_MAX_DEGREE;
  if(pitch<GIMBAL_PITCH_MIN_DEGREE) pitch = GIMBAL_PITCH_MIN_DEGREE;
  gimbal.target_angle.yaw = yaw;
  gimbal.target_angle.pitch = pitch;
  gimbal.is_done = FALSE;
}

void GimbalSetYawTarget(float yaw)
{

  if(yaw>GIMBAL_YAW_MAX_DEGREE) yaw = GIMBAL_YAW_MAX_DEGREE;
  if(yaw<GIMBAL_YAW_MIN_DEGREE) yaw = GIMBAL_YAW_MIN_DEGREE;
  gimbal.target_angle.yaw = yaw;
  gimbal.is_done = FALSE;
}

void GimbalSetPitchTarget(float pitch)
{
  if(pitch>GIMBAL_PITCH_MAX_DEGREE) pitch = GIMBAL_PITCH_MAX_DEGREE;
  if(pitch<GIMBAL_PITCH_MIN_DEGREE) pitch = GIMBAL_PITCH_MIN_DEGREE;
  gimbal.target_angle.pitch = pitch;
  gimbal.is_done = FALSE;
}



unsigned char GimbalIsMovementDone()
{
  return gimbal.is_done;
}

void GimbalUpdate()
{
  gimbal.direction.yaw = Sign(gimbal.target_angle.yaw - gimbal.angle.yaw);
  gimbal.direction.pitch = Sign(gimbal.target_angle.pitch - gimbal.angle.pitch);

  gimbal.angle.yaw += gimbal.direction.yaw*GIMBAL_MAX_VELOCITY*TIME_STEP;
  gimbal.angle.pitch += gimbal.direction.pitch*GIMBAL_MAX_VELOCITY*TIME_STEP;

  if(Sign(gimbal.target_angle.yaw - gimbal.angle.yaw) != gimbal.direction.yaw){
    gimbal.angle.yaw = gimbal.target_angle.yaw;
    gimbal.is_done = TRUE;
  }
  if(Sign(gimbal.target_angle.pitch - gimbal.angle.pitch) != gimbal.direction.pitch){
    gimbal.angle.pitch = gimbal.target_angle.pitch;
    gimbal.is_done = TRUE;
  }
  GimbalSetYawDegree(gimbal.angle.yaw);
  GimbalSetPitchDegree(gimbal.angle.pitch);
}
