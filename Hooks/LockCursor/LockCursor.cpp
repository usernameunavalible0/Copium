#include "LockCursor.h"
#include "../../Features/Menu/Menu.h"

using namespace Hooks;

void __fastcall VGuiSurface::LockCursor::Detour(void* ecx, void* edx)
{
	if (F::Menu.is_open())
		I::VGuiSurface->UnlockCursor();
	else
		Table.Original<FN>(Index)(ecx, edx);
}

void VGuiSurface::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&LockCursor::Detour, LockCursor::Index) == FAILED_TO_HOOK);
}