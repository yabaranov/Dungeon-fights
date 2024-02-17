#pragma once

#include <functional>

class Timer
{
public:
	Timer();
	void update(const double delta);
	void start(const double duration);
	void setCallback(std::function<void()> callBack);

private:
	std::function<void()> m_callBack;
	double m_timeLeft;
	bool m_isRunning;
};