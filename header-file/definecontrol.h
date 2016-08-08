/*--------------------------------------------------------------------------
DEFINRCONTROL.H

Module Chart.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/

/* module*/

char chart[10] = {0x80,0xce,0x24,0x0c,0x4a,0x18,0x10,0xcc,0x00,0x08};
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


/*   温度控制器和1302芯片引脚  */

sbit  DS1302_RST = P1^3;
sbit  DS1302_CLK  = P1^1;
sbit  DS1302_IO  = P1^2;


