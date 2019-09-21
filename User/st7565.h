#ifndef __LCD_H
#define __LCD_H
#include "common.h"

#define LCD_CS PBout(12)
#define LCD_A0 PCout(6)

void LCD_Cls();
void LCD_Data(u8 data);
void LCD_SetPos(u8 x, u8 y);
void LCD_Init();

#endif