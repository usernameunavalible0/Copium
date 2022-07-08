#pragma once

#include "../../SDK/SDK.h"
#include "../Misc/Misc.h"

class CFeatures_Visual
{
private:
	int m_nHudZoom = 0;
public:
	void Run(C_TFPlayer* pLocal, CViewSetup* pSetup);

	bool RemoveScope(int nPanel);
	void Thirdperson(C_TFPlayer* pLocal);
	void DoFov(CViewSetup* pViewSetup);
	void DoSpectators();
};

namespace F { inline CFeatures_Visual Visual; }