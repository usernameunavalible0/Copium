#include "EngineVGui.h"

#include "../../Features/ESP/ESP.h"
#include "../../Features/Visual/Visual.hpp"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Vars.h"
#include "../../SDK/Includes/Icons.h"
#include "../../Features/Menu/Menu.h"

using namespace Hooks;

#define DBG(...) G::Draw.String(EFonts::DEBUG, 5, nY, COLOR_GREY, TXT_DEFAULT, __VA_ARGS__); nY += G::Draw.GetFontHeight(EFonts::DEBUG)

void __fastcall EngineVGui::Paint::Detour(void* ecx, void* edx, int mode)
{
	static const auto pfStartDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(g_Offsets.m_dwStartDrawing);
	static const auto pfFinishDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(g_Offsets.m_dwFinishDrawing);

	Table.Original<FN>(Index)(ecx, edx, mode);

	if (mode & PaintMode_t::PAINT_UIPANELS)
	{
		G::Draw.ReloadMatrix();


		pfStartDrawing(I::MatSystemSurface);
		{
			if (g_Globals.m_bIsInGame && !g_Globals.m_bIsGameUIVisible)
			{
				C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);


				if (pLocal)
				{
					// Old Menu F::menu.draw_fps();
					F::ESP.Render(pLocal);

					F::Misc.WarningRun(pLocal);
				}

			}

			G::Draw.String(EFonts::DEBUG, 5, 5, { 204, 204, 204, 255 }, TXT_DEFAULT, L"Copium TF2 By DepressedPotato");

			gMenu.Run();
		}
		pfFinishDrawing(I::MatSystemSurface);
	}
}

void EngineVGui::Initialize()
{
	XASSERT(Table.Initialize(I::EngineVGui) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&Paint::Detour, Paint::Index) == FAILED_TO_HOOK);
}