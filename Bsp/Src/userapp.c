/*
**************************************************************************************************************
*	@file		KQ6600.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		�û���
***************************************************************************************************************
*/ 
#include "userapp.h"

USERAPPSENSOR_t xUserappSensor;

/**** RTC��ǰ���߼��� ****/
uint32_t ulCurrentSleepTime = 0;

/**** �����ϵ��һ��У׼CO R0���� *****/
bool bUserPowerUp = false;

/**** CO R0���� *****/
float fR0 = 0;

/**** ���������ݱ�����־ *****/
bool bSensorWarning = false;

/**
  * �������ܣ���������ʼ��
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void vUappSensorInit(void)
{
	vKQInit( );
	vPMC7003Init(  );	
	bAhtInit(  );
	init_tm1622(); 
    vStaticDisplay( );
}

/**
  * �������ܣ����ڿ����жϵ�����
  * ����������������ID
  * �� �� ֵ����
  * ˵    ������
  */
void vUapp_IdleCpltCallback( UART_HandleTypeDef * huart )
{
    if( ( __HAL_UART_GET_FLAG( huart, UART_FLAG_IDLE ) != RESET ) ) {

        if( huart == &usartKQ6600 ) {			
			/***** RxXferSize���ܻ��棬RxXferCount��ʣ�໺��  *****/
			xKQUart.ucLen = usartKQ6600.RxXferSize - usartKQ6600.RxXferCount;
			DEBUG_APP(3,"usartKQ6600.RxXferSize = %d, usartKQ6600.RxXferCount = %d",usartKQ6600.RxXferSize, usartKQ6600.RxXferCount);
			DEBUG(3,"usartKQ6600: ");
			for(uint8_t uci = 0; uci < xKQUart.ucLen; ++uci)
			{
				DEBUG(3,"%02x",xKQUart.ucRxBuf[uci]);
			}
			DEBUG(3,"\r\n");
			memset(xKQUart.ucSensorBuf, 0, RX_MAX_LEN);			
			memcpy(xKQUart.ucSensorBuf, xKQUart.ucRxBuf,xKQUart.ucLen);
			memset(xKQUart.ucRxBuf, 0, RX_MAX_LEN);

			HAL_UART_AbortReceive_IT(&usartKQ6600);
			HAL_UART_Receive_IT(&usartKQ6600, xKQUart.ucRxBuf, RX_MAX_LEN );
        } 
		else if( huart == &usartPMC7003 ) {			
			/***** RxXferSize���ܻ��棬RxXferCount��ʣ�໺��  *****/
			xPMCUart.ucLen = usartPMC7003.RxXferSize - usartPMC7003.RxXferCount;
			DEBUG_APP(3,"usartPMC7003.RxXferSize = %d, usartPMC7003.RxXferCount = %d",usartPMC7003.RxXferSize, usartPMC7003.RxXferCount);
			DEBUG(2,"usartPMC7003: ");
			for(uint8_t uci = 0; uci < xPMCUart.ucLen; ++uci)
			{
				DEBUG(2,"%02x",xPMCUart.ucRxBuf[uci]);
			}
			DEBUG(2,"\r\n");
			memset(xPMCUart.ucSensorBuf, 0, RX_MAX_LEN);
			memcpy(xPMCUart.ucSensorBuf, xPMCUart.ucRxBuf,xPMCUart.ucLen);
			memset(xPMCUart.ucRxBuf, 0, RX_MAX_LEN);
			HAL_UART_AbortReceive_IT(&usartPMC7003);
			HAL_UART_Receive_IT(&usartPMC7003, xPMCUart.ucRxBuf, RX_MAX_LEN );
        } 
        __HAL_UART_CLEAR_IDLEFLAG( huart );
    }
}

/**
  * �������ܣ����������ʾ
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void vUppGetBatDisplay(void)
{
	uint8_t cBatteryPercent = 0; 
	cBatteryPercent = ucAdcBattery(  );
	
	if(cBatteryPercent>0 && cBatteryPercent<=25)
	{
		cBatteryPercent = 25;
	}
	else if(cBatteryPercent>25 && cBatteryPercent<=50)
	{
		cBatteryPercent = 50;
	}
	else if(cBatteryPercent>50 && cBatteryPercent<=75)
	{
		cBatteryPercent = 75;
	}
	else if(cBatteryPercent>75 && cBatteryPercent<=100)
	{
		cBatteryPercent = 100;
	}
	else
	{
		cBatteryPercent = 0;
	}
	DEBUG_APP(2, "cBatteryPercent = %d",cBatteryPercent);
	
	/**** û���ʱ����ʾ״̬ ****/
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(BAT_CHARG_GPIO_Port, BAT_CHARG_Pin))
	{
		vBatDisplay(cBatteryPercent);
	}	
}


/**
  * �������ܣ���س����ʾ
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void vUppChargBatDisplay(uint8_t *cBatteryPercent)
{
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(BAT_CHARG_GPIO_Port, BAT_CHARG_Pin))
	{
		vBatDisplay((*cBatteryPercent-1)*25);
		if(5 == *cBatteryPercent)
		{
			*cBatteryPercent = 0;
		}
	}	
	else
	{
		*cBatteryPercent = 0;
	}
}

#define CAL_PPM  10  // У׼������PPMֵ
#define RL		 100  // RL��ֵ

/**
  * �������ܣ�һ����̼������У׼����
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
static float MQ7_PPM_Calibration(float RS)
{
   return RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
}

//// ��ȡ��������ֵ
//float MQ7_GetPPM(void)
//{
//    float Vrl = 3.3f * ADC_Value / 4095.f;
//    float RS = (3.3f - Vrl) / Vrl * RL;
//    if(boot_time_ms < 3000) // ��ȡϵͳִ��ʱ�䣬3sǰ����У׼
//    {
//	MQ7_PPM_Calibration(RS);
//    }
//    float ppm = 98.322f * pow(RS/R0, -1.458f);
//    return  ppm;

//}

/**
  * �������ܣ�һ����̼������ʾ
  * �����������
  * �� �� ֵ���ɼ�״̬
  * ˵    ������
  */
bool bUppGetCODisplay(void)
{	
	if(!bMQHeatStart && (HAL_GetTick() - ulMQ5vHeatTime > MQ_5V_HEAT_TIME))
	{
		MQ7_5V_DIS;
		MQ7_1V5_EN;
		ulMQ1v5HeatTime = HAL_GetTick();
		ulMQ5vHeatTime = HAL_GetTick();
		
		bMQHeatStart = true;
		bMQHeatDone = true;		
		DEBUG_APP(2,"**** bMQHeatDone ****");
	}	
	if(bMQHeatDone && (HAL_GetTick() - ulMQ1v5HeatTime > MQ_1V5_HEAT_TIME))
	{
		DEBUG_APP(2,"**** ulMQ1v5HeatTime ****");
		bMQHeatDone = false;
		bMQHeatStart = false;
		ulMQ1v5HeatTime = HAL_GetTick();
		ulMQ5vHeatTime = HAL_GetTick();
		float fVrl = fAdcMQ();
		MQ7_1V5_DIS;
		if(fVrl>0)
		{
			float fRS = (3.3f - fVrl) / fVrl * RL;
			
			if(!bUserPowerUp)
			{
				bUserPowerUp = true;
				fR0 = MQ7_PPM_Calibration(fRS);
//				fR0 = 0.34; ///���ݲ��Կ�������ȡֵ�����ڿ��Բ����Զ��ٴ�У׼
				DEBUG_APP(2,"**** Calibration fR0 = %.2f ****",fR0);
			}
			if(fR0>0)
			{
				float fPpm = 98.322f * pow(fRS/fR0, -1.458f);
				DEBUG_APP(2,"fR0 = %.2f fPpm = %.2f",fR0,fPpm);
				
				vCODisplay(45, fPpm/100);  ///��λ
				vCODisplay(43, (uint8_t)fPpm%100/10);  ///ʮλ
				vCODisplay(41, (uint8_t)fPpm%10);  ///��λ
				
				if(fPpm>50)
				{
					bSensorWarning = true;
				}
				
				HAL_Delay(2000);
				return true;
			}		
		}
	}
	vCODisplay(45, 10);  ///��λ
	vCODisplay(43, 10);  ///ʮλ
	vCODisplay(41, 10);  ///��λ
	
	return false;
}

/**
  * �������ܣ���ȡ��ȩ������ʾ
  * �����������ȩ���ݻ���
  * �� �� ֵ����
  * ˵    ������
  */
void vUppGetHCHODisplay(uint8_t *ucKQBuf)
{
	uint8_t fHcho = 0;
	vKQReadSensor(ucKQBuf);
		
	DEBUG_APP(2,"ucKQBuf = %02X %02X",ucKQBuf[0],ucKQBuf[1]);
	if(ucKQBuf[0] != 0xff)
	{
		fHcho = (ucKQBuf[0]<<8)|ucKQBuf[1];
		/*****/
	//	X ppm = (Y mg/m3)(24.45)/(������ = 30.026 g/mol)
	//��
	//Y mg/m3 = (X ppm)(������)/24.45
		fHcho *= 1.23;
		vTVOCDisplay(27, fHcho/100); ///��λ
		vTVOCDisplay(29, fHcho%100/10); ///ʮλ
		vTVOCDisplay(31, fHcho%10); ///��λ  
//		fHcho = fHcho/10*22.4/24.45;
		 
		fHcho = (ucKQBuf[2]<<8)|ucKQBuf[3];
		vHCHODisplay(5,  fHcho/100); ///��λ
		vHCHODisplay(3,  fHcho%100/10); ///ʮλ
		vHCHODisplay(1,  fHcho%10); ///��λ		
		
		if(fHcho>10)
		{
			bSensorWarning = true;
		}
		return;
	}
	else
	{
		vTVOCDisplay(27, 10); ///��λ
		vTVOCDisplay(29, 10); ///ʮλ
		vTVOCDisplay(31, 10); ///��λ  
		vHCHODisplay(5,  100); ///��λ
		vHCHODisplay(3,  10); ///ʮλ
		vHCHODisplay(1,  10); ///��λ
	}
}
 
/**
  * �������ܣ���ȡ������ʪ��������ʾ
  * ���������������ʪ�Ȼ���
  * �� �� ֵ����
  * ˵    ������
  */
void vUppGetAhtDisplay(int8_t *ctBuf)
{
	bool bState = bAhtReadCTdata(ctBuf);

	if(bState)
	{	
		if(ctBuf[0]<0)
		{
			vAhtHum10Display(true, 39, ctBuf[1]/10); ///ʮλ
			vAhtHum10Display(true, 37, ctBuf[1]%10); ///��λ
		}
		else
		{
			vAhtHum10Display(false, 39, ctBuf[1]/10); ///ʮλ
			vAhtHum10Display(false, 37, ctBuf[1]%10); ///��λ
		}	
		vAht10TempDisplay(35, ctBuf[0]/10); ///ʮλ
		vAht10TempDisplay(33, ctBuf[0]%10); ///��λ
		vAhtSleep( );
	}
	else
	{
		vAhtHum10Display(false, 39, 10); ///ʮλ
		vAhtHum10Display(false, 37, 10); ///��λ
		vAht10TempDisplay(35, 10); ///ʮλ
		vAht10TempDisplay(33, 10); ///��λ
	}
	
}

/**
  * �������ܣ�PMS7003������ʾ
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void vUppGetPMS7003Display(void)
{
	uint8_t ucSensorBuf[RX_MAX_LEN] = {0};
	uint8_t ucLen = 0;
	uint16_t usPmsData = 0;
	uint32_t ulOvertime = HAL_GetTick();
	
	HAL_GPIO_WritePin(GPIOB, PMC7003_SET_Pin, GPIO_PIN_SET);
	
	/**** ��������5S��ʱ ****/
	while(xPMCUart.ucLen == 0 && (HAL_GetTick()-ulOvertime<5000));
	
	HAL_GPIO_WritePin(GPIOB, PMC7003_SET_Pin, GPIO_PIN_RESET);
	
	memcpy(ucSensorBuf, xPMCUart.ucSensorBuf,xPMCUart.ucLen);
	ucLen = xPMCUart.ucLen;
	xPMCUart.ucLen = 0;	 
	
	DEBUG_APP(2,"ucLen = %d",ucLen);
	if(ucLen == 32)
	{
		usPmsData = ucSensorBuf[10]<<8|ucSensorBuf[11];
		
		vPM1_0Display(15, usPmsData/100); ///��λ
		vPM1_0Display(17, usPmsData%100/10); ///ʮλ
		vPM1_0Display(19, usPmsData%10); ///��λ
		
		usPmsData = ucSensorBuf[12]<<8|ucSensorBuf[13];
		vPM2_5Display(9,  usPmsData/100);  ///��λ
		vPM2_5Display(11, usPmsData%100/10); ///ʮλ
		vPM2_5Display(13, usPmsData%10); ///��λ		
		
		usPmsData = ucSensorBuf[14]<<8|ucSensorBuf[15];
		vPM10Display(21, usPmsData/100); ///��λ
		vPM10Display(25, usPmsData%100/10); ///ʮλ
		vPM10Display(23, usPmsData%10); ///��λ 
	}	
	else
	{
		vPM1_0Display(15, 10); ///��λ
		vPM1_0Display(17, 10); ///ʮλ
		vPM1_0Display(19, 10); ///��λ
		  
		vPM2_5Display(9,  10);  ///��λ
		vPM2_5Display(11, 10); ///ʮλ
		vPM2_5Display(13, 10); ///��λ		
		
		vPM10Display(21, 10); ///��λ
		vPM10Display(25, 10); ///ʮλ
		vPM10Display(23, 10); ///��λ 
	}
}

/**
  * ��������: ������������ʾ
  * �������: ��
  * �� �� ֵ: �ɼ�״̬
  * ˵    ��: 
  */
bool bUppSensorDisplay(void)
{
	bool bGetSensorDone = false;
	uint8_t ucKQBuf[4] = {0xff, 0xff, 0xff, 0xff};
	int8_t  ctBuf[2];
	
	do
	{
		vUppGetBatDisplay( ); 
			  
		vUppGetAhtDisplay(ctBuf);
		vUppGetPMS7003Display( );
//		vUppGetHCHODisplay(ucKQBuf);	
		bGetSensorDone = bUppGetCODisplay( );
		HAL_Delay(1000);
	}while(!bGetSensorDone);
	
	return bGetSensorDone;
}

/**
  * ��������: ����������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: 
  */
void vUppBeepDisplay(void)
{
	if(bSensorWarning)
	{
		bSensorWarning = false;
		for(uint8_t ucTime = 0; ucTime < 5; ++ucTime)
		{
			/*****  �����������Ч 450ʵ����Ч50 ռ�ձ���10% *****/
			__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 450);
			HAL_Delay(100);
			__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 500);
			HAL_Delay(100);
			__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 450);
			HAL_Delay(100);
			__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 500);
			HAL_Delay(600);
		
#if 0 ///�����ƽ���		
			uint16_t pwmVal=0;   //PWMռ�ձ�  
			while (pwmVal< 500)
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
#endif
		}	
	}
}

/**
  * ��������: ���⻽�Ѵ��������»�ȡ����ʱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: HAL_RTC_GetTime������HAL_RTC_GetDateһ��ʹ�ã�����RTCʱ�䲻����
  */
uint32_t ulUppGetCurrentSleepRtc(void)
{
	RTC_TimeTypeDef  RTC_TimeStruct;
	RTC_DateTypeDef  RTC_Datestructure;
	RTC_AlarmTypeDef RTC_AlarmStruct;
	
	uint32_t CurrentRtc = 0;	
	uint32_t AlarmTime = 0;
	
	//�ر�RTC����жϣ�������RTCʵ�����
	__HAL_RTC_WAKEUPTIMER_DISABLE_IT(&hrtc,RTC_IT_WUT);
	__HAL_RTC_TIMESTAMP_DISABLE_IT(&hrtc,RTC_IT_TS);
	__HAL_RTC_ALARM_DISABLE_IT(&hrtc,RTC_IT_ALRA);
	
	//���RTC����жϱ�־λ
	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
	__HAL_RTC_TIMESTAMP_CLEAR_FLAG(&hrtc,RTC_FLAG_TSF); 
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc,RTC_FLAG_WUTF);
	
	/* ������л��ѱ�־λ */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	
	HAL_RTC_WaitForSynchro(&hrtc);
	
	HAL_RTC_DeactivateAlarm( &hrtc, RTC_ALARM_A );
	HAL_RTCEx_DeactivateWakeUpTimer( &hrtc );
		
	HAL_NVIC_DisableIRQ(RTC_IRQn);
		
	HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
	
	HAL_RTC_GetDate(&hrtc, &RTC_Datestructure, RTC_FORMAT_BIN);
	
	HAL_RTC_GetAlarm(&hrtc, &RTC_AlarmStruct, RTC_ALARM_A, RTC_FORMAT_BIN);
		
	CurrentRtc 			= RTC_TimeStruct.Hours * 3600 + RTC_TimeStruct.Minutes * 60 + RTC_TimeStruct.Seconds;
	
	AlarmTime  			= RTC_AlarmStruct.AlarmTime.Hours * 3600 + RTC_AlarmStruct.AlarmTime.Minutes * 60 + RTC_AlarmStruct.AlarmTime.Seconds;
	
	DEBUG_APP(2,"GetcurrentDate:  %d  hour : %d min : %d second : %d",RTC_Datestructure.Date, RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
	
	DEBUG_APP(2,"GetAlarmDate:    %d   hour : %d min : %d second : %d",RTC_AlarmStruct.AlarmDateWeekDay, RTC_AlarmStruct.AlarmTime.Hours,RTC_AlarmStruct.AlarmTime.Minutes,RTC_AlarmStruct.AlarmTime.Seconds);

	HAL_NVIC_EnableIRQ(RTC_IRQn);
	
	return 	(AlarmTime > CurrentRtc)?(AlarmTime - CurrentRtc):0;
}

/**
  * ��������: ����RTC�������߻���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void vUppRtcAlarm(uint16_t ulTime)
{
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_TimeTypeDef  RTC_TimeStruct;
	RTC_DateTypeDef  RTC_DateStruct;
 
	HAL_RTC_WaitForSynchro(&hrtc);
	
	HAL_RTC_DeactivateAlarm( &hrtc, RTC_ALARM_A );
	HAL_RTCEx_DeactivateWakeUpTimer( &hrtc );
	
	HAL_NVIC_DisableIRQ(RTC_IRQn);

	HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
	DEBUG(2,"currenttime hour : %d min : %d second : %d\r\n",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);

	RTC_AlarmStructure.AlarmTime.Seconds = (RTC_TimeStruct.Seconds+ulTime)%60;  
	RTC_AlarmStructure.AlarmTime.Minutes = (RTC_TimeStruct.Minutes + (RTC_TimeStruct.Seconds+ulTime)/60)%60;
	RTC_AlarmStructure.AlarmTime.Hours = (RTC_TimeStruct.Hours + (RTC_TimeStruct.Minutes + (RTC_TimeStruct.Seconds+ulTime)/60)/60)%24;
	RTC_AlarmStructure.AlarmTime.SubSeconds = 0;
	
	DEBUG(2,"wkuptime    hour : %d min : %d second : %d\r\n",RTC_AlarmStructure.AlarmTime.Hours,RTC_AlarmStructure.AlarmTime.Minutes,RTC_AlarmStructure.AlarmTime.Seconds);

	RTC_AlarmStructure.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
	RTC_AlarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	RTC_AlarmStructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;//RTC_ALARMMASK_NONE;Ϊ��׼ƥ��
	RTC_AlarmStructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	RTC_AlarmStructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	RTC_AlarmStructure.Alarm = RTC_ALARM_A;
    	
	if( HAL_RTC_SetAlarm_IT( &hrtc, &RTC_AlarmStructure, RTC_FORMAT_BIN ) != HAL_OK )
	{
		assert_param( FAIL );
	}
    
	HAL_NVIC_EnableIRQ(RTC_IRQn);
}

/**
  * ��������: ����Ӳ��ʧ�ܳ�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void vUppBoradDeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
		
	HAL_ADC_MspDeInit(&hadc);  ///OK
	hadc.State = HAL_ADC_STATE_RESET;
	
	HAL_I2C_MspDeInit(&hi2c1);  ///OK
	hi2c1.State = HAL_I2C_STATE_RESET;
	
	HAL_TIM_Base_MspDeInit(&htim16);  ///OK
	htim16.State = HAL_TIM_STATE_RESET;
		
	///�ر�UART1ʱ��
	HAL_UART_DeInit(&huart1);
	huart1.gState = HAL_UART_STATE_RESET;
	
	///�ر�UART2ʱ��
	HAL_UART_DeInit(&huart2);
	huart2.gState = HAL_UART_STATE_RESET;
	
	///�ر�UART3ʱ��
	HAL_UART_DeInit(&huart3);
	huart3.gState = HAL_UART_STATE_RESET;	 
		
	GPIO_InitStructure.Pin = 0xFFCE;   //PA0 PA4 PA5����״̬
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG; 
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_All;   
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG; 
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
					
	GPIO_InitStructure.Pin = 0xDFF7;  /// PB13��1.5V��PB0��5V��PB1��PMC7003_RST��PB2��PMC7003_SET
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	HAL_SuspendTick();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
}

/**
  * �������ܣ��豸��������ģʽ
  * �����������
  * �� �� ֵ����
  * ˵    ������
  */
void vUppIntoLowPower(void)
{
	BOARD_POWER_DIS;
	POWER_5V_DIS;
	
	vTm1622Close( );
	vUppBoradDeInit(); ///�ر�ʱ����
    	
	SET_BIT( PWR->CR, PWR_CR_CWUF );
	/* Enter Stop Mode */
	HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
	
	/**** ���߻��������ʼ�� ****/
	vUappSensorInit( );
	init_tm1622(); 
	vStaticDisplay( );
}

/**
  * ��������: ���߻���ʱ�ӻָ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void vSystemClockReConfig( void )
{
    __HAL_RCC_PWR_CLK_ENABLE( );

    /* Enable HSE */
    __HAL_RCC_HSE_CONFIG( RCC_HSE_ON );

    /* Wait till HSE is ready */
    while( __HAL_RCC_GET_FLAG( RCC_FLAG_HSERDY ) == RESET )
    {
    }

    /* Enable PLL */
    __HAL_RCC_PLL_ENABLE( );

    /* Wait till PLL is ready */
    while( __HAL_RCC_GET_FLAG( RCC_FLAG_PLLRDY ) == RESET )
    {
    }

    /* Select PLL as system clock source */
    __HAL_RCC_SYSCLK_CONFIG ( RCC_SYSCLKSOURCE_PLLCLK );

    /* Wait till PLL is used as system clock source */
    while( __HAL_RCC_GET_SYSCLK_SOURCE( ) != RCC_SYSCLKSOURCE_STATUS_PLLCLK )
    {
    }
    /* Resume Tick interrupt if disabled prior to sleep mode entry*/
    HAL_ResumeTick();
}

/**
  * ��������: ����Ӳ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void vUappBoradInit(void)
{
	vSystemClockReConfig(  );
	/* Enable Power Control clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	MX_GPIO_Init();
	MX_ADC_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_TIM16_Init();

	/* Initialize interrupts */
	MX_NVIC_Init();
	
	BOARD_POWER_EN;
	POWER_5V_EN;
	/**** CO 5V ���� ****/
	MQ7_5V_EN;
	ulMQ5vHeatTime = HAL_GetTick();
	
	/* USER CODE BEGIN 2 */
	printf("RTC WAKEUP DOING\r\n");
}

