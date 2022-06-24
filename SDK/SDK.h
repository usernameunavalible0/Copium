#pragma once

#include "DrawManager/DrawManager.h"

#define XASSERT(x) if (x) Error(#x)

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

class CUtil
{
public:
	void FixMovement(const QAngle& va, CUserCmd* cmd);

	bool IsVisible(const Vector& vStart, const Vector& vEnd);

	Color GetTeamColor(const int nTeam);
	Color GetHealthColor(const int nHealth, const int nMaxHealth);

	IMaterial* CreateMaterial(const char* szVars);

	bool VisPos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to);

	bool VisPosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int nHitbox);

	bool VisPosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int& nHitboxOut);
};

struct Priority {
	int Mode = 2; // 0 - Friend, 1 - Ignore, 2 - Default, 3 - Rage, 4 - Cheater
};


namespace G { inline CUtil Util; }