/*--------------------------------------------------------------------------
DS1302.H

The utilities for module DS1302
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/

sbit ACC_0 = ACC^0;
sbit ACC_1 = ACC^1;
sbit ACC_2 = ACC^2;
sbit ACC_3 = ACC^3;
sbit ACC_4 = ACC^4;
sbit ACC_5 = ACC^5;
sbit ACC_6 = ACC^6;
sbit ACC_7 = ACC^7; 
//**************************
sbit T_SCLK = P1^1;                    // DS1302时钟信号       7脚
sbit T_DIO= P1^2;                      // DS1302数据信号       6脚
sbit T_CE = P1^3;
