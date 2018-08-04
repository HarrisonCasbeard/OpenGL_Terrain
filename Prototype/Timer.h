#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
private:
	double m_deltaTime;
	double m_lastFrame;

public:
	 Timer();
	~Timer();

	void update	();

	double getDeltaTime();
};

#endif
