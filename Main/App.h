#pragma once
#include "Main/Window.h"
#include "Utility/Timer.h"
#include "Object/Camera.h"
#include <utility>
class App 
{
public:
	App();

	int MainFrame();
private:
	void DoFrame();
public:
	void CameraMove();
	void OnMouseMove();
	void ClipMouse();
private:
	Window wnd;
	Timer timer;
	Camera cam;
private:
	std::pair<int, int> lastMousePos;
	float DeltaTime;
	float CameraSpeed = 1;
	float FPS = 60;
};