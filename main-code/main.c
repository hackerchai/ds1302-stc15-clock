/*--------------------------------------------------------------------------
MAIN.C

Implement all features.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
#define uint unsigned int
#define uchar unsigned char
#include <STC15W402AS.h>
#include <intrins.h>
#include <definecontrol.h>
#include <DS1302.H>
#include<stdio.h>

char sec,min,hour;   //时间变量
char sec_1,sec_2,min_1,min_2,hour_1,hour_2; //位数转换
char year,month,week,day;  //天数变量
int status;     //状态代码
int keyValue;   //按钮代码

uint temp;      //温度数值
uint temp_1,temp_2;   //温度显示拆分
/*
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
      0x90, //15,写涓流充电
      0x91  //16,读涓流充电

}  ;
*/
	 
/*   延迟函数 */
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

void DelayXus(uint us)   //@11.0592MHz
{
	while(us--);
}



/*************DS1302时间模块&DS18B20温度模块相关驱动函数******************************/

/*DS1302驱动函数*/

/*************写入一字节****************/
void DS1302_Input_Byte(char Input)  //向时钟IC写入一字节
{   
    char i;
	T_SCLK = 0;
    delay2us();
    ACC =Input;
    for(i=8; i>0; i--)
    {  
        T_DIO = ACC_0;            //相当于汇编中的 RRC
		delay2us();
        T_SCLK = 1;
		delay2us();
        T_SCLK = 0;
        ACC = ACC >> 1; 
    }  
}

/*************读取一字节****************/
char DS1302_Output_Byte(void)      //从时钟IC读取一字节()
{ 
    char i;   
    for(i=8; i>0; i--)
    {  
     ACC>>=1; 
	 T_DIO= 1;
	 delay2us();
     ACC_7 = T_DIO;
     T_SCLK = 1;                 //相当于汇编中的 RRC 
	 delay2us();
     T_SCLK = 0;   
	 delay2us();
    }
	T_DIO = 0;
    delay2us();
    return(ACC); 
}

/*************写一字节数据****************/
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

/*************读一字节数据****************/
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


/*************写入时间数据****************/
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

/*************时间转换为显示格式****************/
void timeConvert()
{
	sec_1 = sec>>4;
	sec_2 = sec&0x0f;
	min_1 = min>>4;
	min_2 = min&0x0f;
	hour_1 = hour>>4;
	hour_2 = hour&0x0f;
	
}
  
/*************从芯片读取时间****************/
void DS1302_readtime()                           
{
    sec=DS1302_Read(0x81);                    //读秒
    min=DS1302_Read(0x83);                    //读分
    hour=DS1302_Read(0x85);                   //读时
    day=DS1302_Read(0x87);                    //读日
    month=DS1302_Read(0x89);                  //读月
    year=DS1302_Read(0x8d);                   //读年
    week=DS1302_Read(0x8b);                   //读星期
	timeConvert();
	
}



/*   DS18B20温度传感器模块*/
/**********初始化函数**********************/
void DS18B20_Init(void) 
{
	uchar x=0;
	DQ = 1;          //DQ复位
	DelayXus(7); //稍做延时
	DQ = 0;          //单片机将DQ拉低
	DelayXus(500); //精确延时 大于 480us
	DQ = 1;          //拉高总线
	DelayXus(200);  //足够的延迟 确保能让DS18B20发出存在脉冲
}
/***********读一个字节**************/
uchar DS18B20_ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // 给脉冲信号
		DelayXus(1);
		dat>>=1;
		DQ = 1; // 给脉冲信号
		DelayXus(7);
		if(DQ)
			dat|=0x80;
		DelayXus(60);    
	}	
	return(dat);
}
/*************写一个字节****************/
void DS18B20_WriteOneChar(uchar dat)
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DelayXus(1);
		DQ = dat&0x01;
		DelayXus(60);
		DQ = 1;
		dat>>=1;
		DelayXus(7);
	}
}
/**************读取当前温度************/
uint DS18B20_ReadTemp(void)
{
	uchar a;
	uchar b;
	uint readTemp;
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC);     
	DS18B20_WriteOneChar(0x44);  
	while (!DQ);
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC);  
	DS18B20_WriteOneChar(0xBE);  
	a=DS18B20_ReadOneChar();    //读取温度值低位
	b=DS18B20_ReadOneChar();      //读取温度值高位
	readTemp = b<<8;
	readTemp |= a; 
	readTemp = readTemp>>4;
	return readTemp;    
}

/*   显示相关函数*/

/**************温度显示函数************/
void tempshow(int shi,int ge)
{
	E1 = 1;
	P1 = allclear;	    
	
	
	E2 = 0;
	P1 = chart[shi];
    delayMS(5);
	P1 = allclear;
	E2 = 1;
	
	E3 = 0;
	P1 = chart[ge];	
	delayMS(5);
	P1 = allclear;
	E3 = 1;
	
	E4 = 0;
	P1 = chart[11];	
	delayMS(5);
	P1 = allclear;
	E4 = 1;
	
	MDLIGHT = 1;
}
/* ************显示转换函数******************************/
void convertShow(char hour_s1,hour_s2,min_s1,min_s2)
{
	
		E1 = 0;
	    P1 = chart[hour_s1];	    
		delayMS(5);
		P1 = allclear;	 
		E1 = 1;
		
		E2 = 0;
	    P1 = chart[hour_s2];
		delayMS(5);
		P1 = allclear;
		E2 = 1;
		
	    E3 = 0;
	    P1 = chart[min_s1];	
		delayMS(5);
		P1 = allclear;
		E3 = 1;
		
		E4 = 0;
	    P1 = chart[min_s2];	
		delayMS(5);
		P1 = allclear;
		E4 = 1;
		if(sec_2%0x02==0x01)
		{
			MDLIGHT = 0;
		}
		else
			MDLIGHT = 1;
		
}

/*************按钮显示延时******************************/
void delayShow(int count)
{
	int j;
	timeConvert();
	for(j=0;j<count;j++)
	{
		convertShow(hour_1,hour_2,min_1,min_2);
	}
}

/*************温度处理函数******************************/
void tempHandle()
{
	temp_1 = temp%100/10;
	temp_2 = temp%10;
}

/*************修改时间函数******************************/
void modifyTime(int mode)  //mode指明修改小时或分钟   1：小时   2：分钟
{

	   //修改小时（1）+递增（8）
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
	
	   //修改小时（1）+递减（16）
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
	
	   //修改分钟（2）+递增（8）
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
	
	   //修改分钟（2）+递减（16）
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

/*************按钮扫描函数******************************/
void keyScan()     
{
	if (KEY_1==0 || KEY_2==0 || KEY_3==0)
	{
		delayMS(20);   		//20毫秒软件防抖
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

/*************按钮响应处理函数******************************/
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

/*************单片机初始化函数******************************/
void init()
{
	//关闭所有小灯
    E1=1;
    E2=1;
    E3=1;
    E4=1;
	MDLIGHT = 1;
	P1 = allclear;
	T_CE = 0;
	T_SCLK = 0;
	
	//默认初始化时间12:00
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
	//DS1302初始化判断是否存在后备电源
	
	if(DS1302_Read(0x81)&0x80==0x80)
	{
		DS1302_Write_one(0x8e,0x00);
		DS1302_Write_one(0x80,sec);  //起振
		DS1302_Write_one(0x8e,0x80);
	}
	else
	{
		
	}
}

/*************主函数******************************/
void main()
{
	init();
	
	while(1)
	{
		if(status==3)
		{
			temp = DS18B20_ReadTemp();
		    tempHandle();
			tempshow(temp_1,temp_2);
		}
		else
		{
			DS1302_readtime();
			convertShow(hour_1,hour_2,min_1,min_2);
		}
		keyScan();
		if(keyValue!=0&&KEY_1==1&&KEY_2==1&&KEY_3==1)
		{
			keyHandle();
		}
	}
}