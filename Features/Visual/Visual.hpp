#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Visual
{
public:
	void Run(C_TFPlayer* pLocal, CViewSetup* pSetup);

private:
	void Thirdperson(C_TFPlayer* pLocal);
	void DoFov(C_TFPlayer* pLocal, CViewSetup* pViewSetup);
};

namespace F { inline CFeatures_Visual Visual; }