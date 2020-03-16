/*
**************************************************************************************************************
*	@file		Aht10.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		空气温湿度
***************************************************************************************************************
*/ 
#include "Aht10.h"

/**** I2C地址 ****/
#define aht10ADDR     0x38<<1 
/**** 配置寄存器 ****/
#define aht10CONFIG   0XE1
/**** 开启测量 ****/
#define aht10MEASURE  0XAC
/**** 软件复位****/
#define aht10RESET    0XBA

/**
  * 函数功能：复位
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
void vAhtReset(void)
{
	uint8_t ucAhtCmd = 0xBA;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)aht10ADDR,&ucAhtCmd,1,0xFFFF);
}

/**
  * 函数功能：休眠
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
void vAhtSleep(void)
{
	uint8_t ucAhtCmd = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)aht10ADDR,&ucAhtCmd,1,0xFFFF);
}

/**
  * 函数功能：写系统配置寄存器
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
void vAhtConfig(void)
{
	HAL_StatusTypeDef halstatus;
	uint8_t ucAhtCmd[3] = {0xE1, 0x08, 0x00};
	halstatus = HAL_I2C_Master_Transmit(&hi2c1,aht10ADDR, ucAhtCmd,3,0xFFFFFFFF);
	DEBUG_APP(3,"halstatus = %02x",halstatus);
}

/**
  * 函数功能：测量
  * 输入参数：无
  * 返 回 值：无
  * 说    明：启动空气温湿度测量
  */
void vAhtMeasure(void)
{
	uint8_t ucAhtCmd[3] = {0XAC, 0x33, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)aht10ADDR, ucAhtCmd,3,0xFFFF);
}

/**
  * 函数功能：获取AHT10状态
  * 输入参数：无
  * 返 回 值：AHT10状态 
  * 说    明：
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
  * 函数功能：查询cal enable位有没有使能？
  * 输入参数：无
  * 返 回 值：cal enable位状态
  * 说    明：
  */
bool bAhtReadCalEnable(void)
{
	uint8_t ucCal;
	ucCal = ucAhtReadStatus();	
	if((ucCal & 0x68)==0x08)  //判断NOR模式和校准输出是否有效
	{
		return 1;
	}
	else
		return 0;
}

/**
  * 函数功能：读取AHT10的温度和湿度数据
  * 输入参数：温湿度缓存
  * 返 回 值：cal enable位状态
  * 说    明：
  */
bool bAhtReadCTdata(int8_t ctBuf[2])
{
	uint32_t ulTimeOver = 0;
	uint32_t ulReadCtData = 0;
	uint8_t  ucByte[6] = {0};
	int32_t  ltBuf[2] = {0};
	
	/**** 启动测量 ****/
	vAhtMeasure( );
	/**** 等待75ms ****/
	HAL_Delay(75);
	ulTimeOver = HAL_GetTick();
	/**** 等待忙状态结束 ****/
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
	
	/****ucByte[0]：状态字 ucByte[4];高4位为湿度  低4位为温度 ****/
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
	
	/************** 温度 ****************/
	ctBuf[0] = (((ltBuf[1]*200*10)/1024/1024)-500)/10;
	/************** 湿度 ****************/
	ctBuf[1] = ((ltBuf[0]*1000)/1024/1024/10);
	
	DEBUG_APP(2, "湿度 = %d 温度 = %d℃",ltBuf[0]*1000/1024/1024,ltBuf[1]*200*10/1024/1024-500);
	DEBUG_APP(2, "湿度 = %d 温度 = %d℃",ctBuf[1],ctBuf[0]);
	return true;
}

/**
  * 函数功能：AHT10初始化
  * 输入参数：无
  * 返 回 值：初始化状态
  * 说    明：
  */
bool bAhtInit(void)
{
	uint8_t ucInitCounter = 0;
	
	DEBUG_APP(3, "aht10ADDR = 0x%02x",aht10ADDR);
	
	HAL_Delay(40);	
	vAhtConfig();
	HAL_Delay(100);
	
	/**** 需要等待状态字status的Bit[3]=1时才去读数据。如果Bit[3]不等于1 ，发软件复位0xBA给AHT10，再重新初始化AHT10，直至Bit[3]=1 ****/
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
