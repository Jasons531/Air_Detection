/*
**************************************************************************************************************
*	@file		shuma.h
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		用户层
***************************************************************************************************************
*/ 

#ifndef __SHUMA_H
#define __SHUMA_H

#ifdef __cplusplus
	extern "C" {
#endif
		
#include "main.h"
#include "Tm1622.h"

/**** CO: ****/		
#define CODIS			write_101(45,0x8)	
/**** TEMP: ****/
#define TEMPDIS			write_101(43,0x8) 
/**** PPM: ****/
#define PPMDIS			write_101(41,0x8)
/**** HUM: ****/
#define HUMDIS			write_101(39,0x8)
/**** ％ ****/
#define HDIS			write_101(35,0x1)
/**** ℃ ****/
#define CDIS			write_101(33,0x1)
/**** 分隔线 ****/
#define PARTLINEDIS		write_101(31,0x8)
/**** TVOC： ****/
#define TVOCDIS			write_101(29,0x8)
/**** mg/m3 ****/
#define MG_M3DIS		write_101(27,0x8)
/**** PM10： ****/
#define PM10DIS			write_101(25,0x8)
/**** ug/m3 ****/
#define UG_M3_PM10DIS	write_101(21,0x8)
/**** PM1.0： ****/
#define PM1_0DIS		write_101(19-1,0x1)
/**** ug/m3 ****/
#define UG_M3_PM1_0DIS	write_101(15-1,0x1)
/**** PM2.5： ****/
#define PM2_5DIS		write_101(13,0x8)
/**** ug/m3 ****/
#define UG_M3_PM2_5DIS	write_101(9,0x8)
/**** 电池边框 ****/
#define BATDIS			write_101(6,0x2)
/**** HCHO： ****/
#define HCHODIS			write_101(5,0x8)
/**** ug/m3 ****/
#define UG_M3_HCHODIS	write_101(1,0x8)

void vStaticDisplay(void);

void vCODisplay(uint8_t ucDisAddr, uint8_t ucDisData);

void vAhtHum10Display(bool bNegativeData,uint8_t ucDisAddr, uint8_t ucDisData);

void vAht10TempDisplay(uint8_t ucDisAddr, uint8_t ucDisData);

void vTVOCDisplay(uint8_t ucDisAddr, uint8_t ucDisData);

void vPM10Display(uint8_t ucDisAddr, uint8_t ucDisData);

void vPM1_0Display(uint8_t ucDisAddr, uint8_t ucDisData);

void vPM2_5Display(uint8_t ucDisAddr, uint8_t ucDisData);

void vBatDisplay(uint8_t ucDisData);

void vHCHODisplay(uint8_t ucDisAddr, uint8_t ucDisData);

#ifdef __cplusplus
}
#endif

#endif
