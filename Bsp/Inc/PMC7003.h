/*
**************************************************************************************************************
*	@file		PMC7003.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		PM2.5驱动头文件
***************************************************************************************************************
*/ 

#ifndef __PMC7003_H
#define __PMC7003_H

#ifdef __cplusplus
	extern "C" {
#endif
			
#include "debug.h"
#include <stdint.h>	
#include <stdbool.h>	
#include "usart.h"
		
typedef UART_t xPMCUart_t;
		
extern xPMCUart_t xPMCUart;	

void vPMC7003Init(void);
void vPMC7003Reset(void);
bool bPMC7003Wakeup(void);
bool bPMC7003Sleep(void);
bool bPMC7003SetMode(uint8_t ucReadMode);
bool bPMC7003ReadBuf(void);
bool bPMC7003Recv(xPMCUart_t *xPMCUart);
		
#ifdef __cplusplus
}
#endif

#endif

