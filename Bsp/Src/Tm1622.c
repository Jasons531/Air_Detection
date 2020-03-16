/*
**************************************************************************************************************
*	@file		Tm1622.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		ADC��������
***************************************************************************************************************
*/ 
#include "Tm1622.h"

/********************������ƶ˿�**********************/


/***************TM1622��ʼ������: LCD_CS_Pin�����豸Ĭ������**************/
/****** PIN9  ʵ�ʶ�ӦSEG22,��ĻSEG0~SEG22
COM1~COM4 ��Ӧһ����ַ
COM5~COM8 ��Ӧһ����ַ

��ϵ�ܵ�SEG=23 ����22*8=184
ÿ4�����Ӧ1����ַ���ܵ�ַΪ46����Χ0~45
��Ļԭ��ͼ��PIN9��ӦSEG22,��ַ44��45
****/
void init_tm1622(void)
{
	HAL_GPIO_WritePin(GPIOB, LCD_BACK_Pin, GPIO_PIN_SET);
	write_cmd_100(0x01);  //��ϵͳ����
	write_cmd_100(0x03);  //����LCD��ʾ
	write_cmd_100(0x18);  //Ƭ��RC 32KHZ
	write_cmd_100(0xe3);  //����ģʽ

	/**** SEG0~SEG22 = 23*8=184�����Ӧ46����ַ �ߵ�ƽ���� ****/
	for(uint8_t uci=0; uci<46; ++uci)					//��46����ַ����
	{
		write_101(uci,0x0);	
	}
}

/**
  * ��������: ��ȡ��ص���
  * �������: ��
  * �� �� ֵ: �����ٷֱ�
  * ˵    ��: ���裺30/42
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

/***************д101���ݺ���**************/
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
