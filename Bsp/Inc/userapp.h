/*
**************************************************************************************************************
*	@file		userapp.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		用户层头文件
***************************************************************************************************************
*/ 

#ifndef __USERAPP_H
#define __USERAPP_H

#ifdef __cplusplus
	extern "C" {
#endif
	
#include <stdint.h>	
#include <math.h>	
#include "main.h"		
//#include "debug.h"
#include "rtc.h"
#include "usart.h"
#include "KQ6600.h"
#include "PMC7003.h"
#include "AdcHandle.h"
#include "Aht10.h"
#include "shuma.h"
	
typedef struct
{
	uint8_t ucKqBuf[4];
}USERAPPSENSOR_t;
	
extern USERAPPSENSOR_t xUserappSensor;

extern uint32_t ulCurrentSleepTime;

void vUappSensorInit(void);

void vUapp_IdleCpltCallback( UART_HandleTypeDef * huart );

void vUppGetBatDisplay(void);

void vUppGetCODisplay(void);

void vUppGetHCHODisplay(uint8_t *ucKQBuf);

void vUppGetAhtDisplay(int8_t *ctBuf);

void vUppGetPMS7003Display(void);

void vUppSensorDisplay(void);

uint32_t ulUppGetCurrentSleepRtc(void);

void vUppRtcAlarm(uint16_t ulTime);

void vUppIntoLowPower(void);

void vUappBoradInit(void);


#ifdef __cplusplus
}
#endif

#endif

