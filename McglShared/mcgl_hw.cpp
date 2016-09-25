
#include "mcgl_hw.h"
#include <wiringBP/wiringPi.h>

#define PENDING_LED 11
#define UPLOAD_LED	10

#define PENDING 0
#define UPLOAD  1

CMcglHW::CMcglHW(void)
{
	bLoopCount=0;
}

CMcglHW::~CMcglHW(void)
{
	digitalWrite (PENDING_LED, LOW);
	digitalWrite (UPLOAD_LED, LOW);
}
	
void CMcglHW::startHW()
{
	wiringPiSetup();
	pinMode (PENDING_LED, OUTPUT); 
	digitalWrite (PENDING_LED, LOW);
	pinMode (UPLOAD_LED, OUTPUT);
	digitalWrite (UPLOAD_LED, LOW);
	
	memset(leds,0,sizeof(LedState)*2);
	leds[PENDING].iPin=PENDING_LED;
	leds[UPLOAD].iPin=UPLOAD_LED;
	
	timer.setTimer(this,125);
}

void CMcglHW::stopHW()
{
	timer.stop();
	digitalWrite (PENDING_LED, LOW);
	digitalWrite (UPLOAD_LED, LOW);
}

void CMcglHW::setPendingLed(bool bON,uint32_t bBlink,bool bFast)
{
	if (bBlink>0)
	{
		if (bON)
		{
			leds[PENDING].bBlink=true;
			leds[PENDING].BlinkCount=bBlink*2;
			leds[PENDING].bFast=bFast;
		}else
		{
			leds[PENDING].bBlink=false;
			leds[PENDING].bFast=false;
			leds[PENDING].BlinkCount=0;
			digitalWrite (PENDING_LED, leds[PENDING].bOn?HIGH:LOW);
		}
		
	}else
	{
		if (leds[PENDING].bOn!=bON)
		{
			leds[PENDING].bOn=bON;
			if (!leds[PENDING].bBlink)
				digitalWrite (PENDING_LED, bON?HIGH:LOW);
		}
	}
	
}

void CMcglHW::setUploadLed(bool bON,uint32_t bBlink,bool bFast)
{
	
	if (bBlink>0)
	{
		if (bON)
		{
			leds[UPLOAD].bBlink=true;
			leds[UPLOAD].BlinkCount=bBlink*2;
			leds[UPLOAD].bFast=bFast;
		}else
		{
			leds[UPLOAD].bBlink=false;
			leds[UPLOAD].bFast=false;
			leds[UPLOAD].BlinkCount=0;
			digitalWrite (UPLOAD_LED, leds[UPLOAD].bOn?HIGH:LOW);
		}
		
	}else
	{
		leds[UPLOAD].bOn=bON;
		if (!leds[UPLOAD].bBlink)
			digitalWrite (UPLOAD_LED, bON?HIGH:LOW);
	}
	
	
}

int CMcglHW::getPendingLed()
{
	if (leds[PENDING].bFast)
		return 3;
	if (leds[PENDING].bBlink)
		return 2;
	if (leds[PENDING].bOn==HIGH)
		return 1;
	return 0;
}

int CMcglHW::getUploadLed()
{
	if (leds[UPLOAD].bFast)
		return 3;
	if (leds[UPLOAD].bBlink)
		return 2;
	if (leds[UPLOAD].bOn==HIGH)
		return 1;
	return 0;
}
	
void CMcglHW::OnMcglTimer()
{
	bLoopCount++;
	for (int x=0;x<2;x++)
	{
		if (leds[x].bBlink)
		{
			if ((!leds[x].bFast) && (bLoopCount%2))
				continue;
			if (leds[x].BlinkCount>0)
			{
				bool bState=(bool)digitalRead(leds[x].iPin);
				digitalWrite(leds[x].iPin, bState?LOW:HIGH);
				leds[x].BlinkCount--;
			}else
			{
				leds[x].bBlink=false;
				leds[x].BlinkCount=0;
				digitalWrite (leds[x].iPin, leds[x].bOn?HIGH:LOW);
			}
		}
	}
}