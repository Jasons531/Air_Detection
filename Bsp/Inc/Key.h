/*
**************************************************************************************************************
*	@file		KEY.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		按键处理文件
***************************************************************************************************************
*/ 

#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
	extern "C" {
#endif
			
#include "main.h"
#include "rtc.h"
#include "debug.h"

extern bool bRtcSleepMode;
		
void vKeyInit(void);

void vKeyStandbyMode(void);
		
bool bKeyStandbyCheckPwrkey(void);

void vKeyWorkStatusJudgment(void);

#ifdef __cplusplus
}
#endif

#endif

