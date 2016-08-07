/*--------------------------------------------------------------------------
18B20Test.C

Test DS18B20 module to show temperature.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All righDQ reserved.
--------------------------------------------------------------------------*/
#define uint unsigned int
#define uchar unsigned char


#include <STC15W402AS.h>
#include <intrins.h>
#include <definecontrol.h>
#include<stdio.h>

uint temp;
uint temp_1,temp_2;
uint tempFlag;



/*   �ӳٺ��� */
void delayMS(uint ms)   //@11.0592MHz
{
	uchar y;
	while (ms--)
	{
		for (y = 0; y<100; y++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
	}
}	

void Delay1us()		//@11.0592MHz
{
	_nop_();
	_nop_();
	_nop_();
}


void Delay7us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 16;
	while (--i);
}

void Delay60us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 1;
	j = 162;
	do
	{
		while (--j);
	} while (--i);
}


void Delay500us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 6;
	j = 93;
	do
	{
		while (--j);
	} while (--i);
}

void Delay200us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 3;
	j = 35;
	do
	{
		while (--j);
	} while (--i);
}




/*   DS18B20�¶ȴ�����ģ��*/

/**********ds18b20��ʼ������**********************/

void DS18B20_Init(void) 
{
	uchar x=0;
	DQ = 1;          //DQ��λ
	Delay7us(); //������ʱ
	DQ = 0;          //��Ƭ����DQ����
	Delay500us(); //��ȷ��ʱ ���� 480us
	DQ = 1;          //��������
	Delay200us();  //�㹻���ӳ� ȷ������DS18B20������������
	x=DQ;   
	Delay200us();
}

/***********ds18b20��һ���ֽ�**************/

uchar DS18B20_ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // �������ź�
		Delay1us();
		dat>>=1;
		DQ = 1; // �������ź�
		Delay7us();
		if(DQ)
			dat|=0x80;
		Delay60us();     
	}	
	return(dat);
}

/*************ds18b20дһ���ֽ�****************/

void DS18B20_WriteOneChar(uchar dat)
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		Delay1us();
		DQ = dat&0x01;
		Delay60us();
		DQ = 1;
		dat>>=1;
		Delay7us();
	}
}

/**************��ȡds18b20��ǰ�¶�************/

uint DS18B20_ReadTemp(void)
{
	uchar a;
	uchar b;
	float tt;
	uint readTemp;
	

	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC);     
	DS18B20_WriteOneChar(0x44);  
	
	while (!DQ);
      
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC);  
	DS18B20_WriteOneChar(0xBE);  


	a=DS18B20_ReadOneChar();    //��ȡ�¶�ֵ��λ
	b=DS18B20_ReadOneChar();      //��ȡ�¶�ֵ��λ
	

	readTemp = b<<8;
	readTemp |= a; 
	tt = readTemp*0.0625;
	readTemp = tt + 0.5;
	return readTemp;
        
}


/*      �������¶�*/

void tempHandle()
{
	temp_1 = temp%100/10;
	temp_2 = temp%10;
}

/*    �¶���ʾ����*/
void tempshow(int shi,int ge)
{
	E1 = 0;
	P1 = allclear;	    
    delayMS(5);
	E1 = 1;
		
	E2 = 0;
	P1 = chart[shi];
    delayMS(5);
	E2 = 1;
		
	E3 = 0;
	P1 = chart[ge];	
	delayMS(5);
	E3 = 1;
		
	E4 = 0;
	P1 = chart[11];	
	delayMS(5);
	E4 = 1;
	
	MDLIGHT = 1;
}



void init()
{
	//�ر�����С��
	E1=1;
	E2=1;
	E3=1;
	E4=1;
	MDLIGHT = 1;
	P1 = allclear;
}

void main()
{
	init();
	
	while(1)
	{
		temp = DS18B20_ReadTemp();
		tempHandle();
		tempshow(temp_1,temp_2);
	}
	
}