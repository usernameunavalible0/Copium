#pragma once

#include "../../SDK/SDK.h"

class CFeatures_ESP
{
public:
	bool Active = true;
	void Render(C_TFPlayer* pLocal);

private:
	bool GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h);
	int WorldToScreen(Vector* pvPos, Vector* pvOut);
};

namespace F { inline CFeatures_ESP ESP; }