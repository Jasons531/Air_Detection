/*
**************************************************************************************************************
*	@file		AHT10.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/05/30
*	@brief		AHT10空气温湿度驱动头文件
***************************************************************************************************************
*/ 

#ifndef __AHT10_H
#define __AHT10_H

#ifdef __cplusplus
	extern "C" {
#endif
			
#include "debug.h"
#include <stdint.h>	
#include <stdbool.h>	
#include "I2C.h"

void vAhtReset(void);
void vAhtConfig(void);
void vAhtMeasure(void);
uint8_t ucAhtReadStatus(void);
bool bAhtReadCalEnable(void);
bool bAhtReadCTdata(int8_t ctBuf[2]);
bool bAhtInit(void);
		
#ifdef __cplusplus
}
#endif

#endif

