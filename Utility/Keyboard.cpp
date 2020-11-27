#include "stdafx.h"
#include "Keyboard.h"

class Keyboard::Event;

Keyboard::Keyboard()
{
}

bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey()
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else
		return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty() const
{
	return keybuffer.empty();
}

void Keyboard::FlushKey()
{
	keybuffer = std::queue<Keyboard::Event>();
}

char Keyboard::ReadChar()
{
	if (charbuffer.size() > 0u)
	{
		char c = charbuffer.front();
		charbuffer.pop();
		return c;
	}
	else
		return 0;
}

bool Keyboard::CharIsEmpty()
{
	return charbuffer.empty();
}

void Keyboard::FlushChar()
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush()
{
	FlushChar();
	FlushKey();
}

void Keyboard::EnalbleAutorepeat()
{
	autorepeatEnable = true;
}

void Keyboard::DisableAutorepeat()
{
	autorepeatEnable = false;
}

bool Keyboard::AutorepeatIsEnable() const
{
	return autorepeatEnable;
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	keystates[keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character)
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Keyboard::ClearState()
{
	keystates.reset();
}

