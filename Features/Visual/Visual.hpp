#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Visual
{
public:
	void Run(C_TFPlayer* pLocal, CViewSetup* pSetup);

	void Thirdperson(C_TFPlayer* pLocal);
	void DoFov(CViewSetup* pViewSetup);
};

namespace F { inline CFeatures_Visual Visual; }