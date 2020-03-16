/*
**************************************************************************************************************
*	@file		PMC7003.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		PM2.5驱动
***************************************************************************************************************
*/ 
#include "PMC7003.h"

xPMCUart_t xPMCUart = {0, {0}};

/**
  * 函数功能: 初始化PMC7003
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
void vPMC7003Init(void)
{
	HAL_GPIO_WritePin(GPIOB, PMC7003_RST_Pin|PMC7003_SET_Pin, GPIO_PIN_SET);
	
	__HAL_UART_CLEAR_IDLEFLAG(&usartPMC7003);
	__HAL_UART_ENABLE_IT(&usartPMC7003,UART_IT_IDLE);	
	
	HAL_UART_AbortReceive_IT(&usartPMC7003);
	HAL_UART_Receive_IT(&usartPMC7003, xPMCUart.ucRxBuf, RX_MAX_LEN );	
}

/**
  * 函数功能：复位PMC7003
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
void vPMC7003Reset(void)
{
	HAL_GPIO_WritePin(GPIOB, PMC7003_RST_Pin, GPIO_PIN_RESET);
}

/**
  * 函数功能：PMC7003模块唤醒
  * 输入参数：无
  * 返 回 值：成功：true  失败：false
  * 说    明：
  */
bool bPMC7003Wakeup(void)
{
//	uint8_t ucTxBuf[6] = {0x42, 0x4d, 0xe4, 0x01};
//	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3];
//	ucTxBuf[4] = (usCrc&0xFFFF)>>8;
//	ucTxBuf[5] = (usCrc&0xFF)>>0;	
	uint8_t ucTxBuf[7] = {0x42, 0x4d, 0xe4, 0x00, 0x01};
	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3] + ucTxBuf[4];
	ucTxBuf[5] = (usCrc&0xFFFF)>>8;
	ucTxBuf[6] = (usCrc&0xFF)>>0;	
	DEBUG(2,"Send: ");
	for(uint8_t uci = 0; uci < 7; ++uci)
	{
		DEBUG(2,"%02X ",ucTxBuf[uci]);
	}
	DEBUG(2,"\r\n");
	HAL_UART_Transmit(&usartPMC7003, ucTxBuf, 7, 0xffff);
	return bPMC7003Recv(&xPMCUart);
}

/**
  * 函数功能：PMC7003模块休眠
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
bool bPMC7003Sleep(void)
{
//	uint8_t ucTxBuf[6] = {0x42, 0x4d, 0xe4, 0x00};
//	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3];
//	ucTxBuf[4] = (usCrc&0xff00)>>8;
//	ucTxBuf[5] = (usCrc&0xff)>>0;	
	
	uint8_t ucTxBuf[7] = {0x42, 0x4d, 0xe4, 0x00, 0x00};
	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3] + ucTxBuf[4];
	ucTxBuf[5] = (usCrc&0xFFFF)>>8;
	ucTxBuf[6] = (usCrc&0xFF)>>0;	
	
	HAL_UART_Transmit(&usartPMC7003, ucTxBuf, 7, 0xffff);
	return bPMC7003Recv(&xPMCUart);
}

/**
  * 函数功能：PMC7003设置模式
  * 输入参数：0x00：被动模式
  *          0x01：主动模式
  * 返 回 值：无
  * 说    明：状态切换
  */
bool bPMC7003SetMode(uint8_t ucReadMode)
{
//	uint8_t ucTxBuf[6] = {0x42, 0x4d, 0xe1};
//	ucTxBuf[3] = ucReadMode;
//	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3];
	
	uint8_t ucTxBuf[7] = {0x42, 0x4d, 0xe1, 0x00};
	ucTxBuf[4] = ucReadMode;
	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3] + ucTxBuf[4];
	
	ucTxBuf[5] = (usCrc&0xFFFF)>>8;
	ucTxBuf[6] = (usCrc&0xFF)>>0;
	
	DEBUG(2,"Send: ");
	for(uint8_t uci = 0; uci < 7; ++uci)
	{
		DEBUG(2,"%02X ",ucTxBuf[uci]);
	}
	DEBUG(2,"\r\n");
	HAL_UART_Transmit(&usartPMC7003, ucTxBuf, 7, 0xffff);
	return bPMC7003Recv(&xPMCUart);
}

/**
  * 函数功能：PMC7003被动模式读取数据
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
bool bPMC7003ReadBuf(void)
{	
	uint8_t ucTxBuf[5] = {0x42, 0x4d, 0xe2};
	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2];
	ucTxBuf[3] = (usCrc&0xff00)>>8;
	ucTxBuf[4] = (usCrc&0xff)>>0;
	
//	uint8_t ucTxBuf[7] = {0x42, 0x4d, 0xe2, 0x00, 0x00};
//	uint16_t usCrc = ucTxBuf[0] + ucTxBuf[1] + ucTxBuf[2] + ucTxBuf[3] + ucTxBuf[4];
//	ucTxBuf[5] = (usCrc&0xFFFF)>>8;
//	ucTxBuf[6] = (usCrc&0xFF)>>0;
	
	HAL_UART_Transmit(&usartPMC7003, ucTxBuf, 5, 0xffff);	
	DEBUG(2,"Send: ");
	for(uint8_t uci = 0; uci < 5; ++uci)
	{
		DEBUG(2,"%02X ",ucTxBuf[uci]);
	}
	DEBUG(2,"\r\n");
	return bPMC7003Recv(&xPMCUart);
}

/**
  * 函数功能：PMC7003接收数据处理
  * 输入参数：接收数据结构体
  * 返 回 值：无
  * 说    明：
  */
bool bPMC7003Recv(xPMCUart_t *xPMCUart)
{
	uint32_t ulTimeOver = 0;
	uint16_t usCrc = 0;
	
	xPMCUart->ucLen = 0;
	ulTimeOver = HAL_GetTick();
	while(xPMCUart->ucLen == 0 && (HAL_GetTick() - ulTimeOver<2000));
	
	if(xPMCUart->ucLen>0)
	{
		DEBUG(2,"PmRecv: ");
		for(uint8_t ucI = 0; ucI < xPMCUart->ucLen; ++ucI)
		{
			DEBUG(2,"%02X ",xPMCUart->ucSensorBuf[ucI]);
		}
		DEBUG(2,"\r\n");
	}	
	
	if(xPMCUart->ucLen>2)
	{
		usCrc = 0;
		for(uint8_t ucI = 0; ucI < xPMCUart->ucLen-2; ++ucI)
		{
			usCrc += xPMCUart->ucSensorBuf[ucI];
		}
		if(usCrc == (xPMCUart->ucSensorBuf[xPMCUart->ucLen-1] + xPMCUart->ucSensorBuf[xPMCUart->ucLen-2]))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
