#pragma once

#include <functional>

class Timer
{
public:
	Timer();
	void update(const double delta);
	void start(const double duration);
	void setCallback(std::function<void()> callback);
	bool isRunning() { return m_isRunning; }
private:
	std::function<void()> m_callback;
	double m_timeLeft;
	bool m_isRunning;
};