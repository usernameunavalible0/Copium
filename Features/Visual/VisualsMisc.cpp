#include "Visual.hpp"
#include "../Vars.h"

void CFeatures_Visual::Run(C_TFPlayer* pLocal, CViewSetup* pViewSetup)
{

}

bool CFeatures_Visual::RemoveScope(int nPanel)
{
	if (!Vars::Visual::RemoveScope) { return false; }

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