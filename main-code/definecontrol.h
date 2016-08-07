/*--------------------------------------------------------------------------
DEFINRCONTROL.H

Module Chart.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
/* module*/
char chart[12] = {0x81,0xcf,0x25,0x0d,0x4b,0x19,0x11,0xcd,0x01,0x09,0x7f,0xb1};    //0,1,2,3,4,5,6,7,8,9,-,C
char allclear = 0xff;
/*  四个8的控制引脚  */
sbit	E1=P2^6;
sbit	E2=P2^7;
sbit	E3=P5^4;
sbit	E4=P5^5;
/*  ABLIGHTDEFG的控制引脚  */
sbit	ALIGHT=P1^1;
sbit	BLIGHT=P1^4;
sbit	CLIGHT=P1^5;
sbit	DLIGHT=P1^6;
sbit	ELIGHT=P1^3;
sbit	FLIGHT=P1^2;
sbit	GLIGHT=P1^7;
sbit    MDLIGHT=P1^0;
/*  按钮的控制引脚  */
sbit    KEY_1 = P3^1;
sbit    KEY_2 = P3^0;
sbit    KEY_3 = P3^2;   
/*   温度控制器  */
sbit    DQ = P3^3;