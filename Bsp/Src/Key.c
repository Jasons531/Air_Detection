/*
**************************************************************************************************************
*	@file		KEY.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		KEY处理驱动
***************************************************************************************************************
*/ 
#include "key.h"

#define KEY_ON			   1
#define KEY_OFF			   0

/**
  * 函数功能: 按键初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void vKeyInit(void)
{
	/* 定义IO硬件初始化结构体变量 */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* 使能(开启)KEY引脚对应IO端口时钟 */  
	__HAL_RCC_GPIOA_CLK_ENABLE(); 

	/* 配置KEY2 GPIO:中断模式，下降沿触发 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // 特别注意这里要使用中断模式,下拉，上升沿触发
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * 函数功能: 进入待机模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void vKeyStandbyMode(void)
{	
	vTm1622Close( );
	write_cmd_100(0x00);  //关闭系统振荡器
	write_cmd_100(0x02);  //关闭LCD显示
	vUppBoradDeInit(); ///关闭时钟线
	
	__HAL_RCC_PWR_CLK_ENABLE();		
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);//关闭RTC写保护
	
	//关闭RTC相关中断，可能在RTC实验打开了
	__HAL_RTC_WAKEUPTIMER_DISABLE_IT(&hrtc,RTC_IT_WUT);
	__HAL_RTC_TIMESTAMP_DISABLE_IT(&hrtc,RTC_IT_TS);
	__HAL_RTC_ALARM_DISABLE_IT(&hrtc,RTC_IT_ALRA);

	//清除RTC相关中断标志位
	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
	__HAL_RTC_TIMESTAMP_CLEAR_FLAG(&hrtc,RTC_FLAG_TSF); 
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc,RTC_FLAG_WUTF);
	
	//	/* 清除所有唤醒标志位 */
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	
	/* 清除待机标志位 */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	__HAL_RCC_CLEAR_RESET_FLAGS();	
	__HAL_RCC_BACKUPRESET_RELEASE();                    //备份区域复位结束
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);     //使能RTC写保护

	/* 使能唤醒引脚：PA0做为系统唤醒输入 */
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	/* 进入待机模式 */
	HAL_PWR_EnterSTANDBYMode();	
}

extern bool bUserAppSleep;
extern uint32_t ulCurrentSleepTime;
extern uint32_t ulUppGetCurrentSleepRtc(void);
extern void vUappBoradInit( );

bool bRtcSleepMode = false;

/**
  * 函数功能: 用于检测按键是否被长时间按下
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：1 ：按键被长时间按下  0 ：按键没有被长时间按下
  */
bool bKeyStandbyCheckPwrkey(void)
{			
	uint8_t downCnt =0;																//记录按下的次数
	uint8_t upCnt =0;																//记录松开的次数			
    
	while(1)																		//死循环，由return结束
	{			
		if(bUserAppSleep)
		{
			bRtcSleepMode = true;
			vUappBoradInit( );
			ulCurrentSleepTime = ulUppGetCurrentSleepRtc(  );
			HAL_GPIO_WritePin(GPIOB, LCD_BACK_Pin, GPIO_PIN_SET);
			write_cmd_100(0x01);  //打开系统振荡器
			write_cmd_100(0x03);  //开启LCD显示
			write_cmd_100(0x18);  //片内RC 32KHZ
			write_cmd_100(0xe3);  //正常模式
		}		
		HAL_Delay(20);																//延迟一段时间再检测
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET)			//检测到按下按键
		{		
			downCnt++;																//记录按下次数			
			upCnt=0;																//清除按键释放记录
			if(downCnt>=50)														//按下时间足够
			{
				DEBUG_APP(2,"长按电源按钮");
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET); ///等待按键释放
				return true; 															//检测到按键被时间长按下
			}
		}
		else 
		{
			upCnt++; 																//记录释放次数
			if(upCnt>5)																//连续检测到释放超过5次
			{
				DEBUG_APP(2,"按下时间不足");	
				HAL_GPIO_WritePin(GPIOB, LCD_BACK_Pin, GPIO_PIN_SET);
				write_cmd_100(0x01);  //打开系统振荡器
				write_cmd_100(0x03);  //开启LCD显示
				write_cmd_100(0x18);  //片内RC 32KHZ
				write_cmd_100(0xe3);  //正常模式
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET); ///等待按键释放
				return false;									
										//按下时间太短，不是按键长按操作
			}
		}		
	}
}

/**
  * 函数功能: 工作状态跳转
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void vKeyWorkStatusJudgment(void)
{		
	 /* 检测系统是否是从待机模式启动的 */ 	
	if(bKeyStandbyCheckPwrkey())  ///读IO信息，无触发中断
	{
		printf("开机\r\n");		
		HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);	//此处的中断优先级不能比系统时钟的优先级高，否则按键消抖就用不了了
		HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);			//中断使用
	}
	else 
	{	
        DEBUG(2,"系统开机休眠\r\n");
        vKeyStandbyMode( );
	}
}


/**
  * @brief  EXTI callback
  * @param  EXTI : EXTI handle
  * @retval None
  * @brief	
  */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
	DEBUG_APP(2,"%s",__func__);
	
	 if(GPIO_Pin==GPIO_PIN_0)
	{
		if(bKeyStandbyCheckPwrkey())
		{
			DEBUG(2,"关机 \r\n");
			for(uint8_t ucTime = 0; ucTime < 3; ++ucTime)
			{
				uint16_t pwmVal=0;   //PWM占空比  
				while (pwmVal< 150)
				{
				  pwmVal++;
				  __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, pwmVal);    //修改比较值，修改占空比
				  HAL_Delay(1);
				}
				while (pwmVal)
				{
				  pwmVal--;
				  __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, pwmVal);    //修改比较值，修改占空比
				  HAL_Delay(1);
				}
			}
			vKeyStandbyMode();
		}			 
	}										
}
