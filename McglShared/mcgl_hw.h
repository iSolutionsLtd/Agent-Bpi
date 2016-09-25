#ifndef _MCGL_HW
#define _MCGL_HW

//#include "timerClass.h"
#include "mcgl_timer.h"




struct LedState
{
	int iPin;
	bool bOn;
	bool bBlink;
	bool bFast;
	uint32_t BlinkCount;
};

class CMcglHW : public CMcglTimerNot
{
public:
	CMcglHW(void);
	~CMcglHW(void);
	
	void startHW();
	void stopHW();
	void setPendingLed(bool bON=true,uint32_t bBlink=0,bool bFast=false);
	void setUploadLed(bool bON=true,uint32_t bBlink=0,bool bFast=false);
	int getPendingLed();
	int getUploadLed();
	
	LedState leds[2];
	
	void OnMcglTimer();
	
	
protected:
	uint8_t bLoopCount;
	CMcglTimer timer;
};

#endif //_MCGL_HW