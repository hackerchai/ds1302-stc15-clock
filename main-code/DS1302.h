/*--------------------------------------------------------------------------
DS1302.H

The utilities for module DS1302
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
/*��Ƭ���ۼ���*/
sbit ACC_0 = ACC^0;
sbit ACC_1 = ACC^1;
sbit ACC_2 = ACC^2;
sbit ACC_3 = ACC^3;
sbit ACC_4 = ACC^4;
sbit ACC_5 = ACC^5;
sbit ACC_6 = ACC^6;
sbit ACC_7 = ACC^7; 
/*DS1302ʱ��ģ������*/
sbit T_SCLK = P2^5;                    // DS1302ʱ���ź� 
sbit T_DIO= P2^4;                      // DS1302�����ź�
sbit T_CE = P2^3;