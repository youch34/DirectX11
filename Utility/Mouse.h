#pragma once
#include <queue>
#include <optional>
class Mouse 
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x, y;
	public:
		Event(Type type, const Mouse& parent)
			:type(type), leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed),
			x(parent.x), y(parent.y)
		{}
		Type GetType() const { return type; }
		std::pair<int, int> GetPos() const { return{ x, y }; }
		int GetPosX() const { return x; }
		int GetPosY() const { return y; }
		bool LeftIsPressed() const  { return leftIsPressed; }
		bool RightIsPressed() const { return rightIsPressed; }	
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const;
	std::pair<int, int> GetDelta() const;
	int GetPosX() const;
	int GetPosY() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	std::optional<Mouse::Event> Read();
	bool IsEmpty() const 
	{
		return buffer.empty();
	}
	void Flush();
private:
	void OnMouseMove(int x, int y);
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x , int y);
	void OnWheelDelta(int x, int y, int delta);
	void TrimBuffer();
private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int deltax;
	int y;
	int deltay;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	std::queue<Event> buffer;
	int WheelDeltaCarry = 0;
};