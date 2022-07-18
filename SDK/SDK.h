#pragma once

#include "DrawManager/DrawManager.h"

#define XASSERT(x) if (x) Error(#x)

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

class CUtil
{
public:
	void FixMovement(const QAngle& va, CUserCmd* cmd);

	Vector fix_movement(CUserCmd* cmd, CUserCmd original_command) const;

	bool IsVisible(const Vector& vStart, const Vector& vEnd);

	Color GetTeamColor(const int nTeam);
	Color GetHealthColor(const int nHealth, const int nMaxHealth);

	IMaterial* CreateMaterial(const char* szVars);

	bool VisPos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to);

	bool VisPosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int nHitbox);

	bool VisPosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int& nHitboxOut);

	void ClampAngle(QAngle& qaAng)
	{
		while (qaAng.x > 89)
			qaAng.x -= 180;

		while (qaAng.x < -89)
			qaAng.x += 180;

		while (qaAng.y > 180)
			qaAng.y -= 360;

		while (qaAng.y < -180)
			qaAng.y += 360;

		while (qaAng.z != 0)
			qaAng.z = 0;
	}

	bool IsHeadshotWeapon(C_TFPlayer* pLocal, C_BaseCombatWeapon* pWep);

	bool IsVisible2(C_TFPlayer* pLocal, C_BaseEntity* pEntity, Vector vStart, Vector vEnd);

	float flGetDistance(Vector vOrigin, Vector vLocalOrigin);

	void AngleNormalize(Vector& v);

	bool CanShoot(C_TFPlayer* pLocal);

	bool GetProjectileInfo(C_TFPlayer* pLocal, float& speed, float& gravity);

	Vector VecViewAngles(CUserCmd* cmd);


	Vector CalcAngleProjectile(const Vector& source, const Vector& destination);
};

struct Priority {
	int Mode = 2; // 0 - Friend, 1 - Ignore, 2 - Default, 3 - Rage, 4 - Cheater
};



namespace G { inline CUtil Util; }