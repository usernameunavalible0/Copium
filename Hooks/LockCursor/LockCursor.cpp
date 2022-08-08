#include "LockCursor.h"

using namespace Hooks;

void __fastcall VGuiSurface::OnScreenSizeChanged::Detour(void* ecx, void* edx, int oldWidth, int oldHeight)
{
	Table.Original<FN>(Index)(oldWidth, oldHeight);
}

void VGuiSurface::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&OnScreenSizeChanged::Detour, OnScreenSizeChanged::Index) == FAILED_TO_HOOK);
}