#include "AimbotHitscan.h"
#include "../../Vars.h"

bool IsHitboxValid(int nHitbox, int index)
{
	switch (nHitbox)
	{
	case HITBOX_HEAD: return (index & (1 << 0));
	case HITBOX_PELVIS: return (index & (1 << 1));
	case HITBOX_CHEST: return (index & (1 << 2));
	case HITBOX_BODY:
	case HITBOX_UPPER_CHEST:
	case HITBOX_NECK:
	case HITBOX_LEFT_CALF:
	case HITBOX_LEFT_FOREARM:
	case HITBOX_LEFT_FOOT:
	case HITBOX_RIGHT_THIGH:
	case HITBOX_RIGHT_HAND: return (index & (1 << 3));
	case HITBOX_LOWER_NECK:
	case HITBOX_RIGHT_FOOT: return (index & (1 << 4));
	}
	return false;
};

int CAimbotHitscan::GetHitbox(C_TFPlayer* pLocal, C_BaseCombatWeapon* pWeapon)
{
	switch (Vars::Aimbot::Global::AimHitbox)
	{
		case 0: { return HITBOX_HEAD; }
		case 1: { return HITBOX_BODY; }
		case 2:
		{
		const int nClassNum = pLocal->GetTeamNumber();

		if (nClassNum == TF_CLASS_SNIPER)
		{
			if (pWeapon->m_iItemDefinitionIndex() != Sniper_m_TheSydneySleeper)
			{
				return (pLocal->InCond(TF_COND_ZOOMED) ? HITBOX_HEAD : HITBOX_BODY);
			}

			return HITBOX_BODY;
		}
		if (nClassNum == TF_CLASS_SPY)
		{
			const bool bIsAmbassador = (pWeapon->m_iItemDefinitionIndex() == Spy_m_TheAmbassador || 
				pWeapon->m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador);
			
			return (bIsAmbassador ? HITBOX_HEAD : HITBOX_BODY);
		}
		return HITBOX_BODY;
		
		}
	}

	return HITBOX_HEAD;
}

ESortMethod CAimbotHitscan::GetSortMethod()
{
	switch (Vars::Aimbot::Hitscan::SortMethod)
	{
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbotHitscan::GetTargets(C_TFPlayer* pLocal, C_BaseCombatWeapon* pWeapon)
{
	static bool szIgnoreTeam = false;

	const ESortMethod sortMethod = GetSortMethod();

	if (sortMethod == ESortMethod::FOV)
	{
		// do something with menu honestly idk why this is even here
	}

	F::AimbotGlobal.m_vecTargets.clear();

	const Vector vLocalPos = pLocal->Weapon_ShootPosition();
	QAngle vLocalAngles;
	I::EngineClient->GetViewAngles(vLocalAngles);

	//Players
	if (Vars::Aimbot::Global::AimPlayers)
	{
		int n;
		int Team = pLocal->GetTeamNumber();
		int nHitbox = GetHitbox(pLocal, pWeapon);
		// eventually do something with bIsMedigun and figure out how to use TF_WEAPON_MEDIGUN

		for (n = 1; n < g_Globals.m_nMaxClients; n++)
		{
			if (n == g_Globals.m_nLocalIndex)
				continue;

			IClientEntity* pTarget = I::ClientEntityList->GetClientEntity(n);

			if (!pTarget || pTarget->IsDormant())
				continue;

			C_TFPlayer* pPlayer = pTarget->As<C_TFPlayer*>();

			if (!pPlayer || pPlayer->IsAlive() || pPlayer->InLocalTeam())
				continue;

			
			if (Vars::Aimbot::Global::IgnoreInvunirible && pPlayer->IsInvulnerable())
				continue;

			if (Vars::Aimbot::Global::IgnoreCloaked && pPlayer->IsCloaked())
				continue;

			if (Vars::Aimbot::Global::IgnoreTaunting && pPlayer->IsTaunting())
				continue;

			PriorityHitbox = nHitbox;

			Vector vPos = Vector();
			pPlayer->GetHitboxPosition(nHitbox, vPos);
			Vector vAngleTo = CalcAngle(vLocalPos, vPos);
			const float flFOVTo = sortMethod == ESortMethod::FOV ? CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			const float flDistTo = sortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;
			C_TFWeaponBase* pWeapon = pLocal->GetActiveTFWeapon();

			if (sortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Global::AimFOV)
			{
				continue;
			}



			

		}

	}
}