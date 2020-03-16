/*
**************************************************************************************************************
*	@file		AdcHandle.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		ADC处理文件
***************************************************************************************************************
*/ 

#ifndef __ADCHANLDE_H
#define __ADCHANLDE_H

#ifdef __cplusplus
	extern "C" {
#endif
			
#include "debug.h"
#include <stdint.h>	
#include "adc.h"
		
uint8_t ucAdcBattery(void);
void vSM160Heat(void);
uint8_t ucAdcSM160(void);
float fAdcMQ(void);

#ifdef __cplusplus
}
#endif

#endif

