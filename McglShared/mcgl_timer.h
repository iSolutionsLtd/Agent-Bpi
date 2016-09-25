#ifndef _MCGL_TIMER_
#define _MCGL_TIMER_

#include <pthread.h>

class CMcglTimerNot
{
public:
	virtual void OnMcglTimer()=0;
};


class CMcglTimer
{
   public:
      CMcglTimer();
	  ~CMcglTimer();
      int setTimer(CMcglTimerNot* pNot,uint32_t uPeriod);
	  void stop();
   protected:
      int Run();
      static void * EntryPoint(void*);
      virtual void Setup();
      virtual void Execute();
      static uint32_t GetTickCount();
	
private:
	CMcglTimerNot* pNotCls;
	uint32_t uTimerPeriod;
	uint32_t uLastTime;
	pthread_t _thread;
	bool bStop;

};


#endif //_MCGL_TIMER_