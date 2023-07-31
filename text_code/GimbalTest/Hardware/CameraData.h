//
// Created by 58286 on 2023/7/31.
//

#ifndef GIMBALTEST_HARDWARE_CAMERADATA_H_
#define GIMBALTEST_HARDWARE_CAMERADATA_H_


typedef struct {
  float x;
  float y;
}Position_t;

typedef struct {
  Position_t target_position;
  Position_t position;
  Position_t last_error;
  Position_t error;
  Position_t sum;
  unsigned char camera_data[8];
}Camera_t;

void CameraInit();
void CameraDaraReceiveCallback();
void CameraPID();

#endif // GIMBALTEST_HARDWARE_CAMERADATA_H_
