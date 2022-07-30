#include "LockCursor.h"

using namespace Hooks;

void __fastcall VGuiSurface::LockCursor::Detour()
{
	Table.Original<FN>(Index)();
}

void __fastcall VGuiSurface::OnScreenSizeChanged::Detour(int oldWidth, int oldHeight)
{
	Table.Original<FN>(Index)(oldWidth, oldHeight);
}

void VGuiSurface::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&LockCursor::Detour, LockCursor::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&OnScreenSizeChanged::Detour, OnScreenSizeChanged::Index) == FAILED_TO_HOOK);
}