#pragma once
#include "../Vars.h"

class CAimbot
{
public:

	void Run(C_TFPlayer* pLocal, CUserCmd* pCommand);

private:

	int GetBestTarget(C_TFPlayer* pLocal);

	int GetBestHitbox(C_TFPlayer* pLocal, C_BaseEntity* pEntity);
};

namespace F { inline CAimbot Aimbot; }