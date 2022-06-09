#include "Visual.hpp"
#include "../Vars.h"

void CFeatures_Visual::Run(C_TFPlayer* pLocal, CViewSetup* pViewSetup)
{
	if (Vars::Visual::ChangeFov)
		DoFov(pLocal, pViewSetup);
}

void CFeatures_Visual::DoFov(C_TFPlayer* pLocal, CViewSetup* pViewSetup)
{
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