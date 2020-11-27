#pragma once
#include "Main/Graphics.h"

#include "Utility/Exception.h"
#include "Utility/Keyboard.h"
#include "Utility/Mouse.h"

#include <optional>

class Window 
{
public:
	class WndException : public Exception
	{
	public:
		WndException(int line, const char* file, HRESULT hr);
		const char* what() const override;
		virtual const char* GetType() const;
		static std::string TranslationErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
	private:
		HRESULT hr;

	};
private:
	class WindowClass 
	{
	public:
		static const TCHAR* GetName() { return wndClassName; }
		static HINSTANCE GetInstance() { return wndClass.hInst; }
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const TCHAR* wndClassName = L"나의 윈도우";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const TCHAR* name);
	~Window();
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessage();
	class Graphics& Gfx() { return *pGfx; }
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	const HWND& GetHandle() const { return hWnd; }
public:
	Keyboard kbd;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;

};
#define WND_EXCEPTION(hr) Window::WndException(__LINE__,__FILE__,hr); 