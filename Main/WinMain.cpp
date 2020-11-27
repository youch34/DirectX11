#pragma once
#include "stdafx.h"
#include "App.h"
#include "Main/Window.h"
#include <Windows.h>

#define WIN32_LEAN_AND_MEAN //외부 MFC오버헤더를 포함시키지 않는다
#define NOMINMAX

int CALLBACK WinMain
(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow
) 
{
	try
	{
		return App{}.MainFrame();
	}
	catch (const Window::WndException& e)
	{
		MessageBoxA(nullptr, e.what(),e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}