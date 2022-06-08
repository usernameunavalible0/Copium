#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Aimbot
{
public:
	void Run(C_TFPlayer* pLocal, CUserCmd* cmd);
	float GetFOV(Vector angle, Vector src, Vector dst);

	Vector EliminateAbsVelocity(C_BaseEntity* entity);

	void Projectile(C_TFPlayer* pLocal, C_BaseEntity* entity, C_BaseCombatWeapon* pWeapon, int& vec_hitbox);

	bool IsProjectileWeapon(C_TFPlayer* pLocal, C_BaseCombatWeapon* pWeapon) {
		auto pLocalClass = pLocal->m_iClass(), WeaponSlot = pWeapon->GetSlot();
		if (pLocalClass == TF_CLASS_DEMOMAN || pLocalClass == TF_CLASS_SOLDIER || pLocalClass == TF_CLASS_MEDIC)
		{
			if (WeaponSlot == 0) {
				return true;
			}
		}

		if (pLocalClass == TF_CLASS_ENGINEER)
		{
			if (WeaponSlot == 0) {
				if (pWeapon->m_iItemDefinitionIndex() == Engi_m_TheRescueRanger) {
					return true;
				}
			}
		}

		return false;
	};

private:

	void w(bool silent, Vector vAngs, CUserCmd* pCommand);


	void MakeVector(Vector angle, Vector& vector);

	Vector calc_angle(Vector src, Vector dst);

	int GetBestTarget(C_TFPlayer* pLocal, CUserCmd* pCommand);

	int GetBestHitbox(C_TFPlayer* pLocal, CBaseEntity* pEntity);

	bool CanAmbassadorHeadshot(C_TFPlayer* pLocal);
};

namespace F { inline CFeatures_Aimbot Aimbot; }