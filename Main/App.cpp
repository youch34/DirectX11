#include "stdafx.h"
#include "App.h"
#include <sstream>

App::App()
	:wnd(780,640,L"나의 윈도우")
{
	cam.SetLens(0.25 * 3.14f, 640.0f / 780.0f, 0.5f, 10000.0f);
	cam.LookAt(XMFLOAT3(0.0f, 50.0f, -50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	//ClipMouse();
	CameraSpeed = 10;
	DeltaTime = 1 / FPS;
}

int App::MainFrame()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessage())
			return *ecode;
		static float curTime = 0;
		curTime += timer.Mark();
		if (curTime > DeltaTime)
		{
			DoFrame();
			curTime = 0;
		}
	}
}

void App::DoFrame()
{
	cam.UpdateViewMatrix();
	std::ostringstream oss;
	oss << "Mouse Pos : " << wnd.mouse.GetPosX() << ", " << wnd.mouse.GetPosY();
	static float framecount = 0;
	static float curtime = 0;
	framecount += 1;
	curtime += DeltaTime;
	if (curtime > 1)
	{
		wnd.SetTitle(std::to_string(framecount));
		framecount = 0;
		curtime = 0;
	}
	//wnd.SetTitle( oss.str() );
	wnd.Gfx().ClearBuffer(0.2f, 0.2f, 0.2f);
	wnd.Gfx().Upate(cam);
	wnd.Gfx().Render(cam);

	CameraMove();
	OnMouseMove();

	wnd.Gfx().ImGuiRender();

	wnd.Gfx().EndFrame();
}

void App::CameraMove()
{
	if (GetKeyState(VK_SHIFT) & 0x8000)
		CameraSpeed = 30;
	else
		CameraSpeed = 10;
	
	if (GetKeyState('A') & 0x8000)
	{
		cam.Strafe(-DeltaTime * CameraSpeed);
	}
	else if (GetKeyState('D') & 0x8000)
	{
		cam.Strafe(DeltaTime * CameraSpeed);
	}
	if (GetKeyState('W') & 0x8000)
	{
		cam.Walk(DeltaTime * CameraSpeed);
	}
	else if (GetKeyState('S') & 0x8000)
	{
		cam.Walk(-DeltaTime * CameraSpeed);
	}
}

void App::OnMouseMove()
{
	if (GetKeyState(VK_RBUTTON) & 0x8000)
	{	
		auto x = wnd.mouse.GetPosX();
		auto y = wnd.mouse.GetPosY();
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - lastMousePos.first));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - lastMousePos.second));
		cam.RotateY(dx);
		cam.Pitch(dy);
	}
	lastMousePos = wnd.mouse.GetPos();
	
}

void App::ClipMouse()
{
	RECT rc;
	POINT p1, p2;
	GetClientRect(wnd.GetHandle(),&rc);

	p1.x = rc.left;
	p1.y = rc.top;
	p2.x = rc.right;
	p2.y = rc.bottom;

	ClientToScreen(wnd.GetHandle(), &p1);
	ClientToScreen(wnd.GetHandle(), &p2);

	rc.left = p1.x;
	rc.top = p1.y;
	rc.right = p2.x;
	rc.bottom = p2.y;

	ClipCursor(&rc);
}
