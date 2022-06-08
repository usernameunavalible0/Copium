#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Triggerbot
{
public:
	bool Active = true;
	void Run(C_TFPlayer* pLocal, CUserCmd* cmd);
private:

	void Triggerbot(C_TFPlayer* pLocal, CUserCmd* cmd);

};

inline CFeatures_Triggerbot CTrigger;
namespace F { inline CFeatures_Triggerbot Triggerbot; }