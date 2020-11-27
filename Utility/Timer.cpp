#include "stdafx.h"
#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}

float Timer::Mark()//last를 갱신하고 마지막 라스트와의 차이
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Timer::Peek() const // last와 지금의 차이
{
	return duration<float>(steady_clock::now() - last).count();
}
