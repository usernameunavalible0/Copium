#include "WndProc.h"

#include "../../Features/Menu/Menu.h"

using namespace Hooks;

LRESULT CALLBACK WndProc::Detour(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
		case WM_KEYDOWN: {
			gInput.m_Keyboard[wParam] = EKeyState::PRESSED;
			break;
		}
	
		case WM_KEYUP: {
			gInput.m_Keyboard[wParam] = EKeyState::NONE;
			break;
		}
	
		case WM_RBUTTONDOWN:
		{
			if (wParam & MK_RBUTTON)
				gInput.m_RMouse = EKeyState::PRESSED;
	
			break;
		}
	
		case WM_LBUTTONDOWN:
		{
			if (wParam & MK_LBUTTON)
				gInput.m_LMouse = EKeyState::PRESSED;
	
			break;
		}
	
		case WM_MBUTTONDOWN:
		{
			if (wParam & MK_MBUTTON)
				gInput.m_MMouse = EKeyState::PRESSED;
	
			break;
		}
	
		case WM_RBUTTONUP:
		{
			gInput.m_RMouse = EKeyState::NONE;
			break;
		}
	
		case WM_LBUTTONUP:
		{
			gInput.m_LMouse = EKeyState::NONE;
			break;
		}
	
		case WM_MBUTTONUP:
		{
			gInput.m_MMouse = EKeyState::NONE;
			break;
		}
	
		case WM_MOUSEWHEEL: {
			gInput.m_Wheel = ((int)wParam < 0 ? EScrollWheelState::UP : EScrollWheelState::DOWN);
			break;
		}
	}

	//if (gMenu.m_Open && (!I::EngineClient->Con_IsVisible()))
	//	return 1;

	//return (gMenu.m_Open && (I::EngineVGui->IsGameUIVisible() || I::EngineClient->Con_IsVisible())) ? 1 : CallWindowProcW(oWndProc, hwnd, uMsg, wParam, lParam);

	//return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
	
	return (gMenu.m_Open && (I::EngineVGui->IsGameUIVisible() || !I::EngineClient->Con_IsVisible())) ? 1ul : CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

void WndProc::Initialize()
{
	while (!hwGame) 
	{
		hwGame = FindWindowW(L"Valve001", nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongW(hwGame, GWL_WNDPROC, reinterpret_cast<LONG>(Detour)));
}

void WndProc::Uninitialize()
{
	if (oWndProc)
		SetWindowLongW(hwGame, GWL_WNDPROC, reinterpret_cast<LONG>(oWndProc));
}