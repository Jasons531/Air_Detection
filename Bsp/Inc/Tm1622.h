/*
**************************************************************************************************************
*	@file		TM1622.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		ADC处理文件
***************************************************************************************************************
*/ 

#ifndef __TM1622_H
#define __TM1622_H

#ifdef __cplusplus
	extern "C" {
#endif
			
#include "debug.h"
#include <stdint.h>	
		
void init_tm1622(void);
		
void vTm1622Close(void);

void write_cmd_100(unsigned char cmd);
		
void write_101(unsigned char address, unsigned char dat);
		
void vTmNop(void);

#ifdef __cplusplus
}
#endif

#endif

