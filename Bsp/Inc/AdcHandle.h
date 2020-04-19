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

#define MQ7_5V_Pin			MQ7_CTL_Pin 
#define MQ7_5V_GPIO_Port	MQ7_CTL_GPIO_Port 

#define MQ7_1V5_Pin			MQ7_EV_Pin 
#define MQ7_1V5_GPIO_Port	MQ7_EV_GPIO_Port 
		
/***** 5V电源加热，先开启 *****/
#define	MQ7_5V_EN 			HAL_GPIO_WritePin(MQ7_5V_GPIO_Port, MQ7_5V_Pin, GPIO_PIN_SET)
#define	MQ7_5V_DIS 			HAL_GPIO_WritePin(MQ7_5V_GPIO_Port, MQ7_5V_Pin, GPIO_PIN_RESET)
		
/***** 5V电源加热，先开启 *****/
#define	MQ7_1V5_EN 			HAL_GPIO_WritePin(MQ7_1V5_GPIO_Port, MQ7_1V5_Pin, GPIO_PIN_SET)
#define	MQ7_1V5_DIS 		HAL_GPIO_WritePin(MQ7_1V5_GPIO_Port, MQ7_1V5_Pin, GPIO_PIN_RESET)
		
#define MQ_5V_HEAT_TIME		60 * 1000 ///1min
#define MQ_1V5_HEAT_TIME	30 * 1000 ///1min
		
extern uint32_t ulMQ5vHeatTime;
extern uint32_t ulMQ1v5HeatTime;
extern bool bMQHeatStart;
extern bool bMQHeatDone;

uint8_t ucAdcBattery(void);
uint8_t ucAdcSM160(void);
float fAdcMQ(void);

#ifdef __cplusplus
}
#endif

#endif

