
#include "mcgl_timer.h"

CMcglTimer::CMcglTimer()
{
	pNotCls=NULL;
	uTimerPeriod=0;
	uLastTime=0;
	_thread=0;
	bStop=false;
}

CMcglTimer::~CMcglTimer()
{
	stop();
}

int CMcglTimer::setTimer(CMcglTimerNot* pNot,uint32_t uPeriod)
{
   uTimerPeriod = uPeriod;
   pNotCls=pNot;
   bStop=false;
   int code = pthread_create(&_thread, NULL, &CMcglTimer::EntryPoint, this);
   LOG(INFO) << "timer start code " << code;
   return code;
}

int CMcglTimer::Run()
{
   Setup();
   Execute();
   return 1;
}

/*static */
void * CMcglTimer::EntryPoint(void * pthis)
{
   CMcglTimer * pt = (CMcglTimer*)pthis;
   pt->Run( );
   return NULL;
}

void CMcglTimer::Setup()
{
	uLastTime=CMcglTimer::GetTickCount();
}

void CMcglTimer::Execute()
{
	uint32_t now;
    while (!bStop)
	{
		usleep(1000);
		now = CMcglTimer::GetTickCount();
		if (abs((int)now-(int)uLastTime)>=(int)uTimerPeriod)
		{
			if (pNotCls)
				pNotCls->OnMcglTimer();
			uLastTime=now;
		}
	}
	 LOG(INFO) << "Execute End";
}

void CMcglTimer::stop()
{
	bStop=true;
	pthread_join(_thread, NULL);

}

uint32_t CMcglTimer::GetTickCount()
{
	struct timeval tv;
	if(gettimeofday(&tv, NULL) != 0)
		return 0;

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

}