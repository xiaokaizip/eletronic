//
// Created by 58286 on 2022/1/21.
//

#ifndef STM32F401_TEST_CORE_INC_OLED_H_
#define STM32F401_TEST_CORE_INC_OLED_H_
#include "main.h"

//#define OLED_ADDR 0x78
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
extern unsigned char bmp1[];

void OLED_Full(uint8_t data); //全屏填充
void OLED_Init(void); //初始化
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);//单字节
void OLED_ShowString(uint8_t x, uint8_t y, char ch[], uint8_t TextSize); //输出字符串
void OLED_Printf(uint8_t x,uint8_t y,const char *fmt,...);
void OLED_Clear(); //清屏

#endif //STM32F401_TEST_CORE_INC_OLED_H_
