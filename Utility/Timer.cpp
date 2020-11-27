#include "stdafx.h"
#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}

float Timer::Mark()//last�� �����ϰ� ������ ��Ʈ���� ����
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Timer::Peek() const // last�� ������ ����
{
	return duration<float>(steady_clock::now() - last).count();
}
