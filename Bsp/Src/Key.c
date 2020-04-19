/*
**************************************************************************************************************
*	@file		KEY.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		KEY��������
***************************************************************************************************************
*/ 
#include "key.h"

#define KEY_ON			   1
#define KEY_OFF			   0

/**
  * ��������: ������ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void vKeyInit(void)
{
	/* ����IOӲ����ʼ���ṹ����� */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* ʹ��(����)KEY���Ŷ�ӦIO�˿�ʱ�� */  
	__HAL_RCC_GPIOA_CLK_ENABLE(); 

	/* ����KEY2 GPIO:�ж�ģʽ���½��ش��� */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // �ر�ע������Ҫʹ���ж�ģʽ,�����������ش���
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * ��������: �������ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void vKeyStandbyMode(void)
{	
	vTm1622Close( );
	write_cmd_100(0x00);  //�ر�ϵͳ����
	write_cmd_100(0x02);  //�ر�LCD��ʾ
	vUppBoradDeInit(); ///�ر�ʱ����
	
	__HAL_RCC_PWR_CLK_ENABLE();		
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);//�ر�RTCд����
	
	//�ر�RTC����жϣ�������RTCʵ�����
	__HAL_RTC_WAKEUPTIMER_DISABLE_IT(&hrtc,RTC_IT_WUT);
	__HAL_RTC_TIMESTAMP_DISABLE_IT(&hrtc,RTC_IT_TS);
	__HAL_RTC_ALARM_DISABLE_IT(&hrtc,RTC_IT_ALRA);

	//���RTC����жϱ�־λ
	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
	__HAL_RTC_TIMESTAMP_CLEAR_FLAG(&hrtc,RTC_FLAG_TSF); 
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc,RTC_FLAG_WUTF);
	
	//	/* ������л��ѱ�־λ */
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	
	/* ���������־λ */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	__HAL_RCC_CLEAR_RESET_FLAGS();	
	__HAL_RCC_BACKUPRESET_RELEASE();                    //��������λ����
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);     //ʹ��RTCд����

	/* ʹ�ܻ������ţ�PA0��Ϊϵͳ�������� */
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	/* �������ģʽ */
	HAL_PWR_EnterSTANDBYMode();	
}

extern bool bUserAppSleep;
extern uint32_t ulCurrentSleepTime;
extern uint32_t ulUppGetCurrentSleepRtc(void);
extern void vUappBoradInit( );

bool bRtcSleepMode = false;

/**
  * ��������: ���ڼ�ⰴ���Ƿ񱻳�ʱ�䰴��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����1 ����������ʱ�䰴��  0 ������û�б���ʱ�䰴��
  */
bool bKeyStandbyCheckPwrkey(void)
{			
	uint8_t downCnt =0;																//��¼���µĴ���
	uint8_t upCnt =0;																//��¼�ɿ��Ĵ���			
    
	while(1)																		//��ѭ������return����
	{			
		if(bUserAppSleep)
		{
			bRtcSleepMode = true;
			vUappBoradInit( );
			ulCurrentSleepTime = ulUppGetCurrentSleepRtc(  );
			HAL_GPIO_WritePin(GPIOB, LCD_BACK_Pin, GPIO_PIN_SET);
			write_cmd_100(0x01);  //��ϵͳ����
			write_cmd_100(0x03);  //����LCD��ʾ
			write_cmd_100(0x18);  //Ƭ��RC 32KHZ
			write_cmd_100(0xe3);  //����ģʽ
		}		
		HAL_Delay(20);																//�ӳ�һ��ʱ���ټ��
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET)			//��⵽���°���
		{		
			downCnt++;																//��¼���´���			
			upCnt=0;																//��������ͷż�¼
			if(downCnt>=50)														//����ʱ���㹻
			{
				DEBUG_APP(2,"������Դ��ť");
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET); ///�ȴ������ͷ�
				return true; 															//��⵽������ʱ�䳤����
			}
		}
		else 
		{
			upCnt++; 																//��¼�ͷŴ���
			if(upCnt>5)																//������⵽�ͷų���5��
			{
				DEBUG_APP(2,"����ʱ�䲻��");	
				HAL_GPIO_WritePin(GPIOB, LCD_BACK_Pin, GPIO_PIN_SET);
				write_cmd_100(0x01);  //��ϵͳ����
				write_cmd_100(0x03);  //����LCD��ʾ
				write_cmd_100(0x18);  //Ƭ��RC 32KHZ
				write_cmd_100(0xe3);  //����ģʽ
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET); ///�ȴ������ͷ�
				return false;									
										//����ʱ��̫�̣����ǰ�����������
			}
		}		
	}
}

/**
  * ��������: ����״̬��ת
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void vKeyWorkStatusJudgment(void)
{		
	 /* ���ϵͳ�Ƿ��ǴӴ���ģʽ������ */ 	
	if(bKeyStandbyCheckPwrkey())  ///��IO��Ϣ���޴����ж�
	{
		printf("����\r\n");		
		HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);	//�˴����ж����ȼ����ܱ�ϵͳʱ�ӵ����ȼ��ߣ����򰴼��������ò�����
		HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);			//�ж�ʹ��
	}
	else 
	{	
        DEBUG(2,"ϵͳ��������\r\n");
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
			DEBUG(2,"�ػ� \r\n");
			for(uint8_t ucTime = 0; ucTime < 3; ++ucTime)
			{
				uint16_t pwmVal=0;   //PWMռ�ձ�  
				while (pwmVal< 150)
				{
				  pwmVal++;
				  __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, pwmVal);    //�޸ıȽ�ֵ���޸�ռ�ձ�
				  HAL_Delay(1);
				}
				while (pwmVal)
				{
				  pwmVal--;
				  __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, pwmVal);    //�޸ıȽ�ֵ���޸�ռ�ձ�
				  HAL_Delay(1);
				}
			}
			vKeyStandbyMode();
		}			 
	}										
}
