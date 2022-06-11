#include "Visual.hpp"
#include "../Vars.h"

void CFeatures_Visual::Run(C_TFPlayer* pLocal, CViewSetup* pViewSetup)
{

}

void CFeatures_Visual::DoFov(CViewSetup* pViewSetup)
{
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