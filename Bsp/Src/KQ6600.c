/*
**************************************************************************************************************
*	@file		KQ6600.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		KQ6600��ȩ����
***************************************************************************************************************
*/ 
#include "KQ6600.h"

xKQUart_t xKQUart = {0, {0}};

/**
  * �������ܣ���ʼ��KQ6600
  * �����������
  * �� �� ֵ����
  * ˵    ����
  */
void vKQInit(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&usartKQ6600);
	__HAL_UART_ENABLE_IT(&usartKQ6600,UART_IT_IDLE);	
	
	HAL_UART_AbortReceive_IT(&usartKQ6600);
	HAL_UART_Receive_IT(&usartKQ6600, xKQUart.ucRxBuf, RX_MAX_LEN );
	vKQWakeup( );
}

/**
  * �������ܣ�����KQģ������
  * �����������
  * �� �� ֵ����
  * ˵    �������豸�ػ�����ʱʹ�ã�F������3S�������ߣ�������Ҫ����Ԥ��
  */
void vKQSleep(void)
{
	HAL_GPIO_WritePin(GPIOB, KQ6600_F_Pin, GPIO_PIN_RESET);
	HAL_Delay(3000);
}

/**
  * �������ܣ�����KQģ�黽��
  * �����������
  * �� �� ֵ����
  * ˵    �������Ѻ���Ҫ����Ԥ��
  */
void vKQWakeup(void)
{
	HAL_GPIO_WritePin(GPIOB, KQ6600_F_Pin, GPIO_PIN_SET);
}

/**
  * �������ܣ���ȡ����������
  * ������������������ݻ���
  * �� �� ֵ����
  * ˵    ����
  */
void vKQReadSensor(uint8_t ucKQBuf[2])
{
	uint8_t ucCrc = 0;
	
	if(xKQUart.ucLen>1)
	{
		DEBUG(2,"KQRecv: ");
		for(uint8_t ucI = 0; ucI < xKQUart.ucLen; ++ucI)
		{
			DEBUG(2,"%02X ",xKQUart.ucSensorBuf[ucI]);
		}
		DEBUG(2,"\r\n");
		for(uint8_t ucI = 0; ucI < xKQUart.ucLen-1; ++ucI)
		{
			ucCrc += xKQUart.ucSensorBuf[ucI];
		}
		if(ucCrc == xKQUart.ucSensorBuf[xKQUart.ucLen-1])
		{
			memcpy(ucKQBuf, &xKQUart.ucSensorBuf[1], 2);
		}
	}
}

