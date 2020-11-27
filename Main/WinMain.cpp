#pragma once
#include "stdafx.h"
#include "App.h"
#include "Main/Window.h"
#include <Windows.h>

#define WIN32_LEAN_AND_MEAN //�ܺ� MFC��������� ���Խ�Ű�� �ʴ´�
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