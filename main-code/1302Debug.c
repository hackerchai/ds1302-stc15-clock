/*--------------------------------------------------------------------------
1302DEBUG.C

Debug some  DS1302 bugs.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
#include <STC15W402AS.h>
#include <intrins.h>
#include <definecontrol.h>
#include <DS1302.H>
#include<stdio.h>


//**************************
char sec,min,hour;   //时间变量
char sec_1,sec_2,min_1,min_2,hour_1,hour_2; //位数转换
char temp;
char year,month,week,day;
int status = 0; //状态代码
char preSec;
//**************************
char  DS1302_addr[]={
      0x80, //0,写入秒(Second)寄存器
      0x81, //1,读出秒(Second)寄存器
      0x82, //2,写入分(Minute)寄存器
      0x83, //3,读出分(Minute)寄存器
      0x84, //4,写入小时(Hour)寄存器
      0x85, //5,读出小时(Hour)寄存器
      0x86, //6,写入日(Day)寄存器
      0x87, //7,读出日(Day)寄存器
      0x88, //8,写入月份(Month)寄存器
      0x89, //9,读出月份(Month)寄存器
      0x8a, //10,写入周(Week)寄存器
      0x8b, //11,读出周(Week)寄存器
      0x8c, //12,写入年份(Year)寄存器
      0x8d, //13,读出年份(Year)寄存器
      0x8e, //14,写保护_寄存器
      0x8f  //15,涓流充电

}  ;
	  
	  
/*   毫秒延迟函数 */
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
	  
//***************写入一字节***********************
void DS1302_Input_Byte(char Input)  //向时钟IC写入一字节
{   
  char i;
    ACC =Input;
    for(i=8; i>0; i--)
    {  
        T_DIO = ACC_0;            //相当于汇编中的 RRC
        T_SCLK = 1;
        T_SCLK = 0;
        ACC = ACC >> 1; 
    }   
}
//**************读取一字节()*************************
char DS1302_Output_Byte(void)      //从时钟IC读取一字节()
{ 
    char i;   
    for(i=8; i>0; i--)
    {  
     ACC>>=1; T_DIO= 1;
     ACC_7 = T_DIO;
     T_SCLK = 1;                 //相当于汇编中的 RRC 
     T_SCLK = 0;       
    } 
	T_DIO = 0;
    return(ACC); 
}
//**************向时钟IC写数据*************************
void DS1302_Write_one( char addr,dat )       // 写入地址、数据子程序 
{
       T_CE=0;                             //T_CE引脚为低，数据传送中止
       T_SCLK=0;                          //清零时钟总线
       T_CE = 1;                          //T_CE引脚为高，逻辑控制有效
       DS1302_Input_Byte(addr);           // 地址，命令 
       DS1302_Input_Byte(dat);          // 写1Byte数据
       T_SCLK = 1;
       T_CE = 0;     
}
//************从数据读取数据*****************************
char DS1302_Read ( char addr )    //数据读取子程序
{
       char date;
       T_CE=0;           
       T_SCLK=0;             
       T_CE = 1; 
       DS1302_Input_Byte(addr);        // 地址，命令 
       date = DS1302_Output_Byte();         // 读1Byte数据
       T_SCLK = 1;
       T_CE = 0;
       return(date);
}
//************从时间写入数据****************************
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

//时间转换为显示格式
void timeConvert()
{
	sec_1 = sec>>4;
	sec_2 = sec&0x0f;
	min_1 = min>>4;
	min_2 = min&0x0f;
	hour_1 = hour>>4;
	hour_2 = hour&0x0f;
	
}
  
//************从数据读取时间*****************************
void DS1302_readtime()                           
{
    sec=DS1302_Read(0x81);                    //读秒
    min=DS1302_Read(0x83);                  //读分
    hour=DS1302_Read(0x85);                   //读时
    day=DS1302_Read(0x87);                    //读日
    month=DS1302_Read(0x89);                  //读月
    year=DS1302_Read(0x8d);                   //读年
    week=DS1302_Read(0x8b);                   //读星期
	timeConvert();
}
/*   显示转换函数 */
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
	DS1302_Write_one(0x8e,0x00);    //打开保护
	DS1302_Write_one(0x80,0x80);
	DS1302_Write_one(0x8e,0x80);    //回复保护
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
	if(DS1302_Read(0x81)&0x80)
	{
		DS1302_Write(sec,min,hour,day,month,week,year);
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