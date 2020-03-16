/*
**************************************************************************************************************
*	@file		KQ6600.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		KQ6600甲醛驱动头文件
***************************************************************************************************************
*/ 

#ifndef __KQ6600_H
#define __KQ6600_H

#ifdef __cplusplus
	extern "C" {
#endif
			
#include "debug.h"
#include <stdint.h>	
#include "usart.h"
		
typedef UART_t xKQUart_t;
		
extern xKQUart_t xKQUart;	

void vKQInit(void);
void vKQSleep(void);
void vKQWakeup(void);
void vKQReadSensor(uint8_t ucKQBuf[2]);
		
#ifdef __cplusplus
}
#endif

#endif

