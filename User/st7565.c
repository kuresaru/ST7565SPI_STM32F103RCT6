#include "st7565.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

void LCD_Write(u8 data)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET)
        ;
    SPI_I2S_SendData(SPI2, data);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET)
        ;
}

void LCD_Cmd(u8 cmd)
{
    LCD_CS = 0;
    LCD_A0 = 0;
    LCD_Write(cmd);
    LCD_CS = 1;
}

void LCD_Data(u8 data)
{
    LCD_CS = 0;
    LCD_A0 = 1;
    LCD_Write(data);
    LCD_CS = 1;
}

void LCD_Cls()
{
    u8 i, j;
    for (i = 0; i < 8; i++)
    {
        LCD_SetPos(0, i);
        for (j = 0; j < 132; j++)
        {
            LCD_Data(0x00);
        }
    }
}

//x就是x  y要除以8 因为一字节是8行
void LCD_SetPos(u8 x, u8 y)
{
    LCD_Cmd(0xB0 | y);
    LCD_Cmd(0x10 | (x >> 4));
    LCD_Cmd(0x00 | (x & 0xF));
}

void LCD_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    LCD_CS = 1;

    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);

    LCD_Cmd(0x2F); //启动电源
    LCD_Cmd(0x25); //V0对比度 (0x24-0x27)
    LCD_Cmd(0xAF); //显示开
    // LCD_Cmd(0xA5);//Test mode
    // LCD_Cmd(0xA7);//反色
    LCD_Cls();

    // u8 i, j;
    // for (i = 0; i < 8; i++)
    // {
    //     LCD_Cmd(0xB0 + i);
    //     LCD_Cmd(0x10);
    //     LCD_Cmd(0x00);
    //     for (j = 0; j < 1; j++)
    //     {
    //         LCD_Data(0xFF);
    //     }
    //     // LCD_Cmd(0xEE); //stop
    // }
}