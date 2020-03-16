/*
**************************************************************************************************************
*	@file		AdcHandle.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		ADC处理驱动
***************************************************************************************************************
*/ 
#include "AdcHandle.h"

/**
  * 函数功能: 读取电池电量
  * 输入参数: 无
  * 返 回 值: 电量百分比
  * 说    明: 电阻：30/42
  */
uint8_t ucAdcBattery(void)
{
	uint16_t usAdcBuf[2] = {0};
	int16_t  cBatteryPercent = 0;
	
	usAdcBuf[0] = ulAdcReadParameter(ADC_CHANNEL_7, 10);
	float fBattery = adcVREFINT_CAL_VREF*(*adcVREFINT_CAL_ADDR)*usAdcBuf[0]; 
	usAdcBuf[1] = ulAdcReadParameter(ADC_CHANNEL_VREFINT, 10);
	float fAdcTemp = usAdcBuf[1] * adcVFULL;
	DEBUG_APP(2, "BAT = %d adc17 = %d , adc0 = %d, VBAT = %.2fV", *adcVREFINT_CAL_ADDR,  usAdcBuf[1], usAdcBuf[0], (fBattery/fAdcTemp));  	
	cBatteryPercent = (((fBattery/fAdcTemp)*1400) > 3600)?((((fBattery/fAdcTemp)*1400)- 3600)/6):0;
	DEBUG_APP(2, "fBattery = %.2f cBatteryPercent = %d",((fBattery/fAdcTemp)*1400),cBatteryPercent);
	if(cBatteryPercent >= 100)
		cBatteryPercent = 100;
	else if(cBatteryPercent <= 5)
		cBatteryPercent = 0;
	return cBatteryPercent;
}

/**
  * 函数功能: SM160预热
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void vSM160Heat(void)
{
	HAL_Delay(2000);
}

/**
  * 函数功能: 读取SM160
  * 输入参数: 无
  * 返 回 值: 电量百分比
  * 说    明: 无
  */
uint8_t ucAdcSM160(void)
{
	uint16_t usAdcBuf[2] = {0};
	int8_t  cBatteryPercent = 0;
	
	usAdcBuf[0] = ulAdcReadParameter(ADC_CHANNEL_5, 10);
	float fBattery = adcVREFINT_CAL_VREF*(*adcVREFINT_CAL_ADDR)*usAdcBuf[0]; 
	usAdcBuf[1] = ulAdcReadParameter(ADC_CHANNEL_VREFINT, 10);
	float fAdcTemp = usAdcBuf[1] * adcVFULL;
	DEBUG_APP(2, "BAT = %d adc17 = %d , adc0 = %d, VBAT = %.2fmV", *adcVREFINT_CAL_ADDR,  usAdcBuf[1], usAdcBuf[0], (fBattery/fAdcTemp)*2000);  ///100:510	
	cBatteryPercent = ((((fBattery/fAdcTemp)*2000)- 3600)/6);
	DEBUG_APP(2, "fBattery = %.2f cBatteryPercent = %d",((fBattery/fAdcTemp)*2000)- 3600,cBatteryPercent);
	if(cBatteryPercent >= 100)
		cBatteryPercent = 100;
	else if(cBatteryPercent <= 5)
		cBatteryPercent = 0;
	return cBatteryPercent;
}

/**
  * 函数功能: 读取一氧化碳
  * 输入参数: 无
  * 返 回 值: 一氧化碳VRL
  * 说    明: 无
  */

float fAdcMQ(void)
{
	uint16_t usAdcBuf[2] = {0};
	
	usAdcBuf[0] = ulAdcReadParameter(ADC_CHANNEL_6, 10);
	float fBattery = adcVREFINT_CAL_VREF*(*adcVREFINT_CAL_ADDR)*usAdcBuf[0]; 
	usAdcBuf[1] = ulAdcReadParameter(ADC_CHANNEL_VREFINT, 10);
	float fAdcTemp = usAdcBuf[1] * adcVFULL;
	DEBUG_APP(2, "BAT = %d adc17 = %d , adc0 = %d, MQ = %.2fmV", *adcVREFINT_CAL_ADDR,  usAdcBuf[1], usAdcBuf[0], (fBattery/fAdcTemp)*1000);  ///100:510	
	DEBUG_APP(2, "MQ-7 = %.2f",fBattery/fAdcTemp);

	return (fBattery/fAdcTemp);
}
