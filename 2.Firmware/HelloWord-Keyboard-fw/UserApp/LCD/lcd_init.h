#ifndef __LCD_INIT_H
#define __LCD_INIT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common_inc.h"
#define USE_HORIZONTAL 0


#define LCD_W 240
#define LCD_H 240



//-----------------LCD Ports definition----------------

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(TFT_SCL_GPIO_Port,TFT_SCL_Pin,GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(TFT_SCL_GPIO_Port,TFT_SCL_Pin,GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(TFT_SDA_GPIO_Port,TFT_SDA_Pin,GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(TFT_SDA_GPIO_Port,TFT_SDA_Pin,GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(TFT_RES_GPIO_Port,TFT_RES_Pin,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(TFT_RES_GPIO_Port,TFT_RES_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(TFT_DC_GPIO_Port,TFT_DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(TFT_DC_GPIO_Port,TFT_DC_Pin,GPIO_PIN_SET)
 		     
// #define LCD_CS_Clr()   GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
// #define LCD_CS_Set()   GPIO_SetBits(GPIOA,GPIO_Pin_4)
//
// #define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_5)//BLK
// #define LCD_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_5)




void LCD_GPIO_Init(void);
void LCD_Writ_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_Init(void);

#ifdef __cplusplus
}
#endif
#endif




