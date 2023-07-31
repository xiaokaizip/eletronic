//
// Created by 58286 on 2022/1/21.
//

#include "OLED.h"
#include "i2c.h"
#include "gpio.h"

#include "oled.h"
#include "font.h"
#include "bmp.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"

#define OLED_ADD 0x78 //设备地址
/*---------------------------------*/

/*---------------------------------*/
static void OLED_SendCommand(uint8_t i2c_cmd) //发送指令
{
  uint8_t *cmd;
  cmd = &i2c_cmd;
  HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0X00,
                    I2C_MEMADD_SIZE_8BIT, cmd, 1,
                    100);
}

static void OLED_SendData(uint8_t i2c_data) //发送数据
{
  uint8_t *data;
  data = &i2c_data;
  HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0X40,
                    I2C_MEMADD_SIZE_8BIT, data, 1,
                    100);
}

static void OLED_SetOrigin(uint8_t x,uint8_t y) //设置起始点位置
{
  OLED_SendCommand(0xb0+y);
  OLED_SendCommand(((x&0xf0)>>4)|0x10);
  OLED_SendCommand((x&0x0f)|0x01);
}

void OLED_Full(uint8_t data) //全屏填充
{
  uint8_t i,n;
  for(i=0;i<8;i++)
  {
    OLED_SendCommand(0xb0+i);
    OLED_SendCommand(0x00);
    OLED_SendCommand(0x10);
    for(n=0;n<128;n++)
    {
      OLED_SendData(data);
    }
  }
}
void OLED_Clear() //清屏
{
  OLED_Full(0x00);
}
void OLED_Init(void) //初始化
{
  HAL_Delay(100);

  OLED_SendCommand(0xAE);//--display off
  OLED_SendCommand(0x20);//---set low column address
  OLED_SendCommand(0x10);//---set high column address
  OLED_SendCommand(0xb0);//--set start line address
  OLED_SendCommand(0xc8); // contract control
  OLED_SendCommand(0x00);//--128
  OLED_SendCommand(0x10);//set segment remap
  OLED_SendCommand(0x40);//Com scan direction
  OLED_SendCommand(0x81);//--normal / reverse
  OLED_SendCommand(0xff);//--set multiplex ratio(1 to 64)
  OLED_SendCommand(0xa1);//--1/32 duty
  OLED_SendCommand(0xa6);//-set display offset
  OLED_SendCommand(0xa8);//
  OLED_SendCommand(0x3F);//set osc division
  OLED_SendCommand(0xa4);
  OLED_SendCommand(0xd3);//Set Pre-Charge Period
  OLED_SendCommand(0x00);//
  OLED_SendCommand(0xd5);//set com pin configuartion
  OLED_SendCommand(0xf0);//
  OLED_SendCommand(0xd9);//set Vcomh
  OLED_SendCommand(0x22);//
  OLED_SendCommand(0xda);
  OLED_SendCommand(0x12);
  OLED_SendCommand(0xdb);//set charge pump enable
  OLED_SendCommand(0x20);//
  OLED_SendCommand(0x8d);
  OLED_SendCommand(0x14);
  OLED_SendCommand(0xaf);//--turn on oled panel

  OLED_Full(0x00);
}

void OLED_Printf(uint8_t x,uint8_t y,const char *fmt,...)
{
  char LCD_BUF[32] = {0};
  char n=0;
  va_list ap;
  memset(LCD_BUF, '\0', sizeof(LCD_BUF));
  va_start(ap, fmt);
  vsprintf(LCD_BUF, fmt, ap);
  va_end(ap);
  while (LCD_BUF[n] != '\0') {
    OLED_ShowChar(x,y,LCD_BUF[n],1);
//    LCD_SendChar(row, column, LCD_BUF[n], dcolor, bgcolor);
//    row += 6;
    n++;
  }
}

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size) //单字节
{
  unsigned char c=0,i=0;
  c=chr-' ';//得到偏移后的值
  if(x>128-1){x=0;y=y+2;}
  if(Char_Size == 16)
  {
    OLED_SetOrigin(x,y);
    for(i=0;i<8;i++)
      OLED_SendData(F8X16[c*16+i]);
    OLED_SetOrigin(x,y+1);
    for(i=0;i<8;i++)
      OLED_SendData(F8X16[c*16+i+8]);
  }
  else {
    OLED_SetOrigin(x,y);
    for(i=0;i<6;i++)
      OLED_SendData(F6x8[c][i]);

  }
}

void OLED_ShowString(uint8_t x, uint8_t y, char ch[], uint8_t TextSize) //输出字符串
{
  uint8_t c = 0,i = 0,j = 0;
  switch(TextSize)
  {
    case 1:
    {
      while(ch[j] != '\0')
      {
        c = ch[j] - 32;
        if(x > 126)
        {
          x = 0;
          y++;
        }
        OLED_SetOrigin(x, y);
        for(i = 0;i < 6; i++)
          OLED_SendData(F6x8[c][i]);
        x += 6;
        j++;
      }
    }break;
    case 2:
    {
      while(ch[j] != '\0')
      {
        c = ch[j] - 32;
        if(x > 120)
        {
          x = 0;
          y++;
        }
        OLED_SetOrigin(x,y);
        for(i=0;i<8;i++)
          OLED_SendData(F8X16[c*16+i]);
        OLED_SetOrigin(x, y+1);
        for(i=0;i<8;i++)
          OLED_SendData(F8X16[c*16+i+8]);
        x += 8;
        j++;
      }
    }break;
  }
}