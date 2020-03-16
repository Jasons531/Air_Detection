/*
**************************************************************************************************************
*	@file		Tm1622.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		ADC处理驱动
***************************************************************************************************************
*/ 
#include "Tm1622.h"

/********************定义控制端口**********************/


/***************TM1622初始化函数: LCD_CS_Pin必须设备默认拉高**************/
/****** PIN9  实际对应SEG22,屏幕SEG0~SEG22
COM1~COM4 对应一个地址
COM5~COM8 对应一个地址

关系总的SEG=23 点数22*8=184
每4个点对应1个地址则总地址为46，范围0~45
屏幕原理图中PIN9对应SEG22,地址44、45
****/
void init_tm1622(void)
{
	HAL_GPIO_WritePin(GPIOB, LCD_BACK_Pin, GPIO_PIN_SET);
	write_cmd_100(0x01);  //打开系统振荡器
	write_cmd_100(0x03);  //开启LCD显示
	write_cmd_100(0x18);  //片内RC 32KHZ
	write_cmd_100(0xe3);  //正常模式

	/**** SEG0~SEG22 = 23*8=184个点对应46个地址 高电平点亮 ****/
	for(uint8_t uci=0; uci<46; ++uci)					//将46个地址清零
	{
		write_101(uci,0x0);	
	}
}

/**
  * 函数功能: 读取电池电量
  * 输入参数: 无
  * 返 回 值: 电量百分比
  * 说    明: 电阻：30/42
  */
void write_cmd_100(unsigned char cmd)
{
  unsigned char i;

  HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_SET);
  vTmNop();
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();

  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
  vTmNop();
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();

  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
  vTmNop();

  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();

  for(i=0; i<8; i++)
  {
    HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
	if(cmd&0x80)
	  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_SET);
	 else
	   HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
	 cmd<<=1;
  }

  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
  
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
  vTmNop();
  
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();
  
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
  
  HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET);
}

/***************写101数据函数**************/
void write_101(unsigned char address, unsigned char dat)
{
  
  unsigned char i, add;
  add=address<<2;

  HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
	
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_SET);
  vTmNop();
	
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();

  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();
	
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
  vTmNop();
	
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();


  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  vTmNop();

  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_SET);
  vTmNop();

  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
  vTmNop();


  for(i=0; i<6; i++)
  {
    HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
	if(add&0x80)
	  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_SET);
	 else
	   HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
	 add<<=1;
  } 

  for(i=0; i<4; i++)
  {
    HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
	if(dat&0x01)
	  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_SET);
	 else
	   HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_SET);
	 dat>>=1;
  }
  HAL_GPIO_WritePin(GPIOA, LCD_WR_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, LCD_DATA_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET);
}


void vTmNop(void)
{
	for(uint16_t uci = 0; uci < 12; ++uci)
	{
		__nop();
	}
}
