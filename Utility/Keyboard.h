#pragma once
#include <queue>
#include <bitset>

class Keyboard 
{
	friend class Window;
	class Event 
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned int code;
	public:
		Event():type(Type::Invalid),code(0u) {}
		Event(Type type, unsigned int code) : type(type), code(code) {}
		bool IsPressed() const { return type == Type::Press; }
		bool IsReleased() const { return type == Type::Release; }
		bool IsValid() const { return type != Type::Invalid; }
		unsigned int GetCode() const { return code; }
	};
public:
	Keyboard();
	Keyboard(const Keyboard&) = delete;
	Keyboard operator=(Keyboard&) = delete;
	//Key Event Stuff
	bool KeyIsPressed(unsigned char keycode) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	void FlushKey();
	//char Event Stuff
	char ReadChar();
	bool CharIsEmpty();
	void FlushChar();
	void Flush();
	//autorepeat controll
	void EnalbleAutorepeat();
	void DisableAutorepeat();
	bool AutorepeatIsEnable() const;
private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);
	void ClearState();
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int buffersize = 16u;
	bool autorepeatEnable = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > buffersize)
	{
		buffer.pop();
	}
}
