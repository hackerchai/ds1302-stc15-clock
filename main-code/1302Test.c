/*--------------------------------------------------------------------------
1302TEST.C

Test DS1302 module.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
#include <STC15W402AS.h>
#include <intrins.h>
#include <definecontrol.h>
#include <DS1302.H>
#include<stdio.h>


//**************************
char sec,min,hour;   //ʱ������
char sec_1,sec_2,min_1,min_2,hour_1,hour_2; //λ��ת��
char temp;
char year,month,week,day;
int status = 0; //״̬����
char preSec;
//**************************
char  DS1302_addr[]={
  0x80, //0,д����(Second)�Ĵ���
  0x81, //1,������(Second)�Ĵ���
  0x82, //2,д����(Minute)�Ĵ���
  0x83, //3,������(Minute)�Ĵ���
  0x84, //4,д��Сʱ(Hour)�Ĵ���
  0x85, //5,����Сʱ(Hour)�Ĵ���
  0x86, //6,д����(Day)�Ĵ���
  0x87, //7,������(Day)�Ĵ���
  0x88, //8,д���·�(Month)�Ĵ���
  0x89, //9,�����·�(Month)�Ĵ���
  0x8a, //10,д����(Week)�Ĵ���
  0x8b, //11,������(Week)�Ĵ���
  0x8c, //12,д������(Year)�Ĵ���
  0x8d, //13,��������(Year)�Ĵ���
  0x8e, //14,д����_�Ĵ���
  0x8f  //15,��������

}  ;


/*   �����ӳٺ��� */
void delayMS(unsigned int ms)
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

//***************д��һ�ֽ�***********************
void DS1302_Input_Byte(char Input)  //��ʱ��ICд��һ�ֽ�
{
  char i;
  ACC =Input;
  for(i=8; i>0; i--)
  {
    T_DIO = ACC_0;            //�൱�ڻ����е� RRC
    T_SCLK = 1;
    T_SCLK = 0;
    ACC = ACC >> 1;
  }
}
//**************��ȡһ�ֽ�()*************************
char DS1302_Output_Byte(void)      //��ʱ��IC��ȡһ�ֽ�()
{
  char i;
  for(i=8; i>0; i--)
  {
    ACC>>=1; T_DIO= 1;
    ACC_7 = T_DIO;
    T_SCLK = 1;                 //�൱�ڻ����е� RRC
    T_SCLK = 0;
  }
  T_DIO = 0;
  return(ACC);
}
//**************��ʱ��ICд����*************************
void DS1302_Write_one( char addr,dat )       // д����ַ�������ӳ���
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
  DS1302_Write_one(0x80,sec_w);
  DS1302_Write_one(0x82,min_w);
  DS1302_Write_one(0x84,hour_w);
  DS1302_Write_one(0x86,day_w);
  DS1302_Write_one(0x88,month_w);
  DS1302_Write_one(0x8a,week_w);
  DS1302_Write_one(0x8c,year_w);
  DS1302_Write_one(0x8e,0x80);
}

//ʱ��ת��Ϊ��ʾ��ʽ
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
/*   ��ʾת������ */
void convertShow(char hour_s1,hour_s2,min_s1,min_s2)
{

  E1 = 0;
  P1 = chart[hour_s1];
  DS1302_readtime();
  delayMS(2);
  E1 = 1;

  E2 = 0;
  P1 = chart[hour_s2];
  DS1302_readtime();
  delayMS(2);
  E2 = 1;

  E3 = 0;
  P1 = chart[min_s1];
  DS1302_readtime();
  delayMS(2);
  E3 = 1;

  E4 = 0;
  P1 = chart[min_s2];
  DS1302_readtime();
  delayMS(2);
  E4 = 1;
  DS1302_readtime();
  if(sec_2%0x02==0x01)
  {
    MDLIGHT = 0;
  }
  else
  MDLIGHT = 1;

}



void stopClock()
{

  preSec = DS1302_Read(0x80);
  DS1302_Write_one(0x8e,0x00);    //�򿪱���
  DS1302_Write_one(0x80,0x80);
  DS1302_Write_one(0x8e,0x80);    //�ظ�����
}





void init()
{
  //Light off all
  E1=1;
  E2=1;
  E3=1;
  E4=1;
  MDLIGHT = 1;
  P1 = allclear;
  T_CE = 0;
  T_SCLK = 0;
  sec = 0x00;
  min = 0x49;
  hour = 0x15;
  year = 0x01;
  month = 0x01;
  week = 0x01;
  day = 0x01;
  temp = 0x10;
  if(DS1302_Read(0x81)&0x80==0x80)
  {
    DS1302_Write_one(0x8e,0x00);
    DS1302_Write_one(0x80,sec);  //����
    DS1302_Write_one(0x8e,0x80);
  }
}


void main()
{
  init();

  while(1)
  {
    DS1302_readtime();
    convertShow(hour_1,hour_2,sec_1,sec_2);
    if(sec_2 = 0x02)
    {
      stopClock();
    }

  }
}
