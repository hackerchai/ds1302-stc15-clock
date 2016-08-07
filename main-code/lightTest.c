/*--------------------------------------------------------------------------
CHARTTEST.C

Test chart chart and print figures.
Copyright (c) 2015-2016 Northeastern Yucai School,Holy Aliance.
All rights reserved.
--------------------------------------------------------------------------*/
#include <STC15W402AS.h>
#include <intrins.h>
#include <definecontrol.h>

/*   ∫¡√Î—”≥Ÿ∫Ø ˝ */
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

void init()
{
	//Light off all
    E1=1;
    E2=1;
    E3=1;
    E4=1;
	P1 = allclear;
}

void  main()
{
	while(1)
	{
		//Light1 show 1
		E1 = 0;
	    P1 = chart[8];
	    delayMS(5);
		E1 = 1;
		
		//Light2 show 2
		E2 = 0;
		P1 = chart[8];
		delayMS(5);
		E2 = 1;
		
		//Light3 show 3
		E3 = 0;
		P1 = chart[8];
		delayMS(5);
		E3 = 1;
		
		//Light4 show 4
		E4 = 0;
		P1 = chart[8];
		delayMS(5);
		E4 = 1;
	}
	
	
	
}

