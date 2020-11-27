#include "stdafx.h"
#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const
{
	return {x, y};
}

std::pair<int, int> Mouse::GetDelta() const
{
	return { deltax, deltay };
}

int Mouse::GetPosX() const
{
	return x;
}

int Mouse::GetPosY() const
{
	return y;
}

bool Mouse::LeftIsPressed() const
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const
{
	return rightIsPressed;
}

std::optional<Mouse::Event> Mouse::Read()
{
	if (bufferSize > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return {};
}

void Mouse::Flush()
{
	buffer = std::queue<Mouse::Event>();
}

void Mouse::OnMouseMove(int x, int y)
{
	deltax = x - this->x;
	deltay = y - this->y;

	this->x = x;
	this->y = y;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsPressed = true;
	buffer.push({ Mouse::Event::Type::LPress, *this });
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsPressed = false;
	buffer.push({ Mouse::Event::Type::LRelease, *this });
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsPressed = true;
	buffer.push({ Mouse::Event::Type::RPress, *this });
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsPressed = false;
	buffer.push({ Mouse::Event::Type::RRelease, *this });
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	buffer.push({Mouse::Event::Type::WheelUp, *this});
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	buffer.push({ Mouse::Event::Type::WheelDown, *this });
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
	WheelDeltaCarry += delta;

	while (WheelDeltaCarry >= WHEEL_DELTA) 
	{
		WheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (WheelDeltaCarry <= -WHEEL_DELTA)
	{
		WheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimBuffer()
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
