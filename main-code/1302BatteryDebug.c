/*--------------------------------------------------------------------------
1302BATTERYDEBUG.C

Try to fix the DS1302 backup battery problem
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
#include <STC15W402AS.h>
#include <intrins.h>
#include <definecontrol.h>
#include <DS1302.H>
#include<stdio.h>

unsigned char sec,min,hour;   //ʱ�����
unsigned char sec_1,sec_2,min_1,min_2,hour_1,hour_2; //λ��ת��
unsigned char year,month,week,day;
int status; //״̬����
int keyValue;
unsigned char preSec;
/*
char  DS1302_addr[]={
      0x80, //0,д����(Second)�Ĵ���
      0x81, //1,������(Second)�Ĵ���
      0x82, //2,д���(Minute)�Ĵ���
      0x83, //3,������(Minute)�Ĵ���
      0x84, //4,д��Сʱ(Hour)�Ĵ���
      0x85, //5,����Сʱ(Hour)�Ĵ���
      0x86, //6,д����(Day)�Ĵ���
      0x87, //7,������(Day)�Ĵ���
      0x88, //8,д���·�(Month)�Ĵ���
      0x89, //9,�����·�(Month)�Ĵ���
      0x8a, //10,д����(Week)�Ĵ���
      0x8b, //11,������(Week)�Ĵ���
      0x8c, //12,д�����(Year)�Ĵ���
      0x8d, //13,�������(Year)�Ĵ���
      0x8e, //14,д����_�Ĵ���
      0x90, //15,д������
      0x91  //16,��������

}  ;
*/
	 
/*   �ӳٺ��� */
void delayMS(unsigned int ms)   //@11.0592MHz
{
	unsigned char y;
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

void delay2us()		//@11.0592MHz
{
	unsigned char t;

	t = 3;
	while (--t);
}




/*DS1302��������*/
//**************д��һ�ֽ�*************************
void DS1302_Input_Byte(char Input)  //��ʱ��ICд��һ�ֽ�
{   
    char i;
	T_SCLK = 0;
    delay2us();
    ACC =Input;
    for(i=8; i>0; i--)
    {  
        T_DIO = ACC_0;            //�൱�ڻ���е� RRC
		delay2us();
        T_SCLK = 1;
		delay2us();
        T_SCLK = 0;
        ACC = ACC >> 1; 
    }  
}
//**************��ȡһ�ֽ�*************************
char DS1302_Output_Byte(void)      //��ʱ��IC��ȡһ�ֽ�()
{ 
    char i;   
    for(i=8; i>0; i--)
    {  
     ACC>>=1; 
	 T_DIO= 1;
	 delay2us();
     ACC_7 = T_DIO;
     T_SCLK = 1;                 //�൱�ڻ���е� RRC 
	 delay2us();
     T_SCLK = 0;   
	 delay2us();
    }
	T_DIO = 0;
    delay2us();
    return(ACC); 
}
//**************��ʱ��ICд����*************************
void DS1302_Write_one( char addr,dat )       // д���ַ�������ӳ��� 
{
       T_CE=0;                             //T_CE����Ϊ�ͣ����ݴ�����ֹ
       T_SCLK=0;                          //����ʱ������
       T_CE = 1;                          //T_CE����Ϊ�ߣ��߼�������Ч
       DS1302_Input_Byte(addr);           // ��ַ������ 
       DS1302_Input_Byte(dat);          // д1Byte����
       T_SCLK = 1;
       T_CE = 0;     
}
//************�����ݶ�ȡ����*****************************
char DS1302_Read ( char addr )    //���ݶ�ȡ�ӳ���
{
       char date;
       T_CE=0;           
       T_SCLK=0;             
       T_CE = 1; 
       DS1302_Input_Byte(addr);        // ��ַ������ 
       date = DS1302_Output_Byte();         // ��1Byte����
       T_SCLK = 1;
       T_CE = 0;
       return(date);
}
//************��ʱ��д������****************************
void DS1302_Write(char sec_w,min_w,hour_w,day_w,month_w,week_w,year_w)
{
	    DS1302_Write_one(0x8e,0x00);
        DS1302_Write_one(0x82,min_w);
        DS1302_Write_one(0x84,hour_w);
		DS1302_Write_one(0x86,day_w);
        DS1302_Write_one(0x88,month_w);
        DS1302_Write_one(0x8a,week_w);
        DS1302_Write_one(0x8c,year_w);
		DS1302_Write_one(0x80,sec_w);
		DS1302_Write_one(0x8e,0x80);
		
} 


//************ʱ��ת��Ϊ��ʾ��ʽ*****************************
void timeConvert()
{
	sec_1 = sec>>4;
	sec_2 = sec&0x0f;
	min_1 = min>>4;
	min_2 = min&0x0f;
	hour_1 = hour>>4;
	hour_2 = hour&0x0f;
	
}
  
//************�����ݶ�ȡʱ��*****************************
void DS1302_readtime()                           
{
    sec=DS1302_Read(0x81);                    //����
    min=DS1302_Read(0x83);                    //����
    hour=DS1302_Read(0x85);                   //��ʱ
    day=DS1302_Read(0x87);                    //����
    month=DS1302_Read(0x89);                  //����
    year=DS1302_Read(0x8d);                   //����
    week=DS1302_Read(0x8b);                   //������
	timeConvert();
	
}

/*   ��ʾ��غ���*/
/* ************��ʾת������******************************/
void convertShow(char hour_s1,hour_s2,min_s1,min_s2)
{
	
		E1 = 0;
	    P1 = chart[hour_s1];	    
		delayMS(2);
		E1 = 1;
		
		E2 = 0;
	    P1 = chart[hour_s2];
		delayMS(2);
		E2 = 1;
		
	    E3 = 0;
	    P1 = chart[min_s1];	
		delayMS(2);
		E3 = 1;
		
		E4 = 0;
	    P1 = chart[min_s2];	
		delayMS(2);
		E4 = 1;
		if(sec_2%0x02==0x01)
		{
			MDLIGHT = 0;
		}
		else
			MDLIGHT = 1;
		
}

/*************��ť��ʾ��ʱ******************************/
void delayShow(int count)
{
	int j;
	timeConvert();
	for(j=0;j<count;j++)
	{
		convertShow(hour_1,hour_2,min_1,min_2);
	}
}

/*************�޸�ʱ���߼�����******************************/
void modifyTime(int mode)
{

	   //�޸�Сʱ��1��+������8��
	   if(mode == 9)
	   {
		  if(hour==0x23)
		  {
			  hour = 0x00;
		  }
		  else if(hour==0x09)
		  {
			 hour=0x10;
		  }
		  else if(hour==0x19)
		  {
			 hour=0x20;
		  }
		  else
		  {
			  hour=hour+0x01;
		  }
		  
		  timeConvert();
		  convertShow(hour_1,hour_2,min_1,min_2);
	   }
	
	   //�޸�Сʱ��1��+�ݼ���16��
	   if(mode == 17)
	   {
		   if(hour==0x00)
		  {
			  hour = 0x23;
		  }
		  else if(hour==0x10)
		  {
			  hour = 0x09;
		  }
		  else if(hour==0x20)
		  {
			  hour = 0x19;
		  }
		  else
		  {
			  hour = hour-0x01;
		  }
		  
		  timeConvert();
		  convertShow(hour_1,hour_2,min_1,min_2);
	   }
	
	   //�޸ķ��ӣ�2��+������8��
       if(mode == 10)
	   {
		  
		  if(min==0x59)
		  {
			  min=0x00;	  
		  }
		  else if(min-(min_1<<4)==0x09)
		  {
			 min=(min_1+0x01)<<4;
		  }
		  else
		  {
			  min=min+0x01;
		  }
		  
		  timeConvert();
		  convertShow(hour_1,hour_2,min_1,min_2);
	   }
	
	   //�޸ķ��ӣ�2��+�ݼ���16��
       if(mode == 18)
	   {
		   if(min==0x00)
		   { 
		      min=0x59;  
		   }
		   else if(min_2==0x00)
		   {
			  min=min-0x10;
			  min=min+0x09;
		   }
		   else
		   {
		      min=min-0x01;
		   }
		  
		  timeConvert();
		  convertShow(hour_1,hour_2,min_1,min_2);
	    }		
}

/*************��ťɨ�躯��******************************/
void keyScan()
{
	if (KEY_1==0 || KEY_2==0 || KEY_3==0)
	{
		delayMS(20);   		//20�����������
		if (KEY_1 == 0)
		{
			keyValue = 1;
			while(KEY_1==0)
			{
				modifyTime(status+8);
				delayShow(500);
			}
		}
		if (KEY_2 == 0)
	    {
			keyValue = 2;
			while(KEY_2==0)
			{
				modifyTime(status+16);
				delayShow(500);
			}
	    }
	    if (KEY_3 == 0)
	    {
			keyValue = 3;
	    }
	}
}

/*************��ť��Ӧ������******************************/
void keyHandle()
{
	if(keyValue==1)
	{
	    DS1302_Write(sec,min,hour,day,month,week,year);
		keyValue = 0;
	}
	else if(keyValue==2)
	{
		DS1302_Write(sec,min,hour,day,month,week,year);
		keyValue = 0;
	}
	else if(keyValue==3)
	{
	    if (status==1)
		{  
			status = 2;
		}
		else if(status==2)
		{  
			status = 3;
		}
		else if(status==3)
		{  
			status = 1;
		}
		keyValue = 0;
               
	}
}

/*************��Ƭ����ʼ��******************************/
void init()
{
	//�ر�����С��
	E1=1;
	E2=1;
	E3=1;
	E4=1;
	MDLIGHT = 1;
	P1 = allclear;
	T_CE = 0;
	T_SCLK = 0;

	//Ĭ�ϳ�ʼ��ʱ��12:00
	sec = 0x00;
	min = 0x00;
	hour = 0x16;
	year = 0x01;
	month = 0x01;
	week = 0x01;
	day = 0x01;
	status = 1;
	keyValue = 0;
	delayMS(1000);
	//DS1302��ʼ���ж��Ƿ���ں󱸵�Դ
	
	if(DS1302_Read(0x81)&0x80==0x80)
	{
		DS1302_Write_one(0x8e,0x00);
		DS1302_Write_one(0x80,sec);  //����
		DS1302_Write_one(0x8e,0x80);
	}
	else
	{
		
	}
}

/*************������******************************/
void main()
{
	init();
	
	while(1)
	{
		
		DS1302_readtime();
		convertShow(hour_1,hour_2,min_1,min_2);
		keyScan();
		if(keyValue!=0&&KEY_1==1&&KEY_2==1&&KEY_3==1)
		{
			keyHandle();
		}
		
	}
}