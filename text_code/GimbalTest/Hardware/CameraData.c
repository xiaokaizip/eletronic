//
// Created by 58286 on 2023/7/31.
//

#include "CameraData.h"
#include "main.h"
#include "usart.h"
#include "Gimbal.h"

Camera_t camera;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
  CameraDaraReceiveCallback();
  HAL_UART_Receive_IT(&huart1,camera.camera_data,1);
}

void CameraInit()
{
  HAL_UART_Receive_IT(&huart1,camera.camera_data,1);
  camera.target_position.x = 120;
  camera.target_position.y = 160;
  camera.position.y = 160;
  camera.position.x = 120;
  camera.last_error.x = 0;
  camera.last_error.y = 0;
  camera.sum.x = 0;
  camera.sum.y = 0;
}



Position_t PosAdd(Position_t pos1,Position_t pos2)
{
  Position_t a;
  a.x = pos1.x+pos2.x;
  a.y = pos1.y+pos2.y;
  return a;
}

Position_t PosAdd3(Position_t pos1,Position_t pos2,Position_t pos3)
{
  Position_t a = PosAdd(pos1,pos2);
  return PosAdd(a,pos3);
}

Position_t PosSub(Position_t pos1,Position_t pos2)
{
  Position_t a;
  a.x = pos1.x-pos2.x;
  a.y = pos1.y-pos2.y;
  return a;
}

Position_t PosMul(float num,Position_t pos2)
{
  Position_t a;
  a.x = num*pos2.x;
  a.y = num*pos2.y;
  return a;
}

extern Gimbal_t gimbal;

void CameraPID()
{
  short sum_max = 50;
  Position_t set_value;
  camera.error.x = (camera.target_position.x-camera.position.x);
  camera.sum.x += camera.error.x;
  if(camera.sum.x > sum_max) camera.sum.x = sum_max;
  if(camera.sum.x < -sum_max) camera.sum.x = -sum_max;
  set_value.x = 0.1f *camera.error.x +
                0.0f*(camera.error.x - camera.last_error.x) +
                0.0f * camera.sum.x;
//  set_value.x = 0.1f *camera.error.x;
  GimbalSetYawTarget(gimbal.angle.yaw - set_value.x);

//  camera.error.y = (camera.target_position.y-camera.position.y);
//  camera.sum.y += camera.error.y;
//  set_value.y = 0.08f *camera.error.y +
//                0.0f*(camera.error.y - camera.last_error.y) +
//                0.0f * camera.sum.y;
//  GimbalSetPitchTarget(gimbal.angle.pitch - set_value.y);
  camera.last_error = camera.error;
}

uint8_t unpack_state = 0;
uint8_t temp = 0;
uint8_t temp_counter = 0;
#define toNum(x) ((x)-0x30)
/// |AA|cmd|x0|x1|x2|x3|AA|
void CameraDaraReceiveCallback()
{
  uint8_t i=0;
  for(i=0;i<1;i++){
    if(camera.camera_data[0] == 'a'){
      unpack_state = 1;
      break;
    }
    switch(unpack_state){
    case 1:
      if(camera.camera_data[0] == ' '){
        unpack_state = 2;
        camera.position.x = temp;
        temp = 0;
        break;
      }else if(camera.camera_data[0] < '0' || camera.camera_data[0] > '9'){
        unpack_state = 0;
        temp = 0;
        break;
      }else{
        temp *= 10;
        temp += toNum(camera.camera_data[0]);
      }

      break;
    case 2:
      if(camera.camera_data[0] == 'k'){
        unpack_state = 0;
        camera.position.y = temp;
        temp = 0;
        CameraPID();

        break;
      }else if(camera.camera_data[0] < '0' || camera.camera_data[0] > '9'){
        unpack_state = 0;
        temp = 0;
        break;
      }else{
        temp *= 10;
        temp += toNum(camera.camera_data[0]);
      }
      break;
    default:
      unpack_state = 0;
      temp = 0;
      break;
    }
  }

}

