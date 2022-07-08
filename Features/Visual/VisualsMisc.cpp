#include "Visual.hpp"
#include "../Vars.h"

void CFeatures_Visual::DoSpectators()
{
	auto spectators = F::Misc.get_spectators();
	int height;
	int width;

	if (spectators.size() > 0)
		I::VGuiSurface->GetTextSize(G::Draw.m_Fonts.Find(EFonts::MENU_TAHOMA), spectators[0].c_str(), width, height);

	//G::Draw.OutlinedRect(123, 123, width + 4, height + 4, { 0, 0, 0, 255 });
	//G::Draw.Rect(200, 200, width + 2, height + 2, Vars::Menu::Colors::WindowBackground);
	//G::Draw.String(EFonts::MENU_TAHOMA, 125, 125, Vars::Menu::Colors::Text, TXT_CENTERXY, L"stupid");
}

void CFeatures_Visual::Run(C_TFPlayer* pLocal, CViewSetup* pViewSetup)
{

}

bool CFeatures_Visual::RemoveScope(int nPanel)
{
	if (!Vars::Visual::RemoveScope.m_Var) { return false; }

	if (!m_nHudZoom && Hash::IsHudScope(I::VGuiPanel->GetName(nPanel)))
	{
		m_nHudZoom = nPanel;
	}

	return (nPanel == m_nHudZoom);
}

void CFeatures_Visual::DoFov(CViewSetup* pViewSetup)
{
	if (!Vars::Visual::CustomFOV)
		return;

	C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

	if (pLocal && pViewSetup)
	{
		if (pLocal->GetObserverMode() == OBS_MODE_IN_EYE)
			return;

		pViewSetup->fov = Vars::Visual::CustomFOV;

		if (pLocal->IsAlive())
		{
			pLocal->m_iFOV() = Vars::Visual::CustomFOV;
		}
	}
}