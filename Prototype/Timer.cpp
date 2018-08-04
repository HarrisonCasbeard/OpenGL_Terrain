#include "Timer.h"
#include <glfw3.h>

Timer::Timer()
{
	m_deltaTime = 0.0;
	m_lastFrame = 0.0;
}

Timer::~Timer()
{
}

//-------------------------------------------------------------------------------------------------
void Timer::update()
{
	double currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
}

//-------------------------------------------------------------------------------------------------
double Timer::getDeltaTime()
{
	return m_deltaTime;
}