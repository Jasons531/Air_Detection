/*
**************************************************************************************************************
*	@file		shuma.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		�û���
***************************************************************************************************************
*/ 
#include "shuma.h"

/**
  * �������ܣ�����ܾ�̬������ʾ
  * �����������
  * �� �� ֵ����
  * ˵    ������ʾ���з��Ÿ���λ��ʶ
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
	
	vCODisplay(45, 10);  ///��λ
	vCODisplay(43, 10);  ///ʮλ
	vCODisplay(41, 10);  ///��λ
	
	vAhtHum10Display(false, 39, 10); ///ʮλ
	vAhtHum10Display(false, 37, 10); ///��λ
	vAht10TempDisplay(35, 10); ///ʮλ
	vAht10TempDisplay(33, 10); ///��λ
	
	vTVOCDisplay(27, 10); ///��λ
	vTVOCDisplay(29, 10); ///ʮλ
	vTVOCDisplay(31, 10); ///��λ  
	vHCHODisplay(5,  100); ///��λ
	vHCHODisplay(3,  10); ///ʮλ
	vHCHODisplay(1,  10); ///��λ
		
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

/**** CO�������ʾ��0~9 Start****/
/**
  * �������ܣ�CO�������ʾ
  * �������������ܵ�ַ(45~40)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ��8 4 2 1
  * ��ַ45����λ��43��ʮλ��41����λ
  */
void vCODisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ ****/
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

/**** CO�������ʾ��0~9 End****/


/**** ������ʪ���������ʾ��0~9 Start****/
/**
  * �������ܣ�����ʪ���������ʾ
  * ����������¶ȸ�����ʶ������ܵ�ַ(39~37)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ��8 4 2 1
  * ��ַ��39ʮλ 37��λ
  */
void vAhtHum10Display(bool bNegativeData,uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ��HUM, - ****/
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
  * �������ܣ������¶��������ʾ
  * �������������ܵ�ַ(35~32)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ��8 4 2 1
  * ��ַ��35ʮλ 33��λ
  */
void vAht10TempDisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ �� �� ****/
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

/**** ������ʪ���������ʾ��0~9 End****/


/**** TVOC�������ʾ��0~9 Start****/
/**
  * �������ܣ�TVOC�������ʾ
  * �������������ܵ�ַ(31~26)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ��8 4 2 1
  * ��ַ��27��λ 29ʮλ 31��λ
  */
void vTVOCDisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ��mg/m3 TVOC�� �ָ���****/
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
/**** TVOC�������ʾ��0~9 End****/

/**** PM10�������ʾ��0~9 Start****/
/**
  * �������ܣ�PM10�������ʾ
  * �������������ܵ�ַ(25~20)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ(25)��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ(24)��8 4 2 1
  * ��ַ��21��λ 25ʮλ 23��λ
  */
void vPM10Display(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ��ug/m3 PM10��****/
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
/**** PM10�������ʾ��0~9 End****/

/**** PM1_0�������ʾ��0~9 Start****/
/**
  * �������ܣ�PM1.0�������ʾ
  * �������������ܵ�ַ(19~14)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ(19)��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ(14)��8 4 2 1
  * ��ַ��15��λ 17ʮλ 19��λ
  */
void vPM1_0Display(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ��ug/m3 PM1.0��****/
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
/**** PM10�������ʾ��0~9 End****/

/**** PM2.5�������ʾ��0~9 Start****/
/**
  * �������ܣ�PM2.5�������ʾ
  * �������������ܵ�ַ(13~8)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ(13)��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ(8)��8 4 2 1
  * ��ַ��9��λ 11ʮλ 13��λ
  */
void vPM2_5Display(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ��ug/m3 PM2.5��****/
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
/**** PM2.5�������ʾ��0~9 End****/

/**
  * �������ܣ�HCHO�������ʾ
  * �������������ܵ�ַ(7~6)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ(6)��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ(1)��8 4 2 1
  */

void vBatDisplay(uint8_t ucDisData)
{	
	/**** ��ر߿� ****/
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


/**** HCHO�������ʾ��0~9 Start****/
/**
  * �������ܣ�HCHO�������ʾ
  * �������������ܵ�ַ(5~0)����ʾ����
  * �� �� ֵ����
  * ˵    ����COM1~COM4��Ӧ��Ļ�ߵ�ַ(6)��8 4 2 1�� COM5~COM8��Ӧ��Ļ�͵�ַ(1)��8 4 2 1
  * ��ַ��5��λ 3ʮλ 1��λ
  */
void vHCHODisplay(uint8_t ucDisAddr, uint8_t ucDisData)
{
	/**** ������ʶ��mg/m3 HCHO��****/
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
/**** HCHO�������ʾ��0~9 End****/
