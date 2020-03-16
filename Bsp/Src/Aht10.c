/*
**************************************************************************************************************
*	@file		Aht10.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		������ʪ��
***************************************************************************************************************
*/ 
#include "Aht10.h"

/**** I2C��ַ ****/
#define aht10ADDR     0x38<<1 
/**** ���üĴ��� ****/
#define aht10CONFIG   0XE1
/**** �������� ****/
#define aht10MEASURE  0XAC
/**** �����λ****/
#define aht10RESET    0XBA

/**
  * �������ܣ���λ
  * �����������
  * �� �� ֵ����
  * ˵    ����
  */
void vAhtReset(void)
{
	uint8_t ucAhtCmd = 0xBA;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)aht10ADDR,&ucAhtCmd,1,0xFFFF);
}

/**
  * �������ܣ�����
  * �����������
  * �� �� ֵ����
  * ˵    ����
  */
void vAhtSleep(void)
{
	uint8_t ucAhtCmd = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)aht10ADDR,&ucAhtCmd,1,0xFFFF);
}

/**
  * �������ܣ�дϵͳ���üĴ���
  * �����������
  * �� �� ֵ����
  * ˵    ����
  */
void vAhtConfig(void)
{
	HAL_StatusTypeDef halstatus;
	uint8_t ucAhtCmd[3] = {0xE1, 0x08, 0x00};
	halstatus = HAL_I2C_Master_Transmit(&hi2c1,aht10ADDR, ucAhtCmd,3,0xFFFFFFFF);
	DEBUG_APP(3,"halstatus = %02x",halstatus);
}

/**
  * �������ܣ�����
  * �����������
  * �� �� ֵ����
  * ˵    ��������������ʪ�Ȳ���
  */
void vAhtMeasure(void)
{
	uint8_t ucAhtCmd[3] = {0XAC, 0x33, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)aht10ADDR, ucAhtCmd,3,0xFFFF);
}

/**
  * �������ܣ���ȡAHT10״̬
  * �����������
  * �� �� ֵ��AHT10״̬ 
  * ˵    ����
  */
uint8_t ucAhtReadStatus(void)
{
	uint8_t ucAhtStatus;;
	HAL_StatusTypeDef halstatus;
	halstatus = HAL_I2C_Master_Receive(&hi2c1,(uint16_t)((aht10ADDR)+1), &ucAhtStatus,1,0xFFFF);	
	DEBUG_APP(3,"halstatus = %02x, ucAhtStatus = %02x",halstatus,ucAhtStatus);
	return ucAhtStatus;
}

/**
  * �������ܣ���ѯcal enableλ��û��ʹ�ܣ�
  * �����������
  * �� �� ֵ��cal enableλ״̬
  * ˵    ����
  */
bool bAhtReadCalEnable(void)
{
	uint8_t ucCal;
	ucCal = ucAhtReadStatus();	
	if((ucCal & 0x68)==0x08)  //�ж�NORģʽ��У׼����Ƿ���Ч
	{
		return 1;
	}
	else
		return 0;
}

/**
  * �������ܣ���ȡAHT10���¶Ⱥ�ʪ������
  * �����������ʪ�Ȼ���
  * �� �� ֵ��cal enableλ״̬
  * ˵    ����
  */
bool bAhtReadCTdata(int8_t ctBuf[2])
{
	uint32_t ulTimeOver = 0;
	uint32_t ulReadCtData = 0;
	uint8_t  ucByte[6] = {0};
	int32_t  ltBuf[2] = {0};
	
	/**** �������� ****/
	vAhtMeasure( );
	/**** �ȴ�75ms ****/
	HAL_Delay(75);
	ulTimeOver = HAL_GetTick();
	/**** �ȴ�æ״̬���� ****/
	while(((ucAhtReadStatus()&0x80)==0x80))
	{
		HAL_Delay(1);
		if(HAL_GetTick()-ulTimeOver>150)
		{
			break;
		}
	}
	if(HAL_OK != HAL_I2C_Master_Receive(&hi2c1,(uint16_t)((aht10ADDR)+1), ucByte, 6,0xFFFFFFFF))
	{
		return false;
	}
	
	/****ucByte[0]��״̬�� ucByte[4];��4λΪʪ��  ��4λΪ�¶� ****/
	ulReadCtData = (ulReadCtData|ucByte[1])<<8;
	ulReadCtData = (ulReadCtData|ucByte[2])<<8;
	ulReadCtData = (ulReadCtData|ucByte[3]);
	ulReadCtData >>= 4;	
	ltBuf[0] = ulReadCtData;
	
	ulReadCtData = 0;
	ulReadCtData = (ulReadCtData|ucByte[3])<<8;
	ulReadCtData = (ulReadCtData|ucByte[4])<<8;
	ulReadCtData = (ulReadCtData|ucByte[5]);
	ulReadCtData &= 0xfffff;	
	ltBuf[1] = ulReadCtData;	
	
	/************** �¶� ****************/
	ctBuf[0] = (((ltBuf[1]*200*10)/1024/1024)-500)/10;
	/************** ʪ�� ****************/
	ctBuf[1] = ((ltBuf[0]*1000)/1024/1024/10);
	
	DEBUG_APP(2, "ʪ�� = %d �¶� = %d��",ltBuf[0]*1000/1024/1024,ltBuf[1]*200*10/1024/1024-500);
	DEBUG_APP(2, "ʪ�� = %d �¶� = %d��",ctBuf[1],ctBuf[0]);
	return true;
}

/**
  * �������ܣ�AHT10��ʼ��
  * �����������
  * �� �� ֵ����ʼ��״̬
  * ˵    ����
  */
bool bAhtInit(void)
{
	uint8_t ucInitCounter = 0;
	
	DEBUG_APP(3, "aht10ADDR = 0x%02x",aht10ADDR);
	
	HAL_Delay(40);	
	vAhtConfig();
	HAL_Delay(100);
	
	/**** ��Ҫ�ȴ�״̬��status��Bit[3]=1ʱ��ȥ�����ݡ����Bit[3]������1 ���������λ0xBA��AHT10�������³�ʼ��AHT10��ֱ��Bit[3]=1 ****/
	while(!bAhtReadCalEnable())
	{
		vAhtReset();
		HAL_Delay(200);
		vAhtConfig();
		ucInitCounter++;
		if(ucInitCounter>=10)
		{
			return false;
		}
		else
		{
			HAL_Delay(500);
		}
	}
	
	return true;
}
