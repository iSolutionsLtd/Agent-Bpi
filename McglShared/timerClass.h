#ifndef _TIMER_CLASS
#define _TIMER_CLASS

#include <signal.h>
#include <time.h>
//#include <asm/siginfo.h>

class TimerClassNot
{
public:
	virtual void OnTimer(uint32_t timerID)=0;
};

class TimerClass 
{
public:
//constructor
	TimerClass()
	{
		 pNotCls=NULL;
		 timerID=0;
	}
	
	~TimerClass()
	{
		stop();
	}
	uint32_t setTimer(TimerClassNot* pNot,uint32_t period,uint32_t delay=0)
	{
		pNotCls=pNot;
		// Define the timer specification
		// One second till first occurrence
		this->timerSpecs.it_value.tv_sec = period/1000;
		this->timerSpecs.it_value.tv_nsec = (period%1000) * 1000000 ;
		// and then all 3 seconds a timer alarm
		this->timerSpecs.it_interval.tv_sec = period/1000;;
		this->timerSpecs.it_interval.tv_nsec = (period%1000) * 1000000 ;
	 
		// Clear the sa_mask
		sigemptyset(&this->SignalAction.sa_mask);
		// set the SA_SIGINFO flag to use the extended signal-handler function
		this->SignalAction.sa_flags = SA_SIGINFO;
	 
		// Define sigaction method
		// This function will be called by the signal
		this->SignalAction.sa_sigaction = TimerClass::alarmFunction;
	 
		// Define sigEvent
		// This information will be forwarded to the signal-handler function
		memset(&this->signalEvent, 0, sizeof(this->signalEvent));
		// With the SIGEV_SIGNAL flag we say that there is sigev_value
		this->signalEvent.sigev_notify = SIGEV_SIGNAL;
		// Now it's possible to give a pointer to the object
		this->signalEvent.sigev_value.sival_ptr = (void*) this;
		// Declare this signal as Alarm Signal
		this->signalEvent.sigev_signo = SIGALRM;
	 
		// Install the Timer
		if (timer_create(CLOCK_REALTIME, &this->signalEvent, &this->timerID)!= 0) 
		{ 
			// timer id koennte mit private probleme geben
			LOG(ERROR) << "Could not creat the timer";
			return 0;
		}
	 
		// Finally install tic as signal handler
		if (sigaction(SIGALRM, &this->SignalAction, NULL)) 
		{
			stop();
			LOG(ERROR) << "Could not install new signal handler";
			return 0;
		}
		
		// Set the timer and therefore it starts...
		if (timer_settime(this->timerID, 0, &this->timerSpecs, NULL) == -1) 
		{
			stop();
			LOG(ERROR) << "Could not start timer:";
			return 0;
		}
		LOG(INFO) << "Timer Started";
		return (uint32_t)this->timerID;
	}
	
	void setNotifyCls(TimerClassNot* pNot)
	{
		pNotCls=pNot;
	}
	
	void stop()
	{
		if (timerID!=0)
		{
			timer_delete(timerID);
			timerID=0;
		}
	}
	/**
	* The signal handler function with extended signature
	*/
	static void alarmFunction(int sigNumb, siginfo_t *si, void *uc) 
	{
		//LOG(INFO) << "alarmFunction";
		// get the pointer out of the siginfo structure and asign it to a new pointer variable
		TimerClass * ptrTimerClass = reinterpret_cast<TimerClass *> (si->si_value.sival_ptr);
		// call the member function
		ptrTimerClass->memberAlarmFunction();
	}
	 
	// Stored timer ID for alarm
	timer_t timerID;
	 
	// Signal blocking set
	sigset_t SigBlockSet;
	 
	// The according signal event containing the this-pointer
	struct sigevent signalEvent;
	 
	// Defines the action for the signal -> thus signalAction <img class="emoji" draggable="false" alt="í¼ í¹¢ src="https://s.w.org/images/core/emoji/72x72/1f609.png">
	struct sigaction SignalAction;
	 
	// The itimerspec structure for the timer
	struct itimerspec timerSpecs;
	
	TimerClassNot* pNotCls;
private:
	// demo member
	
	 
	void memberAlarmFunction() 
	{
		// of course we can access our object within this member function as we are use to it
		if (pNotCls)
			pNotCls->OnTimer((uint32_t)timerID);
		//std::cout << "Timer expired!! Signal occurred." ;
	}
};

#endif //_TIMER_CLASS
