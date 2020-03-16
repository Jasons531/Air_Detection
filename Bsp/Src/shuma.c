/*
**************************************************************************************************************
*	@file		shuma.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		用户层
***************************************************************************************************************
*/ 
#include "shuma.h"

/**
  * 函数功能：数码管静态数据显示
  * 输入参数：无
  * 返 回 值：无
  * 说    明：显示所有符号跟单位标识
  * 
  */
void vStaticDisplay(void)
{
	CODIS;			
	TEMPDIS;		
	PPMDIS;			
	HUMDIS;			
	HDIS;			
	CDIS;			
	PARTLINEDIS;		
	TVOCDIS;		
	MG_M3DIS;		
	PM10DIS;	
	UG_M3_PM10DIS;	
	PM1_0DIS;		
	UG_M3_PM1_0DIS;	
	PM2_5DIS;		
	UG_M3_PM2_5DIS;
	BATDIS;		
	HCHODIS;			
	UG_M3_HCHODIS;	
	
	vCODisplay(45, 10);  ///百位
	vCODisplay(43, 10);  ///十位
	vCODisplay(41, 10);  ///个位
	
	vAhtHum10Display(false, 39, 10); ///十位
	vAhtHum10Display(false, 37, 10); ///个位
	vAht10TempDisplay(35, 10); ///十位
	vAht10TempDisplay(33, 10); ///个位
	
	vTVOCDisplay(27, 10); ///百位
	vTVOCDisplay(29, 10); ///十位
	vTVOCDisplay(31, 10); ///个位  
	vHCHODisplay(5,  100); ///百位
	vHCHODisplay(3,  10); ///十位
	vHCHODisplay(1,  10); ///个位
		
	vPM1_0Display(15, 10); ///百位
	vPM1_0Display(17, 10); ///十位
	vPM1_0Display(19, 10); ///个位
	  
	vPM2_5Display(9,  10);  ///百位
	vPM2_5Display(11, 10); ///十位
	vPM2_5Display(13, 10); ///个位		
	
	vPM10Display(21, 10); ///百位
	vPM10Display(25, 10); ///十位
	vPM10Display(23, 10); ///个位 
}

/**** CO数码管显示：0~9 Start****/
/**
  * 函数功能：CO数码管显示
  * 输入参数：数码管地址(45~40)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址：8 4 2 1， COM5~COM8对应屏幕低地址：8 4 2 1
  * 地址45：百位，43：十位，41：个位
  */
void vCODisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识 ****/
	uint8_t ucCOSate = 0x8;
	
	//C0: TEMP: PPM
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0x7);			
		break;
		case 1:
			write_101(ucDisAddr,0x2|ucCOSate);
			write_101(ucDisAddr-1,0x4);			
		break;
		case 2:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 3:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 4:
			write_101(ucDisAddr,0x3|ucCOSate);
			write_101(ucDisAddr-1,0xc);			
		break;
		case 5:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 6:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 7:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0x4);			
		break;
		case 8:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 9:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		default:	
			write_101(ucDisAddr-1,0x8);
		break;
		
	}
}

/**** CO数码管显示：0~9 End****/


/**** 空气温湿度数码管显示：0~9 Start****/
/**
  * 函数功能：空气湿度数码管显示
  * 输入参数：温度负数标识，数码管地址(39~37)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址：8 4 2 1， COM5~COM8对应屏幕低地址：8 4 2 1
  * 地址：39十位 37个位
  */
void vAhtHum10Display(bool bNegativeData,uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识：HUM, - ****/
	uint8_t ucCOSate = 0x8;
	
	if(bNegativeData && ucDisAddr == 37)
	{
		ucCOSate = 0x8;
	}
	else if(!bNegativeData && ucDisAddr == 37)
	{
		ucCOSate = 0x0;
	}
	
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0x7);			
		break;
		case 1:
			write_101(ucDisAddr,0x2|ucCOSate);
			write_101(ucDisAddr-1,0x4);			
		break;
		case 2:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 3:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 4:
			write_101(ucDisAddr,0x3|ucCOSate);
			write_101(ucDisAddr-1,0xc);			
		break;
		case 5:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 6:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 7:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0x4);			
		break;
		case 8:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 9:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		default:
			write_101(ucDisAddr-1,0x8);	
		break;
		
	}
}

/**
  * 函数功能：空气温度数码管显示
  * 输入参数：数码管地址(35~32)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址：8 4 2 1， COM5~COM8对应屏幕低地址：8 4 2 1
  * 地址：35十位 33个位
  */
void vAht10TempDisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识 ％ ℃ ****/
	uint8_t ucCOSate = 0x1;
	write_101(ucDisAddr-1,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0xe);
			write_101(ucDisAddr-1,0xe|ucCOSate);			
		break;
		case 1:
			write_101(ucDisAddr,0x4);
			write_101(ucDisAddr-1,0x8|ucCOSate);			
		break;
		case 2:
			write_101(ucDisAddr,0xd);
			write_101(ucDisAddr-1,0x6|ucCOSate);			
		break;
		case 3:
			write_101(ucDisAddr,0xd);
			write_101(ucDisAddr-1,0xa|ucCOSate);			
		break;
		case 4:
			write_101(ucDisAddr,0x7);
			write_101(ucDisAddr-1,0x8|ucCOSate);			
		break;
		case 5:
			write_101(ucDisAddr,0xb);
			write_101(ucDisAddr-1,0xa|ucCOSate);			
		break;
		case 6:
			write_101(ucDisAddr,0xb);
			write_101(ucDisAddr-1,0xe|ucCOSate);			
		break;
		case 7:
			write_101(ucDisAddr,0xc);
			write_101(ucDisAddr-1,0x8|ucCOSate);			
		break;
		case 8:
			write_101(ucDisAddr,0xf);
			write_101(ucDisAddr-1,0xe|ucCOSate);			
		break;
		case 9:
			write_101(ucDisAddr,0xf);
			write_101(ucDisAddr-1,0xa|ucCOSate);			
		break;
		default:
			write_101(ucDisAddr,0x1);
		break;
	}
}

/**** 空气温湿度数码管显示：0~9 End****/


/**** TVOC数码管显示：0~9 Start****/
/**
  * 函数功能：TVOC数码管显示
  * 输入参数：数码管地址(31~26)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址：8 4 2 1， COM5~COM8对应屏幕低地址：8 4 2 1
  * 地址：27百位 29十位 31个位
  */
void vTVOCDisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识：mg/m3 TVOC： 分隔线****/
	uint8_t ucCOSate = 0x8;
	
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0x7);			
		break;
		case 1:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x2);			
		break;
		case 2:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 3:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 4:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0xe);			
		break;
		case 5:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 6:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 7:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x3);			
		break;
		case 8:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 9:
			write_101(ucDisAddr,0xd|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		default:
			write_101(ucDisAddr-1,0x8);	
		break;
		
	}
}
/**** TVOC数码管显示：0~9 End****/

/**** PM10数码管显示：0~9 Start****/
/**
  * 函数功能：PM10数码管显示
  * 输入参数：数码管地址(25~20)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址(25)：8 4 2 1， COM5~COM8对应屏幕低地址(24)：8 4 2 1
  * 地址：21百位 25十位 23个位
  */
void vPM10Display(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识：ug/m3 PM10：****/
	uint8_t ucCOSate = 0x8;
	
	if(ucDisAddr == 23)
	{
		ucCOSate = 0;
	}
	
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0x7);			
		break;
		case 1:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x2);			
		break;
		case 2:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 3:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 4:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0xe);			
		break;
		case 5:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 6:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 7:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x3);			
		break;
		case 8:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 9:
			write_101(ucDisAddr,0xd|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		default:	
			write_101(ucDisAddr-1,0x8);
		break;
		
	}
}
/**** PM10数码管显示：0~9 End****/

/**** PM1_0数码管显示：0~9 Start****/
/**
  * 函数功能：PM1.0数码管显示
  * 输入参数：数码管地址(19~14)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址(19)：8 4 2 1， COM5~COM8对应屏幕低地址(14)：8 4 2 1
  * 地址：15百位 17十位 19个位
  */
void vPM1_0Display(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识：ug/m3 PM1.0：****/
	uint8_t ucCOSate = 0x1;
	
	if(ucDisAddr == 17)
	{
		ucCOSate = 0;
	}
	
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0xe);
			write_101(ucDisAddr-1,0xe|ucCOSate);			
		break;
		case 1:
			write_101(ucDisAddr,0x2);
			write_101(ucDisAddr-1,0x4|ucCOSate);			
		break;
		case 2:
			write_101(ucDisAddr,0xd);
			write_101(ucDisAddr-1,0x6|ucCOSate);			
		break;
		case 3:
			write_101(ucDisAddr,0xb);
			write_101(ucDisAddr-1,0x6|ucCOSate);			
		break;
		case 4:
			write_101(ucDisAddr,0x3);
			write_101(ucDisAddr-1,0xc|ucCOSate);			
		break;
		case 5:
			write_101(ucDisAddr,0xb);
			write_101(ucDisAddr-1,0xa|ucCOSate);			
		break;
		case 6:
			write_101(ucDisAddr,0xf);
			write_101(ucDisAddr-1,0xa|ucCOSate);			
		break;
		case 7:
			write_101(ucDisAddr,0x2);
			write_101(ucDisAddr-1,0x6|ucCOSate);			
		break;
		case 8:
			write_101(ucDisAddr,0xf);
			write_101(ucDisAddr-1,0xe|ucCOSate);			
		break;
		case 9:
			write_101(ucDisAddr,0xb);
			write_101(ucDisAddr-1,0xe|ucCOSate);			
		break;
		default:
			write_101(ucDisAddr,0x1);
		break;
		
	}
}
/**** PM10数码管显示：0~9 End****/

/**** PM2.5数码管显示：0~9 Start****/
/**
  * 函数功能：PM2.5数码管显示
  * 输入参数：数码管地址(13~8)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址(13)：8 4 2 1， COM5~COM8对应屏幕低地址(8)：8 4 2 1
  * 地址：9百位 11十位 13个位
  */
void vPM2_5Display(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识：ug/m3 PM2.5：****/
	uint8_t ucCOSate = 0x8;
	
	if(ucDisAddr == 11)
	{
		ucCOSate = 0;
	}
	
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0x7);			
		break;
		case 1:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x2);			
		break;
		case 2:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 3:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 4:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0xe);			
		break;
		case 5:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 6:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 7:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x3);			
		break;
		case 8:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 9:
			write_101(ucDisAddr,0xd|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		default:
			write_101(ucDisAddr-1,0x8);
		break;
		
	}
}
/**** PM2.5数码管显示：0~9 End****/

/**
  * 函数功能：HCHO数码管显示
  * 输入参数：数码管地址(7~6)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址(6)：8 4 2 1， COM5~COM8对应屏幕低地址(1)：8 4 2 1
  */

void vBatDisplay(uint8_t ucDisData)
{	
	/**** 电池边框 ****/
	write_101(6,0x2);
	switch(ucDisData)
	{
		case 0:
			write_101(7,0x0);
			write_101(6,0x2);
		break;
		case 25:
			write_101(7,0x8);
		break;
		case 50:
			write_101(7,0x8);
			write_101(6,0xa);
		break;
		case 75:
			write_101(7,0x8);
			write_101(6,0xe);
		break;
		case 100:
			write_101(7,0x8);
			write_101(6,0xf);
		break;
		default:		
		break;		
	}
}


/**** HCHO数码管显示：0~9 Start****/
/**
  * 函数功能：HCHO数码管显示
  * 输入参数：数码管地址(5~0)，显示数据
  * 返 回 值：无
  * 说    明：COM1~COM4对应屏幕高地址(6)：8 4 2 1， COM5~COM8对应屏幕低地址(1)：8 4 2 1
  * 地址：5百位 3十位 1个位
  */
void vHCHODisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** 常亮标识：mg/m3 HCHO：****/
	uint8_t ucCOSate = 0x8;
	
	if(ucDisAddr == 3)
	{
		ucCOSate = 0;
	}
	
	write_101(ucDisAddr,ucCOSate);
	
	switch(ucDisData)
	{
		case 0:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0x7);			
		break;
		case 1:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x2);			
		break;
		case 2:
			write_101(ucDisAddr,0x6|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 3:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xb);			
		break;
		case 4:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0xe);			
		break;
		case 5:
			write_101(ucDisAddr,0x5|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 6:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xd);			
		break;
		case 7:
			write_101(ucDisAddr,0x1|ucCOSate);
			write_101(ucDisAddr-1,0x3);			
		break;
		case 8:
			write_101(ucDisAddr,0x7|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		case 9:
			write_101(ucDisAddr,0xd|ucCOSate);
			write_101(ucDisAddr-1,0xf);			
		break;
		default:	
			write_101(ucDisAddr-1,0x8);
		break;		
	}
}
/**** HCHO数码管显示：0~9 End****/
