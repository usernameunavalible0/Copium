#include "Aimbot.h"

void CAimbot::Run(C_TFPlayer* pLocal, CUserCmd* pCommand)
{
	g_Globals.iAimbotIndex = -1;

	if (!Vars::Aimbot::Active.m_Var)
		return;

	if (!GetAsyncKeyState(Vars::Aimbot::AimKey.m_Var))
		return;

	if (!pLocal->GetActiveWeapon())
		return;

	C_BaseEntity* pEntity = UTIL_EntityAs<C_BaseEntity*>(GetBestTarget(pLocal));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity;
	pEntity->GetHitboxPosition(iBestHitbox, vEntity);

	Vector vLocal = pLocal->Weapon_ShootPosition();

	QAngle vAngs;
	VectorAngles((vEntity - vLocal), vAngs);

	ClampAngles(vAngs);
	g_Globals.iAimbotIndex = pEntity->entindex();
	pCommand->viewangles = vAngs;

	I::EngineClient->SetViewAngles(pCommand->viewangles);

	if (Vars::Aimbot::AutoShoot.m_Var)
		pCommand->buttons |= IN_ATTACK;
}

int CAimbot::GetBestTarget(C_TFPlayer* pLocal)
{
	int iBestTarget = -1;
	//this num could be smaller 
	float flDistToBest = 99999.f;

	Vector vLocal = pLocal->Weapon_ShootPosition();

	for (int i = 1; i <= g_Globals.m_nMaxClients; i++)
	{
		if (i == g_Globals.m_nLocalIndex)
			continue;

		C_BaseEntity* pEntity = UTIL_EntityAs<C_BaseEntity*>(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive() ||
			pEntity->GetTeamNumber() == pLocal->GetTeamNumber())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity;
		pEntity->GetHitboxPosition(iBestHitbox, vEntity); //pEntity->GetWorldSpaceCenter(vEntity);

		if (pEntity->InCond(TF_COND_INVULNERABLE) ||
			pEntity->InCond(TF_COND_INVULNERABLE_WEARINGOFF) ||
			pEntity->InCond(TF_COND_PHASE))
			continue;

		float flDistToTarget = (vLocal - vEntity).Length();

		if (flDistToTarget < flDistToBest)
		{
			flDistToBest = flDistToTarget;
			iBestTarget = i;
		}
	}

	return iBestTarget;
}

int CAimbot::GetBestHitbox(C_TFPlayer* pLocal, C_BaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (Vars::Aimbot::AimHitbox.m_Var == 0)
	{
		if (G::Util.IsHeadshotWeapon(pLocal, pLocal->GetActiveWeapon()))
			iBestHitbox = HITBOX_HEAD;
		else
			iBestHitbox = HITBOX_BODY;
	}
	else if (Vars::Aimbot::AimHitbox.m_Var == 1)
	{
		iBestHitbox = HITBOX_HEAD;
	}
	else if (Vars::Aimbot::AimHitbox.m_Var == 2)
	{
		iBestHitbox = HITBOX_BODY;
	}

	Vector vEntity;

	for (int i = 0; i < 17; i++)
	{
		pEntity->GetHitboxPosition(i, vEntity);
		if (G::Util.IsVisible(pLocal->Weapon_ShootPosition(), vEntity))
			return i;
	}

	if (vEntity.IsZero())
		return -1;

	if (!G::Util.IsVisible(pLocal->Weapon_ShootPosition(), vEntity))
		return -1;

	return iBestHitbox;
}