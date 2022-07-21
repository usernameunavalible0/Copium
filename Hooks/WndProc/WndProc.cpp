#include "WndProc.h"

#include "../../Features/Menu/Menu.h"

using namespace Hooks;

LRESULT CALLBACK WndProc::Detour(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	//return (gMenu.m_Open && (I::EngineVGui->IsGameUIVisible() || I::EngineClient->Con_IsVisible())) ? 1 : CallWindowProcW(oWndProc, hwnd, uMsg, wParam, lParam);

	return (F::Menu.is_open() && (I::EngineVGui->IsGameUIVisible() || !I::EngineClient->Con_IsVisible())) ? 1ul : CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
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