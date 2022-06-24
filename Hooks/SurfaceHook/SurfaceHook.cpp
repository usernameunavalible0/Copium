#include "SurfaceHook.h"

#include "../../Features/Menu/Menu.h"

using namespace Hooks;

void __fastcall VGuiSurface::LockCursor::Detour()
{
	gMenu.m_Open ? I::VGuiSurface->UnlockCursor() : Table.Original<FN>(Index)(I::VGuiSurface);
}

void VGuiSurface::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&LockCursor::Detour, LockCursor::Index) == FAILED_TO_HOOK);
}